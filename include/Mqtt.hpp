#include "Mqtt.h"
#include "WordClockState.h"
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <WiFiClient.h>

#define HOMEASSISTANT_DISCOVERY_TOPIC "homeassistant"

#define RETRY_INTERVALL_WITHIN_5_MINUTES 15000 // 15 Seconds
#define MAX_RETRIES_WITHIN_5_MINUTES 20

#define RETRY_INTERVALL 3600000 // 1 Hour

extern WiFiClient client;

// Static PubSubClient callbacks need a way back to the single Mqtt instance.
Mqtt *mqttInstance = nullptr;

PubSubClient mqttClient(client);

// Keep inbound payloads bounded; PubSubClient's default buffer is smaller, but
// discovery raises it and callbacks should still avoid variable-length stacks.
static constexpr unsigned int MQTT_MAX_PAYLOAD_LENGTH = 512;

// Home Assistant's birth message: HA publishes this when it (re)starts, the cue
// to re-announce discovery and state.
#define HOMEASSISTANT_STATUS_TOPIC HOMEASSISTANT_DISCOVERY_TOPIC "/status"

// Stream a (potentially large) retained JSON document to the broker without a
// full-payload buffer. PubSubClient only needs room for the header and topic,
// so the device-based discovery bundle can exceed the configured buffer size.
static bool publishJsonStream(const String &topic, const JsonDocument &doc) {
    if (doc.overflowed()) {
        Serial.print("MQTT: Discovery document overflowed for topic ");
        Serial.println(topic);
        return false;
    }
    size_t len = measureJson(doc);
    if (!mqttClient.beginPublish(topic.c_str(), len, true)) {
        Serial.print("MQTT: Failed to begin discovery publish on ");
        Serial.println(topic);
        return false;
    }
    serializeJson(doc, mqttClient);
    return mqttClient.endPublish();
}

// Add a diagnostic sensor component to a device-based discovery bundle. Keys
// are the Home Assistant short forms (p=platform, dev_cla=device_class, etc.).
// A null argument omits its key. The component id doubles as the unique_id
// suffix so it stays stable.
static void addDiagSensor(JsonObject components, const String &deviceId,
                          const char *id, const char *name,
                          const char *deviceClass, const char *unit,
                          const char *icon, const char *stateClass,
                          const char *valueTemplate) {
    JsonObject c = components.createNestedObject(id);
    c["p"] = "sensor";
    c["uniq_id"] = deviceId + "_" + id;
    c["name"] = name;
    if (deviceClass)
        c["dev_cla"] = deviceClass;
    if (unit)
        c["unit_of_meas"] = unit;
    if (icon)
        c["ic"] = icon;
    if (stateClass)
        c["stat_cla"] = stateClass;
    c["ent_cat"] = "diagnostic";
    c["stat_t"] = String(G.mqtt.topic) + "/diagnostics";
    c["val_tpl"] = valueTemplate;
}

// Clear a retained per-entity discovery config left behind by firmware versions
// that published one topic per entity, so migrating to the device-based bundle
// does not leave duplicate entities in Home Assistant.
static void clearLegacyDiscovery(const String &platform, const String &node) {
    mqttClient.publish((String(HOMEASSISTANT_DISCOVERY_TOPIC) + "/" + platform +
                        "/" + node + "/config")
                           .c_str(),
                       "", true);
}

// Home Assistant "select" entities exchange human-readable labels, while the
// firmware stores small integers. These tables map between the two so no Jinja
// templates are needed in discovery. Special-event transitions (Birthday / New
// Year, enum 97-99) are intentionally omitted: they auto-trigger by date.
struct LabeledValue {
    const char *label;
    uint8_t value;
};

#define LABELED_VALUE_COUNT(table) (sizeof(table) / sizeof((table)[0]))

static const LabeledValue TRANSITION_TYPES[] = {
    {"Off", 0},         {"Roll Up", 1}, {"Roll Down", 2}, {"Shift Left", 3},
    {"Shift Right", 4}, {"Fade", 5},    {"Laser", 6},     {"Matrix Rain", 7},
    {"Balls", 8},       {"Fire", 9},    {"Snake", 10},    {"Random", 11}};

static const LabeledValue TRANSITION_COLORIZE[] = {
    {"Off", 0}, {"Words", 1}, {"Characters", 2}};

static const LabeledValue TRANSITION_DURATION[] = {
    {"Short", 1}, {"Medium", 2}, {"Long", 3}};

// Return the label for a stored value, or fallback if the value is unknown.
static const char *labelForValue(const LabeledValue *table, size_t count,
                                 uint8_t value, const char *fallback) {
    for (size_t i = 0; i < count; i++) {
        if (table[i].value == value)
            return table[i].label;
    }
    return fallback;
}

// Resolve an incoming label to its stored value. Returns false (and leaves out
// untouched) when the label is not in the table, so callers can ignore it.
static bool valueForLabel(const LabeledValue *table, size_t count,
                          const char *label, uint8_t &out) {
    for (size_t i = 0; i < count; i++) {
        if (strcmp(table[i].label, label) == 0) {
            out = table[i].value;
            return true;
        }
    }
    return false;
}

// Add a "select" component (options list with label<->value mapping) to a
// device-based discovery bundle, as a config entity.
static void addSelect(JsonObject components, const String &deviceId,
                      const char *id, const char *name, const char *icon,
                      const LabeledValue *table, size_t count) {
    JsonObject c = components.createNestedObject(id);
    c["p"] = "select";
    c["uniq_id"] = deviceId + "_" + id;
    c["name"] = name;
    if (icon)
        c["ic"] = icon;
    c["ent_cat"] = "config";
    c["stat_t"] = String(G.mqtt.topic) + "/" + id + "/state";
    c["cmd_t"] = String(G.mqtt.topic) + "/" + id + "/set";
    JsonArray options = c.createNestedArray("options");
    for (size_t i = 0; i < count; i++)
        options.add(table[i].label);
}

// Register the instance used by PubSubClient's static callback hook.
Mqtt::Mqtt(ClockWork &cw) : clockWork(cw) { mqttInstance = this; }

Mqtt::~Mqtt() {
    if (mqttInstance == this) {
        mqttInstance = nullptr;
    }
}

//------------------------------------------------------------------------------
// Helper Functions
//------------------------------------------------------------------------------

/* Description:

This function processes the Home Assistant JSON light "state" key. Only valid
ON/OFF values are applied and echoed back; malformed values are ignored.

Input:

const JsonDocument &doc: The JSON document containing the "state" key.

Output:

None
*/

void Mqtt::processState(const JsonDocument &doc) {
    if (doc.containsKey("state")) {
        const char *state = doc["state"] | "";
        bool stateChanged = false;
        if (!strcmp(state, "ON")) {
            Serial.println("MQTT: Turning ON");
            led.setState(true);
            stateChanged = true;
        } else if (!strcmp(state, "OFF")) {
            Serial.println("MQTT: Turning OFF");
            led.setState(false);
            stateChanged = true;
        }
        if (stateChanged && mqttInstance)
            mqttInstance->sendState();
    }
}

//------------------------------------------------------------------------------

/* Description:

This function processes the Home Assistant JSON light "effect" key. Unknown
effect names are ignored so stale or malformed MQTT commands do not change the
clock mode.

Input:

const JsonDocument &doc: The JSON document containing the "effect" key.

Output:

None
*/

void Mqtt::processEffect(const JsonDocument &doc) {
    if (doc.containsKey("effect")) {
        const char *effect = doc["effect"] | "";
        bool effectChanged = false;

        if (!strcmp("Wordclock", effect)) {
            G.prog = COMMAND_MODE_WORD_CLOCK;
            effectChanged = true;
        } else if (!strcmp("Seconds", effect)) {
            G.prog = COMMAND_MODE_SECONDS;
            effectChanged = true;
        } else if (!strcmp("Digitalclock", effect)) {
            G.prog = COMMAND_MODE_DIGITAL_CLOCK;
            effectChanged = true;
        } else if (!strcmp("Scrollingtext", effect)) {
            G.prog = COMMAND_MODE_SCROLLINGTEXT;
            effectChanged = true;
        } else if (!strcmp("Rainbowcycle", effect)) {
            G.prog = COMMAND_MODE_RAINBOWCYCLE;
            effectChanged = true;
        } else if (!strcmp("Rainbow", effect)) {
            G.prog = COMMAND_MODE_RAINBOW;
            effectChanged = true;
        } else if (!strcmp("Color", effect)) {
            G.prog = COMMAND_MODE_COLOR;
            effectChanged = true;
        } else if (!strcmp("Symbol", effect)) {
            G.prog = COMMAND_MODE_SYMBOL;
            effectChanged = true;
        }

        if (effectChanged) {
            G.progInit = true;
            parametersChanged = true;
            if (mqttInstance)
                mqttInstance->sendState();
        }
    }
}

//------------------------------------------------------------------------------

/* Description:

This function processes the "scrolling_text" key. The text is always copied with
the destination buffer size because Home Assistant text entities can send longer
payloads than the clock can store.

Input:

const JsonDocument &doc: The JSON document containing the "scrolling_text" key.

Output:

None
*/

void Mqtt::processScrollingText(const JsonDocument &doc) {
    if (doc.containsKey("scrolling_text")) {
        strlcpy(G.scrollingText, doc["scrolling_text"] | "",
                sizeof(G.scrollingText));

        // Send update to web interface
        StaticJsonDocument<200> webDoc;
        webDoc["command"] = "scrolltext";
        webDoc["scrolling_text"] = G.scrollingText;
        char buffer[200];
        serializeJson(webDoc, buffer);
        webSocket.broadcastTXT(buffer, strlen(buffer));
    }
}

//------------------------------------------------------------------------------

/* Description:

This function processes the Home Assistant JSON light "color" key. Color changes
are applied only when both HS components are present; brightness is left
unchanged.

Input:

const JsonDocument &doc: The JSON document containing the "color" key.

Output:

None
*/

void Mqtt::processColor(const JsonDocument &doc) {
    JsonObjectConst color = doc["color"];
    if (!color.isNull() && color.containsKey("h") && color.containsKey("s")) {
        // Convert values from Home Assistant (0-360 for Hue, 0-100 for
        // Saturation) to clock format (0-1 for both)
        float h = constrain(color["h"].as<float>(), 0, 360) / 360.0f;
        float s = constrain(color["s"].as<float>(), 0, 100) / 100.0f;

        // Keep current brightness
        G.color[Foreground] = HsbColor(h, s, G.color[Foreground].B);
        parametersChanged = true;

        // Send update to web interface
        StaticJsonDocument<200> webDoc;
        webDoc["command"] = "color";
        webDoc["h"] = round(h * 360); // Convert to 0-360 degrees
        webDoc["s"] = round(s * 100); // Convert to 0-100%
        webDoc["v"] = round(G.color[Foreground].B * 100); // Convert to 0-100%
        char buffer[200];
        serializeJson(webDoc, buffer);
        webSocket.broadcastTXT(buffer, strlen(buffer));

        if (mqttInstance) {
            mqttInstance->sendState();
        }
    }
}

//------------------------------------------------------------------------------

/* Description:

This function processes the Home Assistant JSON light "brightness" key. Values
are clamped to the 0-255 MQTT light range before being normalized to the clock's
0.0-1.0 brightness value.

Input:

const JsonDocument &doc: The JSON document containing the "brightness" key.

Output:

None
*/

void Mqtt::processBrightness(const JsonDocument &doc) {
    if (doc.containsKey("brightness")) {
        // Manual brightness commands must not fight the ambient-light loop.
        if (G.autoBrightEnabled) {
            Serial.println("MQTT: Ignoring brightness change - auto brightness "
                           "is enabled");
            return;
        }

        int rawBrightness = constrain(doc["brightness"] | 0, 0, 255);
        float brightness = float(rawBrightness) / 255.0f;
        brightness = max(0.0f, min(1.0f, brightness));

        // Set brightness directly
        G.color[Foreground] =
            HsbColor(G.color[Foreground].H, G.color[Foreground].S, brightness);
        Serial.print("MQTT: Setting manual brightness: ");
        Serial.println(brightness);

        parametersChanged = true;

        if (mqttInstance)
            mqttInstance->sendState();
    }
}

//------------------------------------------------------------------------------

/* Description:

This function checks if a character array representing an MQTT user is empty. An
MQTT user is considered empty if it contains only null characters ('\0') up to
the specified length.

Input:

None
Output:

true if the MQTT user array is empty.
false if the MQTT user array is not empty.
*/

/* static */ bool Mqtt::checkIfMqttUserIsEmpty() {
    for (uint8_t i = 0; i < PAYLOAD_LENGTH; i++) {
        if (G.mqtt.user[i] != '\0' && !isSpace(G.mqtt.user[i])) {
            return false; // Array is not empty
        }
    }
    return true; // Array is empty
}

//------------------------------------------------------------------------------

/* Description:

This function initializes an MQTT client connection on an ESP8266 device using
the PubSubClient library. It sets up the MQTT client with the provided MQTT
server address, port, client ID, and other optional parameters such as user
credentials and topic subscription.

Input:

None
Output:

None
*/

void Mqtt::init() {
    mqttClient.setServer(G.mqtt.serverAdress, G.mqtt.port);
    mqttClient.setCallback(callback);

    // Configure LWT (Last Will and Testament)
    String availabilityTopic = String(G.mqtt.topic) + "/availability";

    if (checkIfMqttUserIsEmpty()) {
        mqttClient.connect(G.mqtt.clientId, availabilityTopic.c_str(),
                           0,          // QoS
                           true,       // retain
                           "offline"); // Last Will Message
    } else {
        mqttClient.connect(G.mqtt.clientId, G.mqtt.user, G.mqtt.password,
                           availabilityTopic.c_str(),
                           0,          // QoS
                           true,       // retain
                           "offline"); // Last Will Message
    }
    delay(50);

    // Send online status immediately after connection
    mqttClient.publish(availabilityTopic.c_str(), "online", true);

    // Main control
    mqttClient.subscribe((std::string(G.mqtt.topic) + "/cmd").c_str());
    delay(50);

    // Re-announce discovery and state when Home Assistant (re)starts.
    mqttClient.subscribe(HOMEASSISTANT_STATUS_TOPIC);
    delay(50);

    // Additional Topics
    mqttClient.subscribe(
        (std::string(G.mqtt.topic) + "/scrolltext/set").c_str());
    delay(50);
    mqttClient.subscribe(
        (std::string(G.mqtt.topic) + "/effect_speed/set").c_str());
    delay(50);
    mqttClient.subscribe(
        (std::string(G.mqtt.topic) + "/auto_brightness/set").c_str());
    delay(50);

    // Transition settings
    mqttClient.subscribe(
        (std::string(G.mqtt.topic) + "/transition_type/set").c_str());
    delay(50);
    mqttClient.subscribe(
        (std::string(G.mqtt.topic) + "/transition_colorize/set").c_str());
    delay(50);
    mqttClient.subscribe(
        (std::string(G.mqtt.topic) + "/transition_duration/set").c_str());
    delay(50);
    mqttClient.subscribe(
        (std::string(G.mqtt.topic) + "/transition_speed/set").c_str());
    delay(50);

    if (isConnected()) {
        Serial.println("MQTT Connected");
        sendDiscovery(); // Re-announce entities so they self-heal after a
                         // broker restart or purge of retained config
        sendState();     // Send initial state
    }
}

//------------------------------------------------------------------------------

/* Description:

This function attempts to reconnect the MQTT client to the broker after a loss
of connection. It includes a retry mechanism with a maximum number of retries
and a timeout duration.

Input:

None
Output:

None
*/

void Mqtt::reInit() {
    static uint8_t retryCount = 0;
    static uint32_t lastRetryTime = 0;
    static uint32_t retryIntervall = RETRY_INTERVALL_WITHIN_5_MINUTES;

    if (millis() - lastRetryTime >= retryIntervall) {
        retryCount++;
        Serial.print("Reconnecting to MQTT Server. Try ");
        Serial.println(retryCount);
        lastRetryTime = millis();

        init();

        if (retryCount >= MAX_RETRIES_WITHIN_5_MINUTES) {
            Serial.println("Switched to hourly MQTT connect retry");
            retryIntervall = RETRY_INTERVALL;
            retryCount = 0;
        }

        if (isConnected()) {
            retryCount = 0;
            retryIntervall = RETRY_INTERVALL_WITHIN_5_MINUTES;
        }
    }
}

//------------------------------------------------------------------------------

/* Description:

This function checks whether the MQTT client is currently connected to the MQTT
broker. It returns a boolean value indicating the connection status.

Input:

None
Output:

Boolean value:
true if the MQTT client is connected to the broker.
false if the MQTT client is not connected to the broker.
 */

bool Mqtt::isConnected() { return mqttClient.connected(); }

//------------------------------------------------------------------------------

/* Description:

This function is responsible for managing the MQTT client's main loop. It checks
the MQTT connection status and reinitializes the connection if necessary.
Additionally, it invokes the loop() function of the underlying MQTT client
library to handle incoming MQTT messages and maintain the connection.

Input:

None
Output:

None
*/

void Mqtt::loop() {
    static uint32_t lastStateUpdate = 0;
    const uint32_t STATE_UPDATE_INTERVAL = 30000; // Alle 30 Sekunden

    if (!isConnected()) {
        reInit();
    }
    mqttClient.loop();

    // Regular status update
    if (millis() - lastStateUpdate >= STATE_UPDATE_INTERVAL) {
        sendState();
        lastStateUpdate = millis();
    }
}

//------------------------------------------------------------------------------

/* Description:

This function handles incoming MQTT messages received from the broker. It
parses the payload as JSON data and updates the device's state and
parameters accordingly.

Input:

char *topic: A pointer to a character array containing the topic of the
received message. byte *payload: A pointer to an array of bytes containing
the payload of the received message. unsigned int length: The length of the
payload in bytes. Output:

None
*/

// Apply a label-based select command to a uint8_t setting, persist it, force a
// transition re-init, and always echo the resulting state label back. Unknown
// labels leave the setting unchanged but still echo the current state.
static void applyTransitionSelect(const char *id, const char *msg,
                                  const LabeledValue *table, size_t count,
                                  uint8_t &field) {
    uint8_t value;
    if (valueForLabel(table, count, msg, value)) {
        field = value;
        G.progInit = true;
        eeprom::write();
    } else {
        Serial.print("MQTT: Unknown option for ");
        Serial.print(id);
        Serial.print(": ");
        Serial.println(msg);
    }
    mqttClient.publish(
        (std::string(G.mqtt.topic) + "/" + id + "/state").c_str(),
        labelForValue(table, count, field, table[0].label), true);
}

void Mqtt::callback(char *topic, byte *payload, unsigned int length) {
    // Determine message type before parsing; not every topic uses JSON.
    String topicStr = String(topic);
    String baseTopic = String(G.mqtt.topic);

    // Copy into a fixed buffer so hostile or malformed MQTT payloads cannot
    // allocate arbitrarily large stack frames.
    if (length >= MQTT_MAX_PAYLOAD_LENGTH) {
        Serial.print("MQTT: Ignoring oversized payload on topic ");
        Serial.println(topicStr);
        return;
    }

    char msg[MQTT_MAX_PAYLOAD_LENGTH];
    memcpy(msg, payload, length);
    msg[length] = '\0';

    // Home Assistant's birth message: re-announce discovery and state so the
    // device repopulates after an HA restart, even if retained config was lost.
    if (topicStr == HOMEASSISTANT_STATUS_TOPIC) {
        if (strcmp(msg, "online") == 0 && mqttInstance) {
            Serial.println("MQTT: Home Assistant online, re-announcing");
            mqttInstance->sendDiscovery();
            mqttInstance->sendState();
        }
        return;
    }

    // MQTT switch commands are plain ON/OFF payloads, not JSON.
    if (topicStr == baseTopic + "/auto_brightness/set") {
        bool autoBrightnessChanged = false;
        if (strcmp(msg, "ON") == 0) {
            G.autoBrightEnabled = 1;
            Serial.println("MQTT: Auto Brightness ON");
            autoBrightnessChanged = true;
        } else if (strcmp(msg, "OFF") == 0) {
            G.autoBrightEnabled = 0;
            Serial.println("MQTT: Auto Brightness OFF");
            autoBrightnessChanged = true;
        } else {
            Serial.print("MQTT: Unknown command: ");
            Serial.println(msg);
        }

        if (autoBrightnessChanged) {
            parametersChanged = true;
        }

        // Always echo the current retained state, even for invalid commands.
        mqttClient.publish(
            (std::string(G.mqtt.topic) + "/auto_brightness/state").c_str(),
            G.autoBrightEnabled ? "ON" : "OFF", true);

        // Send update to web interface
        StaticJsonDocument<200> webDoc;
        webDoc["command"] = "autobright";
        webDoc["value"] = G.autoBrightEnabled;
        char buffer[200];
        serializeJson(webDoc, buffer);
        webSocket.broadcastTXT(buffer, strlen(buffer));
        return;
    }

    // Transition selects/number/switch are plain payloads, not JSON. Each
    // applies the setting, persists it, re-inits so it takes effect, and echoes
    // the resulting state back to Home Assistant.
    if (topicStr == baseTopic + "/transition_type/set") {
        applyTransitionSelect("transition_type", msg, TRANSITION_TYPES,
                              LABELED_VALUE_COUNT(TRANSITION_TYPES),
                              G.transitionType);
        return;
    }
    if (topicStr == baseTopic + "/transition_colorize/set") {
        applyTransitionSelect("transition_colorize", msg, TRANSITION_COLORIZE,
                              LABELED_VALUE_COUNT(TRANSITION_COLORIZE),
                              G.transitionColorize);
        return;
    }
    if (topicStr == baseTopic + "/transition_duration/set") {
        applyTransitionSelect("transition_duration", msg, TRANSITION_DURATION,
                              LABELED_VALUE_COUNT(TRANSITION_DURATION),
                              G.transitionDuration);
        return;
    }
    if (topicStr == baseTopic + "/transition_speed/set") {
        int speed = atoi(msg);
        if (speed >= 0 && speed <= 10) {
            G.transitionSpeed = speed;
            G.progInit = true;
            eeprom::write();
        }
        mqttClient.publish(
            (std::string(G.mqtt.topic) + "/transition_speed/state").c_str(),
            String(G.transitionSpeed).c_str(), true);
        return;
    }

    // Remaining command topics use JSON payloads.
    StaticJsonDocument<512> doc;
    DeserializationError error = deserializeJson(doc, msg);
    if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.c_str());
        return;
    }

    // Process remaining messages
    if (topicStr == baseTopic + "/cmd") {
        processState(doc);
        processEffect(doc);
        processScrollingText(doc);
        processColor(doc);
        processBrightness(doc);
    } else if (topicStr == baseTopic + "/scrolltext/set") {
        processScrollingText(doc);
    } else if (topicStr == baseTopic + "/effect_speed/set") {
        // Process direct string value
        int speed = atoi(msg);
        if (speed >= 1 && speed <= 10) {
            G.effectSpeed = speed;

            // Effect speed is a user setting, so persist accepted values.
            eeprom::write();
            parametersChanged = true;

            // If we are in scrolltext mode, reinitialize the program
            if (G.prog == COMMAND_MODE_SCROLLINGTEXT) {
                G.progInit = true;
            }

            // Send new state to Home Assistant
            mqttClient.publish(
                (std::string(G.mqtt.topic) + "/effect_speed/state").c_str(),
                String(G.effectSpeed).c_str(), true);

            // Send update to web interface
            StaticJsonDocument<200> webDoc;
            webDoc["command"] = "speed";
            webDoc["value"] = G.effectSpeed;
            char buffer[200];
            serializeJson(webDoc, buffer);
            webSocket.broadcastTXT(buffer, strlen(buffer));
        }
    }
}

//------------------------------------------------------------------------------

/* Description:

This function is responsible for publishing the current state of the device
to an MQTT topic. It constructs a JSON message containing information about
the device state, such as the power state (ON or OFF), color settings, and
brightness. The constructed JSON message is then published to the MQTT
broker on a specified topic.

Input:

None
Output:

None
*/

void Mqtt::sendState() {
    // Main status
    {
        StaticJsonDocument<200> doc;
        doc["state"] = (led.getState()) ? "ON" : "OFF";

        // Calculate actual brightness value
        float actualBrightness;
        if (G.autoBrightEnabled) {
            actualBrightness = (ledGain / 100.0f) * G.color[Foreground].B;
        } else {
            actualBrightness = G.color[Foreground].B;
        }
        doc["brightness"] = round(actualBrightness * 255);
        doc["color_mode"] = "hs";

        // Color as object with h and s
        JsonObject color = doc.createNestedObject("color");
        color["h"] = round(G.color[Foreground].H * 360); // Hue 0-360
        color["s"] = round(G.color[Foreground].S * 100); // Saturation 0-100

        switch (G.prog) {
        case COMMAND_MODE_WORD_CLOCK:
            doc["effect"] = "Wordclock";
            break;
        case COMMAND_MODE_SECONDS:
            doc["effect"] = "Seconds";
            break;
        case COMMAND_MODE_DIGITAL_CLOCK:
            doc["effect"] = "Digitalclock";
            break;
        case COMMAND_MODE_SCROLLINGTEXT:
            doc["effect"] = "Scrollingtext";
            break;
        case COMMAND_MODE_RAINBOWCYCLE:
            doc["effect"] = "Rainbowcycle";
            break;
        case COMMAND_MODE_RAINBOW:
            doc["effect"] = "Rainbow";
            break;
        case COMMAND_MODE_COLOR:
            doc["effect"] = "Color";
            break;
        case COMMAND_MODE_SYMBOL:
            doc["effect"] = "Symbol";
            break;
        }

        char buffer[200];
        serializeJson(doc, buffer);
        mqttClient.publish((std::string(G.mqtt.topic) + "/status").c_str(),
                           buffer, true);
    }

    // Send diagnostic values
    {
        StaticJsonDocument<320> doc;
        doc["lux"] = round(clockWork.getLuxValue()); // Round lux value
        doc["led_gain"] = round(ledGain);      // Convert to percent and round
        doc["auto_brightness_source"] =
            autoBrightUsingBH1750 ? "BH1750" : "LDR";
        doc["rssi"] = WiFi.RSSI();             // WiFi signal strength
        doc["ip"] = WiFi.localIP().toString(); // Current IP address
        doc["uptime"] = millis() / 1000;       // Seconds since boot
        char buffer[320];
        serializeJson(doc, buffer);
        mqttClient.publish((std::string(G.mqtt.topic) + "/diagnostics").c_str(),
                           buffer, true);
    }

    // Effect speed status - Always update current value
    {
        mqttClient.publish(
            (std::string(G.mqtt.topic) + "/effect_speed/state").c_str(),
            String(G.effectSpeed).c_str(), true);
    }

    // Scrolling text status
    {
        StaticJsonDocument<200> doc;
        doc["scrolling_text"] = G.scrollingText;
        char buffer[200];
        serializeJson(doc, buffer);
        mqttClient.publish(
            (std::string(G.mqtt.topic) + "/scrolltext/state").c_str(), buffer,
            true);
    }

    // Auto brightness status
    {
        mqttClient.publish(
            (std::string(G.mqtt.topic) + "/auto_brightness/state").c_str(),
            G.autoBrightEnabled ? "ON" : "OFF", true);
    }

    // Transition settings status (selects echo their labels)
    {
        mqttClient.publish(
            (std::string(G.mqtt.topic) + "/transition_type/state").c_str(),
            labelForValue(TRANSITION_TYPES,
                          LABELED_VALUE_COUNT(TRANSITION_TYPES),
                          G.transitionType, TRANSITION_TYPES[0].label),
            true);
        mqttClient.publish(
            (std::string(G.mqtt.topic) + "/transition_colorize/state").c_str(),
            labelForValue(TRANSITION_COLORIZE,
                          LABELED_VALUE_COUNT(TRANSITION_COLORIZE),
                          G.transitionColorize, TRANSITION_COLORIZE[0].label),
            true);
        mqttClient.publish(
            (std::string(G.mqtt.topic) + "/transition_duration/state").c_str(),
            labelForValue(TRANSITION_DURATION,
                          LABELED_VALUE_COUNT(TRANSITION_DURATION),
                          G.transitionDuration, TRANSITION_DURATION[1].label),
            true);
        mqttClient.publish(
            (std::string(G.mqtt.topic) + "/transition_speed/state").c_str(),
            String(G.transitionSpeed).c_str(), true);
    }

    // Update online status
    mqttClient.publish((std::string(G.mqtt.topic) + "/availability").c_str(),
                       "online", true);
}

//------------------------------------------------------------------------------

/* Description:

This function publishes MQTT discovery messages for Home Assistant,
providing configuration details for a light entity. It constructs a JSON
payload according to Home Assistant's MQTT discovery format and
publishes it to the appropriate topic.

Input:

None
Output:

None
*/

void Mqtt::sendDiscovery() {
    // The MAC is the stable device identifier and the base for every entity's
    // unique_id, so renaming the MQTT base topic never orphans the device. The
    // colon-free variant is used in the discovery topic path.
    String unique_id = WiFi.macAddress();
    String discoveryNode = unique_id;
    discoveryNode.replace(":", "");

    // Clear retained per-entity configs published by older firmware so the move
    // to a single device-based bundle leaves no duplicate entities behind.
    mqttClient.publish((String(HOMEASSISTANT_DISCOVERY_TOPIC) + "/light/" +
                        String(G.mqtt.topic) + "/light/config")
                           .c_str(),
                       "", true);
    clearLegacyDiscovery("light", discoveryNode);
    clearLegacyDiscovery("text", discoveryNode + "_scrolltext");
    clearLegacyDiscovery("number", discoveryNode + "_effect_speed");
    clearLegacyDiscovery("sensor", discoveryNode + "_lux");
    clearLegacyDiscovery("sensor", discoveryNode + "_led_gain");
    clearLegacyDiscovery("sensor", discoveryNode + "_adc_value");
    clearLegacyDiscovery("sensor", discoveryNode + "_adc_raw");
    clearLegacyDiscovery("switch", discoveryNode + "_auto_brightness");

    // Single device-based discovery bundle (Home Assistant 2024.11+). Built on
    // the heap and streamed to the broker so the payload needs no large stack
    // buffer. Availability, the device block, and the origin block are declared
    // once at the root and inherited by every component. Full topics are used
    // rather than the "~" shorthand: "~" is not one of Home Assistant's shared
    // root options, so it is not expanded for components in a device bundle.
    String base = String(G.mqtt.topic);
    DynamicJsonDocument doc(8192);

    doc["avty_t"] = base + "/availability";
    doc["pl_avail"] = "online";
    doc["pl_not_avail"] = "offline";

    JsonObject device = doc.createNestedObject("dev");
    device.createNestedArray("ids").add(unique_id);
    JsonArray macConnection =
        device.createNestedArray("cns").createNestedArray();
    macConnection.add("mac");
    macConnection.add(unique_id);
    device["name"] = String(G.mqtt.clientId);
    device["sw"] = VERSION;
    device["mdl"] = "Word Clock";
    device["mf"] = "ESPWortuhr";
    device["cu"] = "http://" + WiFi.localIP().toString();

    JsonObject origin = doc.createNestedObject("o");
    origin["name"] = "ESPWortuhr";
    origin["sw"] = VERSION;
    origin["url"] = "https://github.com/ESPWortuhr/Wortuhr";

    JsonObject cmps = doc.createNestedObject("cmps");

    // Light: the device's primary entity. name=null makes it inherit the device
    // name instead of repeating it (Home Assistant has_entity_name convention).
    {
        JsonObject light = cmps.createNestedObject("light");
        light["p"] = "light";
        light["uniq_id"] = unique_id;
        light["name"] = (const char *)nullptr;
        light["schema"] = "json";
        light["stat_t"] = base + "/status";
        light["cmd_t"] = base + "/cmd";
        light["brightness"] = true;
        light["bri_scl"] = 255;
        light.createNestedArray("sup_clrm").add("hs");
        light["optimistic"] = false;
        light["effect"] = true;
        JsonArray effectList = light.createNestedArray("fx_list");
        effectList.add("Wordclock");
        effectList.add("Seconds");
        effectList.add("Digitalclock");
        effectList.add("Scrollingtext");
        effectList.add("Rainbowcycle");
        effectList.add("Rainbow");
        effectList.add("Color");
        effectList.add("Symbol");
    }

    // Text entity for the scrolling text mode.
    {
        JsonObject text = cmps.createNestedObject("scrolltext");
        text["p"] = "text";
        text["uniq_id"] = unique_id + "_scrolltext";
        text["name"] = "Scrolling Text";
        text["ic"] = "mdi:text";
        text["stat_t"] = base + "/scrolltext/state";
        text["cmd_t"] = base + "/scrolltext/set";
        text["val_tpl"] = "{{ value_json.scrolling_text }}";
        text["cmd_tpl"] = "{ \"scrolling_text\": \"{{ value }}\" }";
        text["max"] = sizeof(G.scrollingText) - 1;
    }

    // Notify entity: pushes a message to the marquee. It targets the JSON
    // command topic with both the text and the Scrollingtext effect, so the
    // clock switches to scrolling-text mode and shows the message immediately.
    {
        JsonObject notify = cmps.createNestedObject("message");
        notify["p"] = "notify";
        notify["uniq_id"] = unique_id + "_message";
        notify["name"] = "Message";
        notify["ic"] = "mdi:message-text";
        notify["cmd_t"] = base + "/cmd";
        notify["cmd_tpl"] = "{\"scrolling_text\": \"{{ value }}\", \"effect\": "
                            "\"Scrollingtext\"}";
    }

    // Number entity for animation/effect speed.
    {
        JsonObject number = cmps.createNestedObject("effect_speed");
        number["p"] = "number";
        number["uniq_id"] = unique_id + "_effect_speed";
        number["name"] = "Effect Speed";
        number["ic"] = "mdi:speedometer";
        number["stat_t"] = base + "/effect_speed/state";
        number["cmd_t"] = base + "/effect_speed/set";
        number["min"] = 1;
        number["max"] = 10;
        number["step"] = 1;
    }

    // Switch entity for ambient-light based brightness control.
    {
        JsonObject autoBright = cmps.createNestedObject("auto_brightness");
        autoBright["p"] = "switch";
        autoBright["uniq_id"] = unique_id + "_auto_brightness";
        autoBright["name"] = "Auto Brightness";
        autoBright["ic"] = "mdi:brightness-auto";
        autoBright["dev_cla"] = "switch";
        autoBright["stat_t"] = base + "/auto_brightness/state";
        autoBright["cmd_t"] = base + "/auto_brightness/set";
        autoBright["pl_on"] = "ON";
        autoBright["pl_off"] = "OFF";
    }

    // Transition settings.
    addSelect(cmps, unique_id, "transition_type", "Transition Type",
              "mdi:transition", TRANSITION_TYPES,
              LABELED_VALUE_COUNT(TRANSITION_TYPES));
    addSelect(cmps, unique_id, "transition_colorize", "Transition Colorize",
              "mdi:palette", TRANSITION_COLORIZE,
              LABELED_VALUE_COUNT(TRANSITION_COLORIZE));
    addSelect(cmps, unique_id, "transition_duration", "Transition Duration",
              "mdi:timer-outline", TRANSITION_DURATION,
              LABELED_VALUE_COUNT(TRANSITION_DURATION));
    {
        JsonObject speed = cmps.createNestedObject("transition_speed");
        speed["p"] = "number";
        speed["uniq_id"] = unique_id + "_transition_speed";
        speed["name"] = "Transition Speed";
        speed["ic"] = "mdi:speedometer";
        speed["ent_cat"] = "config";
        speed["stat_t"] = base + "/transition_speed/state";
        speed["cmd_t"] = base + "/transition_speed/set";
        speed["min"] = 0;
        speed["max"] = 10;
        speed["step"] = 1;
    }

    // Diagnostic sensors, all read from the shared diagnostics topic.
    addDiagSensor(cmps, unique_id, "lux", "Illuminance", "illuminance", "lx",
                  "mdi:white-balance-sunny", "measurement",
                  "{{ value_json.lux }}");
    addDiagSensor(cmps, unique_id, "led_gain", "LED Gain", "power_factor", "%",
                  "mdi:brightness-percent", "measurement",
                  "{{ value_json.led_gain }}");
    addDiagSensor(cmps, unique_id, "auto_brightness_source",
                  "Auto Brightness Source", nullptr, nullptr,
                  "mdi:lightbulb-auto", nullptr,
                  "{{ value_json.auto_brightness_source }}");
    addDiagSensor(cmps, unique_id, "rssi", "WiFi Signal", "signal_strength",
                  "dBm", nullptr, "measurement", "{{ value_json.rssi }}");
    addDiagSensor(cmps, unique_id, "ip", "IP Address", nullptr, nullptr,
                  "mdi:ip-network", nullptr, "{{ value_json.ip }}");
    addDiagSensor(cmps, unique_id, "uptime", "Uptime", "duration", "s", nullptr,
                  "total_increasing", "{{ value_json.uptime }}");

    publishJsonStream(String(HOMEASSISTANT_DISCOVERY_TOPIC) + "/device/" +
                          discoveryNode + "/config",
                      doc);

    // Make the device available now that its entities are announced.
    mqttClient.publish((std::string(G.mqtt.topic) + "/availability").c_str(),
                       "online", true);
}

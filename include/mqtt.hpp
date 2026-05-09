#include "Uhr.h"
#include "mqtt.h"
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

// Serialize into the caller-provided stack buffer and skip publishing if the
// discovery payload would be truncated. Truncated retained discovery JSON is
// painful to diagnose in Home Assistant.
static bool publishJsonRetained(const String &topic, const JsonDocument &doc,
                                char *buffer, size_t bufferSize) {
    size_t len = serializeJson(doc, buffer, bufferSize);
    if (len == 0 || len >= bufferSize) {
        Serial.print("MQTT: Discovery JSON too large for topic ");
        Serial.println(topic);
        return false;
    }
    return mqttClient.publish(topic.c_str(), buffer, true);
}

// Register the instance used by PubSubClient's static callback hook.
Mqtt::Mqtt(ClockWork &cw) : clockWork(cw) { mqttInstance = this; }

Mqtt::~Mqtt() {
    if (mqttInstance == this) {
        mqttInstance = nullptr;
    }
}

// ToDo : MQTT Notify https: // www.home-assistant.io/integrations/notify.mqtt/

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

    if (isConnected()) {
        Serial.println("MQTT Connected");
        sendState(); // Send initial state
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
        StaticJsonDocument<200> doc;
        doc["lux"] = round(clockWork.getLuxValue()); // Round lux value
        doc["led_gain"] = round(ledGain); // Convert to percent and round
        doc["adc_value"] =
            clockWork.getAdcValue(); // Voltage value (already rounded)
        doc["adc_raw"] = clockWork.getAdcRawValue(); // Raw ADC value
        char buffer[200];
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
    // Keep the original Home Assistant unique_id stable, but use a sanitized
    // variant in discovery topics.
    String unique_id = WiFi.macAddress();
    String discoveryNode = unique_id;
    discoveryNode.replace(":", "");

    // Remove the pre-PR retained discovery config to avoid duplicate entities.
    mqttClient.publish((String(HOMEASSISTANT_DISCOVERY_TOPIC) + "/light/" +
                        String(G.mqtt.topic) + "/light/config")
                           .c_str(),
                       "", true);

    // Main JSON light entity.
    {
        StaticJsonDocument<700> root;
        mqttClient.setBufferSize(700);

        // Base configuration
        root["name"] = String(G.mqtt.clientId);
        root["unique_id"] = unique_id;

        // Topics
        root["state_topic"] = String(G.mqtt.topic) + "/status";
        root["command_topic"] = String(G.mqtt.topic) + "/cmd";
        root["availability_topic"] = String(G.mqtt.topic) + "/availability";
        root["payload_available"] = "online";
        root["payload_not_available"] = "offline";

        // Functions
        root["brightness"] = true;
        root["brightness_scale"] = 255;
        JsonArray colorModes = root.createNestedArray("supported_color_modes");
        colorModes.add("hs"); // Correct color mode
        root["optimistic"] = false;

        // Schema
        root["schema"] = "json";

        // Effects
        root["effect"] = true;
        JsonArray effectList = root.createNestedArray("effect_list");
        effectList.add("Wordclock");
        effectList.add("Seconds");
        effectList.add("Digitalclock");
        effectList.add("Scrollingtext");
        effectList.add("Rainbowcycle");
        effectList.add("Rainbow");
        effectList.add("Color");
        effectList.add("Symbol");

        // Device information
        JsonObject device = root.createNestedObject("device");
        JsonArray identifiers = device.createNestedArray("identifiers");
        identifiers.add(G.mqtt.topic);
        device["name"] = G.mqtt.clientId;
        device["sw_version"] = VERSION;
        device["model"] = "Word Clock";
        device["manufacturer"] = "ESPWortuhr";
        device["configuration_url"] = "http://" + WiFi.localIP().toString();

        char buffer[700];
        String discoveryTopic = String(HOMEASSISTANT_DISCOVERY_TOPIC) +
                                "/light/" + discoveryNode + "/config";
        publishJsonRetained(discoveryTopic, root, buffer, sizeof(buffer));
    }

    // Text entity for the scrolling text mode.
    {
        StaticJsonDocument<512> root;

        root["name"] = "Scrolling Text";
        root["unique_id"] = unique_id + "_scrolltext";
        root["icon"] = "mdi:text";

        JsonObject deviceCopy = root.createNestedObject("device");
        JsonArray deviceIdentifiers =
            deviceCopy.createNestedArray("identifiers");
        deviceIdentifiers.add(G.mqtt.topic);
        deviceCopy["name"] = G.mqtt.clientId;
        deviceCopy["sw_version"] = VERSION;
        deviceCopy["model"] = "Word Clock";
        deviceCopy["manufacturer"] = "ESPWortuhr";
        deviceCopy["configuration_url"] = "http://" + WiFi.localIP().toString();

        root["state_topic"] = std::string(G.mqtt.topic) + "/scrolltext/state";
        root["command_topic"] = std::string(G.mqtt.topic) + "/scrolltext/set";
        root["value_template"] = "{{ value_json.scrolling_text }}";
        root["command_template"] = "{ \"scrolling_text\": \"{{ value }}\" }";
        root["max"] = sizeof(G.scrollingText) - 1;

        char buffer[512];
        publishJsonRetained(String(HOMEASSISTANT_DISCOVERY_TOPIC) + "/text/" +
                                discoveryNode + "_scrolltext/config",
                            root, buffer, sizeof(buffer));
    }

    // Number entity for animation/effect speed.
    {
        StaticJsonDocument<512> root;

        root["name"] = "Effect Speed";
        root["unique_id"] = unique_id + "_effect_speed";
        root["icon"] = "mdi:speedometer";
        root["device_class"] = "speed";

        JsonObject deviceCopy = root.createNestedObject("device");
        JsonArray deviceIdentifiers =
            deviceCopy.createNestedArray("identifiers");
        deviceIdentifiers.add(G.mqtt.topic);
        deviceCopy["name"] = G.mqtt.clientId;
        deviceCopy["sw_version"] = VERSION;
        deviceCopy["model"] = "Word Clock";
        deviceCopy["manufacturer"] = "ESPWortuhr";
        deviceCopy["configuration_url"] = "http://" + WiFi.localIP().toString();

        root["state_topic"] = std::string(G.mqtt.topic) + "/effect_speed/state";
        root["command_topic"] = std::string(G.mqtt.topic) + "/effect_speed/set";
        root["min"] = 1;
        root["max"] = 10;
        root["step"] = 1;

        char buffer[512];
        publishJsonRetained(String(HOMEASSISTANT_DISCOVERY_TOPIC) + "/number/" +
                                discoveryNode + "_effect_speed/config",
                            root, buffer, sizeof(buffer));
    }

    // Diagnostic sensors published from the shared diagnostics topic.
    {
        // Lux Sensor
        StaticJsonDocument<512> root;
        root["name"] = "Illuminance";
        root["unique_id"] = unique_id + "_lux";
        root["device_class"] = "illuminance";
        root["unit_of_measurement"] = "lx";
        root["icon"] = "mdi:white-balance-sunny";
        root["state_class"] = "measurement";

        JsonObject deviceCopy = root.createNestedObject("device");
        JsonArray deviceIdentifiers =
            deviceCopy.createNestedArray("identifiers");
        deviceIdentifiers.add(G.mqtt.topic);
        deviceCopy["name"] = G.mqtt.clientId;
        deviceCopy["sw_version"] = VERSION;
        deviceCopy["model"] = "Word Clock";
        deviceCopy["manufacturer"] = "ESPWortuhr";
        deviceCopy["configuration_url"] = "http://" + WiFi.localIP().toString();

        root["state_topic"] = std::string(G.mqtt.topic) + "/diagnostics";
        root["value_template"] = "{{ value_json.lux }}";

        char buffer[512];
        publishJsonRetained(String(HOMEASSISTANT_DISCOVERY_TOPIC) + "/sensor/" +
                                discoveryNode + "_lux/config",
                            root, buffer, sizeof(buffer));
    }

    {
        // LED Gain Sensor
        StaticJsonDocument<512> root;
        root["name"] = "LED Gain";
        root["unique_id"] = unique_id + "_led_gain";
        root["device_class"] = "power_factor";
        root["unit_of_measurement"] = "%";
        root["icon"] = "mdi:brightness-percent";
        root["state_class"] = "measurement";

        JsonObject deviceCopy = root.createNestedObject("device");
        JsonArray deviceIdentifiers =
            deviceCopy.createNestedArray("identifiers");
        deviceIdentifiers.add(G.mqtt.topic);
        deviceCopy["name"] = G.mqtt.clientId;
        deviceCopy["sw_version"] = VERSION;
        deviceCopy["model"] = "Word Clock";
        deviceCopy["manufacturer"] = "ESPWortuhr";
        deviceCopy["configuration_url"] = "http://" + WiFi.localIP().toString();

        root["state_topic"] = std::string(G.mqtt.topic) + "/diagnostics";
        root["value_template"] = "{{ value_json.led_gain }}";

        char buffer[512];
        publishJsonRetained(String(HOMEASSISTANT_DISCOVERY_TOPIC) + "/sensor/" +
                                discoveryNode + "_led_gain/config",
                            root, buffer, sizeof(buffer));
    }

    {
        // ADC Value Sensor
        StaticJsonDocument<512> root;
        root["name"] = "ADC Value";
        root["unique_id"] = unique_id + "_adc_value";
        root["device_class"] = "voltage";
        root["unit_of_measurement"] = "V";
        root["icon"] = "mdi:flash";
        root["state_class"] = "measurement";
        root["value_template"] = "{{ value_json.adc_value | round(2) }}";

        JsonObject deviceCopy = root.createNestedObject("device");
        JsonArray deviceIdentifiers =
            deviceCopy.createNestedArray("identifiers");
        deviceIdentifiers.add(G.mqtt.topic);
        deviceCopy["name"] = G.mqtt.clientId;
        deviceCopy["sw_version"] = VERSION;
        deviceCopy["model"] = "Word Clock";
        deviceCopy["manufacturer"] = "ESPWortuhr";
        deviceCopy["configuration_url"] = "http://" + WiFi.localIP().toString();

        root["state_topic"] = std::string(G.mqtt.topic) + "/diagnostics";

        char buffer[512];
        publishJsonRetained(String(HOMEASSISTANT_DISCOVERY_TOPIC) + "/sensor/" +
                                discoveryNode + "_adc_value/config",
                            root, buffer, sizeof(buffer));
    }

    {
        // ADC Raw Value Sensor
        StaticJsonDocument<512> root;
        root["name"] = "ADC Raw Value";
        root["unique_id"] = unique_id + "_adc_raw";
        root["unit_of_measurement"] = "";
        root["icon"] = "mdi:flash";
        root["state_class"] = "measurement";

        JsonObject deviceCopy = root.createNestedObject("device");
        JsonArray deviceIdentifiers =
            deviceCopy.createNestedArray("identifiers");
        deviceIdentifiers.add(G.mqtt.topic);
        deviceCopy["name"] = G.mqtt.clientId;
        deviceCopy["sw_version"] = VERSION;
        deviceCopy["model"] = "Word Clock";
        deviceCopy["manufacturer"] = "ESPWortuhr";
        deviceCopy["configuration_url"] = "http://" + WiFi.localIP().toString();

        root["state_topic"] = std::string(G.mqtt.topic) + "/diagnostics";
        root["value_template"] = "{{ value_json.adc_raw | int }}";

        char buffer[512];
        publishJsonRetained(String(HOMEASSISTANT_DISCOVERY_TOPIC) + "/sensor/" +
                                discoveryNode + "_adc_raw/config",
                            root, buffer, sizeof(buffer));
    }

    // Publish availability after discovery so new entities become available.
    mqttClient.publish((std::string(G.mqtt.topic) + "/availability").c_str(),
                       "online", true);

    // Switch entity for ambient-light based brightness control.
    {
        StaticJsonDocument<512> root;
        root["name"] = "Auto Brightness";
        root["unique_id"] = unique_id + "_auto_brightness";
        root["icon"] = "mdi:brightness-auto";
        root["device_class"] = "switch";

        JsonObject deviceCopy = root.createNestedObject("device");
        JsonArray deviceIdentifiers =
            deviceCopy.createNestedArray("identifiers");
        deviceIdentifiers.add(G.mqtt.topic);
        deviceCopy["name"] = G.mqtt.clientId;
        deviceCopy["sw_version"] = VERSION;
        deviceCopy["model"] = "Word Clock";
        deviceCopy["manufacturer"] = "ESPWortuhr";
        deviceCopy["configuration_url"] = "http://" + WiFi.localIP().toString();

        root["state_topic"] =
            std::string(G.mqtt.topic) + "/auto_brightness/state";
        root["command_topic"] =
            std::string(G.mqtt.topic) + "/auto_brightness/set";
        root["payload_on"] = "ON";
        root["payload_off"] = "OFF";

        char buffer[512];
        publishJsonRetained(String(HOMEASSISTANT_DISCOVERY_TOPIC) + "/switch/" +
                                discoveryNode + "_auto_brightness/config",
                            root, buffer, sizeof(buffer));
    }
}

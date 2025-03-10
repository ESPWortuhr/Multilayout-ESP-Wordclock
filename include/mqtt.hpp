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

// Globale Instanz
Mqtt *mqttInstance = nullptr;

PubSubClient mqttClient(client);

// Konstruktor und Destruktor
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

This function processes the "state" key in the provided JSON document. If the
"state" key is present, it checks its value and sets the LED state accordingly.
The LED can be turned ON or OFF based on the value of the "state" key.

Input:

const JsonDocument &doc: The JSON document containing the "state" key.

Output:

None
*/

void Mqtt::processState(const JsonDocument &doc) {
    if (doc.containsKey("state")) {
        const char *state = doc["state"];
        if (!strcmp(state, "ON")) {
            Serial.println("MQTT: Turning ON");
            led.setState(true);
        } else if (!strcmp(state, "OFF")) {
            Serial.println("MQTT: Turning OFF");
            led.setState(false);
        }
        if (mqttInstance)
            mqttInstance->sendState();
    }
}

//------------------------------------------------------------------------------

/* Description:

This function processes the "effect" key in the provided JSON document. If the
"effect" key is present, it checks its value and sets the program mode
accordingly. The program mode can be one of several predefined modes such as
Wordclock, Seconds, Digitalclock, etc.

Input:

const JsonDocument &doc: The JSON document containing the "effect" key.

Output:

None
*/

void Mqtt::processEffect(const JsonDocument &doc) {
    if (doc.containsKey("effect")) {
        const char *effect = doc["effect"];
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

This function processes the "scrolling_text" key in the provided JSON document.
If the "scrolling_text" key is present, it copies its value to the global
scrolling text buffer.

Input:

const JsonDocument &doc: The JSON document containing the "scrolling_text" key.

Output:

None
*/

void Mqtt::processScrollingText(const JsonDocument &doc) {
    if (doc.containsKey("scrolling_text")) {
        strcpy(G.scrollingText, doc["scrolling_text"]);
        
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

This function processes the "color" key in the provided JSON document. If the
"color" key is present, it updates the foreground color based on the hue (h) and
saturation (s) values provided in the JSON document. The brightness component of
the color is preserved from the current foreground color.

Input:

const JsonDocument &doc: The JSON document containing the "color" key.

Output:

None
*/

void Mqtt::processColor(const JsonDocument &doc) {
    JsonObjectConst color = doc["color"];
    if (!color.isNull()) {
        uint8_t r = color["r"] | 0;
        uint8_t g = color["g"] | 0;
        uint8_t b = color["b"] | 0;

        // Direkte Umwandlung in HSB
        float h, s;
        float rf = r / 255.0f;
        float gf = g / 255.0f;
        float bf = b / 255.0f;

        float cmax = max(max(rf, gf), bf);
        float cmin = min(min(rf, gf), bf);
        float diff = cmax - cmin;

        // Berechne Hue
        if (diff == 0) {
            h = 0;
        } else if (cmax == rf) {
            h = fmod((60 * ((gf - bf) / diff) + 360), 360) / 360.0f;
        } else if (cmax == gf) {
            h = fmod((60 * ((bf - rf) / diff) + 120), 360) / 360.0f;
        } else {
            h = fmod((60 * ((rf - gf) / diff) + 240), 360) / 360.0f;
        }

        // Berechne Saturation
        s = (cmax == 0) ? 0 : (diff / cmax);

        // Behalte aktuelle Helligkeit bei
        G.color[Foreground] = HsbColor(h, s, G.color[Foreground].B);
        parametersChanged = true;

        // Sende Update an die Weboberfläche
        StaticJsonDocument<200> webDoc;
        webDoc["command"] = "color";
        webDoc["h"] = round(h * 360); 
        // Konvertiere zu 0-360 Grad
        webDoc["s"] = round(s * 100); 
        // Konvertiere zu 0-100%
        webDoc["v"] = round(G.color[Foreground].B * 100); 
        // Konvertiere zu 0-100%
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

This function processes the "brightness" key in the provided JSON document. If
the "brightness" key is present, it updates the brightness of the foreground
color based on the value associated with the "brightness" key. The brightness
value is expected to be in the range of 0 to 255 and is normalized to a float
between 0 and 1. The function also sets a flag indicating that parameters have
changed.

Input:

const JsonDocument &doc: The JSON document containing the "brightness" key.

Output:

None
*/

void Mqtt::processBrightness(const JsonDocument &doc) {
    if (doc.containsKey("brightness")) {
        float brightness = float(uint8_t(doc["brightness"])) / 255.0f;
        brightness = max(0.0f, min(1.0f, brightness));

        G.color[Foreground] =
            HsbColor(G.color[Foreground].H, G.color[Foreground].S, brightness);
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
    
    // LWT (Last Will and Testament) konfigurieren
    String availabilityTopic = String(G.mqtt.topic) + "/availability";
    
    if (checkIfMqttUserIsEmpty()) {
        mqttClient.connect(G.mqtt.clientId, 
                          availabilityTopic.c_str(), 
                          0, // QoS
                          true, // retain
                          "offline"); // LWT Nachricht
    } else {
        mqttClient.connect(G.mqtt.clientId, 
                          G.mqtt.user, 
                          G.mqtt.password,
                          availabilityTopic.c_str(), 
                          0, // QoS
                          true, // retain
                          "offline"); // LWT Nachricht
    }
    delay(50);

    // Sofort nach Verbindung den Online-Status senden
    mqttClient.publish(availabilityTopic.c_str(), "online", true);

    // Hauptsteuerung
    mqttClient.subscribe(
        (std::string(G.mqtt.topic) + "/cmd").c_str());
    delay(50);

    // Zusätzliche Topics
    mqttClient.subscribe(
        (std::string(G.mqtt.topic) + "/scrolltext/set").c_str());
    delay(50);
    mqttClient.subscribe(
        (std::string(G.mqtt.topic) + "/bright_offset/set").c_str());
    delay(50);
    mqttClient.subscribe(
        (std::string(G.mqtt.topic) + "/bright_slope/set").c_str());
    delay(50);
    mqttClient.subscribe(
        (std::string(G.mqtt.topic) + "/scrolling_text_speed/set").c_str());
    delay(50);

    if (isConnected()) {
        Serial.println("MQTT Connected");
        sendState(); // Sende initialen Status
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

    // Regelmäßiges Update des Status
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
    StaticJsonDocument<512> doc;

    // Convert payload to a null-terminated string
    char msg[length + 1];
    memcpy(msg, payload, length);
    msg[length] = '\0';

    Serial.print("Received message [");
    Serial.print(topic);
    Serial.print("] ");
    Serial.println(msg);

    // Deserialize JSON
    DeserializationError error = deserializeJson(doc, msg);
    if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.c_str());
        return;
    }

    // Bestimme den Nachrichtentyp anhand des Topics
    String topicStr = String(topic);
    String baseTopic = String(G.mqtt.topic);

    if (topicStr == baseTopic + "/cmd") {
        // Hauptsteuerung (wie bisher)
        processState(doc);
        processEffect(doc);
        processScrollingText(doc);
        processColor(doc);
        processBrightness(doc);
    } else if (topicStr == baseTopic + "/scrolltext/set") {
        processScrollingText(doc);
    } else if (topicStr == baseTopic + "/bright_offset/set") {
        processBrightOffset(doc);
    } else if (topicStr == baseTopic + "/bright_slope/set") {
        processBrightSlope(doc);
    } else if (topicStr == baseTopic + "/scroll_speed/set") {
        processScrollSpeed(doc);
    } else if (topicStr == baseTopic + "/effect_speed/set") {
        processEffectSpeed(doc);
    } else if (topicStr == baseTopic + "/scrolling_text_speed/set") {
        int speed = atoi(msg); // Konvertiere den String direkt in eine Zahl
        G.effectSpeed = constrain(speed, 1, 10);
        parametersChanged = true;

        // Publish new state
        String stateTopic = String(G.mqtt.topic) + 
                           "/scrolling_text_speed/state";
        mqttClient.publish(stateTopic.c_str(), 
                         String(G.effectSpeed).c_str(),
                         true);

        // Send update to web interface
        StaticJsonDocument<200> webDoc;
        webDoc["command"] = "speed";
        webDoc["value"] = G.effectSpeed;
        char buffer[200];
        serializeJson(webDoc, buffer);
        webSocket.broadcastTXT(buffer, strlen(buffer));
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
    // Hauptstatus
    {
        StaticJsonDocument<200> doc;
        doc["state"] = (led.getState()) ? "ON" : "OFF";
        doc["brightness"] = round(G.color[Foreground].B * 255); 
        // Konvertiere von 0-1 zu 0-255
        doc["color_mode"] = "rgb";

        // Konvertiere HSB zu RGB
        float h = G.color[Foreground].H;
        float s = G.color[Foreground].S;
        float v = 1.0f; // Volle Helligkeit für Farbe

        float c = v * s;
        float x = c * (1 - abs(fmod(h * 6, 2) - 1));
        float m = v - c;

        float r, g, b;
        if (h < 1.0f / 6.0f) {
            r = c;
            g = x;
            b = 0;
        } else if (h < 2.0f / 6.0f) {
            r = x;
            g = c;
            b = 0;
        } else if (h < 3.0f / 6.0f) {
            r = 0;
            g = c;
            b = x;
        } else if (h < 4.0f / 6.0f) {
            r = 0;
            g = x;
            b = c;
        } else if (h < 5.0f / 6.0f) {
            r = x;
            g = 0;
            b = c;
        } else {
            r = c;
            g = 0;
            b = x;
        }

        JsonObject color = doc.createNestedObject("color");
        color["r"] = round((r + m) * 255);
        color["g"] = round((g + m) * 255);
        color["b"] = round((b + m) * 255);

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

    // Diagnose-Werte senden
    {
        StaticJsonDocument<200> doc;
        doc["lux"] = round(clockWork.getLuxValue()); 
        // Runde den Lux-Wert
        doc["led_gain"] = round(ledGain); 
        // Konvertiere zu Prozent und runde
        doc["adc_value"] = clockWork.getAdcValue(); 
        // Spannungswert (bereits gerundet)
        doc["adc_raw"] = clockWork.getAdcRawValue(); 
        // Roher ADC-Wert
        char buffer[200];
        serializeJson(doc, buffer);
        mqttClient.publish(
            (std::string(G.mqtt.topic) + "/diagnostics").c_str(),
            buffer, true);
    }

    // Helligkeitsoffset Status
    {
        mqttClient.publish(
            (std::string(G.mqtt.topic) + "/bright_offset/state").c_str(),
            String(G.autoBrightOffset).c_str(), true);
    }

    // Helligkeitssteigung Status
    {
        mqttClient.publish(
            (std::string(G.mqtt.topic) + "/bright_slope/state").c_str(),
            String(G.autoBrightSlope).c_str(), true);
    }

    // Laufschriftgeschwindigkeit Status
    {
        mqttClient.publish(
            (std::string(G.mqtt.topic) + "/scroll_speed/state").c_str(),
            String(G.effectSpeed).c_str(), true);
    }

    // Scrollingtext Status
    {
        StaticJsonDocument<200> doc;
        doc["scrolling_text"] = G.scrollingText;
        char buffer[200];
        serializeJson(doc, buffer);
        mqttClient.publish(
            (std::string(G.mqtt.topic) + "/scrolltext/state").c_str(), buffer,
            true);
    }

    // Effektgeschwindigkeit Status
    {
        mqttClient.publish(
            (std::string(G.mqtt.topic) + "/effect_speed/state").c_str(),
            String(G.effectSpeed).c_str(), true);
    }

    // Aktualisiere den Online-Status
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

/* Example MQTT Message
{
    "brightness": true,
    "color_mode": true,
    "supported_color_modes": [
        "hs"
    ],
    "schema": "json",
    "name": "ESP",
    "device": {
        "identifiers": [
            "ESPBuro"
        ],
        "name": "ESP",
        "sw_version": "3.3",
        "configuration_url": "http://<IP-Adress>"
    },
    "state_topic": "ESPBuro/status",
    "command_topic": "ESPBuro/cmd",
    "unique_id": "<MAC-Adress>",
    "plattform": "mqtt",
    "effect": true,
    "effect_list": [
        "Wordclock",
        "Seconds",
        "Digitalclock",
        "Scrollingtext",
        "Rainbowcycle",
        "Rainbow",
        "Color",
        "Symbol"
    ]
}
*/

void Mqtt::sendDiscovery() {
    // Erstelle eine unique_id basierend auf MAC-Adresse
    String unique_id = WiFi.macAddress();
    // Entferne die Doppelpunkte aus der MAC-Adresse
    unique_id.replace(":", ""); 

    // Hauptlicht-Entity
    {
        StaticJsonDocument<700> root;
        mqttClient.setBufferSize(700);

        // Basis-Konfiguration
        root["name"] = String(G.mqtt.clientId);
        root["unique_id"] = unique_id;

        // Topics
        root["state_topic"] = String(G.mqtt.topic) + "/status";
        root["command_topic"] = String(G.mqtt.topic) + "/cmd";
        root["availability_topic"] = String(G.mqtt.topic) + "/availability";
        root["payload_available"] = "online";
        root["payload_not_available"] = "offline";

        // Funktionen
        root["brightness"] = true;
        root["brightness_scale"] = 255;
        JsonArray colorModes = root.createNestedArray("supported_color_modes");
        colorModes.add("rgb"); // Korrekter Color Mode
        root["optimistic"] = false;

        // Schema
        root["schema"] = "json";

        // Effekte
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

        // Geräte-Information
        JsonObject device = root.createNestedObject("device");
        JsonArray deviceIdentifiers = device.createNestedArray("identifiers");
        deviceIdentifiers.add(unique_id);
        device["name"] = G.mqtt.clientId;
        device["sw_version"] = VERSION;
        device["model"] = "Word Clock";
        device["manufacturer"] = "ESPWortuhr";
        device["configuration_url"] = "http://" + WiFi.localIP().toString();

        char buffer[700];
        serializeJson(root, buffer);

        String discoveryTopic =
            String(HOMEASSISTANT_DISCOVERY_TOPIC) + "/light/" + unique_id +
            "/config";
        mqttClient.publish(discoveryTopic.c_str(), buffer, true);
    }

    // 2. Lauftext Switch
    {
        StaticJsonDocument<512> root;

        root["name"] = "Scrolling Text";
        root["unique_id"] = unique_id + "_scrolltext";
        root["icon"] = "mdi:text";

        JsonObject deviceCopy = root.createNestedObject("device");
        JsonArray deviceIdentifiers = 
            deviceCopy.createNestedArray("identifiers");
        deviceIdentifiers.add(unique_id);
        deviceCopy["name"] = G.mqtt.clientId;
        deviceCopy["sw_version"] = VERSION;
        deviceCopy["model"] = "Word Clock";
        deviceCopy["manufacturer"] = "ESPWortuhr";
        deviceCopy["configuration_url"] = 
            "http://" + WiFi.localIP().toString();

        root["state_topic"] = std::string(G.mqtt.topic) + "/scrolltext/state";
        root["command_topic"] = std::string(G.mqtt.topic) + "/scrolltext/set";
        root["value_template"] = "{{ value_json.scrolling_text }}";
        root["command_template"] = "{ \"scrolling_text\": \"{{ value }}\" }";

        char buffer[512];
        serializeJson(root, buffer);
        mqttClient.publish(
            (String(HOMEASSISTANT_DISCOVERY_TOPIC) + "/text/" + unique_id +
             "_scrolltext/config")
                .c_str(),
            buffer, true);
    }

    // 3. Lauftext-Geschwindigkeit
    {
        StaticJsonDocument<512> root;

        root["name"] = "Scrolling Speed";
        root["unique_id"] = unique_id + "_scrolling_text_speed";
        root["icon"] = "mdi:speedometer";
        root["device_class"] = "speed"; // Korrekte Device Class

        JsonObject deviceCopy = root.createNestedObject("device");
        JsonArray deviceIdentifiers = 
            deviceCopy.createNestedArray("identifiers");
        deviceIdentifiers.add(unique_id);
        deviceCopy["name"] = G.mqtt.clientId;
        deviceCopy["sw_version"] = VERSION;
        deviceCopy["model"] = "Word Clock";
        deviceCopy["manufacturer"] = "ESPWortuhr";
        deviceCopy["configuration_url"] = 
            "http://" + WiFi.localIP().toString();

        root["state_topic"] =
            std::string(G.mqtt.topic) + "/scrolling_text_speed/state";
        root["command_topic"] =
            std::string(G.mqtt.topic) + "/scrolling_text_speed/set";
        root["min"] = 1;
        root["max"] = 10;
        root["step"] = 1;

        char buffer[512];
        serializeJson(root, buffer);
        mqttClient.publish(
            (String(HOMEASSISTANT_DISCOVERY_TOPIC) + "/number/" + unique_id +
             "_scrolling_text_speed/config")
                .c_str(),
            buffer, true);
    }

    // Diagnose-Entities
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
        deviceIdentifiers.add(unique_id);
        deviceCopy["name"] = G.mqtt.clientId;
        deviceCopy["sw_version"] = VERSION;
        deviceCopy["model"] = "Word Clock";
        deviceCopy["manufacturer"] = "ESPWortuhr";
        deviceCopy["configuration_url"] = 
            "http://" + WiFi.localIP().toString();

        root["state_topic"] = std::string(G.mqtt.topic) + "/diagnostics";
        root["value_template"] = "{{ value_json.lux }}";

        char buffer[512];
        serializeJson(root, buffer);
        mqttClient.publish(
            (String(HOMEASSISTANT_DISCOVERY_TOPIC) + "/sensor/" + unique_id +
             "_lux/config")
                .c_str(),
            buffer, true);
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
        deviceIdentifiers.add(unique_id);
        deviceCopy["name"] = G.mqtt.clientId;
        deviceCopy["sw_version"] = VERSION;
        deviceCopy["model"] = "Word Clock";
        deviceCopy["manufacturer"] = "ESPWortuhr";
        deviceCopy["configuration_url"] = 
            "http://" + WiFi.localIP().toString();

        root["state_topic"] = std::string(G.mqtt.topic) + "/diagnostics";
        root["value_template"] = "{{ value_json.led_gain }}";

        char buffer[512];
        serializeJson(root, buffer);
        mqttClient.publish(
            (String(HOMEASSISTANT_DISCOVERY_TOPIC) + "/sensor/" + unique_id +
             "_led_gain/config")
                .c_str(),
            buffer, true);
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
        deviceIdentifiers.add(unique_id);
        deviceCopy["name"] = G.mqtt.clientId;
        deviceCopy["sw_version"] = VERSION;
        deviceCopy["model"] = "Word Clock";
        deviceCopy["manufacturer"] = "ESPWortuhr";
        deviceCopy["configuration_url"] = 
            "http://" + WiFi.localIP().toString();

        root["state_topic"] = std::string(G.mqtt.topic) + "/diagnostics";

        char buffer[512];
        serializeJson(root, buffer);
        mqttClient.publish(
            (String(HOMEASSISTANT_DISCOVERY_TOPIC) + "/sensor/" + unique_id +
             "_adc_value/config")
                .c_str(),
            buffer, true);
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
        deviceIdentifiers.add(unique_id);
        deviceCopy["name"] = G.mqtt.clientId;
        deviceCopy["sw_version"] = VERSION;
        deviceCopy["model"] = "Word Clock";
        deviceCopy["manufacturer"] = "ESPWortuhr";
        deviceCopy["configuration_url"] = 
            "http://" + WiFi.localIP().toString();

        root["state_topic"] = std::string(G.mqtt.topic) + "/diagnostics";
        root["value_template"] = "{{ value_json.adc_raw | int }}";

        char buffer[512];
        serializeJson(root, buffer);
        mqttClient.publish(
            (String(HOMEASSISTANT_DISCOVERY_TOPIC) + "/sensor/" + unique_id +
             "_adc_raw/config")
                .c_str(),
            buffer, true);
    }

    // Sende Online-Status
    mqttClient.publish((std::string(G.mqtt.topic) + "/availability").c_str(),
                      "online", true);
}

// Neue Callback-Funktionen für die zusätzlichen Entitäten
void Mqtt::processBrightOffset(const JsonDocument &doc) {
    if (doc.containsKey("value")) {
        G.autoBrightOffset = doc["value"];
        parametersChanged = true;
        if (mqttInstance)
            mqttInstance->sendState();
    }
}

void Mqtt::processBrightSlope(const JsonDocument &doc) {
    if (doc.containsKey("value")) {
        G.autoBrightSlope = doc["value"];
        parametersChanged = true;
        if (mqttInstance)
            mqttInstance->sendState();
    }
}

void Mqtt::processScrollSpeed(const JsonDocument &doc) {
    if (doc.containsKey("value")) {
        G.effectSpeed = doc["value"];
        parametersChanged = true;
        if (mqttInstance)
            mqttInstance->sendState();
    }
}

void Mqtt::processEffectSpeed(const JsonDocument &doc) {
    if (doc.containsKey("value")) {
        G.effectSpeed = doc["value"];
        parametersChanged = true;
        if (mqttInstance)
            mqttInstance->sendState();
    }
}



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

PubSubClient mqttClient(client);

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

bool checkIfMqttUserIsEmpty() {
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
    if (checkIfMqttUserIsEmpty()) {
        mqttClient.connect(G.mqtt.clientId);
    } else {
        mqttClient.connect(G.mqtt.clientId, G.mqtt.user, G.mqtt.password);
    }
    delay(50);
    mqttClient.subscribe((std::string(G.mqtt.topic) + "/cmd").c_str());
    delay(50);
    if (isConnected()) {
        Serial.println("MQTT Connected");
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

        // Check if maximum retries reached
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
    if (!isConnected()) {
        reInit();
    }
    mqttClient.loop();
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

    char msg[length + 1];
    // Convert payload to a null-terminated string
    memcpy(msg, payload, length);
    msg[length] = '\0';

    Serial.print("Received message [");
    Serial.print(msg);
    Serial.print("] ");

    // Deserialize JSON
    DeserializationError error = deserializeJson(doc, msg);

    if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.c_str());
        return;
    }

    // Process received JSON data
    if (doc.containsKey("state")) {
        const char *state = doc["state"];
        if (!strcmp(state, "ON")) {
            Serial.println("ON");
            led.setState(true);
        } else if (!strcmp(state, "OFF")) {
            Serial.println("OFF");
            led.setState(false);
        }
    }

    const char *effect = doc["effect"];
    if (doc.containsKey("effect")) {
        if (!strcmp("Wordclock", effect)) {
            G.prog = COMMAND_MODE_WORD_CLOCK;
        } else if (!strcmp("Seconds", effect)) {
            G.prog = COMMAND_MODE_SECONDS;
        } else if (!strcmp("Digitalclock", effect)) {
            G.prog = COMMAND_MODE_DIGITAL_CLOCK;
        } else if (!strcmp("Scrollingtext", effect)) {
            G.prog = COMMAND_MODE_SCROLLINGTEXT;
        } else if (!strcmp("Rainbowcycle", effect)) {
            G.prog = COMMAND_MODE_RAINBOWCYCLE;
        } else if (!strcmp("Rainbow", effect)) {
            G.prog = COMMAND_MODE_RAINBOW;
        } else if (!strcmp("Color", effect)) {
            G.prog = COMMAND_MODE_COLOR;
        } else if (!strcmp("Symbol", effect)) {
            G.prog = COMMAND_MODE_SYMBOL;
        }
    }

    // Copy marquee_text if present
    if (doc.containsKey("marquee_text")) {
        strcpy(G.scrollingText, doc["marquee_text"]);
    }

    // Update color if present
    JsonObject color = doc["color"];
    if (!color.isNull()) {
        G.color[Foreground] =
            HsbColor(float(color["h"]) / 360.f, float(color["s"]) / 100.f,
                     G.color[Foreground].B);
        parametersChanged = true;
    }

    // Update brightness if present
    if (doc.containsKey("brightness")) {
        G.color[Foreground] =
            HsbColor(G.color[Foreground].H, G.color[Foreground].S,
                     uint8_t(doc["brightness"]) / 255.f);
        parametersChanged = true;
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
    StaticJsonDocument<200> doc;

    doc["state"] = (led.getState()) ? "ON" : "OFF";

    JsonObject color = doc.createNestedObject("color");

    color["h"] = G.color[Foreground].H * 360;
    color["s"] = G.color[Foreground].S * 100;

    doc["brightness"] = G.color[Foreground].B * 255;

    char buffer[200];
    serializeJson(doc, buffer);
    mqttClient.publish((std::string(G.mqtt.topic) + "/status").c_str(), buffer);
}

//------------------------------------------------------------------------------

void Mqtt::sendDiscovery() {

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

    StaticJsonDocument<700> root;
    mqttClient.setBufferSize(700);

    root["brightness"] = true;
    root["color_mode"] = true;

    JsonArray colorMode = root.createNestedArray("supported_color_modes");
    colorMode.add("hs");

    root["schema"] = "json";
    root["name"] = G.mqtt.clientId;

    JsonObject device = root.createNestedObject("device");

    JsonArray identifiers = device.createNestedArray("identifiers");
    identifiers.add(G.mqtt.topic);

    device["name"] = G.mqtt.clientId;
    device["sw_version"] = VERSION;
    device["configuration_url"] = "http://" + WiFi.localIP().toString();

    root["state_topic"] = std::string(G.mqtt.topic) + "/status";
    root["command_topic"] = std::string(G.mqtt.topic) + "/cmd";
    root["unique_id"] = WiFi.macAddress();
    root["plattform"] = "mqtt";

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

    char buffer[700];
    serializeJson(root, buffer);
    mqttClient.publish((std::string(HOMEASSISTANT_DISCOVERY_TOPIC) +
                        std::string("/light/") + std::string(G.mqtt.topic) +
                        std::string("/light/config"))
                           .c_str(),
                       buffer, true);
}

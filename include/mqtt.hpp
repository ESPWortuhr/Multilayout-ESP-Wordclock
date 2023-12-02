
#include "Uhr.h"
#include "mqtt.h"
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <WiFiClient.h>

#define HOMEASSISTANT_DISCOVERY_TOPIC "homeassistant"

extern WiFiClient client;

PubSubClient mqttClient(client);

//------------------------------------------------------------------------------

void Mqtt::init() {
    mqttClient.setServer(G.mqtt.serverAdress, G.mqtt.port);
    mqttClient.setCallback(callback);
    mqttClient.connect(G.mqtt.clientId, G.mqtt.user, G.mqtt.password);
    mqttClient.subscribe((std::string(G.mqtt.topic) + "/cmd").c_str());
}

//------------------------------------------------------------------------------

void Mqtt::reInit() {
    mqttClient.connect(G.mqtt.clientId, G.mqtt.user, G.mqtt.password);
    reconnect();
}

//------------------------------------------------------------------------------

bool Mqtt::getConnected() { return mqttClient.connected(); }

//------------------------------------------------------------------------------

void Mqtt::loop() {
    if (!mqttClient.connected()) {
        reconnect();
    }
    mqttClient.loop();
}

//------------------------------------------------------------------------------

void Mqtt::callback(char *topic, byte *payload, unsigned int length) {
    StaticJsonDocument<512> doc;

    Serial.print("Received message [");
    Serial.print(topic);
    Serial.print("] ");
    char msg[length + 1];
    for (uint32_t i = 0; i < length; i++) {
        Serial.print((char)payload[i]);
        msg[i] = (char)payload[i];
    }
    Serial.println();
    msg[length] = '\0';

    DeserializationError error = deserializeJson(doc, msg);

    if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.c_str());
        return;
    }

    if (doc.containsKey("state")) {
        if (!strcmp(doc["state"], "ON")) {
            G.state = true;
        } else if (!strcmp(doc["state"], "OFF")) {
            led.clear();
            led.show();
            G.state = false;
        }
        parametersChanged = true;
    }

    if (doc.containsKey("effect")) {
        if (!strcmp("Wordclock", doc["effect"])) {
            G.prog = COMMAND_MODE_WORD_CLOCK;
        } else if (!strcmp("Seconds", doc["effect"])) {
            G.prog = COMMAND_MODE_SECONDS;
        } else if (!strcmp("Digitalclock", doc["effect"])) {
            G.prog = COMMAND_MODE_DIGITAL_CLOCK;
        } else if (!strcmp("Scrollingtext", doc["effect"])) {
            G.prog = COMMAND_MODE_SCROLLINGTEXT;
        } else if (!strcmp("Rainbowcycle", doc["effect"])) {
            G.prog = COMMAND_MODE_RAINBOWCYCLE;
        } else if (!strcmp("Rainbow", doc["effect"])) {
            G.prog = COMMAND_MODE_RAINBOW;
        } else if (!strcmp("Color", doc["effect"])) {
            G.prog = COMMAND_MODE_COLOR;
        } else if (!strcmp("Symbol", doc["effect"])) {
            G.prog = COMMAND_MODE_SYMBOL;
        }
    }

    if (doc.containsKey("marquee_text")) {
        strcpy(G.scrollingText, doc["marquee_text"]);
    }

    if (doc.containsKey("color")) {
        G.color[Foreground] =
            HsbColor(float(doc["color"]["h"]) / 360.f,
                     float(doc["color"]["s"]) / 100.f, G.color[Foreground].B);
        parametersChanged = true;
    }

    if (doc.containsKey("brightness")) {
        G.color[Foreground] =
            HsbColor(G.color[Foreground].H, G.color[Foreground].S,
                     uint8_t(doc["brightness"]) / 255.f);
        parametersChanged = true;
    }
}

//------------------------------------------------------------------------------

void Mqtt::sendState() {
    StaticJsonDocument<200> doc;

    doc["state"] = (G.state) ? "ON" : "OFF";

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

    // MQTT discovery for Home Assistant
    // {
    //     "brightness": true,
    //     "color_mode": true,
    //     "supported_color_modes": [
    //         "hs"
    //     ],
    //     "schema": "json",
    //     "name": "ESP",
    //     "device": {
    //         "identifiers": [
    //             "ESPBuro"
    //         ],
    //         "name": "ESP",
    //         "sw_version": "3.3",
    //         "configuration_url": "http://<IP-Adress>"
    //     },
    //     "state_topic": "ESPBuro/status",
    //     "command_topic": "ESPBuro/cmd",
    //     "unique_id": "<MAC-Adress>",
    //     "plattform": "mqtt",
    //     "effect": true,
    //     "effect_list": [
    //         "Wordclock",
    //         "Seconds",
    //         "Digitalclock",
    //         "Scrollingtext",
    //         "Rainbowcycle",
    //         "Rainbow",
    //         "Color",
    //         "Symbol"
    //     ]
    // }

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

//------------------------------------------------------------------------------

void Mqtt::reconnect() {
    mqttClient.subscribe((std::string(G.mqtt.topic) + "/cmd").c_str());
    Serial.println("MQTT Connected...");
}

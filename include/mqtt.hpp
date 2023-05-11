
#include "Uhr.h"
#include "mqtt.h"
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <WiFiClient.h>

extern WiFiClient client;

PubSubClient mqttClient(client);

//------------------------------------------------------------------------------

void Mqtt::init() {
    mqttClient.setServer(G.mqtt.serverAdress, G.mqtt.port);
    mqttClient.setCallback(callback);
    mqttClient.connect(G.mqtt.clientId, G.mqtt.user, G.mqtt.password);
    mqttClient.subscribe(G.mqtt.topic);
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
    /*
    {
        "state": "on",
        "color": [0-255, 0-255, 0-255],
        "effect": "rainbow"
    }
    */
    StaticJsonDocument<200> doc;

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

    // Test if parsing succeeds.
    if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.c_str());
        return;
    }

    if (doc.containsKey("state")) {
        if (!strcmp("on", doc["state"])) {
            G.state = true;
        } else {
            led.clear();
            led.show();
            G.state = false;
        }
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
        }
    }

    if (doc.containsKey("marquee_text")) {
        strcpy(G.scrollingText, doc["marquee_text"]);
    }

    if (doc.containsKey("color")) {
        G.color[Foreground] =
            RgbColor(doc["color"][0], doc["color"][1], doc["color"][2]);
        G.color[Foreground].alpha = 1;
    }
}

//------------------------------------------------------------------------------

void Mqtt::reconnect() {
    mqttClient.subscribe(G.mqtt.topic);
    Serial.println("MQTT Connected...");
}

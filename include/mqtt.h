#pragma once

#include <Arduino.h>

class Mqtt {
private:
    void reInit();
    static void callback(char *topic, byte *payload, unsigned int length);
    static void processState(const JsonDocument &doc);
    static void processEffect(const JsonDocument &doc);
    static void processScrollingText(const JsonDocument &doc);
    static void processColor(const JsonDocument &doc);
    static void processBrightness(const JsonDocument &doc);

public:
    Mqtt() = default;
    ~Mqtt() = default;

    void init();
    void loop();
    void sendState();
    void sendDiscovery();

    bool isConnected();
};

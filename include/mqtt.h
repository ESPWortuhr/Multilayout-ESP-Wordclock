#pragma once

#include <Arduino.h>

class Mqtt {
private:
    void reInit();
    static void callback(char *topic, byte *payload, unsigned int length);

public:
    Mqtt() = default;
    ~Mqtt() = default;

    void init();
    void loop();
    void sendState();
    void sendDiscovery();

    bool isConnected();
};

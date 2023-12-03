#pragma once

#include <Arduino.h>

class Mqtt {
private:
    void reconnect();
    static void callback(char *topic, byte *payload, unsigned int length);

public:
    Mqtt() = default;
    ~Mqtt() = default;

    void init();
    void reInit();
    void loop();
    void sendState();
    void sendDiscovery();

    bool getConnected();
};

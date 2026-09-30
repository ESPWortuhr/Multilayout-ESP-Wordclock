#pragma once

#include "ClockWork.h"
#include "WordClockState.h"
#include <ArduinoJson.h>

class Mqtt {
private:
    ClockWork &clockWork;
    void reInit();
    static void callback(char *receivedTopic, byte *payload,
                         unsigned int length);
    static bool processState(const JsonDocument &doc);
    static bool processEffect(const JsonDocument &doc);
    static void processScrollingText(const JsonDocument &doc);
    static bool processColor(const JsonDocument &doc);
    static bool processBrightness(const JsonDocument &doc);
    static bool checkIfMqttUserIsEmpty();
    static const char *getEffectName();

public:
    Mqtt(ClockWork &cw);
    ~Mqtt();

    void init();
    void loop();
    void sendState();
    void sendDiscovery();

    bool isConnected();
};

extern Mqtt *mqttInstance;

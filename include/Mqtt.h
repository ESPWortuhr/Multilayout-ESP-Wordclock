#pragma once

#include "ClockWork.h"
#include "WordClockState.h"
#include <ArduinoJson.h>

class Mqtt {
private:
    ClockWork &clockWork;
    void reInit();
    static void callback(char *topic, byte *payload, unsigned int length);
    static void processState(const JsonDocument &doc);
    static void processEffect(const JsonDocument &doc);
    static bool processSymbol(const JsonDocument &doc);
    static void processScrollingText(const JsonDocument &doc);
    static void processColor(const JsonDocument &doc);
    static void processBrightness(const JsonDocument &doc);
    static void processSpeed(const JsonDocument &doc);
    static bool checkIfMqttUserIsEmpty();
    static const char *getEffectName();
    void sendSymbolCapabilities();

public:
    Mqtt(ClockWork &cw);
    ~Mqtt();

    void init();
    void loop();
    void sendState();
    void sendDiscovery();
    void sendCapabilities();

    bool isConnected();
};

extern Mqtt *mqttInstance;

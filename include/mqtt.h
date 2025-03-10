#pragma once

#include <ArduinoJson.h>
#include "Uhr.h"
#include "clockWork.h"

class Mqtt {
private:
    ClockWork &clockWork;  // Referenz auf ClockWork-Instanz
    void reInit();
    static void callback(char *topic, byte *payload, unsigned int length);
    static void processState(const JsonDocument &doc);
    static void processEffect(const JsonDocument &doc);
    static void processScrollingText(const JsonDocument &doc);
    static void processColor(const JsonDocument &doc);
    static void processBrightness(const JsonDocument &doc);
    static void processAutobrightSwitch(const JsonDocument &doc);
    static void processBrightOffset(const JsonDocument &doc);
    static void processBrightSlope(const JsonDocument &doc);
    static void processScrollSpeed(const JsonDocument &doc);
    static void processEffectSpeed(const JsonDocument &doc);
    static bool checkIfMqttUserIsEmpty();

public:
    Mqtt(ClockWork &cw);  // Nur Deklaration
    ~Mqtt();

    void init();
    void loop();
    void sendState();
    void sendDiscovery();
    bool isConnected();
};

// Globale Instanz
extern Mqtt *mqttInstance;

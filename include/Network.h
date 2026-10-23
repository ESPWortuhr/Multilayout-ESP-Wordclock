#pragma once

#include <Arduino.h> // String
#include <functional>
#include <time.h>

class Network {
public:
    using TimeHandler = std::function<void(time_t)>;
    using ColorHandler = std::function<void(uint8_t, uint8_t, uint8_t)>;
    using ColorProvider = std::function<uint32_t()>;

    Network(/* args */) = default;
    ~Network() = default;

    void info();
    int getQuality();
    void disable();
    void resetSettings();
    String getSSID();
    void rtcMode();
    void changeHostname(const char *hostname);
    void setPortalHandlers(TimeHandler setTime, ColorHandler setColor,
                           ColorProvider getColor);

    void setup(const char *hostname);
    void startConfigPortal();
    void loop();
};

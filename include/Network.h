#pragma once

#include <Arduino.h> // String
#include <functional>
#include <time.h>

class Network {
public:
    using TimeHandler = std::function<void(time_t)>;
    using ArgLookup = std::function<bool(const char *, String &)>;
    using SettingsRenderer = std::function<String()>;
    using SettingsApplier = std::function<void(const ArgLookup &)>;

    Network(/* args */) = default;
    ~Network() = default;

    void info();
    int getQuality();
    void disable();
    void resetSettings();
    String getSSID();
    void rtcMode();
    void changeHostname(const char *hostname);
    void setPortalHandlers(TimeHandler setTime, SettingsRenderer renderSettings,
                           SettingsApplier applySettings);

    void setup(const char *hostname);
    void startConfigPortal();
    void loop();
};

#pragma once

class Network {
public:
    Network(/* args */) = default;
    ~Network() = default;

    void info();
    int getQuality();
    void disable();
    void reboot();
    void resetSettings();
    String getSSID();
    void rtcMode();
    void changeHostname(const char *hostname);

    void setup(const char *hostname);
    void loop();
};

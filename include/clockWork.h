#pragma once

#include <NeoPixelBus.h>

class ClockWork {
private:
    uint8_t count_millis48 = 0;
    unsigned long previous48 = 0;
    const uint16_t interval48 = 1250; // 60000/48
    uint8_t lastSecond48 = 48;

private:
    void loopSecondsFrame();
    void loopWeather();
    void loopLdrLogic();

    void rainbow();
    void rainbowCycle();
    void scrollingText(const char *buf);

    bool changesInClockface();
    void copyClockface(const uint16_t source[], uint16_t destination[]);
    void calcClockface();

    void setClock();
    void setHour(const uint8_t std, const uint8_t voll);
    void setMinute(uint8_t min, uint8_t &offsetH, uint8_t &voll);
    void showMinute(uint8_t min);
    void showWeather();

public:
    ClockWork() = default;
    ~ClockWork() = default;

    iUhrType *getPointer(uint8_t type);

    void loop(struct tm &tm);

    void initBootLed();
    void initBootLedBlink();
    void initBootLedSweep(uint32_t delay);
    void initBootShowIp(const char *buf);
    void initBootWifiSignalStrength(int strength);

    void initLedStrip(uint8_t num);
};

#pragma once

#include <NeoPixelBus.h>

class ClockWork {
private:
    uint16_t countMillisSpeed = 0;
    uint32_t previousMillis = 0;

private:
    void loopLdrLogic();

    void rainbow();
    void rainbowCycle();
    void scrollingText(const char *buf);

    bool changesInClockface();
    void calcClockface();
    void countdownToMidnight();

    void setClock();
    void setHour(const uint8_t std, const bool fullHour);

    void setMinute(uint8_t min, uint8_t &offsetHour, bool &fullHour);
    void showMinute(uint8_t min);
    void showMinuteInWords(uint8_t min);
    uint8_t determineWhichMinuteVariant();
    FrontWord getFrontWordForNum(uint8_t min);

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

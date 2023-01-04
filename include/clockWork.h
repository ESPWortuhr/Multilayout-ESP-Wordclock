#pragma once

#include <NeoPixelBus.h>

class ClockWork {
private:
    inline void checkIfHueIsOutOfBound(float &hue) {
        if (hue > 360) {
            hue = 0;
        }
    }

    void ledSetPixel(uint8_t rr, uint8_t gg, uint8_t bb, uint8_t ww,
                     uint16_t i);
    void ledSetPixelHsb(uint16_t ledIndex, float hue, float sat, float bri,
                        uint8_t alpha);

    void ledSetPixelForChar(uint8_t col, uint8_t row, uint8_t offsetCol,
                            uint8_t offsetRow, unsigned char unsigned_d1);

    uint8_t setBrightnessAuto(uint8_t val);
    void setBrightnessLdr(uint8_t &rr, uint8_t &gg, uint8_t &bb, uint8_t &ww,
                          uint8_t position);
    void setBrightness(uint8_t &rr, uint8_t &gg, uint8_t &bb, uint8_t &ww,
                       uint8_t position, uint8_t percentage);

    inline void ledClearClock();
    inline void ledClearPixel(uint16_t i);
    inline void ledClearRow(uint8_t row);
    inline void ledClearFrontExeptofFontspace(uint8_t offsetRow);
    inline void ledClearFrame();
    void ledShiftColumnToRight();
    void ledShowSeconds();
    void ledShowNumbers(const char d1, const char d2);
    void ledSet(bool changed = false);
    void ledSetColor();
    void ledSetIcon(uint8_t num_icon, uint8_t brightness, bool rgb_icon);
    void ledSetFrameColor();

    void ledClear();
    void ledSingle(uint8_t wait);
    void ledSetAllPixels(uint8_t rr, uint8_t gg, uint8_t bb, uint8_t ww);

    void loopSecondsFrame();
    void loopWeather();
    void loopLdrLogic();

    void rainbow();
    void rainbowCycle();
    void scrollingText(const char *buf);

    bool changesInClockface();
    void copyClockface(const uint16_t source[], uint16_t destination[]);
    void calcClockFace();

    void clockSetHour(const uint8_t std, const uint8_t voll);
    void clockSetMinute(uint8_t min, uint8_t &offsetH, uint8_t &voll);
    void clockShowMinute(uint8_t min);
    void clockSetClock();
    void clockShowWeather();

public:
    ClockWork() = default;
    ~ClockWork() = default;

    iUhrType *getPointer(uint8_t num);
    void initLedStrip(uint8_t num);

    void ledShow();

    RgbColor ledGetPixel(uint16_t i);
    RgbwColor ledGetPixelRgbw(uint16_t i);

    void ledSetPixelColorObject(uint16_t i, RgbColor color);
    void ledSetPixelColorObjectRgbw(uint16_t i, RgbwColor color);

    void loop(struct tm &tm);

    void initBootLed();
    void initBootLedBlink();
    void initBootLedSweep();
    void initBootShowIp(const char *buf);
    void initBootWifiSignalStrength(int strength);
};

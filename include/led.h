#pragma once

#include <NeoPixelBus.h>

class Led {
private:
public:
    Led(/* args */) = default;
    ~Led() = default;

    inline void checkIfHueIsOutOfBound(float &hue);

    void set(bool changed = false);
    void setColor();
    void setIcon(uint8_t num_icon, uint8_t brightness, bool rgb_icon = false);
    void setFrameColor();
    void setSingle(uint8_t wait);
    void setAllPixels(uint8_t rr, uint8_t gg, uint8_t bb, uint8_t ww);
    void setPixelColorObject(uint16_t i, RgbColor color);
    void setPixelColorObject(uint16_t i, RgbwColor color);
    void setPixel(uint8_t rr, uint8_t gg, uint8_t bb, uint8_t ww, uint16_t i);
    void setPixelHsb(uint16_t ledIndex, float hue, float sat, float bri,
                     uint8_t alpha = 0);

    void setPixelForChar(uint8_t col, uint8_t row, uint8_t offsetCol,
                         uint8_t offsetRow, unsigned char unsigned_d1);

    uint8_t setBrightnessAuto(uint8_t val);
    void setBrightnessLdr(uint8_t &rr, uint8_t &gg, uint8_t &bb, uint8_t &ww,
                          uint8_t position);
    void setBrightness(uint8_t &rr, uint8_t &gg, uint8_t &bb, uint8_t &ww,
                       uint8_t position, uint8_t percentage);

    inline void clear();
    inline void clearFrame();
    inline void clearPixel(uint16_t i);
    inline void clearFrontExeptofFontspace(uint8_t offsetRow);
    inline void clearClock();
    inline void clearRow(uint8_t row);

    void shiftColumnToRight();

    void show();
    void showSeconds();
    void showNumbers(const char d1, const char d2);

    RgbColor getPixel(uint16_t i);
    RgbwColor getPixelRgbw(uint16_t i);
};

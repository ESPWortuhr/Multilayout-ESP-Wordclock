#pragma once

#include <NeoPixelBus.h>

class Led {
private:
    //------------------------------------------------------------------------------
    // Helper Functions
    //------------------------------------------------------------------------------
    inline uint8_t reverse8BitOrder(uint8_t x);

public:
    Led(/* args */) = default;
    ~Led() = default;

    //------------------------------------------------------------------------------
    // Helper Functions
    //------------------------------------------------------------------------------
    inline void checkIfHueIsOutOfBound(float &hue);

    //------------------------------------------------------------------------------
    // Manipulate Functions
    //------------------------------------------------------------------------------
    inline void mirrorMinuteArrayVertical();

    //------------------------------------------------------------------------------
    // Brightness Functions
    //------------------------------------------------------------------------------
    uint8_t setBrightnessAuto(uint8_t val);
    void setBrightnessLdr(uint8_t &rr, uint8_t &gg, uint8_t &bb, uint8_t &ww,
                          uint8_t position);
    void setBrightness(uint8_t &rr, uint8_t &gg, uint8_t &bb, uint8_t &ww,
                       uint8_t position, uint8_t percentage);
    void shiftColumnToRight();

    //------------------------------------------------------------------------------
    // Pixel set Functions
    //------------------------------------------------------------------------------
    void setPixel(uint8_t rr, uint8_t gg, uint8_t bb, uint8_t ww, uint16_t i);
    void setPixelHsb(uint16_t ledIndex, float hue, float sat, float bri,
                     uint8_t alpha = 0);
    void setPixelColorObject(uint16_t i, RgbColor color);
    void setPixelColorObject(uint16_t i, RgbwColor color);
    void setbyFrontMatrix(uint8_t ColorPosition);
    void setIcon(uint8_t num_icon, uint8_t brightness, bool rgb_icon = false);
    void setSingle(uint8_t wait);
    void setFrameColor();
    void setPixelForChar(uint8_t col, uint8_t row, uint8_t offsetCol,
                         uint8_t offsetRow, unsigned char unsigned_d1);
    void set(bool changed = false);

    //------------------------------------------------------------------------------
    // Pixel get Functions
    //------------------------------------------------------------------------------
    RgbColor getPixel(uint16_t i);
    RgbwColor getPixelRgbw(uint16_t i);

    //------------------------------------------------------------------------------
    // Pixel Clear Functions
    //------------------------------------------------------------------------------
    inline void clearPixel(uint16_t i);
    inline void clearClock();
    inline void clearRow(uint8_t row);
    inline void clearMinArray();
    inline void clearFrontExeptofFontspace(uint8_t offsetRow);
    inline void clearFrame();
    inline void clear();

    //------------------------------------------------------------------------------
    // Show Functions
    //------------------------------------------------------------------------------
    void showNumbers(const char d1, const char d2);
    void showMinutes();
    void showSeconds();
    void show();
};

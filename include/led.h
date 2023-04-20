#pragma once

#include <NeoPixelBus.h>

class Led {
private:
    //------------------------------------------------------------------------------
    // Helper Functions
    //------------------------------------------------------------------------------
    inline uint8_t reverse8BitOrder(uint8_t x);
    inline uint32_t reverse32BitOrder(uint32_t x);
    bool getCharCol(fontSize font, uint8_t col, uint8_t row,
                    unsigned char unsigned_d1);

public:
    Led(/* args */) = default;
    ~Led() = default;

    //------------------------------------------------------------------------------
    // Helper Functions
    //------------------------------------------------------------------------------
    inline void checkIfHueIsOutOfBound(uint16_t &hue);

    //------------------------------------------------------------------------------
    // Manipulate Functions
    //------------------------------------------------------------------------------
    inline void mirrorMinuteArrayVertical();
    inline void mirrorFrontMatrixVertical();
    inline void mirrorFrontMatrixHorizontal();

    //------------------------------------------------------------------------------
    // Brightness Functions
    //------------------------------------------------------------------------------
    float setBrightnessAuto(float val);
    void getCurrentManualBrightnessSetting(uint8_t &currentBrightness);
    void getColorbyPositionWithAppliedBrightness(Color &color,
                                                 uint8_t position);
    void shiftColumnToRight();

    //------------------------------------------------------------------------------
    // Pixel set Functions
    //------------------------------------------------------------------------------
    void setPixel(uint16_t ledIndex, Color color);
    void setbyFrontMatrix(uint8_t ColorPosition);
    void setbyMinuteArray(uint8_t ColorPosition);
    void setbySecondArray(uint8_t ColorPosition);
    void setIcon(uint8_t num_icon, uint8_t brightness);
    void setSingle(uint8_t wait);
    void setPixelForChar(uint8_t col, uint8_t row, uint8_t offsetCol,
                         uint8_t offsetRow, unsigned char unsigned_d1,
                         fontSize font);
    void set(bool changed = false);

    //------------------------------------------------------------------------------
    // Pixel get Functions
    //------------------------------------------------------------------------------
    HsbColor getPixel(uint16_t i);

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
    void showDigitalClock(const char min1, const char min0, const char h1,
                          const char h0);
    void show();
};

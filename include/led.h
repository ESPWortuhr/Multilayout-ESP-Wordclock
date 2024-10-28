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
    void applyMirroringAndReverseIfDefined();
    fontSize determineFontSize();
    void setupDigitalClock(fontSize &usedFontSize, uint8_t &offsetLetterH0,
                           uint8_t &offsetLetterH1, uint8_t &offsetLetterMin0,
                           uint8_t &offsetLetterMin1, uint8_t &offsetRow0,
                           uint8_t &offsetRow1);
    void toggleDigitalClockSecond(const fontSize &usedFontSize,
                                  const uint8_t &offsetRow1,
                                  const uint8_t &offsetMin0);

public:
    Led(/* args */) = default;
    ~Led() = default;

    //------------------------------------------------------------------------------
    // Helper Functions
    //------------------------------------------------------------------------------
    inline void checkIfHueIsOutOfBound(uint16_t &hue);
    void resetFrontMatrixBuffer();

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
    void getColorbyPositionWithAppliedBrightness(HsbColor &color,
                                                 ColorPosition position);
    void shiftColumnToRight();

    //------------------------------------------------------------------------------
    // Pixel set Functions
    //------------------------------------------------------------------------------
    void setState(const bool newState);
    void setPixel(uint16_t ledIndex, HsbColor color);
    void setPixel(uint8_t row, uint8_t col, HsbColor color);
    void setbyFrontMatrix(ColorPosition position = Foreground,
                          bool applyMirrorAndReverse = true);
    void setbyFrontMatrix(HsbColor color, bool applyMirrorAndReverse = true);
    void setbyMinuteArray(ColorPosition position = Foreground);
    void setbySecondArray(ColorPosition position = Foreground);
    void setIcon(uint8_t iconNum);
    void setSingle(uint8_t wait);
    void setPixelForChar(uint8_t col, uint8_t row, uint8_t offsetCol,
                         uint8_t offsetRow, unsigned char unsigned_d1,
                         fontSize font);
    void set(WordclockChanges changed = WordclockChanges::Parameters);

    //------------------------------------------------------------------------------
    // Pixel get Functions
    //------------------------------------------------------------------------------
    bool getState();
    RgbColor getPixel(uint16_t i);

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
                          const char h0, bool parametersChanged);
    void show();
};

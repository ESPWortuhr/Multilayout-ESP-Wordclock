#pragma once

#include "WordClockState.h"
#include <NeoPixelBus.h>

class Led {
private:
    //------------------------------------------------------------------------------
    // Helper Functions
    //------------------------------------------------------------------------------
    uint8_t reverse8BitOrder(uint8_t x);
    uint32_t reverse32BitOrder(uint32_t x);
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
    void checkIfHueIsOutOfBound(uint16_t &hue);
    void resetFrontMatrixBuffer();
    bool getCharCol(fontSize font, uint8_t col, uint8_t row,
                    unsigned char unsigned_d1);

    //------------------------------------------------------------------------------
    // Manipulate Functions
    //------------------------------------------------------------------------------
    void mirrorMinuteArrayVertical();
    void mirrorFrontMatrixVertical();
    void mirrorFrontMatrixHorizontal();

    //------------------------------------------------------------------------------
    // Brightness Functions
    //------------------------------------------------------------------------------
    float setBrightnessAuto(float val);
    uint8_t getCurrentManualBrightnessSetting();
    HsbColor getColorbyPositionWithAppliedBrightness(ColorPosition position);
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
    void setBitmapSymbol(BitmapSymbol symbolNum, HsbColor color);
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
    void clearPixel(uint16_t i);
    void clearPixel(uint8_t row, uint8_t col);
    void clearClock();
    void clearRow(uint8_t row);
    void clearMinArray();
    void clearFrontExeptofFontspace(uint8_t offsetRow);
    void clearFrame();
    void clear();

    //------------------------------------------------------------------------------
    // Show Functions
    //------------------------------------------------------------------------------
    void showNumbers(const char d1, const char d2);
    void showDigitalClock(const char min1, const char min0, const char h1,
                          const char h0, bool parametersChanged);
    void show();
};

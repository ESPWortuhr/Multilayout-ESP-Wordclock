#include "Animation.h"
#include "NeoMultiFeature.hpp"
#include "Uhr.h"
#include "Uhrtypes/Uhrtype.hpp"
#include "font.h"
#include "icons.h"
#include "led.h"
#include <Arduino.h>

extern NeoPixelBus<NeoMultiFeature, Neo800KbpsMethod> *strip_RGB;
extern NeoPixelBus<NeoGrbwFeature, Neo800KbpsMethod> *strip_RGBW;

extern iUhrType *usedUhrType;
extern Animation *animation;

//------------------------------------------------------------------------------
// Helper Functions
//------------------------------------------------------------------------------

/* Based on https://graphics.stanford.edu/~seander/bithacks.html */

inline uint8_t Led::reverse8BitOrder(uint8_t x) {
    return (x * 0x0202020202ULL & 0x010884422010ULL) % 1023;
}

//------------------------------------------------------------------------------

/* Based on https://graphics.stanford.edu/~seander/bithacks.html */

inline uint32_t Led::reverse32BitOrder(uint32_t x) {
    x = (((x & 0xaaaaaaaa) >> 1) | ((x & 0x55555555) << 1));
    x = (((x & 0xcccccccc) >> 2) | ((x & 0x33333333) << 2));
    x = (((x & 0xf0f0f0f0) >> 4) | ((x & 0x0f0f0f0f) << 4));
    x = (((x & 0xff00ff00) >> 8) | ((x & 0x00ff00ff) << 8));
    return ((x >> 16) | (x << 16));
}

//------------------------------------------------------------------------------

inline void Led::checkIfHueIsOutOfBound(uint16_t &hue) {
    if (hue > 360) {
        hue = 0;
    }
}

//------------------------------------------------------------------------------

bool Led::getCharCol(fontSize font, uint8_t col, uint8_t row,
                     unsigned char uChar) {
    switch (font) {
    case normalSizeASCII:
        return pgm_read_byte(&(font_7x5[uChar][col])) & (1u << row);
        break;

    case smallSizeNumbers:
        return pgm_read_byte(&(font_5x3[uChar][col])) & (1u << row);
        break;

    default:
        return false;
        break;
    }
}

//------------------------------------------------------------------------------

void Led::applyMirroringAndReverseIfDefined() {
    if (G.layoutVariant[ReverseMinDirection]) {
        led.mirrorMinuteArrayVertical();
    }
    if (G.layoutVariant[MirrorVertical]) {
        led.mirrorFrontMatrixVertical();
    }
    if (G.layoutVariant[MirrorHorizontal]) {
        led.mirrorFrontMatrixHorizontal();
    }
}

//------------------------------------------------------------------------------
// Brightness Functions
//------------------------------------------------------------------------------

float Led::setBrightnessAuto(float val) {
    // G.hh contains time-dependent brightness values in %.
    return (val * ldrVal) / 100.f;
}

//------------------------------------------------------------------------------

void Led::getCurrentManualBrightnessSetting(uint8_t &currentBrightness) {
    // Set Brighness hour dependent
    if (_hour < 6) {
        currentBrightness = G.h24;
    } else if (_hour < 8) {
        currentBrightness = G.h6;
    } else if (_hour < 12) {
        currentBrightness = G.h8;
    } else if (_hour < 16) {
        currentBrightness = G.h12;
    } else if (_hour < 18) {
        currentBrightness = G.h16;
    } else if (_hour < 20) {
        currentBrightness = G.h18;
    } else if (_hour < 22) {
        currentBrightness = G.h20;
    } else if (_hour < 24) {
        currentBrightness = G.h22;
    }
}

//------------------------------------------------------------------------------

void Led::getColorbyPositionWithAppliedBrightness(Color &color,
                                                  uint8_t colorPosition) {
    color = G.color[colorPosition];
    uint8_t manBrightnessSetting = 100;
    getCurrentManualBrightnessSetting(manBrightnessSetting);

    if (G.autoLdrEnabled) {
        color.hsb.B = setBrightnessAuto(color.hsb.B);
        color.alpha = setBrightnessAuto(color.alpha);
    } else {
        color.hsb.B *= manBrightnessSetting / 100.f;
        color.alpha *= manBrightnessSetting / 100.f;
    }
}

//------------------------------------------------------------------------------
// Manipulate Functions
//------------------------------------------------------------------------------

inline void Led::mirrorMinuteArrayVertical() {
    minuteArray = reverse8BitOrder(minuteArray);
    minuteArray >>= 4;
}

//------------------------------------------------------------------------------

inline void Led::mirrorFrontMatrixVertical() {
    for (uint8_t row = 0; row < usedUhrType->rowsWordMatrix(); row++) {
        frontMatrix[row] = reverse32BitOrder(frontMatrix[row]);
        frontMatrix[row] >>= (32 - usedUhrType->colsWordMatrix());
    }
}

//------------------------------------------------------------------------------

inline void Led::mirrorFrontMatrixHorizontal() {
    uint32_t tempMatrix[MAX_ROW_SIZE] = {0};
    memcpy(&tempMatrix, &frontMatrix, sizeof tempMatrix);
    for (uint8_t row = 0; row < usedUhrType->rowsWordMatrix(); row++) {
        frontMatrix[row] = tempMatrix[usedUhrType->rowsWordMatrix() - row - 1];
    }
}

//------------------------------------------------------------------------------

void Led::shiftColumnToRight() {
    for (uint8_t row = 0; row < MAX_ROW_SIZE; row++) {
        frontMatrix[row] <<= 1;
    }
}

//------------------------------------------------------------------------------
// Pixel set Functions
//------------------------------------------------------------------------------

void Led::setPixel(uint16_t ledIndex, Color color) {
    if (G.Colortype == Grbw) {
        RgbColor rgbColor = RgbColor(color.hsb);

        strip_RGBW->SetPixelColor(ledIndex, RgbwColor(rgbColor.R, rgbColor.G,
                                                      rgbColor.B, color.alpha));
    } else {
        strip_RGB->SetPixelColor(ledIndex, color.hsb);
    }
}

//------------------------------------------------------------------------------

void Led::setPixel(uint8_t row, uint8_t col, Color color) {
    if (G.Colortype == Grbw) {
        RgbColor rgbColor = RgbColor(color.hsb);

        strip_RGBW->SetPixelColor(
            usedUhrType->getFrontMatrixIndex(row, col),
            RgbwColor(rgbColor.R, rgbColor.G, rgbColor.B, color.alpha));
    } else {
        strip_RGB->SetPixelColor(usedUhrType->getFrontMatrixIndex(row, col),
                                 color.hsb);
    }
}

//------------------------------------------------------------------------------

void Led::setbyFrontMatrix(uint8_t colorPosition, bool applyMirrorAndReverse) {
    if (applyMirrorAndReverse) {
        applyMirroringAndReverseIfDefined();
    }
    Color displayedColor;
    getColorbyPositionWithAppliedBrightness(displayedColor, colorPosition);

    for (uint8_t row = 0; row < usedUhrType->rowsWordMatrix(); row++) {
        for (uint8_t col = 0; col < usedUhrType->colsWordMatrix(); col++) {

            bool boolSetPixel = usedUhrType->getFrontMatrixPixel(row, col);
            if (colorPosition == Background) {
                boolSetPixel = !boolSetPixel;
            }

            if (boolSetPixel) {
                setPixel(row, col, displayedColor);
            } else if (colorPosition != Background) {
                clearPixel(usedUhrType->getFrontMatrixIndex(row, col));
            }
        }
    }
}

//------------------------------------------------------------------------------

void Led::setbyMinuteArray(uint8_t colorPosition) {
    Color displayedColor;
    getColorbyPositionWithAppliedBrightness(displayedColor, colorPosition);

    /* Set minutes According to minute byte */
    for (uint8_t i = 0; i < 4; i++) {
        /* Bitwise check whether Pixel bit is set */
        if ((minuteArray >> i) & 1U) {
            setPixel(minutePixelArray[i], displayedColor);
        }
    }
}

//------------------------------------------------------------------------------

void Led::setbySecondArray(uint8_t colorPosition) {
    Color displayedColor;
    getColorbyPositionWithAppliedBrightness(displayedColor, colorPosition);

    const uint8_t offesetSecondsFrame = 5;
    for (uint8_t i = 0; i < usedUhrType->numPixelsFrameMatrix(); i++) {
        if ((frameArray >> i) & 1U) {
            if (i < usedUhrType->numPixelsFrameMatrix() - offesetSecondsFrame) {
                setPixel(usedUhrType->getFrameMatrixIndex(i) +
                             offesetSecondsFrame,
                         displayedColor);
            } else {
                setPixel(usedUhrType->getFrameMatrixIndex(i) -
                             usedUhrType->numPixelsFrameMatrix() +
                             offesetSecondsFrame,
                         displayedColor);
            }
        }
    }
}

//------------------------------------------------------------------------------

void Led::setIcon(uint8_t num_icon, uint8_t brightness = 100) {
    for (uint8_t col = 0; col < GRAFIK_11X10_COLS; col++) {
        for (uint8_t row = 0; row < GRAFIK_11X10_ROWS; row++) {
            if (pgm_read_word(&(grafik_11x10[num_icon][row])) &
                (1 << (GRAFIK_11X10_COLS - 1 - col))) {
                usedUhrType->setFrontMatrixPixel(row, col);
            } else {
                usedUhrType->setFrontMatrixPixel(row, col, false);
            }
        }
    }
    setbyFrontMatrix(Foreground);
    show();
}

//------------------------------------------------------------------------------

void Led::setSingle(uint8_t wait) {
    uint16_t hue;

    for (uint16_t i = 0; i < usedUhrType->numPixels(); i++) {
        hue = 360.f * i / (usedUhrType->numPixels() - 1);
        hue = hue + 360.f / usedUhrType->numPixels();
        checkIfHueIsOutOfBound(hue);

        clear();
        setPixel(i, HsbColor(hue / 360.f, 1.f, 1.f));
        show();
        delay(wait);
    }
}

//------------------------------------------------------------------------------

void Led::setPixelForChar(uint8_t col, uint8_t row, uint8_t offsetCol,
                          uint8_t offsetRow, unsigned char unsigned_d1,
                          fontSize font = normalSizeASCII) {

    if (getCharCol(font, col, row, unsigned_d1)) {
        usedUhrType->setFrontMatrixPixel(row + offsetRow, col + offsetCol);
    }
}

//------------------------------------------------------------------------------

void Led::set(bool changed) {
    setbyFrontMatrix(Foreground);
    setbyFrontMatrix(Background, false);

    if (G.minuteVariant != MinuteVariant::Off) {
        setbyMinuteArray(Foreground);
    }

    if (G.secondVariant != SecondVariant::Off) {
        setbySecondArray(Foreground);
    }

    if (animation->led_show_notify(changed, _minute)) {
        show();
    }
}

//------------------------------------------------------------------------------
// Pixel get Functions
//------------------------------------------------------------------------------

RgbColor Led::getPixel(uint16_t i) {
    if (G.Colortype == Grbw) {
        return RgbColor(strip_RGBW->GetPixelColor(i));
    }
    return strip_RGB->GetPixelColor(i);
}

//------------------------------------------------------------------------------
// Pixel Clear Functions
//------------------------------------------------------------------------------

inline void Led::clearPixel(uint16_t i) {
    if (G.Colortype == Grbw) {
        strip_RGBW->SetPixelColor(i, 0);
    } else {
        strip_RGB->SetPixelColor(i, 0);
    }
}

//------------------------------------------------------------------------------

inline void Led::clearClock() {
    for (uint8_t row = 0; row < usedUhrType->rowsWordMatrix(); row++) {
        for (uint8_t col = 0; col < usedUhrType->colsWordMatrix(); col++) {
            usedUhrType->setFrontMatrixPixel(row, col, false);
            clearPixel(usedUhrType->getFrontMatrixIndex(row, col));
        }
    }
}

//------------------------------------------------------------------------------

inline void Led::clearRow(uint8_t row) {
    for (uint8_t col = 0; col < usedUhrType->colsWordMatrix(); col++) {
        usedUhrType->setFrontMatrixPixel(row, col, false);
        clearPixel(usedUhrType->getFrontMatrixIndex(row, col));
    }
}

//------------------------------------------------------------------------------

inline void Led::clearMinArray() {
    uint16_t numPixelsWordMatrix =
        usedUhrType->rowsWordMatrix() * usedUhrType->colsWordMatrix();
    for (uint16_t i = numPixelsWordMatrix;
         i < usedUhrType->numPixels() - usedUhrType->numPixelsFrameMatrix();
         i++) {
        clearPixel(i);
    }
    minuteArray = 0;
}

//------------------------------------------------------------------------------

inline void Led::clearFrontExeptofFontspace(uint8_t offsetRow) {
    for (uint8_t i = 0; i < offsetRow; i++) {
        clearRow(i);
    }

    for (uint8_t i = usedUhrType->rowsWordMatrix();
         i > offsetRow + fontHeight[normalSizeASCII]; i--) {
        clearRow(i - 1);
    }
}

//------------------------------------------------------------------------------

inline void Led::clearFrame() {
    for (uint16_t i = 0; i < usedUhrType->numPixelsFrameMatrix(); i++) {
        clearPixel(usedUhrType->getFrameMatrixIndex(i));
    }
}

//------------------------------------------------------------------------------

inline void Led::clear() {
    clearClock();
    clearFrame();
    clearMinArray();
}

//------------------------------------------------------------------------------
// Show Functions
//------------------------------------------------------------------------------

void Led::showNumbers(const char d1, const char d2) {
    clearClock();
    static uint8_t offsetLetter0 = 0;
    static uint8_t offsetLetter1 = fontWidth[normalSizeASCII] + 1;
    uint8_t offsetRow =
        (usedUhrType->rowsWordMatrix() - fontHeight[normalSizeASCII]) / 2;

    if (usedUhrType->has24HourLayout()) {
        offsetLetter0 = 3;
        offsetLetter1 = fontWidth[normalSizeASCII] + 4;
    }

    for (uint8_t col = 0; col < fontWidth[normalSizeASCII]; col++) {
        for (uint8_t row = 0; row < fontHeight[normalSizeASCII]; row++) {
            // 1. Number without Offset
            setPixelForChar(col, row, offsetLetter0, offsetRow,
                            static_cast<unsigned char>(d1));
            // 2. Number with Offset
            setPixelForChar(col, row, offsetLetter1, offsetRow,
                            static_cast<unsigned char>(d2));
        }
    }

    mirrorFrontMatrixVertical(); // Needed for correct displaying of Chars
    setbyFrontMatrix(Effect);
    show();
}

//------------------------------------------------------------------------------

void Led::showDigitalClock(const char min1, const char min0, const char h1,
                           const char h0) {
    // 1st Row of letters vertical Offset
    static uint8_t offsetLetterH0 =
        usedUhrType->colsWordMatrix() / 2 - fontWidth[smallSizeNumbers] - 1;
    static uint8_t offsetLetterH1 =
        offsetLetterH0 + fontWidth[smallSizeNumbers] + 2;

    // 2nd Row of letters vertical Offset
    static uint8_t offsetLetterMin0 = 3;
    static uint8_t offsetLetterMin1 =
        offsetLetterMin0 + fontWidth[smallSizeNumbers] + 2;

    // 1st Row of letters horizontal Offset
    uint8_t offsetRow0 =
        usedUhrType->rowsWordMatrix() / 2 - fontHeight[smallSizeNumbers];
    // 2nd Row of letters horizontal Offset
    uint8_t offsetRow1 = offsetRow0 + fontHeight[smallSizeNumbers];

    // Horizontal offset +1 for clocks > 10 Rows
    if (usedUhrType->rowsWordMatrix() > 10) {
        offsetRow1++;
    }

    // Toggle second dots every second
    if (_second % 2) {
        usedUhrType->setFrontMatrixPixel(offsetRow1 + 1, 1);
        usedUhrType->setFrontMatrixPixel(offsetRow1 + 3, 1);
    }

    for (uint8_t col = 0; col < 3; col++) {
        for (uint8_t row = 0; row < 5; row++) {
            // 1st Row
            setPixelForChar(col, row, offsetLetterH1, offsetRow0,
                            static_cast<unsigned char>(h1), smallSizeNumbers);
            setPixelForChar(col, row, offsetLetterH0, offsetRow0,
                            static_cast<unsigned char>(h0), smallSizeNumbers);
            // 2nd Row
            setPixelForChar(col, row, offsetLetterMin1, offsetRow1,
                            static_cast<unsigned char>(min1), smallSizeNumbers);
            setPixelForChar(col, row, offsetLetterMin0, offsetRow1,
                            static_cast<unsigned char>(min0), smallSizeNumbers);
        }
    }

    mirrorFrontMatrixVertical(); // Needed for correct displaying of Chars
    setbyFrontMatrix(Effect);
    show();
}

//------------------------------------------------------------------------------

void Led::show() {
    if (G.state) {
        if (G.Colortype == Grbw) {
            strip_RGBW->Show();
        } else {
            strip_RGB->Show();
        }
    }
}

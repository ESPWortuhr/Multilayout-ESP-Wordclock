#include "NeoMultiFeature.hpp"
#include "Transitiontypes/Transition.h"
#include "Uhr.h"
#include "Uhrtypes/Uhrtype.hpp"
#include "font.h"
#include "icons.h"
#include "led.h"
#include <Arduino.h>

extern iUhrType *usedUhrType;
extern Transition *transition;

const uint8_t whiteAdjR[3] = {255, 255, 255};
const uint8_t whiteAdjG[3] = {180, 215, 107};
const uint8_t whiteAdjB[3] = {107, 177, 253};

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
        mirrorMinuteArrayVertical();
    }
    if (G.layoutVariant[MirrorVertical]) {
        mirrorFrontMatrixVertical();
    }
    if (G.layoutVariant[MirrorHorizontal]) {
        mirrorFrontMatrixHorizontal();
    }
}

//------------------------------------------------------------------------------

RgbwColor convertRgbToRgbw(RgbColor light, WhiteType wType) {
    RgbwColor returnColor;

    // These values are what the 'white' value would need to
    // be to get the corresponding color value.
    float whiteValueR =
        light.R * 255.0 / whiteAdjR[static_cast<uint8_t>(wType)];
    float whiteValueG =
        light.G * 255.0 / whiteAdjG[static_cast<uint8_t>(wType)];
    float whiteValueB =
        light.B * 255.0 / whiteAdjB[static_cast<uint8_t>(wType)];

    // Set the white value to the highest it can be for the given color
    // (without over saturating any channel - thus the minimum of them).
    float minValW = min(whiteValueR, min(whiteValueG, whiteValueB));
    returnColor.W = (minValW <= 255 ? (uint8_t)minValW : 255);

    // The rest of the channels will just be the original value minus the
    // contribution by the white channel.
    returnColor.R = static_cast<uint8_t>(
        light.R - minValW * whiteAdjR[static_cast<uint8_t>(wType)] / 255);
    returnColor.G = static_cast<uint8_t>(
        light.G - minValW * whiteAdjG[static_cast<uint8_t>(wType)] / 255);
    returnColor.B = static_cast<uint8_t>(
        light.B - minValW * whiteAdjB[static_cast<uint8_t>(wType)] / 255);

    return returnColor;
}

//------------------------------------------------------------------------------

void Led::resetFrontMatrixBuffer() {
    for (uint8_t i = 0; i < usedUhrType->rowsWordMatrix(); i++) {
        frontMatrix[i] = 0;
    }
}

//------------------------------------------------------------------------------
// Brightness Functions
//------------------------------------------------------------------------------

float Led::setBrightnessAuto(float val) {
    // G.hh contains time-dependent brightness values in %.
    return (val * ledGain) / 100.f;
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

void Led::getColorbyPositionWithAppliedBrightness(HsbColor &color,
                                                  ColorPosition position) {
    color = G.color[position];
    uint8_t manBrightnessSetting = 100;
    getCurrentManualBrightnessSetting(manBrightnessSetting);

    if (G.autoBrightEnabled) {
        color.B = setBrightnessAuto(color.B);
    } else {
        color.B *= manBrightnessSetting / 100.f;
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

void Led::setState(bool newState) {
    static uint8_t oldBrightness[3];

    if (newState) {
        for (uint8_t i = 0; i < 3; i++) {
            G.color[i].B = oldBrightness[i] / 100.f;
        }
    } else {
        for (uint8_t i = 0; i < 3; i++) {
            led.clear();
            led.show();
            oldBrightness[i] = G.color[i].B * 100;
            G.color[i].B = 0;
        }
    }

    parametersChanged = true;
}

//------------------------------------------------------------------------------

void Led::setPixel(uint16_t ledIndex, HsbColor color) {
    if (G.Colortype == Grbw) {
        strip_RGBW->SetPixelColor(ledIndex,
                                  convertRgbToRgbw(RgbColor(color), G.wType));
    } else {
        strip_RGB->SetPixelColor(ledIndex, color);
    }
}

//------------------------------------------------------------------------------

void Led::setPixel(uint8_t row, uint8_t col, HsbColor color) {
    setPixel(usedUhrType->getFrontMatrixIndex(row, col), color);
}

//------------------------------------------------------------------------------

void Led::setbyFrontMatrix(ColorPosition colorPosition,
                           bool applyMirrorAndReverse) {
    if (applyMirrorAndReverse) {
        applyMirroringAndReverseIfDefined();
    }
    HsbColor displayedColor;
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

void Led::setbyFrontMatrix(HsbColor color, bool applyMirrorAndReverse) {
    if (applyMirrorAndReverse) {
        applyMirroringAndReverseIfDefined();
    }

    for (uint8_t row = 0; row < usedUhrType->rowsWordMatrix(); row++) {
        for (uint8_t col = 0; col < usedUhrType->colsWordMatrix(); col++) {
            bool boolSetPixel = usedUhrType->getFrontMatrixPixel(row, col);

            if (boolSetPixel) {
                setPixel(row, col, color);
            }
        }
    }
}

//------------------------------------------------------------------------------

void Led::setbyMinuteArray(ColorPosition colorPosition) {
    HsbColor displayedColor;
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

void Led::setbySecondArray(ColorPosition colorPosition) {
    HsbColor displayedColor;
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

void Led::setIcon(uint8_t iconNum) {
    resetFrontMatrixBuffer();
    uint8_t offsetCol = (usedUhrType->colsWordMatrix() - GRAFIK_11X10_COLS) / 2;

    for (uint8_t col = 0; col < GRAFIK_11X10_COLS; col++) {
        for (uint8_t row = 0; row < GRAFIK_11X10_ROWS; row++) {
            if (pgm_read_word(&(grafik_11x10[iconNum][row])) &
                (1 << (GRAFIK_11X10_COLS - col - 1))) {
                usedUhrType->setFrontMatrixPixel(row, col + offsetCol);
            } else {
                usedUhrType->setFrontMatrixPixel(row, col + offsetCol, false);
            }
        }
    }

    setbyFrontMatrix(Foreground);
    show();
}

//------------------------------------------------------------------------------

void Led::setSingle(uint8_t wait) {
    uint16_t hue;
    uint16_t counterLed = 0;
    uint16_t numPixelsWordMatrix =
        usedUhrType->rowsWordMatrix() * usedUhrType->colsWordMatrix();

    for (uint8_t row = 0; row < usedUhrType->rowsWordMatrix(); row++) {
        for (uint8_t col = 0; col < usedUhrType->colsWordMatrix(); col++) {
            hue = 360.f * counterLed / numPixelsWordMatrix;
            checkIfHueIsOutOfBound(hue);

            clear();
            if (row % 2 != 0) {
                setPixel(row, usedUhrType->colsWordMatrix() - 1 - col,
                         HsbColor(hue / 360.f, 1.f, G.effectBri / 100.f));
            } else {
                setPixel(row, col,
                         HsbColor(hue / 360.f, 1.f, G.effectBri / 100.f));
            }
            show();
            delay(wait);
            counterLed++;
        }
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

void Led::set(WordclockChanges changed) {
    setbyFrontMatrix(Foreground);
    setbyFrontMatrix(Background, false);

    if (G.minuteVariant != MinuteVariant::Off) {
        setbyMinuteArray(Foreground);
    }

    if (G.secondVariant != SecondVariant::Off) {
        setbySecondArray(Foreground);
    }

    if (transition->isOverwrittenByTransition(changed, _minute)) {
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

bool Led::getState() {
    for (uint8_t i = 0; i < 3; i++) {
        if (G.color[i].B > 0) {
            return true;
        }
    }
    return false;
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
    for (uint16_t i = minutePixelArray[0]; i <= minutePixelArray[3]; i++) {
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
    static uint8_t offsetLetter0 =
        usedUhrType->colsWordMatrix() / 2 - fontWidth[normalSizeASCII];
    static uint8_t offsetLetter1 = usedUhrType->colsWordMatrix() / 2 + 1;
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
    setbyFrontMatrix(Foreground);
    show();
}

//------------------------------------------------------------------------------

fontSize Led::determineFontSize() {

    if (G.UhrtypeDef == Ger16x18) {
        return normalSizeASCII;
    }
    return smallSizeNumbers;
}

//------------------------------------------------------------------------------

void Led::setupDigitalClock(fontSize &usedFontSize, uint8_t &offsetLetterH0,
                            uint8_t &offsetLetterH1, uint8_t &offsetLetterMin0,
                            uint8_t &offsetLetterMin1, uint8_t &offsetRow0,
                            uint8_t &offsetRow1) {

    uint8_t letterSpacing = 1;
    if (usedUhrType->rowsWordMatrix() >=
        pgm_read_byte(&(fontHeight[usedFontSize])) * 2) {
        letterSpacing++;
    }

    // 1st Row of letters vertical Offset
    offsetLetterH0 = 0;
    offsetLetterH1 = offsetLetterH0 +
                     pgm_read_byte(&(fontWidth[usedFontSize])) + letterSpacing;

    // 2nd Row of letters vertical Offset
    offsetLetterMin1 = usedUhrType->colsWordMatrix() -
                       pgm_read_byte(&(fontWidth[usedFontSize]));
    offsetLetterMin0 = offsetLetterMin1 -
                       pgm_read_byte(&(fontWidth[usedFontSize])) -
                       letterSpacing;

    // 1st Row of letters horizontal Offset
    offsetRow0 = 0;
    // 2nd Row of letters horizontal Offset
    offsetRow1 = usedUhrType->rowsWordMatrix() -
                 pgm_read_byte(&(fontHeight[usedFontSize]));
}

//------------------------------------------------------------------------------

void Led::toggleDigitalClockSecond(const fontSize &usedFontSize,
                                   const uint8_t &offsetRow1,
                                   const uint8_t &offsetMin0) {
    if (_second % 2) {
        if (usedFontSize == normalSizeASCII) {
            usedUhrType->setFrontMatrixPixel(offsetRow1 + 2, offsetMin0 - 3);
            usedUhrType->setFrontMatrixPixel(offsetRow1 + 4, offsetMin0 - 3);
        } else {
            usedUhrType->setFrontMatrixPixel(offsetRow1 + 1, offsetMin0 - 2);
            usedUhrType->setFrontMatrixPixel(offsetRow1 + 3, offsetMin0 - 2);
        }
    }
}

//------------------------------------------------------------------------------

void Led::showDigitalClock(const char min1, const char min0, const char h1,
                           const char h0, bool parametersChanged) {

    static uint8_t offsetLetterH0, offsetLetterH1, offsetLetterMin0,
        offsetLetterMin1, offsetRow0, offsetRow1;

    resetFrontMatrixBuffer();

    fontSize usedFontSize = determineFontSize();

    if (parametersChanged) {
        setupDigitalClock(usedFontSize, offsetLetterH0, offsetLetterH1,
                          offsetLetterMin0, offsetLetterMin1, offsetRow0,
                          offsetRow1);
    }

    toggleDigitalClockSecond(usedFontSize, offsetRow1, offsetLetterMin0);

    for (uint8_t col = 0; col < pgm_read_byte(&(fontWidth[usedFontSize]));
         col++) {
        for (uint8_t row = 0; row < pgm_read_byte(&(fontHeight[usedFontSize]));
             row++) {
            // 1st Row
            setPixelForChar(col, row, offsetLetterH1, offsetRow0,
                            static_cast<unsigned char>(h1), usedFontSize);
            setPixelForChar(col, row, offsetLetterH0, offsetRow0,
                            static_cast<unsigned char>(h0), usedFontSize);
            // 2nd Row
            setPixelForChar(col, row, offsetLetterMin1, offsetRow1,
                            static_cast<unsigned char>(min1), usedFontSize);
            setPixelForChar(col, row, offsetLetterMin0, offsetRow1,
                            static_cast<unsigned char>(min0), usedFontSize);
        }
    }

    mirrorFrontMatrixVertical(); // Needed for correct displaying of Chars
    setbyFrontMatrix(Foreground);
    show();
}

//------------------------------------------------------------------------------

void Led::show() {
    if (G.Colortype == Grbw) {
        strip_RGBW->Show();
    } else {
        strip_RGB->Show();
    }
}

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

inline void Led::checkIfHueIsOutOfBound(float &hue) {
    if (hue > 360) {
        hue = 0;
    }
}

//------------------------------------------------------------------------------
// Brightness Functions
//------------------------------------------------------------------------------

uint8_t Led::setBrightnessAuto(uint8_t val) {
    // G.hh contains time-dependent brightness values in %.
    uint16_t u16 = (val * G.hh) / 100;
    return (static_cast<uint8_t>((u16 * ldrVal) / 100));
}

//------------------------------------------------------------------------------

void Led::setBrightnessLdr(uint8_t &rr, uint8_t &gg, uint8_t &bb, uint8_t &ww,
                           uint8_t position) {
    if (G.autoLdrEnabled) {
        rr = setBrightnessAuto(G.rgbw[position][0]);
        gg = setBrightnessAuto(G.rgbw[position][1]);
        bb = setBrightnessAuto(G.rgbw[position][2]);
        ww = setBrightnessAuto(G.rgbw[position][3]);
    } else {
        if (G.ldr == 1) {
            rr = G.rgbw[position][0] * ldrVal / 100;
            gg = G.rgbw[position][1] * ldrVal / 100;
            bb = G.rgbw[position][2] * ldrVal / 100;
            ww = G.rgbw[position][3] * ldrVal / 100;
        } else {
            rr = G.rgbw[position][0] * G.hh / 100;
            gg = G.rgbw[position][1] * G.hh / 100;
            bb = G.rgbw[position][2] * G.hh / 100;
            ww = G.rgbw[position][3] * G.hh / 100;
        }
    }
}

//------------------------------------------------------------------------------

void Led::setBrightness(uint8_t &rr, uint8_t &gg, uint8_t &bb, uint8_t &ww,
                        uint8_t position, uint8_t percentage = 100) {
    rr = G.rgbw[position][0] * percentage / 100;
    gg = G.rgbw[position][1] * percentage / 100;
    bb = G.rgbw[position][2] * percentage / 100;
    ww = G.rgbw[position][3] * percentage / 100;
    uint16_t zz = rr + gg + bb;
    if (zz > 150) {
        zz = zz * 10 / 150;
        rr = rr * 10 / zz;
        gg = gg * 10 / zz;
        bb = bb * 10 / zz;
        ww = ww * 10 / zz;
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

void Led::setPixel(uint8_t rr, uint8_t gg, uint8_t bb, uint8_t ww, uint16_t i) {

    if (G.Colortype == Grbw) {
        strip_RGBW->SetPixelColor(i, RgbwColor(rr, gg, bb, ww));
    } else {
        strip_RGB->SetPixelColor(i, RgbColor(rr, gg, bb));
    }
}

//------------------------------------------------------------------------------

void Led::setPixelHsb(uint16_t ledIndex, float hue, float sat, float bri,
                      uint8_t alpha) {
    HsbColor hsbColor = HsbColor(hue / 360, sat / 100, bri / 100);

    if (G.Colortype == Grbw) {
        RgbColor rgbColor = RgbColor(hsbColor);

        strip_RGBW->SetPixelColor(
            ledIndex, RgbwColor(rgbColor.R, rgbColor.G, rgbColor.B, alpha));
    } else {
        strip_RGB->SetPixelColor(ledIndex, hsbColor);
    }
}

//------------------------------------------------------------------------------

void Led::setPixelColorObject(uint16_t i, RgbColor color) {

    if (G.Colortype == Grbw) {
        strip_RGBW->SetPixelColor(i, RgbwColor(color));
    } else {
        strip_RGB->SetPixelColor(i, color);
    }
}

//------------------------------------------------------------------------------

void Led::setPixelColorObject(uint16_t i, RgbwColor color) {
    strip_RGBW->SetPixelColor(i, color);
}

//------------------------------------------------------------------------------

void Led::setbyFrontMatrix(uint8_t colorPosition = Foreground) {
    uint8_t rr, gg, bb, ww;
    setBrightnessLdr(rr, gg, bb, ww, colorPosition);
    for (uint16_t i = 0; i < usedUhrType->numPixelsWordMatrix(); i++) {
        bool boolSetPixel = usedUhrType->getFrontMatrixPixel(i);
        if (colorPosition == Background) {
            boolSetPixel = !boolSetPixel;
        }
        if (boolSetPixel) {
            setPixel(rr, gg, bb, ww, i);
        } else if (colorPosition != Background) {
            clearPixel(i);
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
    float hue;

    for (uint16_t i = 0; i < usedUhrType->numPixels(); i++) {
        hue = 360.0 * i / (usedUhrType->numPixels() - 1);
        hue = hue + 360.0 / usedUhrType->numPixels();
        checkIfHueIsOutOfBound(hue);

        clear();
        setPixelHsb(i, hue, 100, 100);
        show();
        delay(wait);
    }
}

//------------------------------------------------------------------------------

void Led::setPixelForChar(uint8_t col, uint8_t row, uint8_t offsetCol,
                          uint8_t offsetRow, unsigned char unsigned_d1) {
    if (pgm_read_byte(&(font_7x5[unsigned_d1][col])) & (1u << row)) {
        usedUhrType->setFrontMatrixPixel(row + offsetRow, col + offsetCol);
    }
}

//------------------------------------------------------------------------------

void Led::set(bool changed) {
    setbyFrontMatrix(Foreground);
    setbyFrontMatrix(Background);

    if (G.minuteVariant != MinuteVariant::Off) {
        showMinutes();
    }

    if (G.secondVariant != SecondVariant::Off) {
        showSeconds();
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
        RgbwColor rgbw = strip_RGBW->GetPixelColor(i);
        return RgbColor(rgbw.R, rgbw.G, rgbw.B);
    }
    return strip_RGB->GetPixelColor(i);
}

//------------------------------------------------------------------------------

RgbwColor Led::getPixelRgbw(uint16_t i) { return strip_RGBW->GetPixelColor(i); }

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
    for (uint16_t i = 0; i < usedUhrType->numPixelsWordMatrix(); i++) {
        usedUhrType->setFrontMatrixPixel(i, false);
        clearPixel(usedUhrType->getWordMatrixIndex(i));
    }
}

//------------------------------------------------------------------------------

inline void Led::clearRow(uint8_t row) {
    for (uint8_t i = 0; i < usedUhrType->colsWordMatrix(); i++) {
        usedUhrType->setFrontMatrixPixel(row, i, false);
        clearPixel(usedUhrType->getFrontMatrixIndex(row, i));
    }
}

//------------------------------------------------------------------------------

inline void Led::clearMinArray() {
    if (usedUhrType->numPixelsFrameMatrix() == 0) {
        for (uint16_t i = usedUhrType->numPixelsWordMatrix();
             i < usedUhrType->numPixels(); i++) {
            clearPixel(i);
        }
        minuteArray = 0;
    }
}

//------------------------------------------------------------------------------

inline void Led::clearFrontExeptofFontspace(uint8_t offsetRow) {
    for (uint8_t i = 0; i < offsetRow; i++) {
        clearRow(i);
    }

    for (uint8_t i = usedUhrType->rowsWordMatrix(); i > offsetRow + fontHeight;
         i--) {
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
    static uint8_t offsetLetter1 = fontWidth + 1;
    uint8_t offsetRow = (usedUhrType->rowsWordMatrix() - fontHeight) / 2;

    if (usedUhrType->has24HourLayout()) {
        offsetLetter0 = 3;
        offsetLetter1 = fontWidth + 4;
    }

    for (uint8_t col = 0; col < fontWidth; col++) {
        for (uint8_t row = 0; row < fontHeight; row++) {
            // 1. Number without Offset
            setPixelForChar(col, row, offsetLetter0, offsetRow,
                            static_cast<unsigned char>(d1));
            // 2. Number with Offset
            setPixelForChar(col, row, offsetLetter1, offsetRow,
                            static_cast<unsigned char>(d2));
        }
    }

    mirrorFrontMatrixVertical();
    setbyFrontMatrix(Effect);
    show();
}

//------------------------------------------------------------------------------

void Led::showMinutes() {
    /* This Code will be replaces soon */
    uint8_t rr, gg, bb, ww;
    uint8_t r2, g2, b2, w2;
    setBrightnessLdr(rr, gg, bb, ww, Foreground);
    setBrightnessLdr(r2, g2, b2, w2, Background);
    /* Till here, by creating colorObjects */

    /* Set minutes According to minute byte */
    for (uint8_t i = 0; i < 4; i++) {
        /* Bitwise check whether Pixel bit is set */
        if ((minuteArray >> i) & 1U) {
            setPixel(rr, gg, bb, ww, minutePixelArray[i]);
        } else {
            /* Only for Background color setting */
            setPixel(r2, g2, b2, w2, minutePixelArray[i]);
        }
    }
}

//------------------------------------------------------------------------------

void Led::showSeconds() {
    const uint8_t offesetSecondsFrame = 5;
    uint8_t rr, gg, bb, ww;
    setBrightness(rr, gg, bb, ww, Foreground);
    for (uint8_t i = 0; i < usedUhrType->numPixelsFrameMatrix(); i++) {
        if ((frameArray >> i) & 1U) {
            if (i < usedUhrType->numPixelsFrameMatrix() - offesetSecondsFrame) {
                setPixel(rr, gg, bb, ww,
                         usedUhrType->getFrameMatrixIndex(i) +
                             offesetSecondsFrame);
            } else {
                setPixel(rr, gg, bb, ww,
                         usedUhrType->getFrameMatrixIndex(i) -
                             usedUhrType->numPixelsFrameMatrix() +
                             offesetSecondsFrame);
            }
        }
    }
}

//------------------------------------------------------------------------------

void Led::show() {
    if (G.Colortype == Grbw) {
        strip_RGBW->Show();
    } else {
        strip_RGB->Show();
    }
}

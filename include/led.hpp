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

void Led::show() {
    if (G.Colortype == Grbw) {
        strip_RGBW->Show();
    } else {
        strip_RGB->Show();
    }
}

//------------------------------------------------------------------------------

inline void Led::clearPixel(uint16_t i) {
    if (G.Colortype == Grbw) {
        strip_RGBW->SetPixelColor(i, 0);
    } else {
        strip_RGB->SetPixelColor(i, 0);
    }
}

//------------------------------------------------------------------------------

inline void Led::clear() {
    for (uint16_t i = 0; i < usedUhrType->numPixels(); i++) {
        frontMatrix[i] = false;
        clearPixel(i);
    }
}

//------------------------------------------------------------------------------

inline void Led::clearClock() {
    for (uint16_t i = 0; i < usedUhrType->numPixelsWordMatrix(); i++) {
        clearPixel(usedUhrType->getWordMatrixIndex(i));
    }
}

//------------------------------------------------------------------------------

inline void Led::clearRow(uint8_t row) {
    for (uint8_t i = 0; i < usedUhrType->colsWordMatrix(); i++) {
        clearPixel(usedUhrType->getFrontMatrix(row, i));
    }
}

//------------------------------------------------------------------------------

inline void Led::clearFrontExeptofFontspace(uint8_t offsetRow) {
    for (uint8_t i = 0; i < offsetRow; i++) {
        clearRow(i);
    }

    for (uint8_t i = usedUhrType->rowsWordMatrix() - 1;
         i > offsetRow + fontHeight; i--) {
        clearRow(i - 1);
    }
}

//------------------------------------------------------------------------------

inline void Led::clearFrame() {
    for (uint16_t i = 0; i < usedUhrType->numPixelsFrameMatrix(); i++) {
        frameArray[i] = false;
        clearPixel(usedUhrType->getFrameMatrixIndex(i));
    }
}

//------------------------------------------------------------------------------

void Led::set(bool changed) {
    uint8_t rr, gg, bb, ww;
    uint8_t r2, g2, b2, w2;
    setBrightnessLdr(rr, gg, bb, ww, Foreground);
    setBrightnessLdr(r2, g2, b2, w2, Background);
    for (uint16_t i = 0; i < usedUhrType->numPixels(); i++) {
        if (lastFrontMatrix[i]) {
            // foreground
            setPixel(rr, gg, bb, ww, i);
        } else {
            // background
            setPixel(r2, g2, b2, w2, i);
        }
    }
    if (G.secondVariant != SecondVariant::Off) {
        showSeconds();
    }
    if (animation->led_show_notify(changed, _minute)) {
        show();
    }
}

//------------------------------------------------------------------------------

void Led::setIcon(uint8_t num_icon, uint8_t brightness = 100, bool rgb_icon) {
    uint8_t rr, gg, bb, ww;
    setBrightness(rr, gg, bb, ww, Foreground, brightness);
    for (uint8_t col = 0; col < GRAFIK_11X10_COLS; col++) {
        if (rgb_icon) {
            rr = col < 3 ? 255 : 0;
            gg = (col > 3) && (col < 7) ? 255 : 0;
            bb = col > 7 ? 255 : 0;
            ww = 0;
        }
        for (uint8_t row = 0; row < GRAFIK_11X10_ROWS; row++) {
            if (pgm_read_word(&(grafik_11x10[num_icon][row])) &
                (1 << (GRAFIK_11X10_COLS - 1 - col))) {
                setPixel(rr, gg, bb, ww, usedUhrType->getFrontMatrix(row, col));
            } else {
                clearPixel(usedUhrType->getFrontMatrix(row, col));
            }
        }
    }
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

void Led::setAllPixels(uint8_t rr, uint8_t gg, uint8_t bb, uint8_t ww) {
    for (uint16_t i = 0; i < usedUhrType->numPixels(); i++) {
        setPixel(rr, gg, bb, ww, i);
    }
}

//------------------------------------------------------------------------------

void Led::setColor() {
    uint8_t rr, gg, bb, ww;
    setBrightness(rr, gg, bb, ww, Effect);
    setAllPixels(rr, gg, bb, ww);
}

//------------------------------------------------------------------------------

void Led::setFrameColor() {
    uint8_t rr, gg, bb, ww;
    setBrightness(rr, gg, bb, ww, Frame);

    for (uint16_t i = 0; i < usedUhrType->numPixelsFrameMatrix(); i++) {
        setPixel(rr, gg, bb, ww, usedUhrType->getFrameMatrixIndex(i));
    }
}

//------------------------------------------------------------------------------

void Led::shiftColumnToRight() {
    for (uint8_t col = 0; col < usedUhrType->colsWordMatrix() - 1; col++) {
        for (uint8_t row = 0;
             row < usedUhrType->rowsWordMatrix() - 1 /* Only Front*/; row++) {
            if (G.Colortype == Grbw) {
                setPixelColorObject(
                    usedUhrType->getFrontMatrix(row, col),
                    getPixelRgbw(usedUhrType->getFrontMatrix(row, col + 1)));
            } else {
                setPixelColorObject(
                    usedUhrType->getFrontMatrix(row, col),
                    getPixel(usedUhrType->getFrontMatrix(row, col + 1)));
            }
        }
    }
}

//------------------------------------------------------------------------------

void Led::setPixelForChar(uint8_t col, uint8_t row, uint8_t offsetCol,
                          uint8_t offsetRow, unsigned char unsigned_d1) {
    if (pgm_read_byte(&(font_7x5[unsigned_d1][col])) & (1u << row)) {
        setPixel(G.rgbw[Effect][0], G.rgbw[Effect][1], G.rgbw[Effect][2],
                 G.rgbw[Effect][3],
                 usedUhrType->getFrontMatrix(row + offsetRow, col + offsetCol));
    }
}

//------------------------------------------------------------------------------

void Led::showNumbers(const char d1, const char d2) {
    clearClock();
    static uint8_t offsetLetter0 = 0;
    static uint8_t offsetLetter1 = fontWidth + 1;
    uint8_t offsetRow = (usedUhrType->rowsWordMatrix() - fontHeight - 1) / 2;

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
    show();
}

//------------------------------------------------------------------------------

void Led::showSeconds() {
    const uint8_t offesetSecondsFrame = 5;
    uint8_t rr, gg, bb, ww;
    setBrightness(rr, gg, bb, ww, Foreground);
    for (uint8_t i = 0; i < usedUhrType->numPixelsFrameMatrix(); i++) {
        if (frameArray[i]) {
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

inline void Led::checkIfHueIsOutOfBound(float &hue) {
    if (hue > 360) {
        hue = 0;
    }
}

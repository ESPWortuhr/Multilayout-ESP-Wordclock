#pragma once

#include <NeoPixelBus.h> // RgbColor, HsbColor
#include <stdint.h>

// ###############################################################################
// RgbColor extended by a flag byte (foreground / overlay) and, for RgbaColor,
// an alpha channel. Comparison operators are inherited from RgbColor and
// therefore compare R, G, B only.
//
// F_xxx is one-hot coded.
// ###############################################################################

#define F_NULL 0
#define F_FOREGROUND 1
#define F_OVERLAY 2
struct RgbfColor : RgbColor {
    RgbfColor() : RgbColor() {
        R = 0;
        G = 0;
        B = 0;
        Flags = F_NULL;
    };
    RgbfColor(RgbColor rgb) : RgbColor(rgb) { Flags = F_NULL; }
    RgbfColor(uint8_t h) : RgbColor(h) { Flags = F_NULL; }
    RgbfColor(uint8_t r, uint8_t g, uint8_t b) : RgbColor(r, g, b) {
        Flags = F_NULL;
    }
    RgbfColor(RgbColor rgb, uint8_t f) : RgbColor(rgb) { Flags = f; }
    RgbfColor(HsbColor hsb, uint8_t f) : RgbColor(hsb) { Flags = f; }
    RgbfColor(uint8_t h, uint8_t f) : RgbColor(h) { Flags = f; }

    void changeRgb(RgbColor color) {
        R = color.R;
        G = color.G;
        B = color.B;
    }
    void changeRgb(HsbColor hsb) {
        RgbColor color = RgbColor(hsb);
        R = color.R;
        G = color.G;
        B = color.B;
    }
    void setForeground(bool flag = true) {
        if (flag) {
            Flags |= F_FOREGROUND;
        } else {
            Flags &= ~F_FOREGROUND;
        }
    }
    bool isForeground() const { return (Flags & F_FOREGROUND) > 0; }
    void setOverlay(bool flag = true) {
        if (flag) {
            Flags |= F_OVERLAY;
        } else {
            Flags &= ~F_OVERLAY;
        }
    }
    bool isOverlay() const { return (Flags & F_OVERLAY) > 0; }
    void setFlags(uint8_t flags) { Flags = flags; }
    uint8_t getFlags() const { return Flags; }

protected:
    uint8_t Flags;
};
struct RgbaColor : RgbfColor {
    RgbaColor() : RgbfColor() { Alpha = 255; };
    RgbaColor(RgbColor rgb) : RgbfColor(rgb) { Alpha = 255; }
    RgbaColor(uint8_t h) : RgbfColor(h) { Alpha = 255; }
    RgbaColor(RgbColor rgb, uint8_t f) : RgbfColor(rgb, f) { Alpha = 255; }
    RgbaColor(HsbColor hsb, uint8_t f) : RgbfColor(hsb, f) { Alpha = 255; }
    RgbaColor(uint8_t h, uint8_t f) : RgbfColor(h, f) { Alpha = 255; }
    // ---------------------
    RgbaColor(RgbColor rgb, float a) : RgbfColor(rgb) {
        Alpha = (uint8_t)(a * 255);
    }
    RgbaColor(uint8_t h, float a) : RgbfColor(h) { Alpha = (uint8_t)(a * 255); }
    RgbaColor(uint8_t r, uint8_t g, uint8_t b, float a) : RgbfColor(r, g, b) {
        Alpha = (uint8_t)(a * 255);
    }
    float getAlpha() const { return Alpha / 255.0; }

protected:
    uint8_t Alpha;
};

static_assert(sizeof(RgbfColor) == 4, "RgbfColor must stay 4 bytes");
static_assert(sizeof(RgbaColor) == 5, "RgbaColor must stay 5 bytes");

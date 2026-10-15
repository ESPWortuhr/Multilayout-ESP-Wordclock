#pragma once
// Minimal color types for host tests. Only the parts the render stage uses.

#include <stdint.h>

struct RgbColor;

struct HsbColor {
    HsbColor() : H(0), S(0), B(0) {}
    HsbColor(float h, float s, float b) : H(h), S(s), B(b) {}
    /* Crude on purpose, like the RgbColor(HsbColor) direction below. */
    HsbColor(const RgbColor &rgb);
    float H, S, B;
};

struct RgbColor {
    RgbColor() : R(0), G(0), B(0) {}
    RgbColor(uint8_t brightness)
        : R(brightness), G(brightness), B(brightness) {}
    RgbColor(uint8_t r, uint8_t g, uint8_t b) : R(r), G(g), B(b) {}
    /* Deliberately crude: the tests assert structure, not colour science. */
    RgbColor(const HsbColor &hsb)
        : R(static_cast<uint8_t>(hsb.H * 255)),
          G(static_cast<uint8_t>(hsb.S * 255)),
          B(static_cast<uint8_t>(hsb.B * 255)) {}
    bool operator==(const RgbColor &o) const {
        return R == o.R && G == o.G && B == o.B;
    }
    bool operator!=(const RgbColor &o) const { return !(*this == o); }

    /* NeoPixelBus returns the largest channel here. */
    uint8_t CalculateBrightness() const {
        uint8_t brightest = R > G ? R : G;
        return brightest > B ? brightest : B;
    }

    static RgbColor LinearBlend(const RgbColor &left, const RgbColor &right,
                                float progress) {
        return RgbColor(
            static_cast<uint8_t>(left.R + (right.R - left.R) * progress),
            static_cast<uint8_t>(left.G + (right.G - left.G) * progress),
            static_cast<uint8_t>(left.B + (right.B - left.B) * progress));
    }
    uint8_t R, G, B;
};

struct RgbwColor {
    uint8_t R = 0, G = 0, B = 0, W = 0;
};

inline HsbColor::HsbColor(const RgbColor &rgb)
    : H(rgb.R / 255.f), S(rgb.G / 255.f), B(rgb.B / 255.f) {}

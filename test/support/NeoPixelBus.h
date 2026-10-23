#pragma once
// Minimal color types for host tests. Only the parts the render stage uses.

#include <stdint.h>

struct RgbColor;

struct HsbColor {
    HsbColor() : H(0), S(0), B(0) {}
    HsbColor(float h, float s, float b) : H(h), S(s), B(b) {}
    HsbColor(const RgbColor &rgb);
    float H, S, B;
};

struct RgbColor {
    RgbColor() : R(0), G(0), B(0) {}
    RgbColor(uint8_t brightness)
        : R(brightness), G(brightness), B(brightness) {}
    RgbColor(uint8_t r, uint8_t g, uint8_t b) : R(r), G(g), B(b) {}
    RgbColor(const HsbColor &hsb);
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

inline HsbColor::HsbColor(const RgbColor &rgb) {
    const float r = rgb.R / 255.f;
    const float g = rgb.G / 255.f;
    const float b = rgb.B / 255.f;
    const float max = (r > g && r > b) ? r : (g > b) ? g : b;
    const float min = (r < g && r < b) ? r : (g < b) ? g : b;
    const float delta = max - min;

    float h = 0.f;
    if (delta != 0.f) {
        if (r == max) {
            h = (g - b) / delta + (g < b ? 6.f : 0.f);
        } else if (g == max) {
            h = (b - r) / delta + 2.f;
        } else {
            h = (r - g) / delta + 4.f;
        }
        h /= 6.f;
    }

    H = h;
    S = (max == 0.f) ? 0.f : (delta / max);
    B = max;
}

inline RgbColor::RgbColor(const HsbColor &hsb) {
    float r, g, b;

    if (hsb.S == 0.f) {
        r = g = b = hsb.B;
    } else {
        float h = hsb.H;
        if (h < 0.f) {
            h += 1.f;
        } else if (h >= 1.f) {
            h -= 1.f;
        }
        h *= 6.f;
        const int sector = static_cast<int>(h);
        const float f = h - sector;
        const float v = hsb.B;
        const float p = v * (1.f - hsb.S);
        const float q = v * (1.f - hsb.S * f);
        const float t = v * (1.f - hsb.S * (1.f - f));

        switch (sector) {
        case 0:
            r = v, g = t, b = p;
            break;
        case 1:
            r = q, g = v, b = p;
            break;
        case 2:
            r = p, g = v, b = t;
            break;
        case 3:
            r = p, g = q, b = v;
            break;
        case 4:
            r = t, g = p, b = v;
            break;
        default:
            r = v, g = p, b = q;
            break;
        }
    }

    R = static_cast<uint8_t>(r * 255);
    G = static_cast<uint8_t>(g * 255);
    B = static_cast<uint8_t>(b * 255);
}

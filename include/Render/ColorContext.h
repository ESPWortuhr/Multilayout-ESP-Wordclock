#pragma once

#include <NeoPixelBus.h> // HsbColor
#include <math.h>
#include <stdint.h>

enum Colorize : uint8_t {
    MONOCHROME = 0,
    POLYCHROME = 1,
    WORD_RANDOM = 2,
};

struct ColorContext {
    HsbColor foreground;
    HsbColor background;
    HsbColor gradientEnd;
    uint8_t mode = MONOCHROME;
};

inline HsbColor gradientColorAt(const HsbColor &from, const HsbColor &to,
                                uint8_t row, uint8_t rows) {
    if ((row == 0) || (rows <= 1)) {
        return from;
    }
    if (row >= (rows - 1)) {
        return to;
    }

    const float t = static_cast<float>(row) / (rows - 1);

    float deltaHue = to.H - from.H;
    if (deltaHue > 0.5f) {
        deltaHue -= 1.f;
    } else if (deltaHue < -0.5f) {
        deltaHue += 1.f;
    }

    HsbColor result;
    result.H = fmodf(from.H + deltaHue * t + 1.f, 1.f);
    result.S = from.S + (to.S - from.S) * t;
    result.B = from.B + (to.B - from.B) * t;
    return result;
}

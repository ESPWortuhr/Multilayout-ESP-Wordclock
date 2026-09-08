#pragma once

#include <NeoPixelBus.h> // HsbColor
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

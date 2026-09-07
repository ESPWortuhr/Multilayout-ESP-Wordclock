#pragma once

#include <NeoPixelBus.h> // HsbColor
#include <stdint.h>

/* How the lit cells of a clock face get their colour. */
enum Colorize : uint8_t {
    OFF = 0,         // every cell keeps the configured foreground colour
    GRADIENT = 1,    // blend from the foreground colour to the gradient colour
    RANDOM_HUES = 2, // pseudo random hues
};

/*
 * Everything a colorizer needs, passed in explicitly. Before this the colours
 * were recovered by reading the LED strip back and guessing which of the two
 * was the foreground.
 */
struct ColorContext {
    HsbColor foreground; // gradient colour no. 1
    HsbColor background;
    HsbColor gradientEnd; // gradient colour no. 2
    uint8_t mode = OFF;
    /*
     * Colour per word instead of per letter. Applies to both GRADIENT (the
     * whole word takes the colour sampled at its first cell) and RANDOM_HUES
     * (the whole word takes one hue) - the latter is what used to be a mode of
     * its own called "coloured words".
     */
    bool perWord = false;
};

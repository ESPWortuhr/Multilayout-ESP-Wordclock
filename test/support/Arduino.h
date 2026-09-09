#pragma once
// Minimal Arduino stub for host tests: only what the headers under test touch.

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#ifndef UINT8_MAX
#define UINT8_MAX 255
#endif

#define PROGMEM
#define F(x) (x)

inline unsigned long millis() {
    static unsigned long fakeClock = 0;
    fakeClock += 50; // one frame per call
    return fakeClock;
}

/*
 * Deterministic but actually varying. A constant would be simpler, but code
 * that draws until it finds a value it likes - HueSequence keeps a minimum
 * distance to the hues it handed out recently - never terminates against one.
 */
inline unsigned long randomBits() {
    static unsigned long state = 0x2545F491;
    state = state * 1103515245UL + 12345UL;
    return (state >> 16) & 0x7FFF;
}

inline long random(long high) {
    return high > 0 ? static_cast<long>(randomBits() % high) : 0;
}
inline long random(long low, long high) {
    return high > low ? low + static_cast<long>(randomBits() % (high - low))
                      : low;
}

struct SerialStub {
    template <typename... Args> void printf(const char *, Args...) {}
    void println(const char * = "") {}
    void print(const char * = "") {}
};
static SerialStub Serial;

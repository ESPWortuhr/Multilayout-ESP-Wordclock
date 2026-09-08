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

inline long random(long high) { return high > 0 ? (high / 2) : 0; }
inline long random(long low, long) { return low; }

struct SerialStub {
    template <typename... Args> void printf(const char *, Args...) {}
    void println(const char * = "") {}
    void print(const char * = "") {}
};
static SerialStub Serial;

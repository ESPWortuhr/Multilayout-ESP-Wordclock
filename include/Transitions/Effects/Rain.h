#pragma once

#include "Render/RgbfColor.h"
#include <Arduino.h> // random()
#include <stdint.h>

/*
 * One falling column of the matrix rain: a bright head followed by a green
 * tail, with a gap before the next drop. Used by MatrixRainEffect; moved out of
 * Transition so the effect owns it.
 */
class Rain {
public:
    Rain(){};
    Rain(uint8_t rows, uint8_t cols) {
        maxRows = rows;
        maxCols = cols;
    }
    virtual ~Rain(){};

    void begin(int32_t frames, int32_t stop, uint8_t helligkeit) {
        white = RgbaColor(helligkeit, 1.0f);
        // white.Lighten(helligkeit);
        green = RgbaColor(0, helligkeit, 0, 0.5);
        // green.Lighten(helligkeit);
        offset = -(maxRows - 1 - 3); // 3 = min deadtime
        speed = 1;
        stopping = false;
        speedlimit = random(1, 4);
        lifetime = random(4, maxRows);
        deadtime = random(3, maxRows); // min = 3, max deadtime
        //    speedlimit = 2;
        //    lifetime = 4;
        //    deadtime = 3;
        phase = 1;
        stopLine = maxRows - 1 - stop;
        stopPhase = frames - speedlimit * maxRows;
    }

    RgbaColor get(int32_t _row) {
        int32_t row = (maxRows - 1 - _row);
        // per image row runs from (maxRows - 1) down to 0

        int32_t pos = (row + offset) % (deadtime + lifetime);

        if (row == 0) { // last row for this image
            if (phase++ == stopPhase) {
                stopTop = maxRows;
                stopBottom = stopLine;
                stopping = true;
            }
            if (speed++ >= speedlimit) {
                speed = 1;
                offset++;
            }
        }

        if (stopping) {
            if ((row >= stopTop) || ((row < stopLine) && (row >= stopBottom))) {
                if ((row == 0) && (speed == 1)) {
                    stopTop--;
                    stopBottom--;
                }
                return transparent;
            }
            if ((row == 0) && (speed == 1)) {
                stopTop--;
                stopBottom--;
            }
        }
        if ((pos) < deadtime) {
            return transparent;
        }
        if ((pos) == deadtime) {
            return white;
        }
        if ((pos) < (deadtime + lifetime)) {
            return green;
        }
        offset = 0;
        return transparent;
    }

protected:
    uint8_t maxRows, maxCols;
    int32_t speed, speedlimit, offset, lifetime, deadtime;
    int32_t phase, frames, stopPhase, stopLine, stopTop, stopBottom;
    bool stopping;
    RgbaColor white = RgbaColor(255, 255, 255, 0.9);
    RgbaColor green = RgbaColor(0, 255, 0, 0.75);
    RgbaColor transparent = RgbaColor(0, 0, 0, 0.0);
};

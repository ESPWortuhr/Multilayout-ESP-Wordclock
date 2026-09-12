#pragma once

#include "Render/RgbfColor.h"
#include <stdint.h>

/*
 * A ball dropping down one column of the front, bouncing with a bit of energy
 * loss. Used by BallsEffect; moved out of Transition so the effect owns it.
 *
 * Fixed point: positions are kept shifted left by 8 bits.
 */
class Ball {
public:
    Ball(){};
    Ball(uint8_t maxRows) {
        unten = ((maxRows - 1) << 8);
        lastPos = ((maxRows - 3) << 8);
    }
    virtual ~Ball(){};

    void begin(int32_t _row, int32_t _col, RgbfColor foreground,
               RgbfColor background, int32_t delay, uint8_t durationSetting) {
        this->delay = delay;
        y = _row << 8; // increase precision
        row = _row;
        col = _col;
        vy = 0;
        colorForeground = foreground;
        colorBackground = background;
        g = 9810 / (durationSetting > 0 ? durationSetting : 1);
        end = (y == unten) ? 1 : 0;
        lastDown = false;
    }

    // ###############################################################################
    // x =  0, y = 0 -> left down
    // x = 10, y = 9 -> right down
    // v positiv    -> down
    int32_t move(int32_t timedelta) {
        if (!end) {
            delay -= timedelta;
            if (delay <= 0) {
                int32_t _vy = vy;
                y += (((g * timedelta) / 1000) * timedelta) / 2000 +
                     (vy * timedelta) / 1000;
                vy += (g * timedelta) / 1000;
                if ((vy > 0) && (y >= unten)) { // lower turning point
                    y = unten;                  // - (y - unten);
                    vy = -(vy * 6) / 10;        // Destroy energy
                    if (lastDown) {
                        end = 1;
                    }
                }
                lastDown |= (_vy < 0) && (vy >= 0) &&
                            (y > lastPos); // upper turning point
                row = y >> 8;
            }
            color = end ? colorBackground : colorForeground;
        }
        return end;
    }

public:
    int32_t row, col; // after calling move() row & col contain new actual val
    RgbfColor color;

protected:
    int32_t unten;
    int32_t lastPos;
    int32_t g, vy, y, end, delay;
    bool lastDown;
    RgbfColor colorForeground, colorBackground;
};

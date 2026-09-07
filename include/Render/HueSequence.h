#pragma once

#include <Arduino.h> // random()
#include <stdint.h>

/*
 * Source of pseudo random hues that keeps a minimum distance from the hues
 * handed out recently, so two neighbouring words never end up in nearly the
 * same colour.
 *
 * Values are tracked as 1000..1999 and returned as 0.0..1.0. Was
 * Transition::pseudoRandomHue(), which kept its state in three function local
 * static variables and therefore could not be reset or used twice.
 */
class HueSequence {
public:
    /* Next hue, 0.0 .. 1.0 */
    float next() {
        if (!m_initialized) {
            reset();
        }
        return pick();
    }

    void reset() {
        m_initialized = true;
        m_inUse = 1;
        m_recent[0] = m_last;
        for (uint8_t i = 1; i < HISTORY; i++) {
            m_recent[i] = EMPTY;
        }
    }

private:
    static const uint8_t HISTORY = 10;
    static const uint16_t EMPTY = 9999;
    static const uint16_t MIN_DISTANCE = 100; // 0.1 of the hue circle

    float pick() {
        if (m_inUse == (HISTORY - 1)) {
            reset();
        }

        uint16_t hue = 1000 + random(1000);
        uint8_t index = 0;
        uint8_t tries = 0;

        while ((index < HISTORY) && (tries < 20)) {
            if (m_recent[index] == EMPTY) {
                m_inUse = index;
                m_last = hue;
                m_recent[index] = hue;
                return static_cast<float>(hue - 1000) / 1000.f;
            }
            if ((hue > (m_recent[index] - MIN_DISTANCE)) &&
                (hue < (m_recent[index] + MIN_DISTANCE))) {
                // too close to a recent hue, start over with a new candidate
                hue = 1000 + random(1000);
                tries++;
                index = 0;
            } else {
                index++;
            }
        }

        reset();
        return pick();
    }

    uint16_t m_recent[HISTORY] = {EMPTY};
    uint16_t m_last = 1000;
    uint8_t m_inUse = 1;
    bool m_initialized = false;
};

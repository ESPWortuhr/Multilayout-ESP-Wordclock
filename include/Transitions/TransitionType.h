#pragma once

#include <stdint.h>

/*
 * The transition types as stored in G.transitionType.
 *
 * Kept apart from Transition.h so code that only needs to validate a stored or
 * received value - the web socket handler, the start-up check - does not have
 * to pull in the whole engine along with its PROGMEM font and symbol tables.
 */

enum Transition_t {
    NO_TRANSITION = 0,
    ROLL_UP = 1,
    ROLL_DOWN = 2,
    SHIFT_LEFT = 3,
    SHIFT_RIGHT = 4,
    FADE = 5,
    LASER = 6,
    MATRIX_RAIN = 7,
    BALLS = 8,
    FIRE = 9,
    SNAKE = 10,
    // only internaly used
    RANDOM = 11,

    BIRTHDAY = 97,
    NEWYEAR_COUNTDOWN = 98,
    NEWYEAR_FIRE = 99
};

/*
 * Values G.transitionType may hold. COLORED (12) used to exist only to switch
 * colouring on, because that was tied to having a transition; colouring is
 * independent now, so a stored 12 falls back to no transition.
 */
inline bool isValidTransitionType(uint8_t type) {
    switch (type) {
    case NO_TRANSITION:
    case ROLL_UP:
    case ROLL_DOWN:
    case SHIFT_LEFT:
    case SHIFT_RIGHT:
    case FADE:
    case LASER:
    case MATRIX_RAIN:
    case BALLS:
    case FIRE:
    case SNAKE:
    case RANDOM:
        return true;
    default:
        return false;
    }
}

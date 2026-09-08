#pragma once

#include "Transitions/TransitionType.h"

/* Transition types a date, not the user, switches on. */
inline bool isSpecialEventTransition(Transition_t type) {
    return (type == NEWYEAR_COUNTDOWN) || (type == NEWYEAR_FIRE) ||
           (type == BIRTHDAY);
}

/* What the schedule needs to know about the current moment. */
struct SpecialEventInput {
    /* 31 December, 23:59. */
    bool newYear = false;
    /* A birthday is configured for today. */
    bool birthday = false;
    /* tm_min: the birthday animation starts 2 minutes after a clock change. */
    uint8_t minute = 0;
    /* The minute edge drives the state machine; nothing happens without it. */
    bool minuteChanged = false;
};

/* How long the New Year fireworks keep running, in minutes. */
#define NEWYEAR_FIRE_MINUTES 10

class SpecialEvents {
public:
    /*
     * Advance the state machine and return the event that is running, or
     * NO_TRANSITION when the configured transition type applies again.
     */
    Transition_t update(const SpecialEventInput &input) {
        if (!input.minuteChanged) {
            return m_type;
        }

        switch (m_type) {
        case NEWYEAR_COUNTDOWN:
            // One minute of countdown, then the fireworks.
            m_type = NEWYEAR_FIRE;
            break;

        case NEWYEAR_FIRE:
            m_minutesRunning++;
            if (m_minutesRunning >= NEWYEAR_FIRE_MINUTES) {
                m_type = NO_TRANSITION;
            }
            break;

        case BIRTHDAY:
            // One minute of fireworks, then back to the time.
            m_type = NO_TRANSITION;
            break;

        default:
            // No event running: a new one may start.
            if (input.newYear) {
                m_minutesRunning = 0;
                m_type = NEWYEAR_COUNTDOWN;
            } else if (input.birthday && (input.minute % 5 == 2)) {
                m_type = BIRTHDAY;
            }
            break;
        }

        return m_type;
    }

    bool running() const { return isSpecialEventTransition(m_type); }

private:
    Transition_t m_type = NO_TRANSITION;
    uint8_t m_minutesRunning = 0;
};

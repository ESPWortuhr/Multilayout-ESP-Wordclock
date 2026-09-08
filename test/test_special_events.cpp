/*
 * The special event schedule used to sit inside Transition and mutate the
 * transition type in place. Pulling it out is what allows a clock with
 * transitions switched off to have no Transition object at all - but only if
 * New Year and birthdays still start one when their minute comes.
 */

#include "Render/SpecialEvents.h"
#include "support/check.h"

namespace {

SpecialEventInput tick(uint8_t minute) {
    SpecialEventInput input;
    input.minute = minute;
    input.minuteChanged = true;
    return input;
}

/* Nothing configured, nothing to celebrate: the schedule stays out of the way.
 */
void quietDaysStartNothing() {
    SpecialEvents events;
    for (uint8_t minute = 0; minute < 60; minute++) {
        check(events.update(tick(minute)) == NO_TRANSITION,
              "events.update(tick(minute)) == NO_TRANSITION");
        check(!events.running(), "!events.running()");
    }
}

/* The state machine may only advance on a minute edge. */
void nothingHappensBetweenMinutes() {
    SpecialEvents events;

    SpecialEventInput newYear = tick(59);
    newYear.newYear = true;
    check(events.update(newYear) == NEWYEAR_COUNTDOWN,
          "events.update(newYear) == NEWYEAR_COUNTDOWN");

    // Same minute, called again and again the way the main loop does it.
    SpecialEventInput sameMinute = newYear;
    sameMinute.minuteChanged = false;
    for (int i = 0; i < 100; i++) {
        check(events.update(sameMinute) == NEWYEAR_COUNTDOWN,
              "events.update(sameMinute) == NEWYEAR_COUNTDOWN");
    }
}

/* Countdown, then exactly ten minutes of fireworks, then back to normal. */
void newYearRunsCountdownThenFireworks() {
    SpecialEvents events;

    SpecialEventInput newYear = tick(59);
    newYear.newYear = true;
    check(events.update(newYear) == NEWYEAR_COUNTDOWN,
          "events.update(newYear) == NEWYEAR_COUNTDOWN");
    check(events.running(), "events.running()");

    // Midnight: isNewYear() is false again from here on.
    check(events.update(tick(0)) == NEWYEAR_FIRE,
          "events.update(tick(0)) == NEWYEAR_FIRE");
    check(events.running(), "events.running()");

    for (uint8_t minute = 1; minute < NEWYEAR_FIRE_MINUTES; minute++) {
        check(events.update(tick(minute)) == NEWYEAR_FIRE,
              "events.update(tick(minute)) == NEWYEAR_FIRE");
    }

    check(events.update(tick(NEWYEAR_FIRE_MINUTES)) == NO_TRANSITION,
          "events.update(tick(NEWYEAR_FIRE_MINUTES)) == NO_TRANSITION");
    check(!events.running(), "!events.running()");
}

/* A birthday animation starts 2 minutes past every clock change and lasts one
 * minute. */
void birthdayStartsTwoMinutesPast() {
    SpecialEvents events;

    for (uint8_t minute = 0; minute < 60; minute++) {
        SpecialEventInput input = tick(minute);
        input.birthday = true;
        const Transition_t type = events.update(input);

        if (minute % 5 == 2) {
            check(type == BIRTHDAY, "type == BIRTHDAY");
        } else if (minute % 5 == 3) {
            // The minute after: back to the configured transition.
            check(type == NO_TRANSITION, "type == NO_TRANSITION");
        } else {
            check(type == NO_TRANSITION, "type == NO_TRANSITION");
        }
    }
}

/* New Year's Eve happens to be nobody's birthday here, but if it were, the
 * countdown must not be cut short by a birthday starting on top of it. */
void newYearWinsOverBirthday() {
    SpecialEvents events;

    SpecialEventInput both = tick(59);
    both.newYear = true;
    both.birthday = true;
    check(events.update(both) == NEWYEAR_COUNTDOWN,
          "events.update(both) == NEWYEAR_COUNTDOWN");

    SpecialEventInput birthdayMinute = tick(2);
    birthdayMinute.birthday = true;
    check(events.update(birthdayMinute) == NEWYEAR_FIRE,
          "events.update(birthdayMinute) == NEWYEAR_FIRE");

    for (uint8_t i = 1; i < NEWYEAR_FIRE_MINUTES; i++) {
        SpecialEventInput input = tick(static_cast<uint8_t>(i * 5 + 2));
        input.birthday = true;
        check(events.update(input) == NEWYEAR_FIRE,
              "events.update(input) == NEWYEAR_FIRE");
    }
}

/* The fireworks counter is a uint8_t that used to be shared with the transition
 * type; a second New Year has to start from zero again. */
void aSecondNewYearRunsTheFullLength() {
    SpecialEvents events;

    for (int round = 0; round < 3; round++) {
        SpecialEventInput newYear = tick(59);
        newYear.newYear = true;
        check(events.update(newYear) == NEWYEAR_COUNTDOWN,
              "events.update(newYear) == NEWYEAR_COUNTDOWN");
        check(events.update(tick(0)) == NEWYEAR_FIRE,
              "events.update(tick(0)) == NEWYEAR_FIRE");

        for (uint8_t minute = 1; minute < NEWYEAR_FIRE_MINUTES; minute++) {
            check(events.update(tick(minute)) == NEWYEAR_FIRE,
                  "events.update(tick(minute)) == NEWYEAR_FIRE");
        }
        check(events.update(tick(NEWYEAR_FIRE_MINUTES)) == NO_TRANSITION,
              "events.update(tick(NEWYEAR_FIRE_MINUTES)) == NO_TRANSITION");
    }
}

} // namespace

int main() {
    quietDaysStartNothing();
    nothingHappensBetweenMinutes();
    newYearRunsCountdownThenFireworks();
    birthdayStartsTwoMinutesPast();
    newYearWinsOverBirthday();
    aSecondNewYearRunsTheFullLength();
    return report("special events");
}

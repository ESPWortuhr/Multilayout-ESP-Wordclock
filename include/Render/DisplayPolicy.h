#pragma once

#include "WordClockState.h" // WordclockChanges

struct DisplayAction {
    /* Led::set() draws the minute LEDs and the seconds frame. */
    bool drawMinutesAndFrame = false;
    /* Led::set() pushes the strip. */
    bool showFromLed = false;
    /* The transition stage owes the push instead. */
    bool redrawFromTransition = false;
    /* Start a new transition animation. */
    bool startTransition = false;
    /* Rebuild the transition matrices from the new clock face. */
    bool rebuildMatrix = false;
    /* Colours or brightness may have changed; rescale what is already there. */
    bool refreshBrightness = false;
};

/*
 * The front matrix is one buffer shared by every mode - the wifi symbol drawn
 * at boot, the digits, a bitmap symbol, the scrolling text all live in it. The
 * pipeline reads it as a clock face and colours and animates it, so it has to
 * stay out until the clock work has actually put a clock face there.
 *
 * Two ways that goes wrong without this: at boot the pipeline runs before the
 * clock work has calculated anything, and on a switch back to the word clock
 * G.prog changes a full loop pass before the face is recalculated.
 */
inline bool pipelineMayRender(uint8_t prog, bool faceCalculated) {
    return faceCalculated && isWordClockMode(prog);
}

//------------------------------------------------------------------------------

inline DisplayAction decideDisplayAction(WordclockChanges change,
                                         bool transitionOwnsDisplay,
                                         bool transitionAnimates,
                                         bool minuteChanged) {
    DisplayAction action;
    const bool parametersOnly = (change == WordclockChanges::Parameters);

    if (!transitionOwnsDisplay) {
        action.drawMinutesAndFrame = true;
        action.showFromLed = true;
        action.rebuildMatrix = !parametersOnly && minuteChanged;
        return action;
    }

    action.redrawFromTransition = true;

    if (parametersOnly) {
        action.refreshBrightness = true;
        return action;
    }

    action.startTransition =
        transitionAnimates && change == WordclockChanges::Words;

    action.rebuildMatrix = (change != WordclockChanges::Minute);
    return action;
}

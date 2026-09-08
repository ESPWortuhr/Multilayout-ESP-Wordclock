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
        transitionAnimates && (change == WordclockChanges::Words ||
                               change == WordclockChanges::Layout);

    action.rebuildMatrix = (change != WordclockChanges::Minute);
    return action;
}

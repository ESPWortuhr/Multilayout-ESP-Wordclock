/*
 * The three bugs this file exists for: with colouring switched on and no
 * transition running, Led::set() steps aside - and nothing else pushed the
 * strip. Colour edits stayed invisible, the minute LEDs froze, and switching
 * colouring back off kept the old colours.
 *
 * All three were the same failure: a change of the clock face with no stage
 * responsible for putting it on the LEDs.
 */

#include "Render/DisplayPolicy.h"
#include "support/check.h"

namespace {

const WordclockChanges ALL_CHANGES[] = {
    WordclockChanges::Null, WordclockChanges::Parameters,
    WordclockChanges::Minute, WordclockChanges::Layout,
    WordclockChanges::Words};

/* The invariant the three bugs violated. */
void everyChangeHasExactlyOnePusher() {
    for (WordclockChanges change : ALL_CHANGES) {
        for (int owns = 0; owns <= 1; owns++) {
            // A stage that animates always owns the display, so the combination
            // "animates but does not own" cannot occur.
            for (int animates = 0; animates <= owns; animates++) {
                for (int minuteChanged = 0; minuteChanged <= 1;
                     minuteChanged++) {
                    const DisplayAction action = decideDisplayAction(
                        change, owns != 0, animates != 0, minuteChanged != 0);

                    check(action.showFromLed != action.redrawFromTransition,
                          "exactly one stage pushes the strip");
                    check(action.drawMinutesAndFrame == action.showFromLed,
                          "whoever pushes also draws minutes and frame");
                    check(!action.startTransition || animates != 0,
                          "no animation is started without a transition");
                }
            }
        }
    }
}

/* Bug 1: dragging the colour wheel did not reach the LEDs. */
void colorEditReachesTheStrip() {
    const DisplayAction action =
        decideDisplayAction(WordclockChanges::Parameters, true, true, false);

    check(action.redrawFromTransition,
          "colour edit is pushed by the transition stage");
    check(action.refreshBrightness, "colour edit rescales the matrices");
    check(!action.rebuildMatrix, "colour edit leaves the words alone");
    check(!action.startTransition, "colour edit starts no animation");
}

/* Bug 2: the minute LEDs froze while colouring was on. */
void minuteChangeReachesTheStrip() {
    const DisplayAction action =
        decideDisplayAction(WordclockChanges::Minute, true, true, false);

    check(action.redrawFromTransition,
          "minute change is pushed by the transition stage");
    check(!action.rebuildMatrix, "a minute change does not rebuild the words");
    check(!action.startTransition, "a minute change starts no animation");
}

/* Bug 3: switching colouring off left the old colours on the strip. */
void ledTakesOverWhenNobodyOwnsTheDisplay() {
    const DisplayAction action =
        decideDisplayAction(WordclockChanges::Words, false, false, true);

    check(action.showFromLed, "Led pushes when no stage owns the display");
    check(action.drawMinutesAndFrame, "Led draws minutes and frame as well");
    check(!action.redrawFromTransition, "the transition stage owes nothing");
}

void wordChangesStartAnimations() {
    check(decideDisplayAction(WordclockChanges::Words, true, true, false)
              .startTransition,
          "changed words start a transition");
    check(decideDisplayAction(WordclockChanges::Layout, true, true, false)
              .startTransition,
          "a layout change starts a transition");
    check(decideDisplayAction(WordclockChanges::Words, true, true, false)
              .rebuildMatrix,
          "changed words rebuild the matrices");
}

void matrixIsRebuiltOnlyOnARealMinuteEdge() {
    check(!decideDisplayAction(WordclockChanges::Words, false, false, false)
               .rebuildMatrix,
          "no minute edge, no rebuild");
    check(decideDisplayAction(WordclockChanges::Words, false, false, true)
              .rebuildMatrix,
          "minute edge rebuilds");
    check(!decideDisplayAction(WordclockChanges::Parameters, false, false, true)
               .rebuildMatrix,
          "a parameter change never rebuilds the words");
}

/*
 * Colouring without a transition: this stage pushes the pixels, but there is no
 * animation. Starting one would leave phase at 1 forever, which freezes the
 * demo mode - it only advances while isIdle().
 */
void colouringWithoutATransitionStartsNoAnimation() {
    const DisplayAction action =
        decideDisplayAction(WordclockChanges::Words, /*owns=*/true,
                            /*animates=*/false, false);

    check(action.redrawFromTransition, "colouring pushes without a transition");
    check(!action.startTransition, "but starts no animation");
    check(action.rebuildMatrix, "the matrices are still rebuilt");
}

} // namespace

/*
 * The bug: at boot the wifi symbol sits in the front matrix, and the pipeline
 * runs a full loop pass before the clock work calculates anything. It coloured
 * that symbol and animated the first transition out of it.
 */
void thePipelineWaitsForACalculatedFace() {
    check(!pipelineMayRender(COMMAND_IDLE, false),
          "nothing is rendered before the first clock face");
    check(pipelineMayRender(COMMAND_IDLE, true),
          "the idle word clock renders once it has a face");
    check(pipelineMayRender(COMMAND_MODE_WORD_CLOCK, true),
          "so does the word clock being recalculated");

    // Every other mode paints the front matrix itself, and none of what it
    // paints is a clock face to colour or animate.
    const uint8_t OTHER_MODES[] = {
        COMMAND_MODE_SECONDS,      COMMAND_MODE_SCROLLINGTEXT,
        COMMAND_MODE_RAINBOWCYCLE, COMMAND_MODE_RAINBOW,
        COMMAND_MODE_COLOR,        COMMAND_MODE_DIGITAL_CLOCK,
        COMMAND_MODE_SYMBOL};

    for (uint8_t prog : OTHER_MODES) {
        check(!pipelineMayRender(prog, true),
              "another mode owns the front matrix and is left alone");
    }
}

int main() {
    everyChangeHasExactlyOnePusher();
    colouringWithoutATransitionStartsNoAnimation();
    colorEditReachesTheStrip();
    minuteChangeReachesTheStrip();
    ledTakesOverWhenNobodyOwnsTheDisplay();
    wordChangesStartAnimations();
    matrixIsRebuiltOnlyOnARealMinuteEdge();
    thePipelineWaitsForACalculatedFace();

    return report("display policy");
}

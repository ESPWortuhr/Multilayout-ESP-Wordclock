#pragma once

#include "Render/ColorMatrix.h"
#include "Transitions/TransitionType.h"
#include <time.h>

/*
 * One animation step of a transition.
 *
 * An effect blends `from` into `to` and writes the result to `out`. Both are
 * already coloured - an effect moves pixels around, it never decides a colour.
 * It owns its own state: what used to be function-local static variables inside
 * Transition, which survived a change of effect and were therefore a source of
 * bugs.
 *
 * Effects work on whole matrices, so there is one virtual call per animation
 * step, not per cell.
 */
struct TransitionContext {
    /*
     * Writable on purpose: MatrixRain reseeds it with whatever is currently on
     * the strip, because it fades from the visible image rather than from the
     * stored one.
     */
    ColorMatrix &from;
    /*
     * Writable on purpose too: the birthday firework replaces the clock face
     * with HAPPY BIRTHDAY, so the target of the blend is its own doing.
     */
    ColorMatrix &to;
    ColorMatrix &out;
    RgbfColor foreground;
    RgbfColor background;
    /*
     * Which variant is running - one firework serves FIRE, NEWYEAR and
     * BIRTHDAY. No default initialisers in this struct: with the ESP32
     * toolchain on C++11 they would stop it being an aggregate.
     */
    Transition_t type;
    /* Wall clock; only the New Year countdown needs it. */
    const struct tm *time;
    /* 1 on the first step of an animation. */
    uint16_t phase;
    /* Milliseconds until the next step; an effect sets its own pace. */
    uint32_t delayMs;
    /* G.transitionDuration: 1 short, 2 medium, 3 long. */
    uint8_t durationSetting;
};

class ITransitionEffect {
public:
    virtual ~ITransitionEffect() = default;

    /* Adapt to a new front matrix geometry; effects owning per column state
     * need this. */
    virtual void resize(uint8_t rows, uint8_t cols) {
        (void)rows;
        (void)cols;
    }

    /* Returns the next phase, or 0 when the animation has finished. */
    virtual uint16_t step(TransitionContext &context) = 0;
};

/*
 * Spread `frames` animation steps over the configured duration.
 * slow == 3 -> 5s, medium == 2 -> 3.5s, fast == 1 -> 2s
 */
inline uint16_t transitionDelayForFrames(uint16_t frames,
                                         uint8_t durationSetting) {
    if (frames == 0) { // avoid div 0
        frames = 10;
    }

    uint32_t pause;
    switch (durationSetting) {
    case 1:
        pause = 2000 / frames;
        break;
    case 3:
        pause = 5000 / frames;
        break;
    default:
        pause = 3500 / frames;
        break;
    }

    return (pause == 0) ? 1 : static_cast<uint16_t>(pause);
}

inline RgbColor linearBlendCell(const RgbfColor &from, const RgbfColor &to,
                                float progress) {
    RgbColor blend;
    return blend.LinearBlend(from, to, progress);
}

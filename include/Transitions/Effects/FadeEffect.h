#pragma once

#include "Transitions/TransitionEffect.h"

/* Cross fade the whole front from the old face to the new one. */
class FadeEffect : public ITransitionEffect {
public:
    uint16_t step(TransitionContext &context) override {
        const uint16_t frames = 100;

        if (context.phase == 1) {
            context.delayMs =
                transitionDelayForFrames(frames, context.durationSetting);
        }

        const float progress = static_cast<float>(context.phase) / frames;

        for (uint8_t col = 0; col < context.out.cols(); col++) {
            for (uint8_t row = 0; row < context.out.rows(); row++) {
                context.out[row][col].changeRgb(linearBlendCell(
                    context.from[row][col], context.to[row][col], progress));
            }
        }

        if (context.phase >= frames) {
            // Take the foreground flags of the new face along.
            context.out.copyFrom(context.to);
            return 0;
        }
        return context.phase + 1;
    }
};

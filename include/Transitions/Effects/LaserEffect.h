#pragma once

#include "Transitions/TransitionEffect.h"

/*
 * A single dot sweeps the front cell by cell, erasing on the way there and
 * writing the new face on the way back.
 */
class LaserEffect : public ITransitionEffect {
public:
    uint16_t step(TransitionContext &context) override {
        if (context.phase == 1) {
            context.delayMs = transitionDelayForFrames(
                context.out.rows() * context.out.cols() * 2,
                context.durationSetting);
            m_row = 0;
            m_col = 0;
            m_erasing = true;
            context.out.copyFrom(context.from);
        }

        context.out[m_row][m_col] =
            m_erasing ? context.background : context.to[m_row][m_col];

        if (++m_col >= context.out.cols()) {
            m_col = 0;
            m_row++;
        }

        if (m_row < context.out.rows()) {
            context.out[m_row][m_col] = m_beam;
            return context.phase + 1;
        }

        m_row = 0;
        if (!m_erasing) {
            return 0;
        }
        m_erasing = false;
        return context.phase + 1;
    }

private:
    uint8_t m_row = 0;
    uint8_t m_col = 0;
    bool m_erasing = true;
    RgbfColor m_beam = RgbfColor(255);
};

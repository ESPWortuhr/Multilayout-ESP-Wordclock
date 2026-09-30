#pragma once

#include "Transitions/TransitionEffect.h"

/*
 * The new face slides in row by row, from above or from below.
 *
 *  phase | 9876543210   old  act   change   | 9876543210   old  act   change
 *    1   | aaaaaaaaan   0-8  9     0        | naaaaaaaaa   1-9  0     9
 *    2   | aaaaaaaann   0-7  8-9   1        | nnaaaaaaaa   2-9  0-1   8
 *   10   | nnnnnnnnnn        0-9   9        | nnnnnnnnnn        0-9   0
 */
class ScrollVerticalEffect : public ITransitionEffect {
public:
    explicit ScrollVerticalEffect(bool downwards) : m_downwards(downwards) {}

    uint16_t step(TransitionContext &context) override {
        const uint8_t rows = context.out.rows();

        if (context.phase == 1) {
            context.delayMs =
                transitionDelayForFrames(rows, context.durationSetting) / 4;
        }

        uint32_t changeAt, rowOld, rowNew;
        if (m_downwards) {
            changeAt = context.phase;
            rowOld = 0;
            rowNew = rows - context.phase;
        } else {
            changeAt = rows - context.phase;
            rowOld = context.phase;
            rowNew = 0;
        }

        for (uint8_t row = 0; row < rows; row++) {
            const bool fromNew = (row >= changeAt) ^ m_downwards;
            for (uint8_t col = 0; col < context.out.cols(); col++) {
                context.out[row][col] = fromNew ? context.to[rowNew][col]
                                                : context.from[rowOld][col];
            }
            if (fromNew) {
                rowNew++;
            } else {
                rowOld++;
            }
        }

        return (context.phase >= rows) ? 0 : context.phase + 1;
    }

private:
    bool m_downwards;
};

//------------------------------------------------------------------------------

/* The new face slides in column by column, from the left or from the right. */
class ScrollHorizontalEffect : public ITransitionEffect {
public:
    explicit ScrollHorizontalEffect(bool rightwards)
        : m_rightwards(rightwards) {}

    uint16_t step(TransitionContext &context) override {
        const uint8_t cols = context.out.cols();

        if (context.phase == 1) {
            context.delayMs =
                transitionDelayForFrames(cols, context.durationSetting) / 4;
        }

        uint32_t changeAt, colOld, colNew;
        if (m_rightwards) {
            changeAt = context.phase;
            colOld = 0;
            colNew = cols - context.phase;
        } else {
            changeAt = cols - context.phase;
            colOld = context.phase;
            colNew = 0;
        }

        for (uint8_t col = 0; col < cols; col++) {
            const bool fromNew = (col >= changeAt) ^ m_rightwards;
            for (uint8_t row = 0; row < context.out.rows(); row++) {
                context.out[row][col] = fromNew ? context.to[row][colNew]
                                                : context.from[row][colOld];
            }
            if (fromNew) {
                colNew++;
            } else {
                colOld++;
            }
        }

        return (context.phase >= cols) ? 0 : context.phase + 1;
    }

private:
    bool m_rightwards;
};

#pragma once

#include "Transitions/Effects/Snake.h"
#include "Transitions/TransitionEffect.h"

/*
 * A snake crosses the front, eating the old face and leaving the new one
 * behind. It needs no colour of its own beyond the complement of the
 * foreground, so unlike the firework it stays free of any global.
 */
class SnakeEffect : public ITransitionEffect {
public:
    void resize(uint8_t rows, uint8_t cols) override {
        m_snake.resize(rows, cols);
    }

    uint16_t step(TransitionContext &context) override {
        if (!m_snake.valid()) {
            return 0;
        }

        if (context.phase == 1) {
            const uint16_t food = 40;
            context.delayMs =
                transitionDelayForFrames(food, context.durationSetting);
            m_snake.begin(context.from, context.to, context.foreground);
        }

        if (!m_snake.draw(context.out, context.background)) {
            return 0;
        }
        return context.phase + 1;
    }

private:
    Snake m_snake;
};

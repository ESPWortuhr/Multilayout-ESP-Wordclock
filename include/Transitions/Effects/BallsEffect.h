#pragma once

#include "Transitions/Effects/Ball.h"
#include "Transitions/TransitionEffect.h"
#include <Arduino.h> // millis()

/*
 * In every column the topmost lit letter drops to the bottom and bounces. The
 * balls are staggered by 100 ms each so the front does not collapse at once.
 */
class BallsEffect : public ITransitionEffect {
public:
    ~BallsEffect() override { delete[] m_balls; }

    void resize(uint8_t rows, uint8_t cols) override {
        delete[] m_balls;
        m_balls = (cols > 0) ? new Ball[cols] : nullptr;
        m_columns = cols;
        for (uint8_t col = 0; col < cols; col++) {
            m_balls[col] = Ball(rows);
        }
    }

    uint16_t step(TransitionContext &context) override {
        if (m_balls == nullptr) {
            return 0;
        }

        if (context.phase == 1) {
            context.delayMs = 50; // 20 frames per second
            m_ballCount = 0;
            for (uint8_t col = 0;
                 (col < context.out.cols()) && (m_ballCount < m_columns);
                 col++) {
                for (uint8_t row = 0; row < context.out.rows(); row++) {
                    if (!context.out[row][col].isForeground()) {
                        continue;
                    }
                    m_balls[m_ballCount].begin(
                        row, col, context.out[row][col], context.background,
                        100 * m_ballCount, context.durationSetting);
                    m_ballCount++;
                    break;
                }
            }
            m_lastMove = millis();
        }

        const uint32_t now = millis();
        const int32_t timeDelta = static_cast<int32_t>(now - m_lastMove);
        m_lastMove = now;

        uint32_t landed = 0;
        for (uint8_t i = 0; i < m_ballCount; i++) {
            int32_t previousRow = m_balls[i].row;
            landed += m_balls[i].move(timeDelta);

            const int32_t row = m_balls[i].row;
            const int32_t col = m_balls[i].col;

            // Erase the trail the ball left behind, in either direction.
            for (; previousRow > row; previousRow--) {
                context.out[previousRow][col] = context.background;
            }
            for (; previousRow < row; previousRow++) {
                context.out[previousRow][col] = context.background;
            }
            context.out[row][col] = m_balls[i].color;
        }

        if (landed >= m_ballCount) {
            context.out.copyFrom(context.to);
            return 0;
        }
        return context.phase + 1;
    }

private:
    Ball *m_balls = nullptr;
    uint8_t m_columns = 0;
    uint8_t m_ballCount = 0;
    uint32_t m_lastMove = 0;
};

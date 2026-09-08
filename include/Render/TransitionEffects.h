#pragma once

#include "Render/Ball.h"
#include "Render/Rain.h"
#include "Render/Snake.h"
#include "Render/TransitionEffect.h"
#include <Arduino.h> // millis()

/*
 * The self contained effects. Each one keeps its own animation state instead of
 * the function-local statics these used to live in.
 */

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

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
        RgbColor color;

        for (uint8_t col = 0; col < context.out.cols(); col++) {
            for (uint8_t row = 0; row < context.out.rows(); row++) {
                color = color.LinearBlend(context.from[row][col],
                                          context.to[row][col], progress);
                context.out[row][col].changeRgb(color);
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

//------------------------------------------------------------------------------

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

//------------------------------------------------------------------------------

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

//------------------------------------------------------------------------------

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

//------------------------------------------------------------------------------

/*
 * Matrix style rain falls over the front while the old face cross fades into
 * the new one. Each column has its own drop with its own speed and gap.
 */
class MatrixRainEffect : public ITransitionEffect {
public:
    ~MatrixRainEffect() override { delete[] m_rain; }

    void resize(uint8_t rows, uint8_t cols) override {
        delete[] m_rain;
        m_rain = (cols > 0) ? new Rain[cols] : nullptr;
        for (uint8_t col = 0; col < cols; col++) {
            m_rain[col] = Rain(rows, cols);
        }
    }

    uint16_t step(TransitionContext &context) override {
        if (m_rain == nullptr) {
            return 0;
        }

        const uint16_t frames = 100;
        const uint8_t rows = context.out.rows();
        const uint8_t cols = context.out.cols();

        if (context.phase == 1) {
            context.delayMs =
                transitionDelayForFrames(frames, context.durationSetting);
            // Fade from what is on the strip right now, not from the stored
            // previous face.
            context.from.copyFrom(context.out);

            const uint8_t brightness = context.foreground.CalculateBrightness();
            for (uint8_t col = 0; col < cols; col++) {
                uint8_t stop = rows - 1;
                for (int8_t row = rows - 1; row >= 0; row--) {
                    if (context.out[row][col].isForeground()) {
                        stop = row;
                        break;
                    }
                }
                m_rain[col].begin(frames, stop, brightness);
            }
        }

        const float progress = static_cast<float>(context.phase) / frames;
        RgbfColor fadeColor;
        RgbaColor rainColor;

        for (uint8_t col = 0; col < cols; col++) {
            for (uint8_t row = 0; row < rows; row++) {
                fadeColor = fadeColor.LinearBlend(
                    context.from[row][col], context.to[row][col], progress);
                rainColor = m_rain[col].get(row);
                rainColor = rainColor.LinearBlend(fadeColor, rainColor,
                                                  rainColor.getAlpha());
                context.out[row][col].changeRgb(rainColor);
            }
        }

        if (context.phase >= frames) {
            // Take all foreground flags from the new face along.
            context.out.copyFrom(context.to);
            return 0;
        }
        return context.phase + 1;
    }

private:
    Rain *m_rain = nullptr;
};

//------------------------------------------------------------------------------

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

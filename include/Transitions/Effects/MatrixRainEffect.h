#pragma once

#include "Transitions/Effects/Rain.h"
#include "Transitions/TransitionEffect.h"

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

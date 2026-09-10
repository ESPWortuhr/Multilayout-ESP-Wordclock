#pragma once

#include "Led.h"
#include "Render/ColorStage.h"
#include "Transitions/TransitionEffect.h"
#include <Arduino.h>

/*
 * The last minute of the year: 59 down to 0, one digit change per second, each
 * digit in its own hue. The clock face is not shown at all while this runs.
 */
class CountdownEffect : public ITransitionEffect {
public:
    uint16_t step(TransitionContext &context) override {
        if (context.time == nullptr) {
            return 0;
        }

        const int8_t second = static_cast<int8_t>(context.time->tm_sec);
        if (second == m_lastSecond) {
            return context.phase;
        }

        if (context.phase == 1) { // Initialize at start of animation
            context.delayMs = 50;
            m_lastSecond = 0;
            m_countDown = 59;
        }
        if (m_countDown < 0) { // Countdown finished?
            m_lastSecond = 0;
            m_countDown = 59;
            return 0;
        }
        m_lastSecond = second;

        context.out.fill(context.background);
        drawNumber(context);

        m_countDown--;
        return context.phase + 1;
    }

private:
    void drawNumber(TransitionContext &context) {
        HsbColor color1 = HsbColor(context.foreground);
        HsbColor color2 = HsbColor(context.foreground);
        color1.H = colorStage.nextHue();
        color2.H = colorStage.nextHue();

        char seconds[8];
        snprintf(seconds, sizeof(seconds), "%d", m_countDown);

        const Led::NumberFont numberFont =
            Led::numberFontFor(context.out.cols(), context.out.rows());
        const unsigned char digit0 = numberFont.glyph(seconds[0]);
        const unsigned char digit1 = numberFont.glyph(seconds[1]);

        const bool isSingleDigit = m_countDown < 10;
        const uint8_t usedWidth =
            isSingleDigit ? numberFont.width : numberFont.width * 2 + 1;
        const uint8_t offsetRow =
            context.out.rows() > numberFont.height
                ? (context.out.rows() - numberFont.height) / 2
                : 0;
        const uint8_t offsetLetter0 = context.out.cols() > usedWidth
                                          ? (context.out.cols() - usedWidth) / 2
                                          : 0;
        const uint8_t offsetLetter1 = offsetLetter0 + numberFont.width + 1;

        for (uint8_t row = 0; row < numberFont.height; row++) {
            for (uint8_t col = 0; col < numberFont.width; col++) {
                if (isSingleDigit) {
                    setPixelForChar(context, col, row, offsetLetter0, offsetRow,
                                    digit0, color1, numberFont.font);
                } else {
                    setPixelForChar(context, col, row, offsetLetter0, offsetRow,
                                    digit0, color1, numberFont.font);
                    setPixelForChar(context, col, row, offsetLetter1, offsetRow,
                                    digit1, color2, numberFont.font);
                }
            }
        }
    }

    static void setPixelForChar(TransitionContext &context, uint8_t col,
                                uint8_t row, uint8_t offsetCol,
                                uint8_t offsetRow, unsigned char character,
                                HsbColor color, fontSize font) {
        if (!led.getCharCol(font, col, row, character)) {
            return;
        }
        if ((row + offsetRow < context.out.rows()) &&
            (col + offsetCol < context.out.cols())) {
            context.out[row + offsetRow][col + offsetCol].changeRgb(color);
        }
    }

    int8_t m_lastSecond = 0;
    int8_t m_countDown = 59;
};

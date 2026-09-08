#pragma once

#include "Font.h"
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

        // fontSize usedFontSize = determineFontSize(); // not applicable due to
        // linkage to digital clock
        fontSize usedFontSize = normalSizeASCII;
        unsigned char digit0 = static_cast<unsigned char>(seconds[0]);
        unsigned char digit1 = static_cast<unsigned char>(seconds[1]);

        if (context.out.cols() <
                (pgm_read_byte(&(fontWidth[usedFontSize])) * 2 + 1) ||
            context.out.rows() < pgm_read_byte(&(fontHeight[usedFontSize]))) {
            usedFontSize = smallSizeNumbers;
            // convert char to int due to different definition in Font.h
            digit0 -= 48;
            digit1 -= 48;
        }

        for (uint8_t row = 0; row < 8; row++) {
            for (uint8_t col = 0; col < 5; col++) {
                if (m_countDown >= 10) {
                    setPixelForChar(context, col, row, 0, digit0, color1,
                                    usedFontSize);
                    setPixelForChar(context, col, row, 6, digit1, color2,
                                    usedFontSize);
                } else {
                    setPixelForChar(context, col, row, 3, digit0, color1,
                                    usedFontSize);
                }
            }
        }
    }

    static void setPixelForChar(TransitionContext &context, uint8_t col,
                                uint8_t row, uint8_t offsetCol,
                                unsigned char character, HsbColor color,
                                fontSize font) {
        if (!led.getCharCol(font, col, row, character)) {
            return;
        }
        // One row down: the digits are not meant to touch the top edge.
        if ((row + 1 < context.out.rows()) &&
            (col + offsetCol < context.out.cols())) {
            context.out[row + 1][col + offsetCol].changeRgb(color);
        }
    }

    int8_t m_lastSecond = 0;
    int8_t m_countDown = 59;
};

#pragma once

#include "Font.h"
#include "Render/ColorStage.h"
#include "Render/Firework.h"
#include "Render/TransitionEffect.h"
#include <Arduino.h>

class FireworkEffect : public ITransitionEffect {
public:
    void resize(uint8_t rows, uint8_t cols) override {
        m_firework.resize(rows, cols);
    }

    uint16_t step(TransitionContext &context) override {
        uint16_t phase = context.phase;

        if (phase == 1) {
            phase = begin(context);
        }

        const bool lastSubPhase = (m_subPhase == BLENDING_FRAMES);
        if (m_subPhase > BLENDING_FRAMES) {
            m_subPhase = 1;
            const uint16_t next = advanceShell(context, phase);
            if (next <= 1) {
                return next; // 1 restarts the sequence, 0 ends it
            }
            phase = next;
        }

        draw(context, phase, lastSubPhase);
        m_subPhase++;
        return phase;
    }

private:
    static const uint8_t BLENDING_FRAMES = 20;

    uint16_t begin(TransitionContext &context) {
        // FIRE_1 .. 6 + 4 = 10 stages
        context.delayMs = transitionDelayForFrames(BLENDING_FRAMES * 10,
                                                   context.durationSetting) /
                          2;
        m_sparkle = false;
        m_subPhase = 1;
        m_firework.prepare(0, m_white, FIRE_1, m_mirrored);

        if ((context.type == BIRTHDAY) &&
            usedClockType->hasSpecialWordHappyBirthday()) {

            context.out.fill(context.background);

            HsbColor hsbColor = HsbColor(context.foreground);
            hsbColor.H = colorStage.nextHue();

            led.clear();
            usedClockType->show(FrontWord::happy_birthday);
            led.setbyFrontMatrix(hsbColor);
            // HAPPY BIRTHDAY in a random hue on black, which is what
            // setbyFrontMatrix() just painted.
            colorStage.build(context.out, RgbfColor(hsbColor, F_FOREGROUND),
                             RgbfColor(0, F_NULL));

            context.to.copyFrom(context.out);
        } else {
            // Blend out of what is currently displayed, into the new face.
            context.from.copyFrom(context.out);
            copyFlags(context.out, context.to);
        }
        return context.phase + 1;
    }

    /* Move on to the next shell. Returns the next phase, or 1 / 0 to restart or
     * end the sequence. */
    uint16_t advanceShell(TransitionContext &context, uint16_t phase) {
        switch (phase) {
        case FIRE_4:
            m_firework.prepare(0, m_red, FIRE_4, m_mirrored);
            break;
        case FIRE_5:
            m_firework.prepare(0, m_red, FIRE_5, m_mirrored);
            break;
        case FIRE_6:
            m_firework.prepare(0, m_red, FIRE_6, m_mirrored);
            break;
        case (FIRE_6 + 1):
            m_firework.prepare(0, m_blue, FIRE_6, m_mirrored);
            m_firework.prepare(1, m_green, FIRE_4, m_mirrored);
            m_firework.prepare(2, m_yellow, FIRE_5, m_mirrored);
            break;
        case (FIRE_6 + 2):
            context.delayMs *= 2;
            m_sparkle = true;
            break;
        case (FIRE_6 + 3):
            break;
        case (FIRE_6 + 4):
            m_mirrored = !m_mirrored;
            context.from.copyFrom(context.to); // 'from' contains artefacts
            if ((context.type == NEWYEAR_FIRE) || (context.type == BIRTHDAY)) {
                // Repeat every 500 ms; the schedule stops it.
                context.delayMs = 500;
                return 1;
            }
            return 0;
        default:
            m_firework.prepare(0, m_white, static_cast<BitmapSymbol>(phase),
                               m_mirrored);
        }
        return phase + 1;
    }

    void draw(TransitionContext &context, uint16_t phase, bool lastSubPhase) {
        float progress = static_cast<float>(m_subPhase) / BLENDING_FRAMES;
        if (progress > 1.f) {
            progress = 1.f;
        }
        float fading = static_cast<float>((phase - FIRE_1) * BLENDING_FRAMES +
                                          m_subPhase) /
                       (BLENDING_FRAMES * (FIRE_6 - FIRE_1 + 2));
        if (fading > 1.f) {
            fading = 1.f;
        }

        RgbColor overlayColor;
        for (uint8_t col = 0; col < context.out.cols(); col++) {
            for (uint8_t row = 0; row < context.out.rows(); row++) {
                const bool overlay =
                    m_firework.getPixel(row, col, overlayColor);
                RgbfColor &cell = context.out[row][col];

                if (m_sparkle) {
                    if (cell.isOverlay()) {
                        // overlayed in previous run
                        cell = context.from[row][col]; // restore color
                        cell.setOverlay(false);
                    } else if (overlay && (random(10) == 0) && !lastSubPhase) {
                        context.from[row][col] = cell; // save color
                        cell.changeRgb(m_white);
                        cell.setOverlay();
                    }
                    continue;
                }

                // blend from -> to
                RgbColor color = color.LinearBlend(
                    context.from[row][col], context.to[row][col], fading);

                if (overlay && !lastSubPhase) {
                    // blend in overlay
                    overlayColor =
                        overlayColor.LinearBlend(color, overlayColor, progress);
                    cell.changeRgb(overlayColor);
                    cell.setOverlay();
                } else if (cell.isOverlay()) {
                    // blend out overlay
                    color = color.LinearBlend(overlayColor, color, progress);
                    cell.changeRgb(color);
                    if (lastSubPhase) {
                        cell.setOverlay(false);
                    }
                } else {
                    // set mix of from and to
                    cell.changeRgb(color);
                }
            }
        }
    }

    static void copyFlags(ColorMatrix &dest, const ColorMatrix &source) {
        for (uint8_t row = 0; row < dest.rows(); row++) {
            for (uint8_t col = 0; col < dest.cols(); col++) {
                dest[row][col].setFlags(source[row][col].getFlags());
            }
        }
    }

    Firework m_firework;
    uint32_t m_subPhase = 1;
    bool m_sparkle = false;
    /* Alternates between runs so consecutive fireworks are not identical. */
    bool m_mirrored = false;

    RgbColor m_white = RgbColor(255, 255, 255);
    RgbColor m_green = RgbColor(0, 255, 0);
    RgbColor m_red = RgbColor(255, 0, 0);
    RgbColor m_blue = RgbColor(0, 0, 255);
    RgbColor m_yellow = RgbColor(255, 255, 0);
};

//------------------------------------------------------------------------------

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

#include "Render/ColorStage.h"
#include "Render/Colorizers.h"
#include "WordClockState.h"
#include <Arduino.h>

//------------------------------------------------------------------------------

/*
 * Random word hues need words to key on, and only the word clock has them. In
 * the digits, a bitmap symbol or the scrolling text every lit pixel carries the
 * same word id, so the whole display would take one random colour and change it
 * on every redraw. Outside the word clock the mode falls back to the plain
 * foreground; the gradient has no such problem and stays available everywhere.
 */
uint8_t ColorStage::mode() const {
    if ((G.colorize == WORD_RANDOM) && !isWordClockMode(G.prog)) {
        return MONOCHROME;
    }
    return G.colorize;
}

//------------------------------------------------------------------------------

bool ColorStage::isColorizing() const { return mode() != MONOCHROME; }

//------------------------------------------------------------------------------

ColorPosition ColorStage::minutePosition() const {
    return (mode() == POLYCHROME) ? GradientEnd : Foreground;
}

//------------------------------------------------------------------------------

bool ColorStage::foregroundIsGradient() const { return mode() == POLYCHROME; }

//------------------------------------------------------------------------------

bool ColorStage::modeChanged() {
    const uint8_t current = mode();
    if (m_lastMode == current) {
        return false;
    }
    m_lastMode = current;
    return true;
}

//------------------------------------------------------------------------------

void ColorStage::displayColors(RgbfColor &foreground,
                               RgbfColor &background) const {
    foreground = RgbfColor(
        led.getColorbyPositionWithAppliedBrightness(Foreground), F_FOREGROUND);
    background = RgbfColor(
        led.getColorbyPositionWithAppliedBrightness(Background), F_NULL);
}

//------------------------------------------------------------------------------

ColorContext ColorStage::contextFor(RgbfColor foreground,
                                    RgbfColor background) const {
    ColorContext context;
    context.foreground = HsbColor(foreground);
    context.background = HsbColor(background);
    context.gradientEnd =
        led.getColorbyPositionWithAppliedBrightness(GradientEnd);
    context.mode = mode();
    return context;
}

//------------------------------------------------------------------------------

void ColorStage::build(ColorMatrix &dest, RgbfColor foreground,
                       RgbfColor background) const {
    for (uint8_t row = 0; row < dest.rows(); row++) {
        for (uint8_t col = 0; col < dest.cols(); col++) {
            dest[row][col] = usedClockType->getFrontMatrixPixel(row, col)
                                 ? foreground
                                 : background;
        }
    }
}

//------------------------------------------------------------------------------

void ColorStage::colorize(ColorMatrix &dest, RgbfColor foreground,
                          RgbfColor background) {
    const ColorContext context = contextFor(foreground, background);
    // What the ramp on the strip was built from. applyColorChange() has
    // nothing else to compare an edit of the secondary colour against.
    m_gradientEnd = RgbfColor(context.gradientEnd, F_NULL);
    colorizerFor(context.mode)->apply(dest, context, m_hues);
}

//------------------------------------------------------------------------------

void ColorStage::render(ColorMatrix &dest, RgbfColor foreground,
                        RgbfColor background) {
    build(dest, foreground, background);
    if (isColorizing()) {
        colorize(dest, foreground, background);
    }
}

//------------------------------------------------------------------------------

/*
 * A colour edit while a transition is configured: the strip is fed from the
 * transition buffers, so rebuilding the clock face would not reach it. All
 * three buffers are patched in place instead - the animation keeps its phase
 * and is not restarted.
 *
 * What "patched" means differs per colour mode, and getting it wrong is
 * invisible in a build log:
 *
 *   - the gradient is derived from the two configured colours, so it has to be
 *     rebuilt outright; rescaling the old hues would keep showing the previous
 *     ramp,
 *   - the random word hues are derived from nothing and must survive - the LDR
 *     drives this path all day, and re-rolling them there would leave the face
 *     flickering through random colours.
 */

bool ColorStage::applyColorChange(ColorMatrix *const *matrices, uint8_t count,
                                  RgbfColor &foreground,
                                  RgbfColor &background) {
    RgbfColor newForeground, newBackground;
    displayColors(newForeground, newBackground);
    const RgbfColor newGradientEnd(
        led.getColorbyPositionWithAppliedBrightness(GradientEnd), F_NULL);

    const bool adjustForeground = newForeground != foreground;
    const bool adjustBackground = newBackground != background;
    // Only the gradient reads the secondary colour, and editing it moves
    // neither foreground nor background - without this the edit would sit
    // unnoticed until the next minute rebuilt the face.
    const bool adjustGradient =
        (mode() == POLYCHROME) && (newGradientEnd != m_gradientEnd);
    m_gradientEnd = newGradientEnd;

    if (!adjustForeground && !adjustBackground && !adjustGradient) {
        return false;
    }

    const float brightness = HsbColor(newForeground).B;
    const bool keepHues = (mode() == WORD_RANDOM);

    for (uint8_t m = 0; m < count; m++) {
        ColorMatrix &matrix = *matrices[m];

        if (adjustBackground) {
            for (uint8_t row = 0; row < matrix.rows(); row++) {
                for (uint8_t col = 0; col < matrix.cols(); col++) {
                    if (!matrix[row][col].isForeground()) {
                        matrix[row][col] = newBackground;
                    }
                }
            }
        }

        if (!adjustForeground && !adjustGradient) {
            continue;
        }

        if (mode() == POLYCHROME) {
            colorize(matrix, newForeground, newBackground);
            continue;
        }

        for (uint8_t row = 0; row < matrix.rows(); row++) {
            for (uint8_t col = 0; col < matrix.cols(); col++) {
                RgbfColor &cell = matrix[row][col];
                if (!cell.isForeground()) {
                    continue;
                }
                if (keepHues) {
                    HsbColor hsbColor = HsbColor(cell);
                    hsbColor.B = brightness;
                    cell.changeRgb(hsbColor);
                } else {
                    cell.changeRgb(newForeground);
                }
            }
        }
    }

    foreground = newForeground;
    background = newBackground;
    return true;
}

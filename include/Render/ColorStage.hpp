#include "Render/ColorStage.h"
#include "Render/Colorizers.h"
#include "WordClockState.h"
#include <Arduino.h>

//------------------------------------------------------------------------------

bool ColorStage::isColorizing() const { return G.colorize != MONOCHROME; }

//------------------------------------------------------------------------------

ColorPosition ColorStage::minutePosition() const {
    return (G.colorize == POLYCHROME) ? GradientEnd : Foreground;
}

//------------------------------------------------------------------------------

bool ColorStage::modeChanged() {
    if (m_lastMode == G.colorize) {
        return false;
    }
    m_lastMode = G.colorize;
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
    context.mode = G.colorize;
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

bool ColorStage::applyColorChange(ColorMatrix *const *matrices, uint8_t count,
                                  RgbfColor &foreground,
                                  RgbfColor &background) const {
    RgbfColor newForeground, newBackground;
    displayColors(newForeground, newBackground);

    const bool adjustForeground = newForeground != foreground;
    const bool adjustBackground = newBackground != background;
    if (!adjustForeground && !adjustBackground) {
        return false;
    }

    const float brightness = HsbColor(newForeground).B;
    const bool keepHues = isColorizing();

    for (uint8_t m = 0; m < count; m++) {
        ColorMatrix &matrix = *matrices[m];
        for (uint8_t row = 0; row < matrix.rows(); row++) {
            for (uint8_t col = 0; col < matrix.cols(); col++) {
                RgbfColor &cell = matrix[row][col];
                if (!cell.isForeground()) {
                    if (adjustBackground) {
                        cell = newBackground;
                    }
                    continue;
                }
                if (!adjustForeground) {
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

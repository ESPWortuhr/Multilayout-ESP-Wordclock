#pragma once

#include "Render/ColorContext.h"
#include "Render/ColorMatrix.h"
#include "Render/HueSequence.h"
#include "WordClockState.h"

class ColorStage {
public:
    /* The colour mode in effect, which is not always the configured one. */
    uint8_t mode() const;
    bool isColorizing() const;
    bool modeChanged();
    ColorPosition minutePosition() const;
    /* True while the foreground is a ramp between two colours, not one. */
    bool foregroundIsGradient() const;
    void displayColors(RgbfColor &foreground, RgbfColor &background) const;
    void render(ColorMatrix &dest, RgbfColor foreground, RgbfColor background);
    void build(ColorMatrix &dest, RgbfColor foreground,
               RgbfColor background) const;
    void colorize(ColorMatrix &dest, RgbfColor foreground,
                  RgbfColor background);
    bool applyColorChange(ColorMatrix *const *matrices, uint8_t count,
                          RgbfColor &foreground, RgbfColor &background);
    float nextHue() { return m_hues.next(); }

private:
    ColorContext contextFor(RgbfColor foreground, RgbfColor background) const;

    HueSequence m_hues;
    uint8_t m_lastMode = MONOCHROME;
    // Neither the foreground nor the background, so a change of it has to be
    // noticed here or nowhere.
    RgbfColor m_gradientEnd;
};

extern ColorStage colorStage;

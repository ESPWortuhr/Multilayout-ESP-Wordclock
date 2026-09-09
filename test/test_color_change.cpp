/*
 * A colour edit has to land on the strip straight away, also while a transition
 * is configured - and without restarting the animation.
 *
 * ColorStage::applyColorChange() is the only path that reaches the transition
 * buffers for a WordclockChanges::Parameters change, so what it does not fix
 * up stays wrong on the wall until the next minute tick.
 *
 * The two directions pull against each other, which is what makes this worth a
 * test: a gradient has to be rebuilt from the configured colours, while random
 * word hues must survive untouched - re-rolling them on every LDR brightness
 * step would make the face flicker through random colours.
 */

#include "WordClockState.h"

// The unity build hands the render stage these globals; the host build has to
// supply stand-ins before ColorStage.hpp is included (see Led.cpp for the same
// pattern).
namespace {
struct FakeLed {
    HsbColor colors[ColorPositionCount];
    HsbColor getColorbyPositionWithAppliedBrightness(ColorPosition p) const {
        return colors[p];
    }
};
struct FakeClockType {
    /* Every cell lit: colouring is what is under test, not the clock face. */
    bool getFrontMatrixPixel(uint8_t, uint8_t) const { return true; }
};
} // namespace

FakeLed led;
FakeClockType clockType;
FakeClockType *usedClockType = &clockType;

#include "Render/ColorStage.hpp"
#include "support/check.h"

GLOBAL G;
ColorStage colorStage;
uint8_t frontWordId[MAX_ROW_SIZE][MAX_COL_SIZE];

namespace {

const uint8_t ROWS = 10;
const uint8_t COLS = 11;

char message[160];

/* Two words side by side, so the random colorizer has something to key on. */
void layOutWords() {
    for (uint8_t row = 0; row < MAX_ROW_SIZE; row++) {
        for (uint8_t col = 0; col < MAX_COL_SIZE; col++) {
            frontWordId[row][col] = static_cast<uint8_t>(1 + (row % 3));
        }
    }
}

void setColor(ColorPosition position, float h, float s, float b) {
    led.colors[position] = HsbColor(h, s, b);
}

RgbfColor configured(ColorPosition position) {
    return RgbfColor(led.colors[position],
                     position == Foreground ? F_FOREGROUND : F_NULL);
}

/* What Led::set() hands the stage: the current foreground and background. */
void displayed(RgbfColor &foreground, RgbfColor &background) {
    colorStage.displayColors(foreground, background);
}

bool sameRgb(const RgbfColor &a, const RgbfColor &b) {
    return a.R == b.R && a.G == b.G && a.B == b.B;
}

/* Three buffers, exactly as Transition::refreshColors() passes them. */
struct Buffers {
    ColorMatrix act, old, work;
    RgbfColor foreground, background;

    void render() {
        act.resize(ROWS, COLS);
        old.resize(ROWS, COLS);
        work.resize(ROWS, COLS);
        displayed(foreground, background);
        colorStage.render(act, foreground, background);
        old.copyFrom(act);
        work.copyFrom(act);
    }

    bool refresh() {
        ColorMatrix *const matrices[3] = {&act, &old, &work};
        return colorStage.applyColorChange(matrices, 3, foreground, background);
    }
};

/*
 * The reference: what the face looks like when built from scratch with the
 * colours that are configured right now.
 */
void renderFresh(ColorMatrix &dest) {
    RgbfColor foreground, background;
    displayed(foreground, background);
    dest.resize(ROWS, COLS);
    colorStage.render(dest, foreground, background);
}

void checkMatchesFreshRender(const Buffers &buffers, const char *what) {
    ColorMatrix fresh;
    renderFresh(fresh);
    for (uint8_t row = 0; row < ROWS; row++) {
        for (uint8_t col = 0; col < COLS; col++) {
            if (!sameRgb(buffers.act[row][col], fresh[row][col])) {
                snprintf(message, sizeof message,
                         "%s: cell %u/%u is %u,%u,%u, a fresh render gives "
                         "%u,%u,%u",
                         what, row, col, buffers.act[row][col].R,
                         buffers.act[row][col].G, buffers.act[row][col].B,
                         fresh[row][col].R, fresh[row][col].G,
                         fresh[row][col].B);
                check(false, message);
                return;
            }
        }
    }
    check(true, what);
}

//------------------------------------------------------------------------------

void monochromeFollowsTheForeground() {
    G.colorize = MONOCHROME;
    setColor(Foreground, .1f, .5f, .9f);
    setColor(Background, 0, 0, 0);

    Buffers buffers;
    buffers.render();

    setColor(Foreground, .7f, .5f, .9f);
    check(buffers.refresh(), "monochrome: a foreground edit is picked up");
    checkMatchesFreshRender(buffers, "monochrome: the new foreground is shown");
}

void polychromeFollowsThePrimary() {
    G.colorize = POLYCHROME;
    setColor(Foreground, .1f, .5f, .9f);
    setColor(GradientEnd, .6f, .5f, .9f);
    setColor(Background, 0, 0, 0);

    Buffers buffers;
    buffers.render();

    setColor(Foreground, .3f, .5f, .9f);
    check(buffers.refresh(), "polychrome: a primary edit is picked up");
    checkMatchesFreshRender(buffers, "polychrome: the new primary starts it");
}

/* The secondary colour moves neither foreground nor background. */
void polychromeFollowsTheSecondary() {
    G.colorize = POLYCHROME;
    setColor(Foreground, .1f, .5f, .9f);
    setColor(GradientEnd, .6f, .5f, .9f);
    setColor(Background, 0, 0, 0);

    Buffers buffers;
    buffers.render();

    setColor(GradientEnd, .9f, .5f, .9f);
    check(buffers.refresh(), "polychrome: a secondary edit is picked up");
    checkMatchesFreshRender(buffers, "polychrome: the new secondary ends it");
}

/* Auto-brightness runs this path all day; the hues have to sit still. */
void randomKeepsItsHues() {
    G.colorize = WORD_RANDOM;
    setColor(Foreground, .1f, .5f, .9f);
    setColor(Background, 0, 0, 0);

    Buffers buffers;
    buffers.render();

    float hues[ROWS][COLS];
    for (uint8_t row = 0; row < ROWS; row++) {
        for (uint8_t col = 0; col < COLS; col++) {
            hues[row][col] = HsbColor(buffers.act[row][col]).H;
        }
    }

    setColor(Foreground, .1f, .5f, .4f);
    check(buffers.refresh(), "random: a brightness change is picked up");

    for (uint8_t row = 0; row < ROWS; row++) {
        for (uint8_t col = 0; col < COLS; col++) {
            const HsbColor cell = HsbColor(buffers.act[row][col]);
            if (cell.H != hues[row][col]) {
                snprintf(message, sizeof message,
                         "random: hue of cell %u/%u moved from %.3f to %.3f",
                         row, col, static_cast<double>(hues[row][col]),
                         static_cast<double>(cell.H));
                check(false, message);
                return;
            }
        }
    }
    check(true, "random: the word hues survive a brightness change");
}

/* All three buffers feed the animation; a missed one shows up mid-blend. */
void everyBufferIsUpdated() {
    G.colorize = POLYCHROME;
    setColor(Foreground, .1f, .5f, .9f);
    setColor(GradientEnd, .6f, .5f, .9f);
    setColor(Background, 0, 0, 0);

    Buffers buffers;
    buffers.render();

    setColor(GradientEnd, .2f, .5f, .9f);
    buffers.refresh();

    for (uint8_t row = 0; row < ROWS; row++) {
        for (uint8_t col = 0; col < COLS; col++) {
            if (!sameRgb(buffers.act[row][col], buffers.old[row][col]) ||
                !sameRgb(buffers.act[row][col], buffers.work[row][col])) {
                snprintf(message, sizeof message,
                         "cell %u/%u differs between the three buffers", row,
                         col);
                check(false, message);
                return;
            }
        }
    }
    check(true, "all three transition buffers are recoloured");
}

/*
 * The very first call after boot, before anything has recorded which secondary
 * colour is on the strip. Guessing "changed" here would recolour and push once
 * for nothing; guessing it from a default would be wrong the other way round
 * as soon as the configured secondary happens to match that default.
 */
void aFreshRenderIsNotAChange() {
    G.colorize = POLYCHROME;
    setColor(Foreground, .1f, .5f, .9f);
    setColor(GradientEnd, .6f, .5f, .9f);
    setColor(Background, 0, 0, 0);

    Buffers buffers;
    buffers.render();

    check(!buffers.refresh(),
          "polychrome: the first redraw after boot is not reported as a "
          "colour change");
}

/* Returning true pushes the strip; doing so on every pass would never stop. */
void unchangedColorsDoNothing() {
    for (uint8_t mode = MONOCHROME; mode <= WORD_RANDOM; mode++) {
        G.colorize = mode;
        setColor(Foreground, .1f, .5f, .9f);
        setColor(GradientEnd, .6f, .5f, .9f);
        setColor(Background, 0, 0, 0);

        Buffers buffers;
        buffers.render();

        snprintf(message, sizeof message,
                 "mode %u: an unchanged palette is not reported as a change",
                 mode);
        check(!buffers.refresh(), message);
    }
}

/* Background is not part of the gradient and has to be tracked separately. */
void backgroundIsUpdatedInEveryMode() {
    for (uint8_t mode = MONOCHROME; mode <= WORD_RANDOM; mode++) {
        G.colorize = mode;
        setColor(Foreground, .1f, .5f, .9f);
        setColor(GradientEnd, .6f, .5f, .9f);
        setColor(Background, 0, 0, 0);

        Buffers buffers;
        buffers.render();

        setColor(Background, .5f, .5f, .2f);
        snprintf(message, sizeof message,
                 "mode %u: a background edit is picked up", mode);
        check(buffers.refresh(), message);

        const RgbfColor expected = configured(Background);
        bool ok = true;
        for (uint8_t row = 0; row < ROWS && ok; row++) {
            for (uint8_t col = 0; col < COLS && ok; col++) {
                if (!buffers.act[row][col].isForeground()) {
                    ok = sameRgb(buffers.act[row][col], expected);
                }
            }
        }
        snprintf(message, sizeof message,
                 "mode %u: unlit cells carry the new background", mode);
        check(ok, message);
    }
}

} // namespace

int main() {
    layOutWords();

    // First, so they run against the state the stage boots in.
    aFreshRenderIsNotAChange();
    unchangedColorsDoNothing();

    monochromeFollowsTheForeground();
    polychromeFollowsThePrimary();
    polychromeFollowsTheSecondary();
    randomKeepsItsHues();
    everyBufferIsUpdated();
    backgroundIsUpdatedInEveryMode();

    return report("colour change");
}

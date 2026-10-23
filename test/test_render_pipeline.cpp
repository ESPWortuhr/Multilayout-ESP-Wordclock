#include "Render/ColorMatrix.h"
#include "WordClockState.h"
#include "WordClockTypes/FrontWord.h"
#include <math.h>
#include <new>
#ifndef pgm_read_word
#define pgm_read_word(addr) (*reinterpret_cast<const uint16_t *>(addr))
#endif

namespace {
const uint8_t ROWS = 10;
const uint8_t COLS = 11;
} // namespace

struct FakeLed {
    HsbColor colors[ColorPositionCount];
    float gain = 1.f;
    ColorMatrix shown;
    HsbColor getColorbyPositionWithAppliedBrightness(ColorPosition p) const {
        HsbColor color = colors[p];
        color.B *= gain;
        return color;
    }
    void setbyColorMatrix(const ColorMatrix &matrix) {
        shown.resize(matrix.rows(), matrix.cols());
        shown.copyFrom(matrix);
    }
    void setbyMinuteArray(ColorPosition) {}
    void setbySecondArray(ColorPosition) {}
    void show() {}
    bool getCharCol(fontSize, uint8_t, uint8_t, unsigned char) { return false; }
    void clear() {}
    void applyMirroringAndReverseIfDefined() {}
};

struct FakeClockType {
    bool getFrontMatrixPixel(uint8_t row, uint8_t col) const {
        return (row % 2) == 0 && col < 6;
    }
    uint8_t rowsWordMatrix() const { return ROWS; }
    uint8_t colsWordMatrix() const { return COLS; }
    bool hasSpecialWordHappyBirthday() const { return false; }
    void show(FrontWord) {}
    bool hasOnlyQuarterLayout() const { return false; }
    bool has60MinuteLayout() const { return false; }
};

FakeLed led;
FakeClockType clockType;
FakeClockType *usedClockType = &clockType;
uint8_t _minute = 0;
uint8_t _hour = 0;

#include "Render/ColorStage.hpp"
#include "Render/RenderPipeline.hpp"
#include "Transitions/Transition.hpp"
#include "support/check.h"

GLOBAL G;
ColorStage colorStage;
RenderPipeline renderPipeline;
uint8_t frontWordId[MAX_ROW_SIZE][MAX_COL_SIZE];

namespace {

char message[200];

struct Snapshot {
    RgbColor cells[ROWS][COLS];
};

void layOutWords() {
    for (uint8_t row = 0; row < MAX_ROW_SIZE; row++) {
        for (uint8_t col = 0; col < MAX_COL_SIZE; col++) {
            frontWordId[row][col] = static_cast<uint8_t>(1 + row);
        }
    }
}

void ledSet(WordclockChanges change) {
    const bool owns = renderPipeline.ownsDisplay();
    const bool minuteChanged = owns ? false : renderPipeline.hasMinuteChanged();
    const DisplayAction action = decideDisplayAction(
        change, owns, renderPipeline.animates(), minuteChanged);
    if (action.showFromLed) {
        ColorMatrix plain;
        plain.resize(ROWS, COLS);
        RgbfColor foreground, background;
        colorStage.displayColors(foreground, background);
        colorStage.build(plain, foreground, background);
        led.setbyColorMatrix(plain);
    }
    renderPipeline.applyDisplayAction(action, _minute);
}

void pump(struct tm &tm) {
    for (int i = 0; i < 400; i++) {
        renderPipeline.loop(tm);
    }
}

void startClock(uint8_t transitionType, struct tm &tm) {
    renderPipeline.~RenderPipeline();
    new (&renderPipeline) RenderPipeline();

    G.prog = COMMAND_IDLE;
    G.colorize = WORD_RANDOM;
    G.transitionType = transitionType;
    G.transitionDuration = 1;
    G.minuteVariant = MinuteVariant::Off;
    G.secondVariant = SecondVariant::Off;
    led.colors[Foreground] = HsbColor(0.f, 1.f, .8f);
    led.colors[Background] = HsbColor(0, 0, 0);
    led.colors[GradientEnd] = HsbColor(.5f, 1.f, .8f);
    led.gain = 1.f;
    _minute = 0;

    renderPipeline.resize(ROWS, COLS);
    ledSet(WordclockChanges::Words);
    pump(tm);
    ledSet(WordclockChanges::Words);
    pump(tm);
}

void setGain(float gain, struct tm &tm) {
    led.gain = gain;
    ledSet(WordclockChanges::Parameters);
    pump(tm);
}

Snapshot snapshot() {
    Snapshot result;
    for (uint8_t row = 0; row < ROWS; row++) {
        for (uint8_t col = 0; col < COLS; col++) {
            result.cells[row][col] = led.shown[row][col];
        }
    }
    return result;
}

int whiteLitCells() {
    int count = 0;
    for (uint8_t row = 0; row < ROWS; row++) {
        for (uint8_t col = 0; col < COLS; col++) {
            const RgbfColor &cell = led.shown[row][col];
            if (clockType.getFrontMatrixPixel(row, col) && cell.R == cell.G &&
                cell.G == cell.B && cell.R > 0) {
                count++;
            }
        }
    }
    return count;
}

float maxHueShift(const Snapshot &before) {
    float worst = 0;
    for (uint8_t row = 0; row < ROWS; row++) {
        for (uint8_t col = 0; col < COLS; col++) {
            if (!clockType.getFrontMatrixPixel(row, col)) {
                continue;
            }
            const HsbColor a(before.cells[row][col]);
            const HsbColor b(led.shown[row][col]);
            float shift = fabsf(a.H - b.H);
            if (shift > .5f) {
                shift = 1.f - shift;
            }
            if (shift > worst) {
                worst = shift;
            }
        }
    }
    return worst;
}

void brightnessSequence(uint8_t transitionType, const float *gains,
                        uint8_t count, const char *label) {
    struct tm tm = {};
    startClock(transitionType, tm);
    const Snapshot initial = snapshot();

    for (uint8_t i = 0; i < count; i++) {
        setGain(gains[i], tm);
    }

    snprintf(message, sizeof message,
             "%s: %d lit cells are white at full brightness", label,
             whiteLitCells());
    check(whiteLitCells() == 0, message);

    snprintf(message, sizeof message, "%s: the word hues moved by %.3f", label,
             static_cast<double>(maxHueShift(initial)));
    check(maxHueShift(initial) < .02f, message);
}

void randomWordsSurviveBrightnessChanges() {
    const float offAndOn[] = {0.f, 1.f};
    const float dimAndBack[] = {.02f, 1.f};
    const float sensorRamp[] = {.5f, .2f, .05f, .02f, .05f, .2f, .5f, 1.f};
    const float smallStep[] = {.95f};

    brightnessSequence(FADE, offAndOn, 2, "transition, 0 % and back");
    brightnessSequence(FADE, dimAndBack, 2, "transition, 2 % and back");
    brightnessSequence(FADE, sensorRamp, 8, "transition, sensor ramp");
    brightnessSequence(NO_TRANSITION, offAndOn, 2,
                       "no transition, 0 % and back");
    brightnessSequence(NO_TRANSITION, smallStep, 1,
                       "no transition, 100 % to 95 %");
    brightnessSequence(NO_TRANSITION, sensorRamp, 8,
                       "no transition, sensor ramp");
}

void aMinuteTickKeepsTheWordHues() {
    struct tm tm = {};
    startClock(NO_TRANSITION, tm);
    const Snapshot initial = snapshot();

    _minute = 1;
    ledSet(WordclockChanges::Minute);
    pump(tm);

    snprintf(message, sizeof message,
             "no transition, minute tick: the word hues moved by %.3f",
             static_cast<double>(maxHueShift(initial)));
    check(maxHueShift(initial) < .02f, message);
}

void newWordsInTheDarkComeBackColoured() {
    const uint8_t types[] = {FADE, NO_TRANSITION};

    for (const uint8_t type : types) {
        struct tm tm = {};
        startClock(type, tm);
        setGain(0.f, tm);
        ledSet(WordclockChanges::Words);
        pump(tm);
        setGain(1.f, tm);

        snprintf(message, sizeof message,
                 "transition type %u, words changed at 0 %%: %d white cells",
                 type, whiteLitCells());
        check(whiteLitCells() == 0, message);
    }
}

} // namespace

int main() {
    layOutWords();
    randomWordsSurviveBrightnessChanges();
    aMinuteTickKeepsTheWordHues();
    newWordsInTheDarkComeBackColoured();
    return report("render pipeline");
}

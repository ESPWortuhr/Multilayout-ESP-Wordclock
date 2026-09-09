/*
 * Every animation has to end, and it has to end on the new clock face.
 *
 * A transition that never returns 0 leaves the clock stuck mid-animation; one
 * that ends on something other than the target face leaves the wrong time on
 * the wall. Neither is visible in a build log.
 */

#include "Transitions/Effects/BallsEffect.h"
#include "Transitions/Effects/FadeEffect.h"
#include "Transitions/Effects/LaserEffect.h"
#include "Transitions/Effects/MatrixRainEffect.h"
#include "Transitions/Effects/ScrollEffect.h"
#include "Transitions/Effects/SnakeEffect.h"
#include "support/check.h"

namespace {

const uint8_t ROWS = 10;
const uint8_t COLS = 11;
/* Far more steps than any effect needs; a runaway animation trips this. */
const uint16_t STEP_LIMIT = 5000;

char message[160];

void fill(ColorMatrix &matrix, RgbfColor color) {
    for (uint8_t row = 0; row < matrix.rows(); row++) {
        for (uint8_t col = 0; col < matrix.cols(); col++) {
            matrix[row][col] = color;
        }
    }
}

/*
 * Colours AND flags. RgbfColor inherits its comparison from RgbColor, which
 * only looks at R, G and B - so comparing with == alone would not notice a lost
 * foreground flag, and the flags are what colorize() and the effects read.
 */
bool equal(const ColorMatrix &a, const ColorMatrix &b) {
    for (uint8_t row = 0; row < a.rows(); row++) {
        for (uint8_t col = 0; col < a.cols(); col++) {
            if (a[row][col] != b[row][col] ||
                a[row][col].getFlags() != b[row][col].getFlags()) {
                return false;
            }
        }
    }
    return true;
}

/* A distinct value per row, so a scroll that maps rows wrongly is visible. */
void fillPerRow(ColorMatrix &matrix) {
    for (uint8_t row = 0; row < matrix.rows(); row++) {
        for (uint8_t col = 0; col < matrix.cols(); col++) {
            matrix[row][col] =
                RgbfColor(static_cast<uint8_t>(100 + row * 10), F_FOREGROUND);
        }
    }
}

struct Run {
    uint16_t steps = 0;
    bool finished = false;
};

Run runToCompletion(ITransitionEffect &effect, ColorMatrix &from,
                    ColorMatrix &to, ColorMatrix &out) {
    RgbfColor foreground(200, F_FOREGROUND);
    RgbfColor background(20, F_NULL);

    Run run;
    uint16_t phase = 1;
    uint32_t delayMs = 100;

    while (phase != 0 && run.steps < STEP_LIMIT) {
        TransitionContext context{from,
                                  to,
                                  out,
                                  foreground,
                                  background,
                                  NO_TRANSITION,
                                  /*time*/ nullptr,
                                  phase,
                                  delayMs,
                                  /*duration*/ 2};
        phase = effect.step(context);
        delayMs = context.delayMs;
        run.steps++;
    }

    run.finished = (phase == 0);
    return run;
}

/*
 * Effects owning per column state must survive a layout change: resize() has to
 * rebuild their arrays, or they index into a buffer of the previous geometry.
 */
void effectsFollowALayoutChange() {
    BallsEffect balls;
    MatrixRainEffect matrixRain;
    SnakeEffect snake;

    const uint8_t geometries[][2] = {{10, 11}, {16, 18}, {8, 8}, {22, 11}};

    for (const auto &geometry : geometries) {
        const uint8_t rows = geometry[0];
        const uint8_t cols = geometry[1];

        balls.resize(rows, cols);
        matrixRain.resize(rows, cols);
        snake.resize(rows, cols);

        ColorMatrix from, to, out;
        from.resize(rows, cols);
        to.resize(rows, cols);
        out.resize(rows, cols);
        fill(from, RgbfColor(20, F_NULL));
        fillPerRow(to);
        fill(out, RgbfColor(200, F_FOREGROUND));

        snprintf(message, sizeof(message),
                 "Balls: %ux%u runs to completion after a resize", rows, cols);
        check(runToCompletion(balls, from, to, out).finished, message);

        fill(out, RgbfColor(200, F_FOREGROUND));
        snprintf(message, sizeof(message),
                 "MatrixRain: %ux%u runs to completion after a resize", rows,
                 cols);
        check(runToCompletion(matrixRain, from, to, out).finished, message);

        fill(out, RgbfColor(200, F_FOREGROUND));
        snprintf(message, sizeof(message),
                 "Snake: %ux%u runs to completion after a resize", rows, cols);
        check(runToCompletion(snake, from, to, out).finished, message);
    }
}

void everyEffectTerminates() {
    FadeEffect fade;
    ScrollVerticalEffect rollUp(false), rollDown(true);
    ScrollHorizontalEffect shiftLeft(false), shiftRight(true);
    LaserEffect laser;
    BallsEffect balls;
    MatrixRainEffect matrixRain;
    SnakeEffect snake;
    balls.resize(ROWS, COLS);
    matrixRain.resize(ROWS, COLS);
    snake.resize(ROWS, COLS);

    struct Case {
        ITransitionEffect *effect;
        const char *name;
        bool endsOnTarget;
    };
    const Case cases[] = {

        {&fade, "Fade", true},
        {&rollUp, "RollUp", true},
        {&rollDown, "RollDown", true},
        {&shiftLeft, "ShiftLeft", true},
        {&shiftRight, "ShiftRight", true},
        {&laser, "Laser", true},
        {&balls, "Balls", true},
        {&matrixRain, "MatrixRain", true},
        // The snake restores the target face behind its tail, but the cells it
        // never crossed keep whatever was in 'out'.
        {&snake, "Snake", false},
    };

    for (const Case &c : cases) {
        ColorMatrix from, to, out;
        from.resize(ROWS, COLS);
        to.resize(ROWS, COLS);
        out.resize(ROWS, COLS);
        fill(from, RgbfColor(20, F_NULL));
        fillPerRow(to);
        fill(out, RgbfColor(20, F_NULL));

        const Run run = runToCompletion(*c.effect, from, to, out);

        snprintf(message, sizeof(message), "%s: the animation ends", c.name);
        check(run.finished, message);

        snprintf(message, sizeof(message), "%s: it takes at least one step",
                 c.name);
        check(run.steps > 0, message);

        if (c.endsOnTarget) {
            snprintf(message, sizeof(message),
                     "%s: it ends on the new clock face", c.name);
            check(equal(out, to), message);
        }
    }
}

/* A scroll must not be instant - it is supposed to slide row by row. */
void scrollingTakesOneStepPerRowOrColumn() {
    ColorMatrix from, to, out;
    from.resize(ROWS, COLS);
    to.resize(ROWS, COLS);
    out.resize(ROWS, COLS);
    fill(from, RgbfColor(20, F_NULL));
    fillPerRow(to);

    ScrollVerticalEffect rollDown(true);
    check(runToCompletion(rollDown, from, to, out).steps == ROWS,
          "a vertical scroll takes one step per row");

    ScrollHorizontalEffect shiftRight(true);
    check(runToCompletion(shiftRight, from, to, out).steps == COLS,
          "a horizontal scroll takes one step per column");
}

/* The pace has to follow the configured duration, not be hard coded. */
void durationSettingChangesThePace() {
    ColorMatrix from, to, out;
    from.resize(ROWS, COLS);
    to.resize(ROWS, COLS);
    out.resize(ROWS, COLS);

    const uint16_t shortRun = transitionDelayForFrames(100, 1);
    const uint16_t mediumRun = transitionDelayForFrames(100, 2);
    const uint16_t longRun = transitionDelayForFrames(100, 3);

    check(shortRun < mediumRun, "a short duration steps faster than medium");
    check(mediumRun < longRun, "a long duration steps slower than medium");
    check(transitionDelayForFrames(0, 2) > 0,
          "zero frames does not divide by zero");
}

} // namespace

int main() {
    everyEffectTerminates();
    effectsFollowALayoutChange();
    scrollingTakesOneStepPerRowOrColumn();
    durationSettingChangesThePace();
    return report("transition effects");
}

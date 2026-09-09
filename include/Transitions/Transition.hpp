#include "Transitions/Transition.h"
#include "WordClockState.h"
#include <Arduino.h>

#define MAX_RANDOM 10

void Transition::allocate(uint8_t rows, uint8_t cols) {
    maxRows = rows;
    maxCols = cols;
    old.resize(rows, cols);
    act.resize(rows, cols);
    work.resize(rows, cols);
    resizeEffects(rows, cols);
}

//------------------------------------------------------------------------------

void Transition::release() {
    old.release();
    act.release();
    work.release();
    maxRows = 0;
    maxCols = 0;
}

//------------------------------------------------------------------------------

Transition::Transition(uint8_t rows, uint8_t cols) { allocate(rows, cols); }

//------------------------------------------------------------------------------

/*
The buffers are the whole point of this object; without them every effect would
write past an allocation that was never made.
*/

bool Transition::valid() const {
    return (old.rows() == maxRows) && (act.rows() == maxRows) &&
           (work.rows() == maxRows) && (maxRows > 0);
}

//------------------------------------------------------------------------------

Transition::~Transition() { release(); }

//------------------------------------------------------------------------------

void Transition::resize(uint8_t rows, uint8_t cols) {
    if (rows == maxRows && cols == maxCols) {
        // Same geometry (e.g. Ger10x11 -> Ger10x11Nero): keep the buffers so
        // the running animation is not interrupted.
        return;
    }

    release();
    allocate(rows, cols);

    // Drop a running animation: its phase counter refers to the old geometry.
    phase = 0;
}

//------------------------------------------------------------------------------

/*
Take over what is currently on the strip. All three buffers hold the same face,
so nothing is animated until the next advanceTo().
*/

void Transition::seed(const ColorMatrix &face, RgbfColor foreground,
                      RgbfColor background) {
    this->foreground = foreground;
    this->background = background;
    copyMatrix(act, face);
    copyMatrix(old, face);
    copyMatrix(work, face);
    phase = 0;
}

//------------------------------------------------------------------------------

/*
A new clock face has been coloured. What was on screen - 'act' - becomes the
source of the blend.

start() has already run when an animation is due, so 'work' is only replaced
while idle: the effects that begin from what is on the strip (balls, matrix
rain, firework, snake) read it in their first step and must find the previous
face there, not the new one.
*/

void Transition::advanceTo(const ColorMatrix &face, RgbfColor foreground,
                           RgbfColor background) {
    copyMatrix(old, act);
    copyMatrix(act, face);
    if (isIdle()) {
        copyMatrix(work, face);
    }
    this->foreground = foreground;
    this->background = background;
}

//------------------------------------------------------------------------------

void Transition::start() {
    nextActionTime = 0;
    phase = 1;
}

//------------------------------------------------------------------------------

void Transition::recolor(RgbfColor foreground, RgbfColor background) {
    colorStage.colorize(isIdle() ? work : act, foreground, background);
}

//------------------------------------------------------------------------------

bool Transition::refreshColors() {
    if (transitionType == BIRTHDAY) {
        return false;
    }
    ColorMatrix *const matrices[3] = {&act, &old, &work};
    return colorStage.applyColorChange(matrices, 3, foreground, background);
}

//------------------------------------------------------------------------------

void Transition::copyMatrix(ColorMatrix &dest, const ColorMatrix &source) {
    dest.copyFrom(source);
}

//------------------------------------------------------------------------------

void Transition::resizeEffects(uint8_t rows, uint8_t cols) {
    ITransitionEffect *effects[] = {
        &fadeEffect,       &rollUpEffect,   &rollDownEffect, &shiftLeftEffect,
        &shiftRightEffect, &laserEffect,    &ballsEffect,    &matrixRainEffect,
        &snakeEffect,      &fireworkEffect, &countdownEffect};

    for (ITransitionEffect *effect : effects) {
        effect->resize(rows, cols);
    }
}

//------------------------------------------------------------------------------

ITransitionEffect *Transition::effectFor(Transition_t type) {
    switch (type) {
    case ROLL_UP:
        return &rollUpEffect;
    case ROLL_DOWN:
        return &rollDownEffect;
    case SHIFT_LEFT:
        return &shiftLeftEffect;
    case SHIFT_RIGHT:
        return &shiftRightEffect;
    case FADE:
        return &fadeEffect;
    case LASER:
        return &laserEffect;
    case BALLS:
        return &ballsEffect;
    case MATRIX_RAIN:
        return &matrixRainEffect;
    case SNAKE:
        return &snakeEffect;
    case FIRE:
    case NEWYEAR_FIRE:
    case BIRTHDAY:
        return &fireworkEffect;
    case NEWYEAR_COUNTDOWN:
        return &countdownEffect;
    default:
        // NO_TRANSITION and RANDOM animate nothing themselves.
        return nullptr;
    }
}

//------------------------------------------------------------------------------

uint16_t Transition::runEffect(ITransitionEffect &effect, struct tm &tm,
                               Transition_t type) {
    TransitionContext context{
        old,  act, work,  foreground,      background,
        type, &tm, phase, transitionDelay, G.transitionDuration};
    const uint16_t next = effect.step(context);
    transitionDelay = context.delayMs;
    return next;
}

//------------------------------------------------------------------------------

/*
One animation step, if the previous one has been on screen long enough. The
effect decides its own pace and reports the next phase, 0 meaning done.
*/

bool Transition::step(struct tm &tm, Transition_t type) {
    transitionType = type;

    const uint32_t now = millis();
    if (isIdle() || (now < nextActionTime)) {
        return false;
    }
    nextActionTime = now + transitionDelay;

    ITransitionEffect *effect = effectFor(type);
    if (effect == nullptr) {
        return false;
    }
    phase = runEffect(*effect, tm, type);
    return true;
}

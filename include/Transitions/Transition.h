#pragma once

#include "Render/ColorMatrix.h"
#include "Render/ColorStage.h"
#include "Transitions/Effects/BallsEffect.h"
#include "Transitions/Effects/CountdownEffect.h"
#include "Transitions/Effects/FadeEffect.h"
#include "Transitions/Effects/FireworkEffect.h"
#include "Transitions/Effects/LaserEffect.h"
#include "Transitions/Effects/MatrixRainEffect.h"
#include "Transitions/Effects/ScrollEffect.h"
#include "Transitions/Effects/SnakeEffect.h"
#include "Transitions/TransitionType.h"

// ###############################################################################

/*
 * One animation between two coloured clock faces.
 *
 * Whether an animation is due at all is decided by RenderPipeline: this object
 * only exists while there is something to animate. It owns nothing but the
 * three buffers it blends between and the effect that does the blending -
 * no scheduling, no colour decisions, no output.
 */
class Transition {
public:
    static const Transition_t transitionTypeFirst = ROLL_UP;
    static const Transition_t transitionTypeLast = SNAKE;

protected:
    uint16_t phase = 0;
    uint32_t transitionDelay = 100;
    Transition_t transitionType = NO_TRANSITION;
    uint32_t nextActionTime = 0;

    /* The two colours the buffers below were built with. */
    RgbfColor foreground = RgbfColor(0, F_FOREGROUND);
    RgbfColor background = RgbfColor(100, F_NULL);

    /*
     * Preallocated members: no heap traffic when the transition type changes,
     * which with RANDOM happens once a minute.
     */
    FadeEffect fadeEffect;
    ScrollVerticalEffect rollUpEffect{false};
    ScrollVerticalEffect rollDownEffect{true};
    ScrollHorizontalEffect shiftLeftEffect{false};
    ScrollHorizontalEffect shiftRightEffect{true};
    LaserEffect laserEffect;
    BallsEffect ballsEffect;
    MatrixRainEffect matrixRainEffect;
    SnakeEffect snakeEffect;
    FireworkEffect fireworkEffect;
    CountdownEffect countdownEffect;

    uint8_t maxRows = 0, maxCols = 0;
    ColorMatrix old;
    ColorMatrix act;
    ColorMatrix work;

protected:
    //------------------------------------------------------------------------------
    // Buffer Management
    //------------------------------------------------------------------------------
    void allocate(uint8_t rows, uint8_t cols);
    void release();

    //------------------------------------------------------------------------------
    // Helper Functions
    //------------------------------------------------------------------------------
    void copyMatrix(ColorMatrix &dest, const ColorMatrix &source);
    /* The effect for a type, or nullptr when the type animates nothing. */
    ITransitionEffect *effectFor(Transition_t type);
    uint16_t runEffect(ITransitionEffect &effect, struct tm &tm,
                       Transition_t type);
    void resizeEffects(uint8_t rows, uint8_t cols);

public:
    Transition(uint8_t rows, uint8_t cols);
    ~Transition();

    /* False when the buffers could not be allocated. */
    bool valid() const;

    /* Adapt the buffers to a new front matrix geometry (clock type change). */
    void resize(uint8_t rows, uint8_t cols);

    //------------------------------------------------------------------------------
    // Driven by RenderPipeline
    //------------------------------------------------------------------------------

    /* Adopt the face that is currently on the strip; nothing to animate yet. */
    void seed(const ColorMatrix &face, RgbfColor foreground,
              RgbfColor background);

    /* A new clock face has been coloured: what was shown becomes the source. */
    void advanceTo(const ColorMatrix &face, RgbfColor foreground,
                   RgbfColor background);

    /* Begin the animation towards the face taken in by advanceTo(). */
    void start();

    bool isIdle() const { return phase == 0; }

    /* Re-apply changed display colours to all three buffers. */
    bool refreshColors();

    /* Re-colour the face in flight after a colour mode change. */
    void recolor(RgbfColor foreground, RgbfColor background);

    /* Advance one step, if the current one has been on screen long enough. */
    bool step(struct tm &tm, Transition_t type);

    /* What is to be shown right now. */
    ColorMatrix &output() { return work; }
};

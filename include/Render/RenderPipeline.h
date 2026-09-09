#pragma once

#include "Render/ColorMatrix.h"
#include "Render/DisplayPolicy.h"
#include "Render/SpecialEvents.h"
#include "Transitions/TransitionType.h"

class Transition;

class RenderPipeline {
public:
    ~RenderPipeline();

    void resize(uint8_t rows, uint8_t cols);
    void init();

    //--------------------------------------------------------------------------
    // What Led::set() asks
    //--------------------------------------------------------------------------

    bool ownsDisplay() const;
    bool animates() const;
    bool hasMinuteChanged();
    void applyDisplayAction(const DisplayAction &action, uint8_t minute);

    //--------------------------------------------------------------------------
    // Main loop
    //--------------------------------------------------------------------------

    void loop(struct tm &tm);
    void demoMode(uint8_t &hour, uint8_t &minute, uint8_t second);

private:
    bool geometryMatchesClockType() const;
    void scheduleType(struct tm &tm);
    Transition_t configuredType();
    bool transitionSettingsChanged();

    bool ensureTransition();
    void releaseTransition();
    void ensureFace();
    void releaseFace();

    void renderFace();
    void present(const ColorMatrix &matrix, bool withMinutesAndFrame);

    void plainStep();
    void transitionStep(struct tm &tm);

    ColorMatrix m_face;
    RgbfColor m_foreground = RgbfColor(0, F_FOREGROUND);
    RgbfColor m_background = RgbfColor(100, F_NULL);

    uint8_t m_rows = 0, m_cols = 0;

    SpecialEvents m_events;
    Transition *m_transition = nullptr;
    Transition_t m_type = NO_TRANSITION;
    Transition_t m_randomPick = ROLL_UP;

    // The clock work has put a clock face into the front matrix. Until then
    // the buffer holds whatever the boot sequence or another mode left there.
    bool m_faceCalculated = false;
    bool m_matrixChanged = false;
    bool m_redrawPending = false;
    bool m_startPending = false;
    bool m_allocationFailed = false;

    uint8_t m_lastMinute = 100;
    Transition_t m_lastType = NO_TRANSITION;
    uint8_t m_lastDemo = 0;
    uint8_t m_lastDuration = 0;
};

extern RenderPipeline renderPipeline;

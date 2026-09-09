#include "Render/RenderPipeline.h"
#include "Transitions/Transition.h"
#include "WordClockState.h"
#include <Arduino.h>

//------------------------------------------------------------------------------
// Special event dates
//------------------------------------------------------------------------------

static bool isBirthday(struct tm &tm) {
    // tm_mday=1..31, tm.tm_mon=0=Jan..11=Dec, tm_year=0=1900..n=1900+n
    for (uint8_t i = 0; i < MAX_BIRTHDAY_COUNT; i++) {
        if ((G.birthday[i].month == tm.tm_mon + 1) &&
            (G.birthday[i].day == tm.tm_mday)) {
            return true;
        }
    }
    return false;
}

//------------------------------------------------------------------------------

static bool isNewYear(struct tm &tm) {
    return (tm.tm_mon == 11) && (tm.tm_mday == 31) && (tm.tm_hour == 23) &&
           (tm.tm_min == 59);
}

//------------------------------------------------------------------------------

RenderPipeline::~RenderPipeline() { delete m_transition; }

//------------------------------------------------------------------------------
// Geometry
//------------------------------------------------------------------------------

void RenderPipeline::resize(uint8_t rows, uint8_t cols) {
    m_rows = rows;
    m_cols = cols;

    if (m_face.rows() != 0) {
        m_face.resize(rows, cols);
    }
    if (m_transition != nullptr) {
        m_transition->resize(rows, cols);
    }
    init();
}

//------------------------------------------------------------------------------

void RenderPipeline::init() { m_matrixChanged = true; }

//------------------------------------------------------------------------------

bool RenderPipeline::geometryMatchesClockType() const {
    return (m_rows == usedClockType->rowsWordMatrix()) &&
           (m_cols == usedClockType->colsWordMatrix());
}

//------------------------------------------------------------------------------

void RenderPipeline::ensureFace() { m_face.resize(m_rows, m_cols); }

//------------------------------------------------------------------------------

void RenderPipeline::releaseFace() { m_face.release(); }

//------------------------------------------------------------------------------
// Scheduling
//------------------------------------------------------------------------------

bool RenderPipeline::animates() const {
    return (G.transitionType != NO_TRANSITION) || m_events.running();
}

//------------------------------------------------------------------------------

bool RenderPipeline::ownsDisplay() const {
    // Before the first face there is nothing to own: Led::set() has to push it
    // itself, which is also what opens the gate below.
    return m_faceCalculated && (animates() || colorStage.isColorizing());
}

//------------------------------------------------------------------------------

Transition_t RenderPipeline::configuredType() {
    if (G.transitionType != RANDOM) {
        return static_cast<Transition_t>(G.transitionType);
    }

    if (m_matrixChanged) {
        m_randomPick = static_cast<Transition_t>(
            random(Transition::transitionTypeFirst,
                   Transition::transitionTypeLast + 1));
    }
    return m_randomPick;
}

//------------------------------------------------------------------------------

void RenderPipeline::scheduleType(struct tm &tm) {
    SpecialEventInput input;
    input.newYear = isNewYear(tm);
    input.birthday = isBirthday(tm);
    input.minute = tm.tm_min;
    input.minuteChanged = hasMinuteChanged();

    const Transition_t event = m_events.update(input);
    m_type = (event != NO_TRANSITION) ? event : configuredType();
}

//------------------------------------------------------------------------------

bool RenderPipeline::transitionSettingsChanged() {
    if ((m_type == m_lastType) && (G.transitionDuration == m_lastDuration) &&
        (G.transitionDemo == m_lastDemo)) {
        return false;
    }
    m_lastType = m_type;
    m_lastDuration = G.transitionDuration;
    m_lastDemo = G.transitionDemo;
    return true;
}

//------------------------------------------------------------------------------
// The optional transition stage
//------------------------------------------------------------------------------

bool RenderPipeline::ensureTransition() {
    if (m_transition != nullptr) {
        return true;
    }
    if ((m_rows == 0) || (m_cols == 0)) {
        return false;
    }

    if (m_allocationFailed && !m_matrixChanged) {
        // Out of heap last time: try again once per clock face change, not on
        // every loop pass, so a low heap is not churned further.
        return false;
    }

    Transition *created = new Transition(m_rows, m_cols);
    if ((created == nullptr) || !created->valid()) {
        // Out of heap: keep showing the clock face without an animation
        // rather than writing into buffers that were never allocated.
        delete created;
        m_allocationFailed = true;
        return false;
    }
    m_allocationFailed = false;

    // Start from what is on the strip right now, so the first change animates
    // out of the current face instead of out of an empty buffer.
    ensureFace();
    renderFace();
    created->seed(m_face, m_foreground, m_background);
    m_transition = created;
    return true;
}

//------------------------------------------------------------------------------

void RenderPipeline::releaseTransition() {
    delete m_transition;
    m_transition = nullptr;
}

//------------------------------------------------------------------------------
// Colour and output
//------------------------------------------------------------------------------

void RenderPipeline::renderFace() {
    ensureFace();
    colorStage.displayColors(m_foreground, m_background);
    colorStage.render(m_face, m_foreground, m_background);
}

//------------------------------------------------------------------------------

void RenderPipeline::present(const ColorMatrix &matrix,
                             bool withMinutesAndFrame) {
    led.setbyColorMatrix(matrix);

    if (withMinutesAndFrame) {
        if (G.minuteVariant != MinuteVariant::Off) {
            led.setbyMinuteArray(colorStage.minutePosition());
        }
        if (G.secondVariant != SecondVariant::Off) {
            led.setbySecondArray(Frame);
        }
    }

    led.show();
}

//------------------------------------------------------------------------------
// Loop helpers
//------------------------------------------------------------------------------

bool RenderPipeline::hasMinuteChanged() {
    if (m_lastMinute != _minute) {
        m_lastMinute = _minute;
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------

void RenderPipeline::applyDisplayAction(const DisplayAction &action,
                                        uint8_t minute) {
    // Led::set() only ever runs for the word clock, so reaching here means the
    // front matrix holds a calculated clock face.
    m_faceCalculated = true;

    if (action.rebuildMatrix) {
        m_matrixChanged = true;
    }

    if (!action.redrawFromTransition) {
        return;
    }

    m_redrawPending = true;

    if (action.startTransition) {
        // The object may not exist yet; loop() starts the animation as soon as
        // it does.
        m_startPending = true;
        if (m_transition != nullptr) {
            m_transition->start();
            m_startPending = false;
        }
    }

    if (action.refreshBrightness) {
        // Only a running animation has to be rescaled in place - the plain
        // face is rebuilt from scratch on the next pass anyway.
        if ((m_transition != nullptr) && m_transition->refreshColors()) {
            led.setbyColorMatrix(m_transition->output());
        }
    } else {
        m_lastMinute = minute;
    }
}

//------------------------------------------------------------------------------

void RenderPipeline::demoMode(uint8_t &_hour, uint8_t &_minute,
                              uint8_t _second) {
    static uint8_t test_second = _second;
    static uint8_t test_minute = _minute;
    static uint8_t test_hour = _hour;

    if (!G.transitionDemo) {
        return;
    }

    // Increment every other second, but only once the animation is done.
    const bool idle = (m_transition == nullptr) || m_transition->isIdle();
    if (idle && ((_second % 2) == 0) && (test_second != _second)) {
        if (usedClockType != nullptr) {
            if (usedClockType->hasOnlyQuarterLayout()) {
                test_minute += 15;
            } else if (usedClockType->has60MinuteLayout()) {
                test_minute += 1;
            } else {
                test_minute += 5;
            }
        }
        if (test_minute >= 60) {
            test_minute %= 60;
            test_hour = (test_hour + 1) % 24;
        }
        Serial.printf("Demo time: %02d:%02d\n", test_hour, test_minute);
    }

    _hour = test_hour;
    _minute = test_minute;
    test_second = _second;
}

//------------------------------------------------------------------------------
// Loop
//------------------------------------------------------------------------------

void RenderPipeline::loop(struct tm &tm) {
    if (!isWordClockMode(G.prog)) {
        // Another mode owns the front matrix now; what it draws there is not a
        // clock face, and it will not be one again until the clock work says
        // so.
        m_faceCalculated = false;
        return;
    }
    if (!pipelineMayRender(G.prog, m_faceCalculated)) {
        return;
    }

    if (!geometryMatchesClockType()) {
        static bool alreadyReported = false;
        if (!alreadyReported) {
            alreadyReported = true;
            Serial.println(F("[ERROR] RenderPipeline: geometry mismatch"));
        }
        return;
    }

    scheduleType(tm);

    if ((m_type == NO_TRANSITION) || !ensureTransition()) {
        releaseTransition();
        plainStep();
        return;
    }

    transitionStep(tm);
}

//------------------------------------------------------------------------------

void RenderPipeline::plainStep() {
    m_matrixChanged = false;
    m_startPending = false;

    bool redraw = false;
    if (m_redrawPending) {
        // Led::set() stepped aside, so this pipeline owes the update.
        m_redrawPending = false;
        redraw = true;
    }
    if (transitionSettingsChanged()) {
        // Among other things this is how the clock face comes back after a
        // special event has finished.
        redraw = true;
    }
    if (colorStage.modeChanged()) {
        redraw = true;
    }

    if (redraw) {
        renderFace();
        present(m_face, true);
    }

    // Keep the buffer while a transition is configured but could not be
    // allocated, so a low heap is not churned further.
    if (!animates()) {
        releaseFace();
    }
}

//------------------------------------------------------------------------------

void RenderPipeline::transitionStep(struct tm &tm) {
    if (transitionSettingsChanged() || m_startPending) {
        m_startPending = false;
        m_transition->start();
    }

    if (m_matrixChanged) {
        m_matrixChanged = false;
        renderFace();
        m_transition->advanceTo(m_face, m_foreground, m_background);
    }
    // A transition pushes the strip on every pass anyway.
    m_redrawPending = false;

    if (colorStage.modeChanged()) {
        m_transition->recolor(m_foreground, m_background);
    }

    m_transition->step(tm, m_type);
    present(m_transition->output(), !m_events.running());
}

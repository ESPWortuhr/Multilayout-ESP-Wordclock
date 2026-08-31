#pragma once

#include "Led.h"
#include "WordClockState.h"

extern Led led;

class SecondsFrame {
private:
    uint8_t numFramePixels;
    uint16_t frameIntervall;
    uint8_t lastSecondFrame;
    uint8_t lastSyncedSecond;
    uint16_t countMillisFrameIntervall;
    uint16_t countMillisSpeed;
    uint16_t previousMillis;

private:
    void setInitFrameSector();
    void frameLogic();
    bool checkIfFrameLoopShouldRun() const;
    void updateMillisCounters();
    bool checkIfFrameInit() const;
    bool checkIftoRunFrameLogic() const;
    uint16_t calcCurrentSecondFrameVariable();
    void resyncToDisplayedTime();
    void handleFrameSectorToggle();
    void handleFrameSector();
    void toggleFrameSectorUpToCurrent();
    bool isFullMinute() const;
    void handleFullMinute();
    void handleSecondFrameChange();
    void updateLedsIfClockworkMode();

public:
    SecondsFrame(const uint8_t num);
    ~SecondsFrame();

    void initFrame();
    void loop();
};

//------------------------------------------------------------------------------

SecondsFrame::~SecondsFrame() {}

//------------------------------------------------------------------------------

SecondsFrame::SecondsFrame(const uint8_t num) {
    numFramePixels = num;
    frameIntervall = 60000 / numFramePixels;
    lastSecondFrame = numFramePixels;
    // 0xFF cannot collide with a second, so the first loop() resyncs.
    lastSyncedSecond = 0xFF;
    countMillisFrameIntervall = 0;
    countMillisSpeed = 0;
    previousMillis = 0;
}

//------------------------------------------------------------------------------
// Helper functions
//------------------------------------------------------------------------------

bool SecondsFrame::checkIfFrameLoopShouldRun() const {
    return usedClockType->numPixelsFrameMatrix() == 0;
}

//------------------------------------------------------------------------------

void SecondsFrame::updateMillisCounters() {
    unsigned long currentMillis = millis();
    countMillisFrameIntervall += currentMillis - previousMillis;
    previousMillis = currentMillis;
}

//------------------------------------------------------------------------------

bool SecondsFrame::checkIfFrameInit() const {
    return G.progInit && G.prog == COMMAND_IDLE;
}

//------------------------------------------------------------------------------

bool SecondsFrame::checkIftoRunFrameLogic() const {
    return countMillisFrameIntervall >= frameIntervall;
}

//------------------------------------------------------------------------------

uint16_t SecondsFrame::calcCurrentSecondFrameVariable() {
    return _second / (60.f / numFramePixels);
}

//------------------------------------------------------------------------------

void SecondsFrame::resyncToDisplayedTime() {
    if (_second == lastSyncedSecond) {
        return;
    }
    lastSyncedSecond = _second;

    const int16_t expected =
        static_cast<int16_t>(calcCurrentSecondFrameVariable());
    int16_t deviation = static_cast<int16_t>(_secondFrame) - expected;

    if (deviation > numFramePixels / 2) {
        deviation -= numFramePixels;
    } else if (deviation < -numFramePixels / 2) {
        deviation += numFramePixels;
    }

    if (deviation >= -1 && deviation <= 1) {
        return;
    }

    _secondFrame = expected;
    lastSecondFrame = _secondFrame;
    countMillisFrameIntervall = 0;
    setInitFrameSector();
    updateLedsIfClockworkMode();
}

//------------------------------------------------------------------------------

void SecondsFrame::handleFrameSectorToggle() {
    if (_minute % 2 == 1) {
        memset(frameArray, true, sizeof(frameArray));
    }
    toggleFrameSectorUpToCurrent();
}

//------------------------------------------------------------------------------

void SecondsFrame::handleFrameSector() { toggleFrameSectorUpToCurrent(); }

//------------------------------------------------------------------------------

void SecondsFrame::toggleFrameSectorUpToCurrent() {
    for (uint8_t i = 0; i <= _secondFrame; i++) {
        frameArray[i] = !frameArray[i];
    }
}

//------------------------------------------------------------------------------

void SecondsFrame::setInitFrameSector() {
    memset(frameArray, false, sizeof(frameArray)); // Clear the frame array

    switch (G.secondVariant) {
    case SecondVariant::FrameSectorToggle:
        handleFrameSectorToggle();
        break;
    case SecondVariant::FrameSector:
        handleFrameSector();
        break;
    case SecondVariant::FrameDot:
        frameArray[_secondFrame] = true;
        break;
    case SecondVariant::Off:
        break;
    default:
        Serial.println("[ERROR] Frame.h - Invalid second variant");
        break;
    }
}

//------------------------------------------------------------------------------

void SecondsFrame::initFrame() {
    led.clearFrame();
    _secondFrame = calcCurrentSecondFrameVariable();
    setInitFrameSector();
    parametersChanged = true;
}

//------------------------------------------------------------------------------

bool SecondsFrame::isFullMinute() const {
    return _secondFrame == numFramePixels;
}

//------------------------------------------------------------------------------

void SecondsFrame::handleFullMinute() {
    led.clearFrame();
    memset(frameArray, false, sizeof(frameArray));
    _secondFrame = 0;
    setInitFrameSector();
}

//------------------------------------------------------------------------------

void SecondsFrame::handleSecondFrameChange() {
    switch (G.secondVariant) {
    case SecondVariant::FrameDot:
        frameArray[_secondFrame] = true;
        if (_secondFrame != 0) {
            frameArray[_secondFrame - 1] = false;
        }
        break;
    case SecondVariant::FrameSector:
    case SecondVariant::FrameSectorToggle:
        frameArray[_secondFrame] = !frameArray[_secondFrame];
        break;
    default:
        break;
    }
}

//------------------------------------------------------------------------------

void SecondsFrame::updateLedsIfClockworkMode() {
    if (G.prog == COMMAND_IDLE && G.conf == COMMAND_IDLE) {
        led.clearFrame();
        parametersChanged = true;
        G.prog = COMMAND_MODE_WORD_CLOCK;
    }
}

//------------------------------------------------------------------------------

void SecondsFrame::frameLogic() {
    countMillisFrameIntervall -= frameIntervall;
    _secondFrame++;

    if (isFullMinute()) {
        handleFullMinute();
        lastSecondFrame = _secondFrame;
        updateLedsIfClockworkMode();
        return;
    }

    if (lastSecondFrame != _secondFrame) {
        handleSecondFrameChange();
        lastSecondFrame = _secondFrame;
    }

    updateLedsIfClockworkMode();
}

//------------------------------------------------------------------------------
// Loop function for SecondsFrame
//------------------------------------------------------------------------------

void SecondsFrame::loop() {
    if (checkIfFrameLoopShouldRun()) {
        return;
    }

    updateMillisCounters();

    if (checkIfFrameInit()) {
        initFrame();
        G.progInit = false;
    }

    resyncToDisplayedTime();

    if (checkIftoRunFrameLogic()) {
        frameLogic();
    }
}

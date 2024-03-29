#pragma once

#include "Uhr.h"
#include "led.h"

extern Led led;

class SecondsFrame {
private:
    uint8_t numFramePixels;
    uint16_t frameIntervall;
    uint8_t lastSecondFrame;
    uint16_t countMillisFrameIntervall;
    uint16_t countMillisSpeed;
    uint16_t previousMillis;

private:
    void setInitFrameSector();
    void frameLogic();

public:
    SecondsFrame(const uint8_t num);
    ~SecondsFrame();

    void setup();
    void loop();
};

//------------------------------------------------------------------------------

SecondsFrame::~SecondsFrame() {}

//------------------------------------------------------------------------------

SecondsFrame::SecondsFrame(const uint8_t num) {
    numFramePixels = num;
    frameIntervall = 60000 / numFramePixels;
    lastSecondFrame = numFramePixels;
    countMillisFrameIntervall = 0;
    countMillisSpeed = 0;
    previousMillis = 0;
}

//------------------------------------------------------------------------------

void SecondsFrame::setInitFrameSector() {
    switch (G.secondVariant) {

    case SecondVariant::FrameSectorToggle:
        if (_minute % 2 == 1) {
            for (uint8_t i = 0; i <= numFramePixels; i++) {
                frameArray |= 1ULL << i; // Setting Bits
            }
        }
        /* intentianally no break */
    case SecondVariant::FrameSector:
        for (uint8_t i = 0; i <= _secondFrame; i++) {
            frameArray ^= 1ULL << i; // Toggling Bits
        }
        break;
    default:
        break;
    }
}

//------------------------------------------------------------------------------

void SecondsFrame::setup() {
    led.clearFrame();
    _secondFrame = _second / (60.f / numFramePixels);
    setInitFrameSector();
    parametersChanged = true;
}

//------------------------------------------------------------------------------

void SecondsFrame::frameLogic() {
    countMillisFrameIntervall = 0;
    _secondFrame++;

    /*Every full minute */
    if (_secondFrame == numFramePixels) {
        led.clearFrame();
        frameArray = 0;
        setInitFrameSector();
        _secondFrame = 0;
    }

    /*Every (Frame-)Second*/
    if (lastSecondFrame != _secondFrame) {

        switch (G.secondVariant) {
        case SecondVariant::FrameDot:
            frameArray |= 1ULL << _secondFrame; // Setting Bit to 1
            if (_secondFrame != 0) {
                frameArray &= ~(1ULL << (_secondFrame - 1)); // Setting Bit to 0
            }
            break;
        case SecondVariant::FrameSector:
        case SecondVariant::FrameSectorToggle:
            frameArray ^= 1ULL << _secondFrame; // Toggle Bit
            break;
        default:
            break;
        }

        lastSecondFrame = _secondFrame;
    }

    /*Update LEDs corrosponding with mode Clockwork*/
    if (G.prog == 0 && G.conf == 0) {
        led.clear();
        parametersChanged = true;
        G.prog = COMMAND_MODE_WORD_CLOCK;
    }
}

//------------------------------------------------------------------------------

void SecondsFrame::loop() {
    unsigned long currentMillis = millis();
    countMillisFrameIntervall += currentMillis - previousMillis;
    previousMillis = currentMillis;

    if (G.progInit && G.prog == 0) {
        setup();
        G.progInit = false;
    }
    if (countMillisFrameIntervall >= frameIntervall) {
        frameLogic();
    }
}
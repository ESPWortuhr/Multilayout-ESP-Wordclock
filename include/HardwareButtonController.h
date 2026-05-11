#pragma once

#include "Uhr.h"
#include <Arduino.h>

enum class HardwareButtonAction : uint8_t {
    None,
    TogglePower,
    NextMode,
    NextTransition,
    IncreaseBrightness,
    NextHue
};

class HardwareButtonController {
private:
    enum ButtonId : uint8_t {
        PowerButton = 0,
        ModeButton = 1,
        SpeedButton = 2,
        ButtonCount = 3
    };

    struct ButtonState {
        bool isPressed = false;
        bool stableLevel = HIGH;
        bool lastLevel = HIGH;
        uint32_t lastChangeMillis = 0;
        uint32_t pressedMillis = 0;
    };

    ButtonState buttons[ButtonCount];
    HardwarePins pins{};

    uint8_t pinFor(ButtonId button) const;
    HardwareButtonAction actionFor(ButtonId button,
                                   uint32_t pressDurationMillis) const;

public:
    void begin(const HardwarePins &hardwarePins);
    HardwareButtonAction loop();
};

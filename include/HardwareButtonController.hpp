#pragma once

#include "HardwareButtonController.h"

static constexpr uint32_t HARDWARE_BUTTON_DEBOUNCE_MS = 40;
static constexpr uint32_t HARDWARE_BUTTON_LONG_PRESS_MS = 2000;

uint8_t HardwareButtonController::pinFor(ButtonId button) const {
    switch (button) {
    case PowerButton:
        return pins.powerButton;
    case ModeButton:
        return pins.modeButton;
    case SpeedButton:
        return pins.speedButton;
    default:
        return pins.powerButton;
    }
}

//------------------------------------------------------------------------------

HardwareButtonAction
HardwareButtonController::actionFor(ButtonId button,
                                    uint32_t pressDurationMillis) const {
    switch (button) {
    case PowerButton:
        return HardwareButtonAction::TogglePower;
    case ModeButton:
        return pressDurationMillis >= HARDWARE_BUTTON_LONG_PRESS_MS
                   ? HardwareButtonAction::NextTransition
                   : HardwareButtonAction::NextMode;
    case SpeedButton:
        return pressDurationMillis >= HARDWARE_BUTTON_LONG_PRESS_MS
                   ? HardwareButtonAction::NextHue
                   : HardwareButtonAction::IncreaseBrightness;
    default:
        return HardwareButtonAction::None;
    }
}

//------------------------------------------------------------------------------

void HardwareButtonController::begin(const HardwarePins &hardwarePins) {
    pins = hardwarePins;

    for (uint8_t i = 0; i < ButtonCount; i++) {
        const auto button = static_cast<ButtonId>(i);
        pinMode(pinFor(button), INPUT_PULLUP);
        buttons[i] = ButtonState{};
        buttons[i].stableLevel = digitalRead(pinFor(button));
        buttons[i].lastLevel = buttons[i].stableLevel;
    }
}

//------------------------------------------------------------------------------

HardwareButtonAction HardwareButtonController::loop() {
    const uint32_t now = millis();

    for (uint8_t i = 0; i < ButtonCount; i++) {
        const auto button = static_cast<ButtonId>(i);
        ButtonState &state = buttons[i];
        const bool level = digitalRead(pinFor(button));

        if (level != state.lastLevel) {
            state.lastLevel = level;
            state.lastChangeMillis = now;
        }

        if ((now - state.lastChangeMillis) < HARDWARE_BUTTON_DEBOUNCE_MS ||
            level == state.stableLevel) {
            continue;
        }

        state.stableLevel = level;
        if (state.stableLevel == LOW) {
            state.isPressed = true;
            state.pressedMillis = now;
        } else if (state.isPressed) {
            state.isPressed = false;
            return actionFor(button, now - state.pressedMillis);
        }
    }

    return HardwareButtonAction::None;
}

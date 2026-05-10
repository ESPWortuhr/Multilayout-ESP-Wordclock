#pragma once

#include <Arduino.h>
#include <NeoPixelBus.h>

class ClockWork {
private:
    enum HardwareButtonId : uint8_t {
        PowerButton = 0,
        ModeButton = 1,
        SpeedButton = 2,
        HardwareButtonCount = 3
    };

    struct HardwareButtonState {
        bool isPressed = false;
        bool stableLevel = HIGH;
        bool lastLevel = HIGH;
        uint32_t lastChangeMillis = 0;
        uint32_t pressedMillis = 0;
    };

    uint16_t countMillisSpeed = 0;
    uint32_t previousMillis = 0;
    uint32_t lux = 0;
    HardwareButtonState hardwareButtons[HardwareButtonCount];
    HsbColor restoredButtonColors[3];
    bool hasRestoredButtonColors = false;

private:
    //------------------------------------------------------------------------------
    // Helper Functions
    //------------------------------------------------------------------------------
    void loopAutoBrightLogic();
    void loopHardwareButtons();
    void handleHardwareButtonPress(HardwareButtonId button,
                                   uint32_t pressDurationMillis);
    uint8_t hardwareButtonPin(HardwareButtonId button) const;
    void toggleHardwareButtonPower();
    void nextHardwareButtonMode();
    void nextHardwareButtonTransition();
    void increaseHardwareButtonBrightness();
    void nextHardwareButtonHue();
    void requestHardwareButtonDisplayRefresh();
    uint32_t num32BitWithOnesAccordingToColumns();

    //------------------------------------------------------------------------------
    // Front Effect Functions
    //------------------------------------------------------------------------------
    void rainbow();
    void rainbowCycle();
    void rainbowSpiralCycle();
    void scrollingText(const char *buf);
    void displaySymbols(BitmapSymbol bitmapSymbolnum);
    void countdownToMidnight();

    //------------------------------------------------------------------------------
    // Minute Functions
    //------------------------------------------------------------------------------
    void showSpecialWordBeen(const uint8_t min);
    void showMinuteInWords(uint8_t min);
    bool checkIfClockHasMinuteInWordsAndItIsSet();
    void showMinute(uint8_t min);
    void checkForValidLayoutVariant();
    void resetMinVariantIfNotAvailable();
    FrontWord getFrontWordForNum(uint8_t min);
    bool checkTwentyUsage();
    bool hasTwentyAndCheckForUsage();
    bool hasDreiviertelAndCheckForUsage();
    void setMinute(uint8_t min, uint8_t &offsetHour, bool &fullHour);

    //------------------------------------------------------------------------------
    // Hour Functions
    //------------------------------------------------------------------------------
    void setHour(uint8_t std, const bool fullHour);

    //------------------------------------------------------------------------------
    // Loop Helper Functions
    //------------------------------------------------------------------------------

    bool isMidnight(const uint8_t hour);
    WordclockChanges changesInClockface();
    void calcClockface();
    void setClock();
    void setItIs(uint8_t min, const uint8_t offsetHour);
    bool DetermineIfItIsIsShown(const uint8_t min);
    void DetermineWhichItIsToShow(uint8_t offsetHour, uint8_t min);
    void clearClockByProgInit();

public:
    // ClockWork() = default;
    ~ClockWork() = default;

    //------------------------------------------------------------------------------
    // Helper Functions
    //------------------------------------------------------------------------------
    iUhrType *getPointer(uint8_t type);
    void initLedStrip(uint8_t num);
    void initHardwareButtons();
    uint32_t getLuxValue() const { return lux; }
    float getAdcValue() const {
        uint16_t adcRaw = analogRead(A0);
        float voltage = (adcRaw * 3.3f) / 1023.0f;
        return round(voltage * 100.0f) / 100.0f;
    }
    uint16_t getAdcRawValue() const { return analogRead(A0); }

    //------------------------------------------------------------------------------
    // Minute Functions
    //------------------------------------------------------------------------------
    uint8_t determineWhichMinuteVariant();

    //------------------------------------------------------------------------------
    // Boot Functions
    //------------------------------------------------------------------------------
    void initBootLed();
    void initBootLedBlink();
    void initBootLedSweep(uint32_t delay);
    void initBootShowIp(const char *buf);

    //------------------------------------------------------------------------------
    // Loop Functions
    //------------------------------------------------------------------------------
    void loop(struct tm &tm);
};

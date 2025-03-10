#pragma once

#include <NeoPixelBus.h>

class ClockWork {
private:
    uint16_t countMillisSpeed = 0;
    uint32_t previousMillis = 0;
    enum class stateBH1750Type {
        toBeInitialized = 0,
        Initialized = 1,
        cannotBeInitialized = 2,
    };
    stateBH1750Type stateBH1750 = stateBH1750Type::toBeInitialized;
    float lux = 0.0;
    float ledGain = 1.0;
    uint16_t adcValue0Lux =
        10; // Hier wird der niedrigste LDR-ADC Wert getrackt,
            // für eine dynamische offset korrektur bei 0 LUX

private:
    //------------------------------------------------------------------------------
    // Helper Functions
    //------------------------------------------------------------------------------
    void initBH1750Logic();
    void loopAutoBrightLogic();
    uint32_t num32BitWithOnesAccordingToColumns();
    bool isRomanLanguage();

    //------------------------------------------------------------------------------
    // Front Effect Functions
    //------------------------------------------------------------------------------
    void rainbow();
    void rainbowCycle();
    void scrollingText(const char *buf);
    void displaySymbols(uint8_t iconNum);
    void countdownToMidnight();

    //------------------------------------------------------------------------------
    // Minute Functions
    //------------------------------------------------------------------------------
    void showSpecialWordBeen(const uint8_t min);
    void showMinuteInWords(uint8_t min);
    bool checkIfClockHasMinuteInWordsAndItIsSet();
    void showMinute(uint8_t min);
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
    float getLuxValue() const { return lux; }
    float getLedGain() const { return ledGain; }
    float getAdcValue() const {
        uint16_t adcRaw = analogRead(A0);
        float voltage = (adcRaw * 3.3f) / 1023.0f;
        return round(voltage * 100.0f) / 100.0f; // Runde auf 2 Nachkommastellen
    }
    uint16_t getAdcRawValue() const {
        return analogRead(A0); // Roher ADC-Wert (0-1023)
    }

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

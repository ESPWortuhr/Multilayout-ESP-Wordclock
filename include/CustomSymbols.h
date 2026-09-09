#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>

#include "WordClockState.h"

class CustomSymbols {
public:
    static constexpr uint8_t MAX_CUSTOM_SYMBOLS = 8;
    static constexpr uint8_t MAX_STORED_SYMBOLS =
        MAX_CUSTOM_SYMBOLS + static_cast<uint8_t>(BitmapSymbol::MAX_BITMAP_SYMBOLS);
    static constexpr uint8_t MAX_NAME_LENGTH = 24;
    static constexpr uint8_t MAX_ROWS = MAX_ROW_SIZE;
    static constexpr uint8_t MAX_COLS = 32;

    bool begin();
    bool select(const char *name);
    bool upsert(const char *name, JsonArrayConst leds, uint8_t rows,
                uint8_t cols);
    bool remove(const char *name);
    bool renderActive() const;
    bool appendLeds(const char *name, JsonArray leds, uint8_t rows,
                    uint8_t cols) const;
    bool appendRowMasks(const char *name, JsonArray bitmap, uint8_t rows,
                        uint8_t cols) const;

    const char *activeName() const;
    bool activeIsCustom() const { return activeCustom >= 0; }
    uint8_t customCount() const { return userDefinedCount(); }
    const char *customName(uint8_t index) const;

    static const char *builtinName(BitmapSymbol symbol);
    static bool findBuiltin(const char *name, BitmapSymbol &symbol);

private:
    struct Entry {
        char name[MAX_NAME_LENGTH + 1] = {0};
        uint8_t rows = 0;
        uint8_t cols = 0;
        uint32_t bitmap[MAX_ROWS] = {0};
    };

    Entry entries[MAX_STORED_SYMBOLS];
    uint8_t count = 0;
    int8_t activeCustom = -1;
    BitmapSymbol activeBuiltin = BitmapSymbol::HEART;

    int8_t findCustom(const char *name) const;
    uint8_t userDefinedCount() const;
    static bool entriesEqual(const Entry &left, const Entry &right);
    bool save() const;
};

extern CustomSymbols customSymbols;

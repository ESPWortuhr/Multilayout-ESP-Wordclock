#include "CustomSymbols.h"

#ifdef ESP8266
#include <LittleFS.h>
#elif defined(ESP32)
#include <LittleFS.h>
#endif

#include "Symbols.h"
#include "WordClockTypes/ClockType.hpp"

extern ClockType *usedClockType;

CustomSymbols customSymbols;

namespace {
constexpr char SYMBOL_FILE[] = "/custom-symbols.jsonl";
constexpr char SYMBOL_TEMP_FILE[] = "/custom-symbols.tmp";
constexpr char SYMBOL_BACKUP_FILE[] = "/custom-symbols.bak";

const char *const BUILTIN_NAMES[] = {"WiFi", "Fire1", "Fire2", "Fire3",
                                    "Fire4", "Fire5", "Fire6", "Heart",
                                    "Smiley", "Note", "Snowflake", "Mail",
                                    "Bell", "Stop", "Standby"};

bool validName(const char *name) {
    if (!name || !name[0] || strlen(name) > CustomSymbols::MAX_NAME_LENGTH)
        return false;
    for (const char *p = name; *p; ++p) {
        if (!(isalnum(static_cast<unsigned char>(*p)) || *p == '-' ||
              *p == '_'))
            return false;
    }
    return true;
}
} // namespace

const char *CustomSymbols::builtinName(BitmapSymbol symbol) {
    return symbol < BitmapSymbol::MAX_BITMAP_SYMBOLS
               ? BUILTIN_NAMES[static_cast<uint8_t>(symbol)]
               : nullptr;
}

bool CustomSymbols::findBuiltin(const char *name, BitmapSymbol &symbol) {
    if (!name)
        return false;
    for (uint8_t i = 0; i < BitmapSymbol::MAX_BITMAP_SYMBOLS; ++i) {
        if (!strcasecmp(name, BUILTIN_NAMES[i])) {
            symbol = static_cast<BitmapSymbol>(i);
            return true;
        }
    }
    return false;
}

int8_t CustomSymbols::findCustom(const char *name) const {
    if (!name)
        return -1;
    for (uint8_t i = 0; i < count; ++i) {
        if (!strcmp(name, entries[i].name))
            return i;
    }
    return -1;
}

bool CustomSymbols::entriesEqual(const Entry &left, const Entry &right) {
    if (strcmp(left.name, right.name) || left.rows != right.rows ||
        left.cols != right.cols)
        return false;
    for (uint8_t row = 0; row < left.rows; ++row) {
        if (left.bitmap[row] != right.bitmap[row])
            return false;
    }
    return true;
}

bool CustomSymbols::begin() {
    count = 0;
    if (!LittleFS.begin()) {
        Serial.println("Symbols: LittleFS mount failed");
        return false;
    }
    if (!LittleFS.exists(SYMBOL_FILE) && LittleFS.exists(SYMBOL_BACKUP_FILE))
        LittleFS.rename(SYMBOL_BACKUP_FILE, SYMBOL_FILE);
    File file = LittleFS.open(SYMBOL_FILE, "r");
    if (!file)
        return true;

    while (file.available() && count < MAX_STORED_SYMBOLS) {
        String line = file.readStringUntil('\n');
        if (line.length() > 512) {
            Serial.println("Symbols: Skipping oversized stored entry");
            continue;
        }
        StaticJsonDocument<512> doc;
        if (deserializeJson(doc, line)) {
            Serial.println("Symbols: Skipping corrupt stored entry");
            continue;
        }
        const char *name = doc["name"] | "";
        const int rows = doc["rows"] | 0;
        const int cols = doc["cols"] | 0;
        JsonArrayConst bitmap = doc["bitmap"].as<JsonArrayConst>();
        if (!validName(name) || rows < 1 || rows > MAX_ROWS || cols < 1 ||
            cols > MAX_COLS || bitmap.size() != static_cast<size_t>(rows)) {
            Serial.println("Symbols: Skipping invalid stored entry");
            continue;
        }
        Entry &entry = entries[count];
        strlcpy(entry.name, name, sizeof(entry.name));
        entry.rows = rows;
        entry.cols = cols;
        bool valid = true;
        for (uint8_t row = 0; row < entry.rows; ++row) {
            JsonVariantConst value = bitmap[row];
            if (!value.is<uint32_t>()) {
                valid = false;
                break;
            }
            entry.bitmap[row] = value.as<uint32_t>();
            if (cols < 32 && (entry.bitmap[row] >> cols) != 0)
                valid = false;
        }
        if (valid)
            ++count;
        else
            Serial.println("Symbols: Skipping invalid bitmap entry");
    }
    return true;
}

bool CustomSymbols::select(const char *name) {
    BitmapSymbol builtin;
    if (findBuiltin(name, builtin)) {
        activeBuiltin = builtin;
        const int8_t overrideIndex = findCustom(name);
        activeCustom = overrideIndex;
        if (overrideIndex >= 0 && usedClockType &&
            (entries[overrideIndex].rows != usedClockType->rowsWordMatrix() ||
             entries[overrideIndex].cols != usedClockType->colsWordMatrix()))
            activeCustom = -1;
        G.bitmapSymbol = builtin;
        return true;
    }
    const int8_t index = findCustom(name);
    if (index < 0)
        return false;
    if (usedClockType &&
        (entries[index].rows != usedClockType->rowsWordMatrix() ||
         entries[index].cols != usedClockType->colsWordMatrix())) {
        Serial.println(
            "Symbols: Custom symbol dimensions do not match current layout");
        return false;
    }
    activeCustom = index;
    return true;
}

bool CustomSymbols::upsert(const char *name, JsonArrayConst leds, uint8_t rows,
                           uint8_t cols) {
    if (!validName(name) || rows < 1 || rows > MAX_ROWS || cols < 1 ||
        cols > MAX_COLS || leds.size() > static_cast<size_t>(rows) * cols) {
        Serial.println("Symbols: Invalid name, dimensions, or LED count");
        return false;
    }
    BitmapSymbol builtin;
    const bool isBuiltin = findBuiltin(name, builtin);

    Entry candidate;
    strlcpy(candidate.name, name, sizeof(candidate.name));
    candidate.rows = rows;
    candidate.cols = cols;
    for (JsonVariantConst value : leds) {
        if (!value.is<int>()) {
            Serial.println("Symbols: LED positions must be integers");
            return false;
        }
        const int position = value.as<int>();
        if (position < 0 || position >= rows * cols) {
            Serial.println("Symbols: LED position outside logical matrix");
            return false;
        }
        const uint8_t row = position / cols;
        const uint8_t col = position % cols;
        candidate.bitmap[row] |= 1UL << col;
    }

    int8_t index = findCustom(name);
    if (index < 0) {
        if (count >= MAX_STORED_SYMBOLS ||
            (!isBuiltin && userDefinedCount() >= MAX_CUSTOM_SYMBOLS)) {
            Serial.println("Symbols: Custom symbol limit reached");
            return false;
        }
        index = count++;
    } else if (entriesEqual(entries[index], candidate)) {
        activeCustom = index;
        return true; // Do not write unchanged data to flash.
    }

    Entry previous = entries[index];
    entries[index] = candidate;
    if (!save()) {
        entries[index] = previous;
        if (index == count - 1 && !previous.name[0])
            --count;
        Serial.println("Symbols: Failed to persist custom symbol");
        return false;
    }
    activeCustom = index;
    return true;
}

bool CustomSymbols::remove(const char *name) {
    if (!validName(name)) {
        Serial.println("Symbols: Invalid name for deletion");
        return false;
    }
    BitmapSymbol builtin;
    const bool isBuiltin = findBuiltin(name, builtin);

    const int8_t index = findCustom(name);
    if (index < 0) {
        // Resetting an untouched built-in is already the desired state.
        return isBuiltin;
    }

    const Entry removed = entries[index];
    for (uint8_t i = index; i + 1 < count; ++i)
        entries[i] = entries[i + 1];
    entries[count - 1] = Entry();
    --count;

    if (!save()) {
        for (uint8_t i = count; i > static_cast<uint8_t>(index); --i)
            entries[i] = entries[i - 1];
        entries[index] = removed;
        ++count;
        Serial.println("Symbols: Failed to persist symbol deletion");
        return false;
    }

    if (activeCustom == index && isBuiltin) {
        activeCustom = -1;
        activeBuiltin = builtin;
        G.bitmapSymbol = builtin;
    } else if (activeCustom == index) {
        activeCustom = -1;
        activeBuiltin = BitmapSymbol::HEART;
        G.bitmapSymbol = BitmapSymbol::HEART;
    } else if (activeCustom > index) {
        --activeCustom;
    }
    return true;
}

bool CustomSymbols::save() const {
    File file = LittleFS.open(SYMBOL_TEMP_FILE, "w");
    if (!file)
        return false;
    for (uint8_t i = 0; i < count; ++i) {
        StaticJsonDocument<512> doc;
        doc["name"] = entries[i].name;
        doc["rows"] = entries[i].rows;
        doc["cols"] = entries[i].cols;
        JsonArray bitmap = doc.createNestedArray("bitmap");
        for (uint8_t row = 0; row < entries[i].rows; ++row)
            bitmap.add(entries[i].bitmap[row]);
        if (serializeJson(doc, file) == 0 || file.println() == 0) {
            file.close();
            LittleFS.remove(SYMBOL_TEMP_FILE);
            return false;
        }
    }
    file.flush();
    if (file.getWriteError()) {
        file.close();
        LittleFS.remove(SYMBOL_TEMP_FILE);
        return false;
    }
    file.close();
    LittleFS.remove(SYMBOL_BACKUP_FILE);
    if (LittleFS.exists(SYMBOL_FILE) &&
        !LittleFS.rename(SYMBOL_FILE, SYMBOL_BACKUP_FILE)) {
        LittleFS.remove(SYMBOL_TEMP_FILE);
        return false;
    }
    if (!LittleFS.rename(SYMBOL_TEMP_FILE, SYMBOL_FILE)) {
        LittleFS.rename(SYMBOL_BACKUP_FILE, SYMBOL_FILE);
        return false;
    }
    LittleFS.remove(SYMBOL_BACKUP_FILE);
    return true;
}

bool CustomSymbols::renderActive() const {
    if (!usedClockType)
        return false;
    const uint8_t targetRows = usedClockType->rowsWordMatrix();
    const uint8_t targetCols = usedClockType->colsWordMatrix();

    if (activeCustom >= 0) {
        const Entry &entry = entries[activeCustom];
        if (entry.rows != targetRows || entry.cols != targetCols) {
            Serial.println(
                "Symbols: Custom symbol dimensions do not match current layout");
            return false;
        }
    }

    for (uint8_t row = 0; row < targetRows; ++row)
        frontMatrix[row] = 0;

    if (activeCustom >= 0) {
        const Entry &entry = entries[activeCustom];
        for (uint8_t row = 0; row < entry.rows; ++row) {
            for (uint8_t col = 0; col < entry.cols; ++col) {
                if (entry.bitmap[row] & (1UL << col))
                    // getFrontMatrixPixel() reads the logical columns in
                    // reverse bit order. Convert the documented row-major
                    // column here so position 0 remains the top-left pixel.
                    usedClockType->setFrontMatrixPixel(
                        row, entry.cols - 1 - col);
            }
        }
        return true;
    }

    const bool compact = targetCols < 11 || targetRows < 10;
    const uint8_t sourceRows = compact ? SYMBOL_8X8_ROWS : SYMBOL_11X10_ROWS;
    const uint8_t sourceCols = compact ? SYMBOL_8X8_COLS : SYMBOL_11X10_COLS;
    const int8_t offsetRow = 0;
    const int8_t offsetCol = (targetCols - sourceCols) / 2;
    for (uint8_t row = 0; row < sourceRows; ++row) {
        const uint16_t rowData = compact
            ? pgm_read_word(&(symbol_8x8[activeBuiltin][row]))
            : pgm_read_word(&(symbol_11x10[activeBuiltin][row]));
        for (uint8_t col = 0; col < sourceCols; ++col) {
            if (rowData & (1U << (sourceCols - col - 1)))
                usedClockType->setFrontMatrixPixel(row + offsetRow,
                                                   col + offsetCol);
        }
    }
    return true;
}

const char *CustomSymbols::activeName() const {
    return activeCustom >= 0 ? entries[activeCustom].name
                             : builtinName(activeBuiltin);
}

const char *CustomSymbols::customName(uint8_t index) const {
    uint8_t current = 0;
    for (uint8_t i = 0; i < count; ++i) {
        BitmapSymbol builtin;
        if (!findBuiltin(entries[i].name, builtin) && current++ == index)
            return entries[i].name;
    }
    return nullptr;
}

uint8_t CustomSymbols::userDefinedCount() const {
    uint8_t result = 0;
    for (uint8_t i = 0; i < count; ++i) {
        BitmapSymbol builtin;
        if (!findBuiltin(entries[i].name, builtin))
            ++result;
    }
    return result;
}

bool CustomSymbols::appendLeds(const char *name, JsonArray leds, uint8_t rows,
                               uint8_t cols) const {
    if (!name || rows < 1 || cols < 1)
        return false;
    const int8_t stored = findCustom(name);
    if (stored >= 0 && entries[stored].rows == rows &&
        entries[stored].cols == cols) {
        for (uint8_t row = 0; row < rows; ++row)
            for (uint8_t col = 0; col < cols; ++col)
                if (entries[stored].bitmap[row] & (1UL << col))
                    leds.add(static_cast<uint16_t>(row) * cols + col);
        return true;
    }

    BitmapSymbol builtin;
    if (!findBuiltin(name, builtin))
        return false;
    const bool compact = cols < 11 || rows < 10;
    const uint8_t sourceRows = compact ? SYMBOL_8X8_ROWS : SYMBOL_11X10_ROWS;
    const uint8_t sourceCols = compact ? SYMBOL_8X8_COLS : SYMBOL_11X10_COLS;
    const int8_t offsetCol = (cols - sourceCols) / 2;
    for (uint8_t row = 0; row < sourceRows && row < rows; ++row) {
        const uint16_t rowData = compact
            ? pgm_read_word(&(symbol_8x8[builtin][row]))
            : pgm_read_word(&(symbol_11x10[builtin][row]));
        for (uint8_t col = 0; col < sourceCols; ++col) {
            const int16_t targetCol = col + offsetCol;
            if (targetCol >= 0 && targetCol < cols &&
                (rowData & (1U << (sourceCols - col - 1))))
                // Custom LED arrays use the matrix bit order. Rendering those
                // arrays reverses the column before setFrontMatrixPixel(), so
                // expose built-ins in that same coordinate system.
                leds.add(static_cast<uint16_t>(row) * cols +
                         (cols - 1 - targetCol));
        }
    }
    return true;
}

bool CustomSymbols::appendRowMasks(const char *name, JsonArray bitmap,
                                   uint8_t rows, uint8_t cols) const {
    if (!name || rows < 1 || cols < 1 || cols > MAX_COLS)
        return false;
    const int8_t stored = findCustom(name);
    if (stored >= 0) {
        if (entries[stored].rows != rows || entries[stored].cols != cols)
            return false;
        for (uint8_t row = 0; row < rows; ++row)
            bitmap.add(entries[stored].bitmap[row]);
        return true;
    }

    BitmapSymbol builtin;
    if (!findBuiltin(name, builtin))
        return false;
    uint32_t rowMasks[MAX_ROWS] = {0};
    const bool compact = cols < 11 || rows < 10;
    const uint8_t sourceRows = compact ? SYMBOL_8X8_ROWS : SYMBOL_11X10_ROWS;
    const uint8_t sourceCols = compact ? SYMBOL_8X8_COLS : SYMBOL_11X10_COLS;
    const int8_t offsetCol = (cols - sourceCols) / 2;
    for (uint8_t row = 0; row < sourceRows && row < rows; ++row) {
        const uint16_t rowData = compact
            ? pgm_read_word(&(symbol_8x8[builtin][row]))
            : pgm_read_word(&(symbol_11x10[builtin][row]));
        for (uint8_t col = 0; col < sourceCols; ++col) {
            const int16_t targetCol = col + offsetCol;
            if (targetCol >= 0 && targetCol < cols &&
                (rowData & (1U << (sourceCols - col - 1))))
                rowMasks[row] |= 1UL << (cols - 1 - targetCol);
        }
    }
    for (uint8_t row = 0; row < rows; ++row)
        bitmap.add(rowMasks[row]);
    return true;
}

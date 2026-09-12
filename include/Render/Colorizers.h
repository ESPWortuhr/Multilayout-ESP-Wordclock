#pragma once

#include "Render/ColorContext.h"
#include "Render/ColorMatrix.h"
#include "Render/HueSequence.h"
#include "WordClockState.h" // frontWordId, MAX_COL_SIZE
#include <math.h>

/*
 * Colouring is its own pipeline stage: the clock work decides which cells are
 * lit, a colorizer decides what colour they get, and only then does a
 * transition blend one matrix into the next. Colouring does not depend on a
 * transition being active.
 *
 * A colorizer works on a whole matrix, so there is one virtual call per frame
 * rather than one per cell.
 */

// Distinct words that can share one clock face. setClock() shows a handful, the
// weather layout is the widest user. Beyond the limit colouring stays correct,
// later words simply get their own value each time.
#define MAX_COLORIZED_WORDS 16

//------------------------------------------------------------------------------

/*
 * Remembers one value per word - a hue, or a position along the gradient - so
 * every cell of a word can be given the same colour no matter how the word is
 * laid out: ZWEI spanning two rows, EINS running down a column.
 */
class WordValues {
public:
    bool find(uint8_t wordId, float &value) const {
        for (uint8_t i = 0; i < m_count; i++) {
            if (m_entries[i].wordId == wordId) {
                value = m_entries[i].value;
                return true;
            }
        }
        return false;
    }

    void add(uint8_t wordId, float value) {
        if (m_count < MAX_COLORIZED_WORDS) {
            m_entries[m_count].wordId = wordId;
            m_entries[m_count].value = value;
            m_count++;
        }
    }

private:
    struct Entry {
        uint8_t wordId;
        float value;
    };
    Entry m_entries[MAX_COLORIZED_WORDS];
    uint8_t m_count = 0;
};

//------------------------------------------------------------------------------

inline uint8_t wordIdAt(uint8_t row, uint8_t col) {
    return (col < MAX_COL_SIZE) ? frontWordId[row][col] : WORD_ID_NONE;
}

//------------------------------------------------------------------------------

class IColorizer {
public:
    virtual ~IColorizer() = default;
    virtual void apply(ColorMatrix &matrix, const ColorContext &context,
                       HueSequence &hues) = 0;
};

//------------------------------------------------------------------------------

/* Every lit cell keeps the configured foreground colour. */
class SolidColorizer : public IColorizer {
public:
    void apply(ColorMatrix &matrix, const ColorContext &context,
               HueSequence &) override {
        for (uint8_t row = 0; row < matrix.rows(); row++) {
            for (uint8_t col = 0; col < matrix.cols(); col++) {
                if (matrix[row][col].isForeground()) {
                    matrix[row][col].changeRgb(context.foreground);
                }
            }
        }
    }
};

//------------------------------------------------------------------------------

class GradientColorizer : public IColorizer {
public:
    void apply(ColorMatrix &matrix, const ColorContext &context,
               HueSequence &) override {
        for (uint8_t row = 0; row < matrix.rows(); row++) {
            const HsbColor rowColor = gradientColorAt(
                context.foreground, context.gradientEnd, row, matrix.rows());

            for (uint8_t col = 0; col < matrix.cols(); col++) {
                if (matrix[row][col].isForeground()) {
                    matrix[row][col].changeRgb(rowColor);
                }
            }
        }
    }
};

//------------------------------------------------------------------------------

class RandomColorizer : public IColorizer {
public:
    void apply(ColorMatrix &matrix, const ColorContext &context,
               HueSequence &hues) override {
        WordValues words;
        HsbColor color = context.foreground;

        for (uint8_t row = 0; row < matrix.rows(); row++) {
            for (uint8_t col = 0; col < matrix.cols(); col++) {
                if (!matrix[row][col].isForeground()) {
                    continue;
                }

                const uint8_t wordId = wordIdAt(row, col);
                float hue;
                if (!words.find(wordId, hue)) {
                    hue = hues.next();
                    words.add(wordId, hue);
                }

                color.H = hue;
                matrix[row][col].changeRgb(color);
            }
        }
    }
};

//------------------------------------------------------------------------------

/*
 * The colorizers are stateless and preallocated: no heap traffic when the mode
 * changes, which on the ESP8266 would otherwise happen on every settings edit.
 */
inline IColorizer *colorizerFor(uint8_t mode) {
    static SolidColorizer solid;
    static GradientColorizer gradient;
    static RandomColorizer random;

    switch (mode) {
    case POLYCHROME:
        return &gradient;
    case WORD_RANDOM:
        return &random;
    case MONOCHROME:
    default:
        return &solid;
    }
}

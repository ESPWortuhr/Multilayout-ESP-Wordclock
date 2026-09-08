#pragma once

#include "Symbols.h"
#include <NeoPixelBus.h>
#include <stdint.h>

/* Mirror the low `graphicCols` bits of a symbol row. */
inline uint16_t reverseBits(uint16_t num, bool mirrored, uint8_t graphicCols) {
    if (!mirrored) {
        return num;
    }
    uint16_t reversed = 0;
    for (uint8_t i = 0; i < graphicCols; i++) {
        if (num & (1 << i)) {
            reversed |= (1 << (graphicCols - 1)) >> i;
        }
    }
    return reversed;
}

class Firework {
public:
    void resize(uint8_t rows, uint8_t cols) {
        m_rows = rows;
        m_cols = cols;
    }

    // layers must be prepared in ascending order !!!
    void prepare(int32_t layer, RgbColor &color, BitmapSymbol newSymbol,
                 bool mirrored) {
        if (layer == 0) {
            m_symbols[1] = static_cast<BitmapSymbol>(0);
            m_symbols[2] = static_cast<BitmapSymbol>(0);
            m_mirrored = mirrored;
        }
        m_colors[layer] = color;
        m_symbols[static_cast<BitmapSymbol>(layer)] = newSymbol;
        m_maxLayer = layer;
    }

    /* True when this cell is covered by the shell; `color` is then its colour.
     */
    bool getPixel(uint8_t row, uint8_t col, RgbColor &color) const {
        if ((row >= 10) || (row >= m_rows) || (col >= 11) || (col >= m_cols)) {
            return false;
        }

        for (int32_t layer = 0; layer <= m_maxLayer; layer++) {
            if (m_symbols[layer] == static_cast<BitmapSymbol>(0)) {
                continue;
            }

            uint16_t pixels;
            if (m_cols < 11 || m_rows < 10) {
                pixels = reverseBits(
                    pgm_read_word(&(symbol_8x8[m_symbols[layer]][row])),
                    m_mirrored, SYMBOL_8X8_COLS);
            } else {
                pixels = reverseBits(
                    pgm_read_word(&(symbol_11x10[m_symbols[layer]][row])),
                    m_mirrored, SYMBOL_11X10_COLS);
            }
            if (pixels & (1 << col)) {
                color = m_colors[layer];
                return true;
            }
        }
        return false;
    }

private:
    RgbColor m_colors[3];
    BitmapSymbol m_symbols[3] = {};
    bool m_mirrored = false;
    int32_t m_maxLayer = 0;
    uint8_t m_rows = 0, m_cols = 0;
};

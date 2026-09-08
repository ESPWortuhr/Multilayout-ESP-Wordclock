#pragma once

#include "Render/RgbfColor.h"
#include <stdint.h>
#include <string.h>

/*
 * A rows x cols grid of RgbfColor, one cell per letter of the front matrix.
 *
 * Two properties matter here:
 *
 * 1. One flat allocation instead of a pointer array plus one array per row.
 *    Fewer heap blocks, no pointer chasing, and copying is a single memcpy.
 *
 * 2. resize() is the only place that (re)allocates, and the frame knows its own
 *    geometry. Everything iterating over a frame must use rows()/cols() and
 *    never ask the clock type - the clock type can change at runtime while the
 *    buffer still has the previous geometry.
 *
 * operator[] hands out a row so existing frame[row][col] code keeps working.
 */
class ColorMatrix {
public:
    ColorMatrix() = default;
    ~ColorMatrix() { release(); }

    // Owning, non-copyable: use copyFrom() to duplicate content.
    ColorMatrix(const ColorMatrix &) = delete;
    ColorMatrix &operator=(const ColorMatrix &) = delete;

    /* Reallocate for a new geometry. Content is discarded, not preserved. */
    void resize(uint8_t rows, uint8_t cols) {
        if (rows == m_rows && cols == m_cols) {
            return;
        }
        release();
        if (rows == 0 || cols == 0) {
            return;
        }
        m_cells = new RgbfColor[static_cast<uint16_t>(rows) * cols];
        if (m_cells == nullptr) {
            return;
        }
        m_rows = rows;
        m_cols = cols;
    }

    void release() {
        delete[] m_cells;
        m_cells = nullptr;
        m_rows = 0;
        m_cols = 0;
    }

    RgbfColor *operator[](uint8_t row) { return &m_cells[row * m_cols]; }
    const RgbfColor *operator[](uint8_t row) const {
        return &m_cells[row * m_cols];
    }

    uint8_t rows() const { return m_rows; }
    uint8_t cols() const { return m_cols; }
    uint16_t cellCount() const {
        return static_cast<uint16_t>(m_rows) * m_cols;
    }

    /* Copy content from a frame of identical geometry; a mismatch is ignored.
     */
    void copyFrom(const ColorMatrix &source) {
        if (source.m_rows != m_rows || source.m_cols != m_cols) {
            return;
        }
        memcpy(m_cells, source.m_cells, cellCount() * sizeof(RgbfColor));
    }

    void fill(RgbfColor color) {
        const uint16_t count = cellCount();
        for (uint16_t i = 0; i < count; i++) {
            m_cells[i] = color;
        }
    }

private:
    RgbfColor *m_cells = nullptr;
    uint8_t m_rows = 0;
    uint8_t m_cols = 0;
};

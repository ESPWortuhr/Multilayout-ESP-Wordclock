#pragma once

#include "Render/ColorMatrix.h"
#include <math.h>
#include <queue>

#define SNAKE_LENGTH 8

class Snake {
public:
    ~Snake() { delete[] m_motions; }

    void resize(uint8_t rows, uint8_t cols) {
        delete[] m_motions;
        m_motions = (rows > 0) ? new GoToPos[rows * 2 + 2] : nullptr;
        m_rows = rows;
        m_cols = cols;
    }

    bool valid() const { return m_motions != nullptr; }

    /* Plan the path across both faces and pick the snake's own colour. */
    void begin(const ColorMatrix &from, const ColorMatrix &to,
               RgbfColor foreground) {
        m_to = &to;

        // The complementary hue, so the snake never disappears into the face
        // it is crossing.
        HsbColor hsbColor = HsbColor(foreground);
        hsbColor.H = fmodf(hsbColor.H + 0.5f, 1.f);
        m_color = RgbfColor(hsbColor, true);

        m_goRight = true;
        m_head = {false, 0, -1};
        m_index = 0;
        while (!m_body.empty()) {
            m_body.pop();
        }

        planMotions(from, false, 0, 1);
        planMotions(to, true, m_rows - 1, -1);
        m_motions[m_index] = {false, -1, 0, 0}; // move out on left side
        m_motions[m_index + 1] = {false, -2, 0,
                                  (int8_t)(m_cols - 1)}; // last on right side
        m_index = 0;
    }

    /* Advance one cell. Returns false once the snake has left the front. */
    bool draw(ColorMatrix &out, RgbfColor background) {
        const bool moving = nextMove();

        if ((moving && (m_body.size() >= SNAKE_LENGTH)) ||
            (!moving && m_body.size())) {
            const Coord tail = m_body.front();
            m_body.pop();
            // Behind the snake the arriving face stays put.
            out[tail.row][tail.col] =
                tail.useTo ? (*m_to)[tail.row][tail.col] : background;
        }
        if (moving) {
            m_body.push(m_head);
            out[m_head.row][m_head.col] = m_color;
        }
        return m_body.size();
    }

private:
    struct Coord {
        bool useTo;
        int8_t row;
        int8_t col;
    };
    struct GoToPos {
        bool useTo;
        int8_t row;
        int8_t min;
        int8_t max;
    };

    void planMotions(const ColorMatrix &matrix, bool useTo, int8_t row,
                     int8_t delta) {
        // order in motions: from -> down, to -> up
        int8_t rowCounter = m_rows;
        while (rowCounter-- > 0) {
            int8_t left = m_cols;
            int8_t right = 0;
            for (int32_t col = 0; col < m_cols; col++) {
                if (matrix[row][col].isForeground()) {
                    // search for right most foreground
                    right = col;
                }
                if (matrix[row][m_cols - 1 - col].isForeground()) {
                    // search for left most foreground
                    left = m_cols - 1 - col;
                }
            }
            if (left <= right) {
                m_motions[m_index] = {useTo, row, left, right};
                if (useTo && (m_index > 0) &&
                    (row == m_motions[m_index - 1].row)) {
                    // don't increment index when the old and the new face have
                    // the same last row
                    m_motions[m_index - 1].useTo = true;
                    m_motions[m_index - 1].min =
                        (left < m_motions[m_index - 1].min)
                            ? left
                            : m_motions[m_index - 1].min;
                    m_motions[m_index - 1].max =
                        (right > m_motions[m_index - 1].max)
                            ? right
                            : m_motions[m_index - 1].max;
                } else {
                    m_index++;
                }
            }
            row += delta;
        }
    }

    bool nextMove() {
        bool verticalMove = true;
        const int8_t left = m_motions[m_index].min;
        const int8_t right = m_motions[m_index].max;
        const int8_t leftNext = m_motions[m_index + 1].min;
        const int8_t rightNext = m_motions[m_index + 1].max;
        const int8_t rowNext = m_motions[m_index + 1].row;

        if (m_head.row == m_motions[m_index].row) {
            if (m_goRight) {
                if (m_head.col < ((right > rightNext) ? right : rightNext)) {
                    m_head.col++;
                    verticalMove = false;
                }
            } else {
                if (m_head.col > ((left < leftNext) ? left : leftNext)) {
                    m_head.col--;
                    verticalMove = false;
                }
            }
        }
        if (verticalMove) {
            if (m_head.row != rowNext) {
                m_head.row =
                    m_head.row < rowNext ? m_head.row + 1 : m_head.row - 1;
            }
            if (m_head.row == rowNext) {
                m_index++;
            }
            m_goRight = m_index % 2 ? false : true;
        }
        m_head.useTo = m_motions[m_index].useTo;
        return (m_head.row >= 0) && (m_head.col >= 0);
    }

    uint8_t m_rows = 0, m_cols = 0;
    GoToPos *m_motions = nullptr;
    Coord m_head = {false, 0, -1};
    bool m_goRight = true;
    int32_t m_index = 0;
    std::queue<Coord> m_body;
    RgbfColor m_color;
    const ColorMatrix *m_to = nullptr;
};

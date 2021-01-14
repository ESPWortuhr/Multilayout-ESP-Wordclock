#pragma once

void Letter_set(const unsigned int i) { Word_array[i] = i; }

class iUhrType {
public:
    virtual void show(uint8_t text) = 0;

    virtual const uint16_t NUM_PIXELS() = 0;

    virtual const uint16_t NUM_SMATRIX() = 0;

    virtual const uint16_t ROWS_MATRIX() = 0;

    virtual const uint16_t COLS_MATRIX() = 0;

    virtual const uint16_t NUM_RMATRIX() = 0;

    virtual const uint16_t getSMatrix(uint16_t index) = 0;

    virtual const uint16_t getRMatrix(uint16_t index) = 0;

    virtual const uint16_t getMatrix(uint8_t col, uint8_t row) = 0;

    virtual const uint16_t getMinArr(uint8_t col, uint8_t row) = 0;
};

#pragma once

void Letter_set(const unsigned int i) { Word_array[i] = i; }

class iUhrType {
public:
    virtual void show(uint8_t text) = 0;

    virtual const uint8_t NUM_PIXELS() = 0;

    virtual const uint8_t NUM_SMATRIX() = 0;

    virtual const uint8_t ROWS_MATRIX() = 0;

    virtual const uint8_t NUM_RMATRIX() = 0;

    virtual const uint8_t getSMatrix(uint8_t index) = 0;

    virtual const uint8_t getRMatrix(uint8_t index) = 0;

    virtual const uint8_t getMatrix(uint8_t col, uint8_t row) = 0;

    virtual const uint8_t getMinArr(uint8_t col, uint8_t row) = 0;
};

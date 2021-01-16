#pragma once

class iUhrType {
public:
    virtual void Letter_set(const uint16_t index) { Word_array[index] = index; }

    virtual void show(uint8_t text) = 0;

    virtual inline const uint16_t NUM_PIXELS() = 0;

    virtual inline const uint16_t NUM_SMATRIX() = 0;

    virtual inline const uint16_t ROWS_MATRIX() = 0;

    virtual inline const uint16_t COLS_MATRIX() = 0;

    virtual inline const uint16_t NUM_RMATRIX() = 0;

    virtual const uint16_t getSMatrix(uint16_t index) = 0;

    virtual const uint16_t getRMatrix(uint16_t index) = 0;

    virtual const uint16_t getFrontMatrix(uint8_t row, uint8_t col) {
        if (row % 2 != 0) {
            col = COLS_MATRIX() - col - 1;
        }
        uint16_t returnValue = col + (row * COLS_MATRIX());
        if (returnValue > NUM_PIXELS()) {
            Serial.println("[ERROR] getMatrix() ReturnValue out of Bounds");
        }
        return returnValue;
    };

    virtual const uint16_t getMinArr(uint8_t col, uint8_t row) = 0;
};

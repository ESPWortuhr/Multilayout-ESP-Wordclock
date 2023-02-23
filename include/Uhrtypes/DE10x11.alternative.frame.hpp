#pragma once

#include "DE10x11.alternative.hpp"

/*
 * Layout Front
 *
 * E S K I S T L F Ü N F
 * Z E H N Z W A N Z I G
 * D R E I V I E R T E L
 * N A C H A P P Y V O R
 * H A L B I R T H D A Y
 * D R Z W Ö L F Ü N F X
 * Z E H N E U N D R E I
 * Z W E I N S I E B E N
 * E L F V I E R A C H T
 * S E C H S I U H R Y E
 *
 */

class De10x11AlternativeFrame_t : public De10x11Alternative_t {
public:
    virtual const uint16_t numPixels() override { return 169; };

    //------------------------------------------------------------------------------

    virtual inline const uint16_t numPixelsFrameMatrix() { return 48; }

    //------------------------------------------------------------------------------

    virtual const bool hasSecondsFrame() override { return true; }

    //------------------------------------------------------------------------------

    virtual const uint16_t getFrameMatrixIndex(uint16_t index) override {
        return 114 + index;
    };

    //------------------------------------------------------------------------------

    virtual const void getMinuteArray(uint16_t *returnArr,
                                      uint8_t col) override {
        for (uint8_t i = 0; i < 4; i++) {
            returnArr[i] = 110 + i;
        }
    };
};

De10x11AlternativeFrame_t _de10x11AlternativeFrame;
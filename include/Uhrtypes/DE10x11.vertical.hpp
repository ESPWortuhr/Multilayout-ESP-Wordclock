#pragma once

#include "DE10x11.2Clock.hpp"

/*
 *           Layout Front
 *                COL
 *       X 9 8 7 6 5 4 3 2 1 0              Minutes 110 111 112 113
 * ROW + - - - - - - - - - - -
 *  0  | E S K I S T A F Ü N F    009 010 029 030 049 050 069 070 089 090 109
 *  1  | Z E H N Z W A N Z I G    008 011 028 031 048 051 068 071 088 091 108
 *  2  | D R E I V I E R T E L    007 012 027 032 047 052 067 072 087 092 107
 *  3  | V O R F U N K N A C H    006 013 026 033 046 053 066 073 086 093 106
 *  4  | H A L B A E L F Ü N F    005 014 025 034 045 054 065 074 085 094 105
 *  5  | E I N S X A M Z W E I    004 015 024 035 044 055 064 075 084 095 104
 *  6  | D R E I A U J V I E R    003 016 023 036 043 056 063 076 083 096 103
 *  7  | S E C H S N L A C H T    002 017 022 037 042 057 062 077 082 097 102
 *  8  | S I E B E N Z W Ö L F    001 018 021 038 041 058 061 078 081 098 101
 *  9  | Z E H N E U N K U H R    000 019 020 039 040 059 060 079 080 099 100
 */

class De10x11Vertical_t : public De10x11Clock_t {
public:
    virtual const uint16_t getFrontMatrixIndex(uint8_t row,
                                               uint8_t col) override {
        if (col % 2 == 0) {
            row = rowsWordMatrix() - 1 - row;
        }
        uint16_t returnValue =
            row + rowsWordMatrix() * (colsWordMatrix() - 1 - col);

        if (returnValue > (rowsWordMatrix() * colsWordMatrix())) {
            Serial.println(
                "[ERROR] getFrontMatrixIndex() returnValue out of Bounds");
        }
        return returnValue;
    };
};

De10x11Vertical_t _de10x11Vertical;
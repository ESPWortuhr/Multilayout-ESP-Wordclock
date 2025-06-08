#pragma once

#include "DE11x11.hpp"

/*
 *           Layout Front
 *                COL
 *       X 9 8 7 6 5 4 3 2 1 0
 * ROW + - - - - - - - - - - -
 *  0  | E S K I S T L F Ü N F
 *  1  | Z E H N Z W A N Z I G
 *  2  | D R E I V I E R T E L
 *  3  | T G N A C H V O R J M
 *  4  | H A L B X Z W Ö L F P
 *  5  | Z W E I N S I E B E N
 *  6  | K D R E I R H F Ü N F
 *  7  | E L F N E U N V I E R
 *  8  | W A C H T Z E H N R S
 *  9  | B S E C H S F M U H R
 *  X  | E V * X * N * S * P I
 */

class De11x11V3_t : public De11x11_t {
public:
    virtual const void getMinuteArray(uint16_t *returnArr,
                                      uint8_t col) override {

        uint16_t numPixelsWordMatrix = rowsWordMatrix() * colsWordMatrix();

        if (G.buildTypeDef == BuildTypeDef::DoubleResM1) {
            numPixelsWordMatrix = rowsWordMatrix() * (colsWordMatrix() * 2 - 1);
        }

        for (uint8_t i = 0; i < 4; i++) {
            switch (col) {
            case 0:                         // LEDs for "LED4x" minute display
                returnArr[i] = 3 + (i * 2); // 3,5,7,9
                break;

            case 1:                           // LEDs for "LED7x" minute display
                returnArr[i] = 113 + (i * 2); // 3,5,7,9
                break;

            case 2: // LEDs für "Corners" type minute display
                returnArr[i] = numPixelsWordMatrix + i;
                break;

            default:
                break;
            }
        }
    };

    //------------------------------------------------------------------------------

    void show(FrontWord word) override {
        switch (word) {

        case FrontWord::es_ist:
            setFrontMatrixWord(0, 9, 10);
            setFrontMatrixWord(0, 5, 7);
            break;

        case FrontWord::viertel:
            setFrontMatrixWord(2, 0, 6);
            break;

        case FrontWord::dreiviertel:
            setFrontMatrixWord(2, 0, 10);
            break;

        case FrontWord::min_5:
            setFrontMatrixWord(0, 0, 3);
            break;

        case FrontWord::min_10:
            setFrontMatrixWord(1, 7, 10);
            break;

        case FrontWord::min_20:
            setFrontMatrixWord(1, 0, 6);
            break;

        case FrontWord::halb:
            setFrontMatrixWord(4, 7, 10);
            break;

        case FrontWord::eins:
            setFrontMatrixWord(5, 5, 8);
            break;

        case FrontWord::nach:
        case FrontWord::v_nach:
            setFrontMatrixWord(3, 5, 8);
            break;

        case FrontWord::vor:
        case FrontWord::v_vor:
            setFrontMatrixWord(3, 2, 4);
            break;

        case FrontWord::uhr:
            setFrontMatrixWord(9, 0, 2);
            break;

        case FrontWord::hour_1:
            setFrontMatrixWord(5, 6, 8);
            break;

        case FrontWord::hour_2:
            setFrontMatrixWord(5, 7, 10);
            break;

        case FrontWord::hour_3:
            setFrontMatrixWord(6, 6, 9);
            break;

        case FrontWord::hour_4:
            setFrontMatrixWord(7, 0, 3);
            break;

        case FrontWord::hour_5:
            setFrontMatrixWord(6, 0, 3);
            break;

        case FrontWord::hour_6:
            setFrontMatrixWord(9, 5, 9);
            break;

        case FrontWord::hour_7:
            setFrontMatrixWord(5, 0, 5);
            break;

        case FrontWord::hour_8:
            setFrontMatrixWord(8, 6, 9);
            break;

        case FrontWord::hour_9:
            setFrontMatrixWord(7, 4, 7);
            break;

        case FrontWord::hour_10:
            setFrontMatrixWord(8, 2, 5);
            break;

        case FrontWord::hour_11:
            setFrontMatrixWord(7, 8, 10);
            break;

        case FrontWord::hour_12:
            setFrontMatrixWord(4, 1, 5);
            break;

        case FrontWord::funk:
            break;

        default:
            break;
        };
    };
};

De11x11V3_t _de11x11V3;

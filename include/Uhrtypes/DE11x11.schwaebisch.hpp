#pragma once

#include "Uhrtype.hpp"

/*
 *           Layout Front
 *                COL
 *       X 9 8 7 6 5 4 3 2 1 0
 * ROW + - - - - - - - - - - -
 *  0  | S T I S C H A F E M F
 *  1  | Z E H N Z W A N O C H
 *  2  | V O R I V I E R T L E
 *  3  | B C H A L B E R A C H
 *  4  | E D R E I V I E R T L
 *  5  | O I S R S I E B E N E
 *  6  | Z W O I K S E C H S E
 *  7  | D R E I S Z W E L F E
 *  8  | V I E R E N F E M F E
 *  9  | A C H D E P N E I N E
 *  X  | Z E H N E U N E L F E
 */

class De11x11schwaebisch_t : public iUhrType {
public:
    virtual LanguageAbbreviation usedLang() override {
        return LanguageAbbreviation::DE;
    };

    //------------------------------------------------------------------------------

    virtual const void getMinuteArray(uint16_t *returnArr,
                                      uint8_t col) override {

        uint16_t numPixelsWordMatrix = rowsWordMatrix() * colsWordMatrix();

        if (G.buildTypeDef == BuildTypeDef::DoubleResM1) {
            numPixelsWordMatrix = rowsWordMatrix() * (colsWordMatrix() * 2 - 1);
        }

        for (uint8_t i = 0; i < 4; i++) {
            switch (col) {
            case 0: // LEDs for "LED4x" minute display
                returnArr[i] = numPixelsWordMatrix - (7 - i);
                break;

            case 1: // LEDs for "LED7x" minute display
                returnArr[i] = numPixelsWordMatrix - (7 - (i * 2));
                break;

            case 2: // LEDs fuer "Corners" type minute display
                returnArr[i] = numPixelsWordMatrix + i;
                break;

            default:
                break;
            }
        }
    };

    //------------------------------------------------------------------------------

    virtual const uint8_t rowsWordMatrix() override { return 11; };

    //------------------------------------------------------------------------------

    virtual const bool hasDreiviertel() override { return true; }

    //------------------------------------------------------------------------------

    void show(FrontWord word) override {
        switch (word) {

        case FrontWord::es_ist:
            setFrontMatrixWord(0, 5, 10);
            break;

        case FrontWord::viertel:
            setFrontMatrixWord(2, 0, 6);
            break;

        case FrontWord::dreiviertel:
            setFrontMatrixWord(4, 0, 9);
            break;

        case FrontWord::min_5:
            setFrontMatrixWord(0, 0, 3);
            break;

        case FrontWord::min_10:
            setFrontMatrixWord(1, 7, 10);
            break;

        case FrontWord::min_20:
            setFrontMatrixWord(1, 3, 6);
            break;

        case FrontWord::halb:
            setFrontMatrixWord(3, 3, 8);
            break;

        case FrontWord::eins:
            setFrontMatrixWord(5, 8, 10);
            break;

        case FrontWord::nach:
        case FrontWord::v_nach:
            setFrontMatrixWord(1, 0, 3);
            break;

        case FrontWord::vor:
        case FrontWord::v_vor:
            setFrontMatrixWord(2, 8, 10);
            break;

        case FrontWord::hour_1:
            setFrontMatrixWord(5, 8, 10);
            break;

        case FrontWord::hour_2:
            setFrontMatrixWord(6, 7, 10);
            break;

        case FrontWord::hour_3:
            setFrontMatrixWord(7, 6, 10);
            break;

        case FrontWord::hour_4:
            setFrontMatrixWord(8, 6, 10);
            break;

        case FrontWord::hour_5:
            setFrontMatrixWord(8, 0, 4);
            break;

        case FrontWord::hour_6:
            setFrontMatrixWord(6, 0, 5);
            break;

        case FrontWord::hour_7:
            setFrontMatrixWord(5, 0, 6);
            break;

        case FrontWord::hour_8:
            setFrontMatrixWord(9, 6, 10);
            break;

        case FrontWord::hour_9:
            setFrontMatrixWord(9, 0, 4);
            break;

        case FrontWord::hour_10:
            setFrontMatrixWord(10, 6, 10);
            break;

        case FrontWord::hour_11:
            setFrontMatrixWord(10, 0, 3);
            break;

        case FrontWord::hour_12:
            setFrontMatrixWord(7, 0, 5);
            break;

        default:
            break;
        };
    };
};

De11x11schwaebisch_t _de11x11schwaebisch;

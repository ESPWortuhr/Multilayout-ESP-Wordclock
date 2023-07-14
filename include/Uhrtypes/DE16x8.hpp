#pragma once

#include "Uhrtype.hpp"

/*
 *                 Layout Front
 *                      COL
 *       5 4 3 2 1 X 9 8 7 6 5 4 3 2 1 0
 * ROW + - - - - - - - - - - - - - - - -
 *  0  | E S H I S T C W L A N Z E I T X
 *  1  | Z W A N Z I G H A L B O Z E H N
 *  2  | F Ü N F V I E R T E L T N A C H
 *  3  | V O R E H A L B D Z E H N E L F
 *  4  | Z W E I D R E I N E U N V I E R
 *  5  | A C H T E I N S E C H S F Ü N F
 *  6  | S I E B E N Z W Ö L F T A U H R
 *  7  | U + D 1 2 3 4 P M I N U T E N X
 */

class De16x8_t : public iUhrType {
public:
    virtual LanguageAbbreviation usedLang() override {
        return LanguageAbbreviation::DE;
    };

    //------------------------------------------------------------------------------

    virtual void setFrontMatrixPixel(const int row, const int col,
                                     bool state = true) {
        if (state) {
            frontMatrix[row] |= 1UL << col;
        } else {
            frontMatrix[row] &= ~(1UL << col);
        }
    }

    //------------------------------------------------------------------------------

    virtual const uint16_t getFrontMatrixIndex(uint8_t row,
                                               uint8_t col) override {

        row *= 2;
        uint8_t newColsWordMatrix = colsWordMatrix();
        uint16_t numPixelsWordMatrix = rowsWordMatrix() * colsWordMatrix() * 2;

        if (G.buildTypeDef == BuildTypeDef::DoubleResM1) {
            newColsWordMatrix = 2 * colsWordMatrix() - 1;
            numPixelsWordMatrix = rowsWordMatrix() * newColsWordMatrix * 2;
            col *= 2;
        }
        if (row % 2 != 0) {
            col = newColsWordMatrix - col - 1;
        }
        uint16_t returnValue = col + (row * newColsWordMatrix);

        if (returnValue > numPixelsWordMatrix) {
            Serial.println(
                "[ERROR] getFrontMatrixIndex() returnValue out of Bounds");
        }

        return returnValue;
    };

    //------------------------------------------------------------------------------

    virtual const uint8_t rowsWordMatrix() override { return 8; };

    //------------------------------------------------------------------------------

    virtual const uint8_t colsWordMatrix() override { return 16; };

    //------------------------------------------------------------------------------

    virtual const bool hasAbsoluteMinLayout() { return true; }

    //------------------------------------------------------------------------------

    virtual const void getMinuteArray(uint16_t *returnArr, uint8_t col) {
        for (uint8_t i = 0; i < 4; i++) {
            returnArr[i] = 236 - i;
        }
    };

    //------------------------------------------------------------------------------

    void show(FrontWord word) override {
        switch (word) {

        case FrontWord::es_ist:
            // Es
            setFrontMatrixWord(0, 14, 15);
            // Ist
            setFrontMatrixWord(0, 10, 12);
            break;

        case FrontWord::nach:
        case FrontWord::v_nach:
            setFrontMatrixWord(2, 0, 3);
            break;

        case FrontWord::vor:
        case FrontWord::v_vor:
            setFrontMatrixWord(3, 13, 15);
            break;

        case FrontWord::viertel:
            setFrontMatrixWord(2, 5, 11);
            break;

        case FrontWord::m_fuenf:
            setFrontMatrixWord(2, 12, 15);
            break;

        case FrontWord::m_zehn:
            setFrontMatrixWord(1, 0, 3);
            break;

        case FrontWord::m_zwanzig:
            setFrontMatrixWord(1, 9, 15);
            break;

        case FrontWord::halb:
            setFrontMatrixWord(3, 8, 11);
            break;

        case FrontWord::eins:
            setFrontMatrixWord(5, 8, 11);
            break;

        case FrontWord::uhr:
            setFrontMatrixWord(6, 0, 2);
            break;

        case FrontWord::h_ein:
            setFrontMatrixWord(4, 7, 9);
            break;

        case FrontWord::h_zwei:
            setFrontMatrixWord(4, 12, 15);
            break;

        case FrontWord::h_drei:
            setFrontMatrixWord(4, 8, 11);
            break;

        case FrontWord::h_vier:
            setFrontMatrixWord(4, 0, 3);
            break;

        case FrontWord::h_fuenf:
            setFrontMatrixWord(5, 0, 3);
            break;

        case FrontWord::h_sechs:
            setFrontMatrixWord(5, 4, 8);
            break;

        case FrontWord::h_sieben:
            setFrontMatrixWord(6, 10, 15);
            break;

        case FrontWord::h_acht:
            setFrontMatrixWord(5, 12, 15);
            break;

        case FrontWord::h_neun:
            setFrontMatrixWord(4, 7, 10);
            break;

        case FrontWord::h_zehn:
            setFrontMatrixWord(3, 3, 6);
            break;

        case FrontWord::h_elf:
            setFrontMatrixWord(3, 0, 2);
            break;

        case FrontWord::h_zwoelf:
            setFrontMatrixWord(6, 5, 9);
            break;

        case FrontWord::minuten:
            setFrontMatrixWord(7, 1, 7);
            break;

        case FrontWord::minute:
            setFrontMatrixWord(7, 2, 7);
            break;

        case FrontWord::plus:
            setFrontMatrixWord(7, 14, 14);
            break;

        default:
            break;
        };
    };
};

De16x8_t _de16x8;
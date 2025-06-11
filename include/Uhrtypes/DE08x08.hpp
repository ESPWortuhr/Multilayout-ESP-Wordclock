#pragma once

#include "Uhrtype.hpp"

/*
 *        Layout Front
 *             COL
 *       7 6 5 4 3 2 1 0
 * ROW + - - - - - - - -
 *  0  | F Ü N F Z E H N
 *  1  | V O R N A C H E
 *  2  | H A L B D R E I
 *  3  | A C H T Z E H N
 *  4  | E L F S E C H S
 *  5  | V I E R N E U N
 *  6  | Z W Ö L F Ü N F
 *  7  | E I S I E B E N
 */

class De08x08_t : public iUhrType {
public:
    virtual LanguageAbbreviation usedLang() override {
        return LanguageAbbreviation::DE;
    };

    //------------------------------------------------------------------------------

    virtual const uint8_t rowsWordMatrix() override { return 8; };

    //------------------------------------------------------------------------------

    virtual const uint8_t colsWordMatrix() override { return 8; };

    //------------------------------------------------------------------------------

    virtual const bool hasTwenty() override { return false; }

    //------------------------------------------------------------------------------

    void show(FrontWord word) override {

        switch (word) {

        case FrontWord::nach:
        case FrontWord::v_nach:
            setFrontMatrixWord(1, 1, 4); // NACH
            break;

        case FrontWord::vor:
        case FrontWord::v_vor:
            setFrontMatrixWord(1, 5, 7); // VOR
            break;

        case FrontWord::min_5:
            setFrontMatrixWord(0, 4, 7); // FÜNF
            break;

        case FrontWord::min_10:
            setFrontMatrixWord(0, 0, 3); // ZEHN
            break;

        case FrontWord::viertel:
            setFrontMatrixWord(0, 0, 7); // FÜNFZEHN
            break;

        case FrontWord::halb:
            setFrontMatrixWord(2, 4, 7); // HALB
            break;

        case FrontWord::eins:
        case FrontWord::hour_1:
            setFrontMatrixPixel(1, 0); // E
            setFrontMatrixPixel(2, 0); // I
            setFrontMatrixPixel(3, 0); // N
            setFrontMatrixPixel(4, 0); // S
            break;

        case FrontWord::hour_2:
            setFrontMatrixWord(6, 6, 7); // ZW
            setFrontMatrixWord(7, 6, 7); // EI
            break;

        case FrontWord::hour_3:
            setFrontMatrixWord(2, 0, 3); // DREI
            break;

        case FrontWord::hour_4:
            setFrontMatrixWord(5, 4, 7); // VIER
            break;

        case FrontWord::hour_5:
            setFrontMatrixWord(6, 0, 3); // FÜNF
            break;

        case FrontWord::hour_6:
            setFrontMatrixWord(4, 0, 4); // SECHS
            break;

        case FrontWord::hour_7:
            setFrontMatrixWord(7, 0, 5); // SIEBEN
            break;

        case FrontWord::hour_8:
            setFrontMatrixWord(3, 4, 7); // ACHT
            break;

        case FrontWord::hour_9:
            setFrontMatrixWord(5, 0, 3); // NEUN
            break;

        case FrontWord::hour_10:
            setFrontMatrixWord(3, 0, 3); // ZEHN
            break;

        case FrontWord::hour_11:
            setFrontMatrixWord(4, 5, 7); // ELF
            break;

        case FrontWord::hour_12:
            setFrontMatrixWord(6, 3, 7); // ZWÖLF
            break;

        default:
            break;
        };
    };
};

De08x08_t _de08x08;
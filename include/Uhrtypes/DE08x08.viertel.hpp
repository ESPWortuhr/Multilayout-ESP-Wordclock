#pragma once

#include "Uhrtype.hpp"

/*
 *        Layout Front
 *             COL
 *       7 6 5 4 3 2 1 0
 * ROW + - - - - - - - -
 *  0  | V I E R T E L N
 *  1  | H A L B V O R A
 *  2  | Z E H N E U N C
 *  3  | D R E I N S B H
 *  4  | Z W Ö L F Ü N F
 *  5  | E I S I E B E N
 *  6  | S E C H S E L F
 *  7  | V I E R A C H T
 */

class De08x08Viertel_t : public iUhrType {
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

    virtual const bool hasOnlyQuarterLayout() override { return true; }

    //------------------------------------------------------------------------------

    void show(FrontWord word) override {

        switch (word) {

        case FrontWord::nach:
        case FrontWord::v_nach:
            setFrontMatrixPixel(0, 0); // n
            setFrontMatrixPixel(1, 0); // a
            setFrontMatrixPixel(2, 0); // c
            setFrontMatrixPixel(3, 0); // h
            break;

        case FrontWord::vor:
        case FrontWord::v_vor:
            setFrontMatrixWord(1, 1, 3); // vor
            break;

        case FrontWord::viertel:
            setFrontMatrixWord(0, 1, 7); // viertel
            break;

        case FrontWord::halb:
            setFrontMatrixWord(1, 4, 7); // halb
            break;

        case FrontWord::eins:
            setFrontMatrixWord(3, 2, 5);
            break;

        case FrontWord::hour_1:
            setFrontMatrixWord(3, 3, 5);
            break;

        case FrontWord::hour_2:
            setFrontMatrixWord(4, 6, 7);
            setFrontMatrixWord(5, 6, 7);
            break;

        case FrontWord::hour_3:
            setFrontMatrixWord(3, 4, 7);
            break;

        case FrontWord::hour_4:
            setFrontMatrixWord(7, 4, 7);
            break;

        case FrontWord::hour_5:
            setFrontMatrixWord(4, 0, 3);
            break;

        case FrontWord::hour_6:
            setFrontMatrixWord(6, 3, 7);
            break;

        case FrontWord::hour_7:
            setFrontMatrixWord(5, 0, 5);
            break;

        case FrontWord::hour_8:
            setFrontMatrixWord(7, 0, 3);
            break;

        case FrontWord::hour_9:
            setFrontMatrixWord(2, 1, 4);
            break;

        case FrontWord::hour_10:
            setFrontMatrixWord(2, 4, 7);
            break;

        case FrontWord::hour_11:
            setFrontMatrixWord(6, 0, 2);
            break;

        case FrontWord::hour_12:
            setFrontMatrixWord(4, 3, 7);
            break;

        default:
            break;
        };
    };
};

De08x08Viertel_t _de08x08Viertel;
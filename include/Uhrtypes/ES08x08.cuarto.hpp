#pragma once

#include "Uhrtype.hpp"

/*
 *        Layout Front
 *             COL
 *       7 6 5 4 3 2 1 0
 * ROW + - - - - - - - -
 *  0  | S E I S I E T E
 *  1  | C I N C O C H O
 *  2  | N U E V E U N A
 *  3  | C U A T R O D O
 *  4  | T R E S O N C E
 *  5  | D O S D I E Z Y
 *  6  | M E N O S C U A
 *  7  | M E D I A R T O
 */

class Es08x08Cuarto_t : public iUhrType {
public:
    virtual LanguageAbbreviation usedLang() override {
        return LanguageAbbreviation::ES;
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
            break;

        case FrontWord::eins:
        case FrontWord::hour_1:
            setFrontMatrixWord(2, 0, 2);
            break;

        case FrontWord::hour_2:
            setFrontMatrixWord(5, 5, 7);
            break;

        case FrontWord::hour_3:
            setFrontMatrixWord(4, 4, 7);
            break;

        case FrontWord::hour_4:
            setFrontMatrixWord(3, 2, 7);
            break;

        case FrontWord::hour_5:
            setFrontMatrixWord(1, 3, 7);
            break;

        case FrontWord::hour_6:
            setFrontMatrixWord(0, 4, 7);
            break;

        case FrontWord::hour_7:
            setFrontMatrixWord(0, 0, 4);
            break;

        case FrontWord::hour_8:
            setFrontMatrixWord(1, 0, 3);
            break;

        case FrontWord::hour_9:
            setFrontMatrixWord(2, 3, 7);
            break;

        case FrontWord::hour_10:
            setFrontMatrixWord(5, 1, 4);
            break;

        case FrontWord::hour_11:
            setFrontMatrixWord(4, 0, 3);
            break;

        case FrontWord::hour_12:
            setFrontMatrixWord(3, 0, 1);
            setFrontMatrixWord(4, 0, 1);
            break;

        case FrontWord::nach:
        case FrontWord::v_nach:
            setFrontMatrixPixel(5, 0); // y
            break;

        case FrontWord::vor:
        case FrontWord::v_vor:
            setFrontMatrixWord(6, 3, 7); // menos
            break;

        case FrontWord::viertel:
            setFrontMatrixWord(6, 0, 2); // cua
            setFrontMatrixWord(7, 0, 2); // rto
            break;

        case FrontWord::halb:
            setFrontMatrixPixel(5, 0);   // y
            setFrontMatrixWord(7, 3, 7); // media

        default:
            break;
        };
    };
};

Es08x08Cuarto_t _es08x08Cuarto;
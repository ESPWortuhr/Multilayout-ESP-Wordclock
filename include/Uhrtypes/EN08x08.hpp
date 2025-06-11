#pragma once

#include "Uhrtype.hpp"

/*
 *        Layout Front
 *             COL
 *       7 6 5 4 3 2 1 0
 * ROW + - - - - - - - -
 *  0  | T W F I F T H A
 *  1  | E N V E E N L F
 *  2  | T Y P A S T O T
 *  3  | T W E L V E L E
 *  4  | F O U R N V E N
 *  5  | S I X E I G H T
 *  6  | S E V E N O N E
 *  7  | T H R E E T W O
 */

class En08x08_t : public iUhrType {
public:
    virtual LanguageAbbreviation usedLang() override {
        return LanguageAbbreviation::EN;
    };

    //------------------------------------------------------------------------------

    virtual const uint8_t rowsWordMatrix() override { return 8; };

    //------------------------------------------------------------------------------

    virtual const uint8_t colsWordMatrix() override { return 8; };

    //------------------------------------------------------------------------------

    virtual const bool hasTwenty() override { return true; };

    //------------------------------------------------------------------------------

    virtual const bool hasTwentyfive() override { return true; };

    //------------------------------------------------------------------------------

    void show(FrontWord word) override {

        switch (word) {

        case FrontWord::nach:
        case FrontWord::v_nach:
            setFrontMatrixWord(2, 2, 5); // PAST
            break;

        case FrontWord::vor:
        case FrontWord::v_vor:
            setFrontMatrixWord(2, 1, 2); // TO
            break;

        case FrontWord::min_5:
            setFrontMatrixWord(0, 4, 5); // FI
            setFrontMatrixWord(1, 4, 5); // VE
            break;

        case FrontWord::min_10:
            setFrontMatrixPixel(0, 7); // T
            setFrontMatrixPixel(1, 7); // E
            setFrontMatrixPixel(1, 6); // N
            break;

        case FrontWord::viertel:
            setFrontMatrixWord(0, 2, 5); // FIFT
            setFrontMatrixWord(1, 2, 4); //  EEN
            break;

        case FrontWord::min_20:
            setFrontMatrixWord(0, 6, 7); // TW
            setFrontMatrixWord(1, 6, 7); // EN
            setFrontMatrixWord(2, 6, 7); // TY
            break;

        case FrontWord::min_25:
            show(FrontWord::min_20);
            show(FrontWord::min_5);
            break;

        case FrontWord::halb:
            setFrontMatrixWord(0, 0, 1); // HA
            setFrontMatrixWord(1, 0, 1); // LF
            break;

        case FrontWord::hour_1:
            setFrontMatrixWord(6, 0, 2); // ONE
            break;

        case FrontWord::hour_2:
            setFrontMatrixWord(7, 0, 2); // TWO
            break;

        case FrontWord::hour_3:
            setFrontMatrixWord(7, 3, 7); // THREE
            break;

        case FrontWord::hour_4:
            setFrontMatrixWord(4, 4, 7); // FOUR
            break;

        case FrontWord::hour_5:
            setFrontMatrixPixel(4, 7); // F
            setFrontMatrixPixel(5, 6); // I
            setFrontMatrixPixel(6, 5); // V
            setFrontMatrixPixel(7, 4); // E
            break;

        case FrontWord::hour_6:
            setFrontMatrixWord(5, 5, 7); // SIX
            break;

        case FrontWord::hour_7:
            setFrontMatrixWord(6, 3, 7); // SEVEN
            break;

        case FrontWord::hour_8:
            setFrontMatrixWord(5, 0, 4); // EIGHT
            break;

        case FrontWord::hour_9:
            setFrontMatrixPixel(4, 3); // N
            setFrontMatrixPixel(5, 3); // I
            setFrontMatrixPixel(6, 3); // N
            setFrontMatrixPixel(7, 3); // E
            break;

        case FrontWord::hour_10:
            setFrontMatrixPixel(2, 0); // T
            setFrontMatrixPixel(3, 0); // E
            setFrontMatrixPixel(4, 0); // N
            break;

        case FrontWord::hour_11:
            setFrontMatrixWord(3, 0, 2); // ELE
            setFrontMatrixWord(4, 0, 2); // VEN
            break;

        case FrontWord::hour_12:
            setFrontMatrixWord(3, 2, 7); // TWELVE
            break;

        default:
            break;
        };
    };
};

En08x08_t _en08x08;
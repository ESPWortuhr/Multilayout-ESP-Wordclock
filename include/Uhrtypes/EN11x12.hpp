#pragma once

#include "Uhrtype.hpp"

/*
 *           Layout Front
 *                COL
 *       1 X 9 8 7 6 5 4 3 2 1 0
 * ROW + - - - - - - - - - - -
 *  0  | I T L I S A S T H P M A
 *  1  | A C F I F T E E N D C O
 *  2  | T W E N T Y F I V E X W
 *  3  | T H I R T Y X T E N X W
 *  4  | M I N U T E S E T O U R
 *  5  | P A S T O R U F O U R T
 *  6  | S E V E N X T W E L V E
 *  7  | N I N E F I V E C T W O
 *  8  | E I G H T F E L E V E N
 *  9  | S I X T H R E E O N E G
 *  X  | T E N S E Z O C L O C K
 */

class En11x12_t : public iUhrType {
public:
    virtual LanguageAbbreviation usedLang() override {
        return LanguageAbbreviation::EN;
    };

    //------------------------------------------------------------------------------

    virtual const uint8_t rowsWordMatrix() override { return 11; };

    //------------------------------------------------------------------------------

    virtual const uint8_t colsWordMatrix() override { return 12; };

    //------------------------------------------------------------------------------

    virtual const bool hasTwenty() override { return true; }

    //------------------------------------------------------------------------------

    virtual const bool hasTwentyfive() override { return true; }

    //------------------------------------------------------------------------------

    virtual const bool hasMinuteInWords() { return true; }

    //------------------------------------------------------------------------------

    void show(FrontWord word) override {
        switch (word) {

        case FrontWord::es_ist:
            // It is
            setFrontMatrixWord(0, 10, 11);
            setFrontMatrixWord(0, 7, 8);
            break;

        case FrontWord::hour_1:
            // One
            setFrontMatrixWord(9, 1, 3);
            break;

        case FrontWord::hour_2:
            // Two
            setFrontMatrixWord(7, 0, 2);
            break;

        case FrontWord::hour_3:
            // Three
            setFrontMatrixWord(9, 4, 8);
            break;

        case FrontWord::hour_4:
            // Four
            setFrontMatrixWord(5, 1, 4);
            break;

        case FrontWord::hour_5:
            // Five
            setFrontMatrixWord(7, 4, 7);
            break;

        case FrontWord::hour_6:
            // Six
            setFrontMatrixWord(9, 9, 11);
            break;

        case FrontWord::hour_7:
            // Seven
            setFrontMatrixWord(6, 7, 11);
            break;

        case FrontWord::hour_8:
            // Eight
            setFrontMatrixWord(8, 7, 11);
            break;

        case FrontWord::hour_9:
            // Nine
            setFrontMatrixWord(7, 8, 11);
            break;

        case FrontWord::hour_10:
            // Ten
            setFrontMatrixWord(10, 9, 11);
            break;

        case FrontWord::hour_11:
            // Eleven
            setFrontMatrixWord(8, 0, 5);
            break;

        case FrontWord::hour_12:
            // Twelve
            setFrontMatrixWord(6, 0, 5);
            break;

        case FrontWord::min_5:
            // Five
            setFrontMatrixWord(2, 2, 5);
            // Minutes
            setFrontMatrixWord(4, 5, 11);
            break;

        case FrontWord::min_10:
            // Ten
            setFrontMatrixWord(3, 2, 4);
            // Minutes
            setFrontMatrixWord(4, 5, 11);
            break;

        case FrontWord::a_quarter:
        case FrontWord::viertel:
            // Fifteen
            setFrontMatrixWord(1, 3, 9);
            // Minutes
            setFrontMatrixWord(4, 5, 11);
            break;

        case FrontWord::min_20:
            // Twenty
            setFrontMatrixWord(2, 6, 11);
            // Minutes
            setFrontMatrixWord(4, 5, 11);
            break;

        case FrontWord::min_25:
            // Twentyfive
            setFrontMatrixWord(2, 2, 11);
            // Minutes
            setFrontMatrixWord(4, 5, 11);
            break;

        case FrontWord::halb:
            // Thirty
            setFrontMatrixWord(3, 6, 11);
            // Minutes
            setFrontMatrixWord(4, 5, 11);
            // To
            setFrontMatrixWord(4, 2, 3);
            break;

        case FrontWord::nach:
        case FrontWord::v_nach:
            // Past
            setFrontMatrixWord(5, 8, 11);
            break;

        case FrontWord::vor:
        case FrontWord::v_vor:
            // To
            setFrontMatrixWord(4, 2, 3);
            break;

        case FrontWord::uhr:
            // O'Clock
            setFrontMatrixWord(10, 0, 5);
            break;

        case FrontWord::minute:
            // Minute
            setFrontMatrixWord(4, 6, 11);
            break;

        case FrontWord::minuten:
            // Minutes
            setFrontMatrixWord(4, 5, 11);
            break;

        default:
            break;
        };
    };
};

En11x12_t _en11x12;

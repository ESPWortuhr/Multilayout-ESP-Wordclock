#pragma once

#include "Uhrtype.hpp"

/*
 *           Layout Front
 *                COL
 *       X 9 8 7 6 5 4 3 2 1 0
 * ROW + - - - - - - - - - - -
 *  0  | E S J I S T L F Ü N F
 *  1  | Z E H N Z W A N Z I G
 *  2  | D R E I V I E R T E L
 *  3  | T F N A C H V O R J M
 *  4  | H A L B X Z W Ö L F T
 *  5  | Z W E I N S I E B E N
 *  6  | L D R E I C I F Ü N F
 *  7  | E L F N E U N V I E R
 *  8  | U A C H T Z E H N S I
 *  9  | C S E C H S T L U H R
 */

class De10x11Nero_t : public iUhrType {
public:
    virtual LanguageAbbreviation usedLang() override {
        return LanguageAbbreviation::DE;
    };

    //------------------------------------------------------------------------------

    virtual const bool hasDreiviertel() override { return true; }

    //------------------------------------------------------------------------------

    void show(FrontWord word) override {
        switch (word) {

        case FrontWord::es_ist:
            setFrontMatrixWord(0, 9, 10);
            setFrontMatrixWord(0, 5, 7);
            break;

        case FrontWord::nach:
        case FrontWord::v_nach:
            setFrontMatrixWord(3, 5, 8);
            break;

        case FrontWord::vor:
        case FrontWord::v_vor:
            setFrontMatrixWord(3, 2, 4);
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

        default:
            break;
        };
    };
};

De10x11Nero_t _de10x11Nero;
#pragma once

#include "Uhrtype.hpp"

/*
 *           Layout Front
 *                COL
 *       X 9 8 7 6 5 4 3 2 1 0
 * ROW + - - - - - - - - - - -
 *  0  | E S K I S C H F U N K
 *  1  | D R E I V I E R T L A
 *  2  | Z E H N B I E F Ü N F
 *  3  | N A C H G E R T V O R
 *  4  | H A L B X F Ü N F E I
 *  5  | O I S E C H S E L F E
 *  6  | Z W O I E A C H T E D
 *  7  | D R E I E Z W Ö L F E
 *  8  | Z E H N E U N E U H L
 *  9  | S I E B N E V I E R E
 */

class De10x11schwaebisch_t : public iUhrType {
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
            setFrontMatrixWord(0, 4, 7);
            break;

        case FrontWord::nach:
        case FrontWord::v_nach:
            setFrontMatrixWord(3, 7, 10);
            break;

        case FrontWord::vor:
        case FrontWord::v_vor:
            setFrontMatrixWord(3, 0, 2);
            break;

        case FrontWord::viertel:
            setFrontMatrixWord(1, 1, 6);
            break;

        case FrontWord::dreiviertel:
            setFrontMatrixWord(1, 1, 10);
            break;

        case FrontWord::min_5:
            setFrontMatrixWord(2, 0, 3);
            break;

        case FrontWord::min_10:
            setFrontMatrixWord(2, 7, 10);
            break;

        case FrontWord::halb:
            setFrontMatrixWord(4, 7, 10);
            break;

        case FrontWord::eins:
            setFrontMatrixWord(5, 7, 10);
            break;

        case FrontWord::hour_1:
            setFrontMatrixWord(5, 7, 10);
            break;

        case FrontWord::hour_2:
            setFrontMatrixWord(6, 6, 10);
            break;

        case FrontWord::hour_3:
            setFrontMatrixWord(7, 6, 10);
            break;

        case FrontWord::hour_4:
            setFrontMatrixWord(9, 0, 4);
            break;

        case FrontWord::hour_5:
            setFrontMatrixWord(4, 1, 5);
            break;

        case FrontWord::hour_6:
            setFrontMatrixWord(5, 3, 8);
            break;

        case FrontWord::hour_7:
            setFrontMatrixWord(9, 5, 10);
            break;

        case FrontWord::hour_8:
            setFrontMatrixWord(6, 1, 5);
            break;

        case FrontWord::hour_9:
            setFrontMatrixWord(8, 3, 7);
            break;

        case FrontWord::hour_10:
            setFrontMatrixWord(8, 6, 10);
            break;

        case FrontWord::hour_11:
            setFrontMatrixWord(5, 0, 3);
            break;

        case FrontWord::hour_12:
            setFrontMatrixWord(7, 0, 5);
            break;

        default:
            break;
        };
    };
};

De10x11schwaebisch_t _de10x11schwaebisch;
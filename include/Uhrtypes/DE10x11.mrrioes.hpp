#pragma once

#include "Uhrtype.hpp"

/*
 *           Layout Front
 *                COL
 *       X 9 8 7 6 5 4 3 2 1 0
 * ROW + - - - - - - - - - - -
 *  0  | E S K I S T Ä F Ü N F
 *  1  | Z E H N V I E R T E L
 *  2  | Z W A N Z I G K V O R
 *  3  | P N A C H G H A L B M
 *  4  | E I N S R Z W E I P Y 
 *  5  | K D R E I Y V I E R F
 *  6  | F Ü N F Q S E C H S K
 *  7  | M S I E B E N A C H T
 *  8  | N E U N Z E H N E L F
 *  9  | B Z W Ö L F J U H R X
 */

class De10x11Mrrioes_t : public iUhrType {
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
            setFrontMatrixWord(3, 6, 9);
            break;

        case FrontWord::vor:
        case FrontWord::v_vor:
            setFrontMatrixWord(2, 0, 2);
            break;

        case FrontWord::viertel:
            setFrontMatrixWord(1, 0, 6);
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
            setFrontMatrixWord(2, 4, 10);
            break;

        case FrontWord::halb:
            setFrontMatrixWord(3, 1, 4);
            break;

        case FrontWord::eins:
            setFrontMatrixWord(4, 7, 10);
            break;

        case FrontWord::uhr:
            setFrontMatrixWord(9, 1, 3);
            break;

        case FrontWord::hour_1:
            setFrontMatrixWord(4, 8, 10);
            break;

        case FrontWord::hour_2:
            setFrontMatrixWord(4, 2, 5);
            break;

        case FrontWord::hour_3:
            setFrontMatrixWord(5, 6, 9);
            break;

        case FrontWord::hour_4:
            setFrontMatrixWord(5, 1, 4);
            break;

        case FrontWord::hour_5:
            setFrontMatrixWord(6, 7, 10);
            break;

        case FrontWord::hour_6:
            setFrontMatrixWord(6, 1, 5);
            break;

        case FrontWord::hour_7:
            setFrontMatrixWord(7, 4, 9);
            break;

        case FrontWord::hour_8:
            setFrontMatrixWord(7, 0, 3);
            break;

        case FrontWord::hour_9:
            setFrontMatrixWord(8, 7, 10);
            break;

        case FrontWord::hour_10:
            setFrontMatrixWord(8, 3, 6);
            break;

        case FrontWord::hour_11:
            setFrontMatrixWord(8, 0, 2);
            break;

        case FrontWord::hour_12:
            setFrontMatrixWord(9, 5, 9);
            break;

        default:
            break;
        };
    };
};

De10x11Mrrioes_t _de10x11Mrrioes;

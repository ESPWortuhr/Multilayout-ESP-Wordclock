#pragma once

#include "Uhrtype.hpp"

/*
 *           Layout Front
 *                COL
 *       X 9 8 7 6 5 4 3 2 1 0
 * ROW + - - - - - - - - - - -
 *  0  | E T Z K I S T F Ü N F
 *  1  | Z E H N E Z W A N Z G
 *  2  | D R E I V I E R T L E
 *  3  | J T N O C H V O R X M
 *  4  | H O I B E P Z D R E I
 *  5  | Z W O A N S S I E M E
 *  6  | K A C H T E F Ü N F E
 *  7  | N E I N E V I E R E Q
 *  8  | Z W O I F E Z E H N E
 *  9  | Y S E C H S E F U H R
 */

class De10x11bayerisch_t : public iUhrType {
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
            setFrontMatrixWord(0, 8, 10);
            setFrontMatrixWord(0, 5, 6);
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
            setFrontMatrixWord(2, 1, 6);
            break;

        case FrontWord::dreiviertel:
            setFrontMatrixWord(2, 1, 10);
            break;

        case FrontWord::min_5:
            setFrontMatrixWord(0, 0, 3);
            break;

        case FrontWord::min_10:
            setFrontMatrixWord(1, 7, 10);
            break;

        case FrontWord::min_20:
            setFrontMatrixWord(1, 0, 5);
            break;

        case FrontWord::halb:
            setFrontMatrixWord(4, 6, 10);
            break;

        case FrontWord::eins:
            setFrontMatrixWord(5, 5, 8);
            break;

        case FrontWord::hour_1:
            setFrontMatrixWord(5, 5, 8);
            break;

        case FrontWord::hour_2:
            setFrontMatrixWord(5, 7, 10);
            break;

        case FrontWord::hour_3:
            setFrontMatrixWord(4, 0, 3);
            break;

        case FrontWord::hour_4:
            setFrontMatrixWord(7, 1, 5);
            break;

        case FrontWord::hour_5:
            setFrontMatrixWord(6, 0, 4);
            break;

        case FrontWord::hour_6:
            setFrontMatrixWord(9, 4, 9);
            break;

        case FrontWord::hour_7:
            setFrontMatrixWord(5, 0, 4);
            break;

        case FrontWord::hour_8:
            setFrontMatrixWord(6, 5, 9);
            break;

        case FrontWord::hour_9:
            setFrontMatrixWord(7, 6, 10);
            break;

        case FrontWord::hour_10:
            setFrontMatrixWord(8, 0, 4);
            break;

        case FrontWord::hour_11:
            setFrontMatrixWord(8, 5, 8);
            break;

        case FrontWord::hour_12:
            setFrontMatrixWord(8, 5, 10);
            break;

        default:
            break;
        };
    };
};

De10x11bayerisch_t _de10x11bayerisch;

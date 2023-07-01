#pragma once

#include "Uhrtype.hpp"

/*
 *           Layout Front
 *                COL
 *       X 9 8 7 6 5 4 3 2 1 0
 * ROW + - - - - - - - - - - -
 *  0  | E S K I S T A F Ü N F
 *  1  | Z E H N Z W A N Z I G
 *  2  | D R E I V I E R T E L
 *  3  | V O R F U N K N A C H
 *  4  | H A L B A E L F Ü N F
 *  5  | E I N S X A M Z W E I
 *  6  | D R E I A U J V I E R
 *  7  | S E C H S N L A C H T
 *  8  | S I E B E N Z W Ö L F
 *  9  | Z E H N E U N K U H R
 *
 */

class De10x11Clock_t : public iUhrType {
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
            setFrontMatrixWord(3, 0, 3);
            break;

        case FrontWord::vor:
        case FrontWord::v_vor:
            setFrontMatrixWord(3, 8, 10);
            break;

        case FrontWord::viertel:
            setFrontMatrixWord(2, 0, 6);
            break;

        case FrontWord::dreiviertel:
            setFrontMatrixWord(2, 0, 10);
            break;

        case FrontWord::fuenf:
            setFrontMatrixWord(0, 0, 3);
            break;

        case FrontWord::zehn:
            setFrontMatrixWord(1, 7, 10);
            break;

        case FrontWord::zwanzig:
            setFrontMatrixWord(1, 0, 6);
            break;

        case FrontWord::halb:
            setFrontMatrixWord(4, 7, 10);
            break;

        case FrontWord::eins:
            setFrontMatrixWord(5, 7, 10);
            break;

        case FrontWord::uhr:
            setFrontMatrixWord(9, 0, 2);
            break;

        case FrontWord::h_ein:
            setFrontMatrixWord(5, 8, 10);
            break;

        case FrontWord::h_zwei:
            setFrontMatrixWord(5, 0, 3);
            break;

        case FrontWord::h_drei:
            setFrontMatrixWord(6, 7, 10);
            break;

        case FrontWord::h_vier:
            setFrontMatrixWord(6, 0, 3);
            break;

        case FrontWord::h_fuenf:
            setFrontMatrixWord(4, 0, 3);
            break;

        case FrontWord::h_sechs:
            setFrontMatrixWord(7, 6, 10);
            break;

        case FrontWord::h_sieben:
            setFrontMatrixWord(8, 5, 10);
            break;

        case FrontWord::h_acht:
            setFrontMatrixWord(7, 0, 3);
            break;

        case FrontWord::h_neun:
            setFrontMatrixWord(9, 4, 7);
            break;

        case FrontWord::h_zehn:
            setFrontMatrixWord(9, 7, 10);
            break;

        case FrontWord::h_elf:
            setFrontMatrixWord(4, 3, 5);
            break;

        case FrontWord::h_zwoelf:
            setFrontMatrixWord(8, 0, 4);
            break;

        case FrontWord::funk:
            setFrontMatrixWord(3, 4, 7);
            break;

        default:
            break;
        };
    };
};

De10x11Clock_t _de10x11Clock;
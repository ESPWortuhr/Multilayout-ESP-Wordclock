#pragma once

#include "Uhrtype.hpp"

/*
 *           Layout Front
 *                COL
 *       X 9 8 7 6 5 4 3 2 1 0
 * ROW + - - - - - - - - - - -
 *  0  | E S K I S T R F Ü N F
 *  1  | Z E H N Z W A N Z I G
 *  2  | D R E I V I E R T E L
 *  3  | T G N A C H V O R U M
 *  4  | H A L B G Z W Ö L F J
 *  5  | Z W E I N S I E B E N
 *  6  | K D R E I R H F Ü N F
 *  7  | E L F N E U N V I E R
 *  8  | N A C H T Z E H N B X
 *  9  | U S E C H S F U H R Y
 */

class De10x11_t : public iUhrType {
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

        case FrontWord::m_fuenf:
            setFrontMatrixWord(0, 0, 3);
            break;

        case FrontWord::m_zehn:
            setFrontMatrixWord(1, 7, 10);
            break;

        case FrontWord::m_zwanzig:
            setFrontMatrixWord(1, 0, 6);
            break;

        case FrontWord::halb:
            setFrontMatrixWord(4, 7, 10);
            break;

        case FrontWord::eins:
            setFrontMatrixWord(5, 5, 8);
            break;

        case FrontWord::uhr:
            setFrontMatrixWord(9, 1, 3);
            break;

        case FrontWord::h_ein:
            setFrontMatrixWord(5, 6, 8);
            break;

        case FrontWord::h_zwei:
            setFrontMatrixWord(5, 7, 10);
            break;

        case FrontWord::h_drei:
            setFrontMatrixWord(6, 6, 9);
            break;

        case FrontWord::h_vier:
            setFrontMatrixWord(7, 0, 3);
            break;

        case FrontWord::h_fuenf:
            setFrontMatrixWord(6, 0, 3);
            break;

        case FrontWord::h_sechs:
            setFrontMatrixWord(9, 5, 9);
            break;

        case FrontWord::h_sieben:
            setFrontMatrixWord(5, 0, 5);
            break;

        case FrontWord::h_acht:
            setFrontMatrixWord(8, 6, 9);
            break;

        case FrontWord::h_neun:
            setFrontMatrixWord(7, 4, 7);
            break;

        case FrontWord::h_zehn:
            setFrontMatrixWord(8, 2, 5);
            break;

        case FrontWord::h_elf:
            setFrontMatrixWord(7, 8, 10);
            break;

        case FrontWord::h_zwoelf:
            setFrontMatrixWord(4, 1, 5);
            break;

        default:
            break;
        };
    };
};

De10x11_t _de10x11;
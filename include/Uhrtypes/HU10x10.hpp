#pragma once

#include "Uhrtype.hpp"

/*
 *           Layout Front
 *                COL
 *       X 9 8 7 6 5 4 3 2 1 0
 * ROW + - - - - - - - - - - -
 *  0  | Ö T B T Í Z J A S U
 *  1  | P E R C C E L E W O
 *  2  | M Ú L T A M Ú L V A
 *  3  | H Á R O M U F É L J
 *  4  | N E G Y E D X H A T
 *  5  | N É G Y A H Á R O M
 *  6  | N Y O L C Ö T H É T
 *  7  | T I Z E N E G Y E B
 *  8  | T I Z E N K E T T Ő
 *  9  | K I L E N C Q Ó R A
 *
 */

class Hu10x10_t : public iUhrType {
public:
    virtual LanguageAbbreviation usedLang() override {
        return LanguageAbbreviation::HU;
    };

    virtual const uint8_t rowsWordMatrix() override { return 10; };

    //------------------------------------------------------------------------------

    virtual const uint8_t colsWordMatrix() override { return 10; };

    //------------------------------------------------------------------------------

    virtual const bool hasDreiviertel() override { return true; }

    //------------------------------------------------------------------------------

    virtual const bool hasZwanzig() override { return false; }

    //------------------------------------------------------------------------------

    void show(FrontWord word) override {
        switch (word) {

        case FrontWord::nach:
        case FrontWord::v_nach:
            setFrontMatrixWord(1, 3, 9);
            setFrontMatrixWord(2, 6, 9);
            break;

        case FrontWord::vor:
        case FrontWord::v_vor:
            setFrontMatrixWord(1, 6, 9);
            setFrontMatrixWord(2, 0, 4);
            break;

        case FrontWord::viertel:
            setFrontMatrixWord(4, 4, 9);
            break;

        case FrontWord::dreiviertel:
            setFrontMatrixWord(3, 5, 9);
            setFrontMatrixWord(4, 4, 9);
            break;

        case FrontWord::m_fuenf:
            setFrontMatrixWord(0, 8, 9);
            break;

        case FrontWord::m_zehn:
            setFrontMatrixWord(0, 4, 6);
            break;

        case FrontWord::halb:
            setFrontMatrixWord(3, 1, 3);
            break;

        case FrontWord::uhr:
            setFrontMatrixWord(9, 0, 2);
            break;

        case FrontWord::h_ein:
            setFrontMatrixWord(7, 2, 4);
            break;

        case FrontWord::h_zwei:
            setFrontMatrixWord(8, 0, 4);
            break;

        case FrontWord::h_drei:
            setFrontMatrixWord(5, 0, 4);
            break;

        case FrontWord::h_vier:
            setFrontMatrixWord(5, 6, 9);
            break;

        case FrontWord::h_fuenf:
            setFrontMatrixWord(6, 3, 4);
            break;

        case FrontWord::h_sechs:
            setFrontMatrixWord(4, 0, 2);
            break;

        case FrontWord::h_sieben:
            setFrontMatrixWord(6, 0, 2);
            break;

        case FrontWord::h_acht:
            setFrontMatrixWord(6, 5, 9);
            break;  

        case FrontWord::h_neun:
            setFrontMatrixWord(9, 4, 9);
            break;

        case FrontWord::h_zehn:
            setFrontMatrixWord(7, 7, 9);
            break;

        case FrontWord::h_elf:
            setFrontMatrixWord(7, 2, 9);
            break;

        case FrontWord::h_zwoelf:
            setFrontMatrixWord(8, 0, 9);
            break;

        default:
            break;
        };
    };
};

Hu10x10_t _hu10x10;
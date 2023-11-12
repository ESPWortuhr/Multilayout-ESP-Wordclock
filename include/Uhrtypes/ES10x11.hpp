#pragma once

#include "Uhrtype.hpp"

/*
 *           Layout Front
 *                COL
 *       X 9 8 7 6 5 4 3 2 1 0
 * ROW + - - - - - - - - - - -
 *  0  | E S O N E L A S U N A
 *  1  | D O S I T R E S O R E
 *  2  | C U A T R O C I N C O
 *  3  | S E I S A S I E T E N
 *  4  | O C H O N U E V E Y O
 *  5  | L A D I E Z S O N C E
 *  6  | D O C E L Y M E N O S
 *  7  | O V E I N T E D I E Z
 *  8  | V E I N T I C I N C O
 *  9  | M E D I A C U A R T O
 */

class Es10x11_t : public iUhrType {
public:
    virtual LanguageAbbreviation usedLang() override {
        return LanguageAbbreviation::ES;
    };

    //------------------------------------------------------------------------------

    virtual const bool hasZwanzig() override { return true; }

    //------------------------------------------------------------------------------

    virtual const bool hasTwentyfive() override { return true; }

    //------------------------------------------------------------------------------

    void show(FrontWord word) override {
        switch (word) {

        case FrontWord::es_ist:
            // SON LAS
            setFrontMatrixWord(0, 7, 9);
            setFrontMatrixWord(0, 3, 5);

        case FrontWord::es_ist___plural___:
            // ES LA
            setFrontMatrixWord(0, 9, 10);
            setFrontMatrixWord(0, 4, 5);
            break;

        case FrontWord::h_ein:
            // UNA
            setFrontMatrixWord(0, 0, 2);
            break;

        case FrontWord::h_zwei:
            // DOS
            setFrontMatrixWord(1, 8, 10);
            break;

        case FrontWord::h_drei:
            // TRES
            setFrontMatrixWord(1, 3, 6);
            break;

        case FrontWord::h_vier:
            // CUATRO
            setFrontMatrixWord(2, 5, 10);
            break;

        case FrontWord::h_fuenf:
            // CINCO
            setFrontMatrixWord(2, 0, 4);
            break;

        case FrontWord::h_sechs:
            // SEIS
            setFrontMatrixWord(3, 7, 10);
            break;

        case FrontWord::h_sieben:
            // SIETE
            setFrontMatrixWord(3, 1, 5);
            break;

        case FrontWord::h_acht:
            // OCHO
            setFrontMatrixWord(4, 7, 10);
            break;

        case FrontWord::h_neun:
            // NUEVE
            setFrontMatrixWord(4, 2, 6);
            break;

        case FrontWord::h_zehn:
            // DIEZ
            setFrontMatrixWord(5, 5, 8);
            break;

        case FrontWord::h_elf:
            // ONCE
            setFrontMatrixWord(5, 0, 3);
            break;

        case FrontWord::h_zwoelf:
            // DOCE
            setFrontMatrixWord(6, 7, 10);
            break;

        case FrontWord::m_fuenf:
            // CINCO
            setFrontMatrixWord(8, 0, 4);
            break;

        case FrontWord::m_zehn:
            // DIEZ
            setFrontMatrixWord(7, 0, 3);
            break;

        case FrontWord::viertel:
            // CUARTO
            setFrontMatrixWord(9, 0, 5);
            break;

        case FrontWord::m_zwanzig:
            // VEINTE
            setFrontMatrixWord(7, 4, 9);
            break;

        case FrontWord::m_twentyfive:
            // VEINTICINCO
            setFrontMatrixWord(8, 0, 10);
            break;

        case FrontWord::halb:
            // MEDIA
            setFrontMatrixWord(9, 6, 10);
            break;

        case FrontWord::nach:
        case FrontWord::v_nach:
            // Y ("UND")
            setFrontMatrixWord(6, 5, 5);
            break;

        case FrontWord::vor:
        case FrontWord::v_vor:
            // MENOS ("Minus")
            setFrontMatrixWord(6, 0, 4);
            break;

        default:
            break;
        };
    };
};

Es10x11_t _es10x11;
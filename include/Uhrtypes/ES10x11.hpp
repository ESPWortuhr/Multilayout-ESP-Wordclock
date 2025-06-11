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

    virtual const bool hasTwenty() override { return true; }

    //------------------------------------------------------------------------------

    virtual const bool hasTwentyfive() override { return true; }

    //------------------------------------------------------------------------------

    void show(FrontWord word) override {
        switch (word) {

        case FrontWord::es_ist:
            // SON LAS
            setFrontMatrixWord(0, 7, 9);
            setFrontMatrixWord(0, 3, 5);
            break;

        case FrontWord::es_ist___plural___:
            // ES LA
            setFrontMatrixWord(0, 9, 10);
            setFrontMatrixWord(0, 4, 5);
            break;

        case FrontWord::hour_1:
            // UNA
            setFrontMatrixWord(0, 0, 2);
            break;

        case FrontWord::hour_2:
            // DOS
            setFrontMatrixWord(1, 8, 10);
            break;

        case FrontWord::hour_3:
            // TRES
            setFrontMatrixWord(1, 3, 6);
            break;

        case FrontWord::hour_4:
            // CUATRO
            setFrontMatrixWord(2, 5, 10);
            break;

        case FrontWord::hour_5:
            // CINCO
            setFrontMatrixWord(2, 0, 4);
            break;

        case FrontWord::hour_6:
            // SEIS
            setFrontMatrixWord(3, 7, 10);
            break;

        case FrontWord::hour_7:
            // SIETE
            setFrontMatrixWord(3, 1, 5);
            break;

        case FrontWord::hour_8:
            // OCHO
            setFrontMatrixWord(4, 7, 10);
            break;

        case FrontWord::hour_9:
            // NUEVE
            setFrontMatrixWord(4, 2, 6);
            break;

        case FrontWord::hour_10:
            // DIEZ
            setFrontMatrixWord(5, 5, 8);
            break;

        case FrontWord::hour_11:
            // ONCE
            setFrontMatrixWord(5, 0, 3);
            break;

        case FrontWord::hour_12:
            // DOCE
            setFrontMatrixWord(6, 7, 10);
            break;

        case FrontWord::min_5:
            // CINCO
            setFrontMatrixWord(8, 0, 4);
            break;

        case FrontWord::min_10:
            // DIEZ
            setFrontMatrixWord(7, 0, 3);
            break;

        case FrontWord::viertel:
            // CUARTO
            setFrontMatrixWord(9, 0, 5);
            break;

        case FrontWord::min_20:
            // VEINTE
            setFrontMatrixWord(7, 4, 9);
            break;

        case FrontWord::min_25:
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
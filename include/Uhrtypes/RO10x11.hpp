#pragma once

#include "Uhrtype.hpp"

/*
 *           Layout Front
 *                COL
 *       X 9 8 7 6 5 4 3 2 1 0
 * ROW + - - - - - - - - - - -
 *  0  | E S T E Z O R A P M O
 *  1  | D O U Ă N S P R E A M
 *  2  | U N S P R E Z E C E L
 *  3  | N O U Ă O P T Ş A S E
 *  4  | P A T R U N U T R E I
 *  5  | Ş A P T E C I N C I A
 *  6  | Ş I B T R E I Z E C I
 *  7  | F Ă R Ă O Z E C E U N
 *  8  | D O U Ă Z E C I V Ş I
 *  9  | C I N C I U S F E R T
 */

class RO10x11_t : public iUhrType {
public:
    virtual LanguageAbbreviation usedLang() override {
        return LanguageAbbreviation::RO;
    };

    //------------------------------------------------------------------------------

    virtual const bool hasTwenty() override { return true; }

    //------------------------------------------------------------------------------

    virtual const bool hasTwentyfive() override { return true; }

    //------------------------------------------------------------------------------

    virtual const bool hasThirtyfive() override { return true; }

    //------------------------------------------------------------------------------

    void show(FrontWord word) override {
        switch (word) {

        case FrontWord::es_ist:
            // ESTE ORA
            setFrontMatrixWord(0, 7, 10);
            setFrontMatrixWord(0, 3, 5);
            break;

        case FrontWord::hour_1:
            // UNU
            setFrontMatrixWord(4, 4, 6);
            break;

        case FrontWord::hour_2:
            // DOUĂ
            setFrontMatrixWord(1, 7, 10);
            break;

        case FrontWord::hour_3:
            // TREI
            setFrontMatrixWord(4, 0, 3);
            break;

        case FrontWord::hour_4:
            // PATRU
            setFrontMatrixWord(4, 6, 10);
            break;

        case FrontWord::hour_5:
            // CINCI
            setFrontMatrixWord(5, 1, 5);
            break;

        case FrontWord::hour_6:
            // ŞASE
            setFrontMatrixWord(3, 0, 3);
            break;

        case FrontWord::hour_7:
            // ŞAPTE
            setFrontMatrixWord(5, 6, 10);
            break;

        case FrontWord::hour_8:
            // OPT
            setFrontMatrixWord(3, 4, 6);
            break;

        case FrontWord::hour_9:
            // NOUĂ
            setFrontMatrixWord(3, 7, 10);
            break;

        case FrontWord::hour_10:
            // ZECE
            setFrontMatrixWord(2, 1, 4);
            break;

        case FrontWord::hour_11:
            // UNSPREZECE
            setFrontMatrixWord(2, 1, 10);
            break;

        case FrontWord::hour_12:
            // DOUĂ SPRE ZECE
            setFrontMatrixWord(1, 7, 10);
            setFrontMatrixWord(1, 2, 5);
            setFrontMatrixWord(2, 1, 4);
            break;

        case FrontWord::min_5:
            // min CINCI
            setFrontMatrixWord(9, 6, 10);
            break;

        case FrontWord::min_10:
            // min ZECE
            setFrontMatrixWord(7, 2, 5);
            break;

        case FrontWord::a_quarter:
        case FrontWord::viertel:
            // min UN SFERT
            setFrontMatrixWord(7, 0, 1);
            setFrontMatrixWord(9, 0, 4);
            break;

        case FrontWord::min_20:
            // min DOUĂZECI
            setFrontMatrixWord(8, 3, 10);
            break;

        case FrontWord::min_25:
            // min DOUĂZECI ŞI CINCI
            setFrontMatrixWord(8, 3, 10);
            setFrontMatrixWord(8, 0, 1);
            setFrontMatrixWord(9, 6, 10);
            break;

        case FrontWord::halb:
            // TREIZECI
            setFrontMatrixWord(6, 0, 7);
            break;

        case FrontWord::min_35:
            // TREIZECI ŞI CINCI
            setFrontMatrixWord(6, 0, 7);
            setFrontMatrixWord(8, 0, 1);
            setFrontMatrixWord(9, 6, 10);
            break;

        case FrontWord::nach:
        case FrontWord::v_nach:
            // ŞI
            setFrontMatrixWord(6, 9, 10);
            break;

        case FrontWord::vor:
        case FrontWord::v_vor:
            // FĂRĂ
            setFrontMatrixWord(7, 7, 10);
            break;

        default:
            break;
        };
    };
};

RO10x11_t _ro10x11;

#pragma once

#include "Uhrtype.hpp"

/*
 *           Layout Front
 *                COL
 *       X 9 8 7 6 5 4 3 2 1 0
 * ROW + - - - - - - - - - - -
 *  0  | K L O C K A N T Ä R K
 *  1  | F E M Y I S T I O N I
 *  2  | K V A R T Q I E N Z O
 *  3  | T J U G O L I V I P M
 *  4  | Ö V E R K A M H A L V
 *  5  | E T T U S V L X T V Å
 *  6  | T R E M Y K Y F Y R A
 *  7  | F E M S F L O R S E X
 *  8  | S J U Å T T A I N I O
 *  9  | T I O E L V A T O L V
 */

class Se10x11_t : public iUhrType {
public:
    virtual LanguageAbbreviation usedLang() override {
        return LanguageAbbreviation::SE;
    };

    //------------------------------------------------------------------------------

    virtual const bool hasTwenty() override { return true; }

    //------------------------------------------------------------------------------

    void show(FrontWord word) override {
        switch (word) {

        case FrontWord::es_ist:
            // Klockan är
            setFrontMatrixWord(0, 4, 10);
            setFrontMatrixWord(0, 1, 2);
            break;

        case FrontWord::hour_1:
            // Ett
            setFrontMatrixWord(5, 8, 10);
            break;

        case FrontWord::hour_2:
            // Två
            setFrontMatrixWord(5, 0, 2);
            break;

        case FrontWord::hour_3:
            // Tre
            setFrontMatrixWord(6, 8, 10);
            break;

        case FrontWord::hour_4:
            // Fyra
            setFrontMatrixWord(6, 0, 3);
            break;

        case FrontWord::hour_5:
            // Fem
            setFrontMatrixWord(7, 8, 10);
            break;

        case FrontWord::hour_6:
            // Sex
            setFrontMatrixWord(7, 0, 2);
            break;

        case FrontWord::hour_7:
            // Sju
            setFrontMatrixWord(8, 8, 10);
            break;

        case FrontWord::hour_8:
            // Åtta
            setFrontMatrixWord(8, 4, 7);
            break;

        case FrontWord::hour_9:
            // Nio
            setFrontMatrixWord(8, 0, 2);
            break;

        case FrontWord::hour_10:
            // Tio
            setFrontMatrixWord(9, 8, 10);
            break;

        case FrontWord::hour_11:
            // Elva
            setFrontMatrixWord(9, 4, 7);
            break;

        case FrontWord::hour_12:
            // Tolv
            setFrontMatrixWord(9, 0, 3);
            break;

        case FrontWord::min_5:
            // Fem
            setFrontMatrixWord(1, 8, 10);
            break;

        case FrontWord::min_10:
            // Tio
            setFrontMatrixWord(1, 2, 4);
            break;

        case FrontWord::viertel:
            // Kvart
            setFrontMatrixWord(2, 6, 10);
            break;

        case FrontWord::min_20:
            // Tjugo
            setFrontMatrixWord(3, 6, 10);
            break;

        case FrontWord::halb:
            // Halv
            setFrontMatrixWord(4, 0, 3);
            break;

        case FrontWord::nach:
        case FrontWord::v_nach:
            // Över
            setFrontMatrixWord(4, 7, 10);
            break;

        case FrontWord::vor:
        case FrontWord::v_vor:
            // I
            setFrontMatrixWord(3, 2, 2);
            break;

        default:
            break;
        };
    };
};

Se10x11_t _se10x11;
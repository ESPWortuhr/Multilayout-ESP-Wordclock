#pragma once

#include "Uhrtype.hpp"

/*
 *           Layout Front
 *                COL
 *       X 9 8 7 6 5 4 3 2 1 0
 * ROW + - - - - - - - - - - -
 *  0  | I L N E S T O D E U X
 *  1  | Q U A T R E T R O I S
 *  2  | N E U F U N E S E P T
 *  3  | H U I T S I X C I N Q
 *  4  | M I D I X M I N U I T
 *  5  | O N Z E R H E U R E S
 *  6  | M O I N S O L E D I X
 *  7  | E T R Q U A R T P M D
 *  8  | V I N G T - C I N Q U
 *  9  | E T S D E M I E P A M
 */

class Fr10x11_t : public iUhrType {
public:
    virtual LanguageAbbreviation usedLang() override {
        return LanguageAbbreviation::FR;
    };

    //------------------------------------------------------------------------------

    virtual const bool hasTwenty() override { return true; }

    //------------------------------------------------------------------------------

    virtual const bool hasTwentyfive() override { return true; }

    //------------------------------------------------------------------------------

    virtual const bool hasMitternacht() override { return true; }

    //------------------------------------------------------------------------------

    void show(FrontWord word) override {
        switch (word) {

        case FrontWord::es_ist:
            // IL EST
            setFrontMatrixWord(0, 9, 10);
            setFrontMatrixWord(0, 5, 7);
            break;

        case FrontWord::hour_1:
            // UNE HEURE
            setFrontMatrixWord(2, 4, 6);
            setFrontMatrixWord(5, 1, 5);
            break;

        case FrontWord::hour_2:
            // DEUX HEURES
            setFrontMatrixWord(0, 0, 3);
            setFrontMatrixWord(5, 0, 5);
            break;

        case FrontWord::hour_3:
            // TROIS HEURES
            setFrontMatrixWord(1, 0, 4);
            setFrontMatrixWord(5, 0, 5);
            break;

        case FrontWord::hour_4:
            // QUATRE HEURES
            setFrontMatrixWord(1, 5, 10);
            setFrontMatrixWord(5, 0, 5);
            break;

        case FrontWord::hour_5:
            // CINQ HEURES
            setFrontMatrixWord(3, 0, 3);
            setFrontMatrixWord(5, 0, 5);
            break;

        case FrontWord::hour_6:
            // SIX HEURES
            setFrontMatrixWord(3, 4, 6);
            setFrontMatrixWord(5, 0, 5);
            break;

        case FrontWord::hour_7:
            // SEPT HEURES
            setFrontMatrixWord(2, 0, 3);
            setFrontMatrixWord(5, 0, 5);
            break;

        case FrontWord::hour_8:
            // HUIT HEURES
            setFrontMatrixWord(3, 7, 10);
            setFrontMatrixWord(5, 0, 5);
            break;

        case FrontWord::hour_9:
            // NEUF HEURES
            setFrontMatrixWord(2, 7, 10);
            setFrontMatrixWord(5, 0, 5);
            break;

        case FrontWord::hour_10:
            // DIX HEURES
            setFrontMatrixWord(4, 6, 8);
            setFrontMatrixWord(5, 0, 5);
            break;

        case FrontWord::hour_11:
            // ONZE HEURES
            setFrontMatrixWord(5, 7, 10);
            setFrontMatrixWord(5, 0, 5);
            break;

        case FrontWord::hour_12:
            // MIDI
            setFrontMatrixWord(4, 7, 10);
            break;

        case FrontWord::hour_0:
            // MINUIT
            setFrontMatrixWord(4, 0, 5);
            break;

        case FrontWord::min_5:
            // CINCQ
            setFrontMatrixWord(8, 1, 4);
            break;

        case FrontWord::min_10:
            // DIX
            setFrontMatrixWord(6, 0, 2);
            break;

        case FrontWord::viertel:
            // QUART
            setFrontMatrixWord(7, 3, 7);
            break;

        case FrontWord::min_20:
            // VINGT
            setFrontMatrixWord(8, 6, 10);
            break;

        case FrontWord::min_25:
            // VENGT-CINQ
            setFrontMatrixWord(8, 1, 10);
            break;

        case FrontWord::halb:
            // ET DEMI
            setFrontMatrixWord(9, 9, 10);
            setFrontMatrixWord(9, 4, 7);
            break;

        case FrontWord::nach:
        case FrontWord::v_nach:
            // ET ("UND")
            setFrontMatrixWord(7, 9, 10);
            break;

        case FrontWord::v_vor:
            // [MOINS] LE
            setFrontMatrixWord(6, 3, 4);

        case FrontWord::vor:
            // MOINS ("Minus")
            setFrontMatrixWord(6, 6, 10);
            break;

        default:
            break;
        };
    };
};

Fr10x11_t _fr10x11;

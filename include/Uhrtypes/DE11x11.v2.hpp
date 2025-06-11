#pragma once

#include "DE11x11.hpp"

/*
 *           Layout Front
 *                COL
 *       X 9 8 7 6 5 4 3 2 1 0
 * ROW + - - - - - - - - - - -
 *  0  | E S K I S T A F Ü N F
 *  1  | Z E H N D A U V O R G
 *  2  | N A C H V I E R T E L
 *  3  | H A L B V O R N A C H
 *  4  | E I N S K U R Z W E I
 *  5  | D R E I A U J V I E R
 *  6  | F Ü N F T O S E C H S
 *  7  | S I E B E N L A C H T
 *  8  | A N E U N M H Z E H N
 *  9  | Z W Ö L F D T F E L F
 *  X  | W A S D F U N K U H R
 */

class De11x11V2_t : public De11x11_t {
public:
    //------------------------------------------------------------------------------

    virtual const bool hasTwenty() override { return false; }

    //------------------------------------------------------------------------------

    void show(FrontWord word) override {
        switch (word) {

        case FrontWord::es_ist:
            // Es
            setFrontMatrixWord(0, 9, 10);
            // Ist
            setFrontMatrixWord(0, 5, 7);
            break;

        case FrontWord::nach:
            setFrontMatrixWord(2, 7, 10);
            break;

        case FrontWord::vor:
            setFrontMatrixWord(1, 1, 3);
            break;

        case FrontWord::viertel:
            setFrontMatrixWord(2, 0, 6);
            break;

        case FrontWord::min_5:
            setFrontMatrixWord(0, 0, 3);
            break;

        case FrontWord::min_10:
            setFrontMatrixWord(1, 7, 10);
            break;

        case FrontWord::halb:
            setFrontMatrixWord(3, 7, 10);
            break;

        case FrontWord::eins:
            setFrontMatrixWord(4, 7, 10);
            break;

        case FrontWord::uhr:
            setFrontMatrixWord(10, 0, 2);
            break;

        case FrontWord::v_nach:
            setFrontMatrixWord(3, 0, 3);
            break;

        case FrontWord::v_vor:
            setFrontMatrixWord(3, 4, 6);
            break;

        case FrontWord::hour_1:
            setFrontMatrixWord(4, 8, 10);
            break;

        case FrontWord::hour_2:
            setFrontMatrixWord(4, 0, 3);
            break;

        case FrontWord::hour_3:
            setFrontMatrixWord(5, 7, 10);
            break;

        case FrontWord::hour_4:
            setFrontMatrixWord(5, 0, 3);
            break;

        case FrontWord::hour_5:
            setFrontMatrixWord(6, 7, 10);
            break;

        case FrontWord::hour_6:
            setFrontMatrixWord(6, 0, 4);
            break;

        case FrontWord::hour_7:
            setFrontMatrixWord(7, 5, 10);
            break;

        case FrontWord::hour_8:
            setFrontMatrixWord(7, 0, 3);
            break;

        case FrontWord::hour_9:
            setFrontMatrixWord(8, 6, 9);
            break;

        case FrontWord::hour_10:
            setFrontMatrixWord(8, 0, 3);
            break;

        case FrontWord::hour_11:
            setFrontMatrixWord(9, 0, 2);
            break;

        case FrontWord::hour_12:
            setFrontMatrixWord(9, 6, 10);
            break;

        case FrontWord::funk:
            setFrontMatrixWord(10, 3, 6);
            break;

        default:
            break;
        };
    };
};

De11x11V2_t _de11x11V2;
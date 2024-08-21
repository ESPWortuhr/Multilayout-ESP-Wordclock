#pragma once

#include "Uhrtype.hpp"

/*
 *           Layout Front
 *                COL
 *       X 9 8 7 6 5 4 3 2 1 0
 * ROW + - - - - - - - - - - -
 *  0  | E S K I S T L F Ü N F
 *  1  | Z E H N Z W A N Z I G
 *  2  | D R E I V I E R T E L
 *  3  | N A C H A P P Y V O R
 *  4  | H A L B I R T H D A Y
 *  5  | D R Z W Ö L F Ü N F X
 *  6  | Z E H N E U N D R E I
 *  7  | Z W E I N S I E B E N
 *  8  | E L F V I E R A C H T
 *  9  | S E C H S I U H R Y E
 *
 */

class De10x11Alternative_t : public iUhrType {
public:
    virtual LanguageAbbreviation usedLang() override {
        return LanguageAbbreviation::DE;
    };

    //------------------------------------------------------------------------------

    virtual const bool hasDreiviertel() override { return true; }

    //------------------------------------------------------------------------------

    virtual const bool hasHappyBirthday() { return true; }

    //------------------------------------------------------------------------------

    void show(FrontWord word) override {
        switch (word) {

        case FrontWord::es_ist:
            setFrontMatrixWord(0, 9, 10);
            setFrontMatrixWord(0, 5, 7);
            break;

        case FrontWord::nach:
        case FrontWord::v_nach:
            setFrontMatrixWord(3, 7, 10);
            break;

        case FrontWord::vor:
        case FrontWord::v_vor:
            setFrontMatrixWord(3, 0, 2);
            break;

        case FrontWord::viertel:
            setFrontMatrixWord(2, 0, 6);
            break;

        case FrontWord::dreiviertel:
            setFrontMatrixWord(2, 0, 10);
            break;

        case FrontWord::min_5:
            setFrontMatrixWord(0, 0, 3);
            break;

        case FrontWord::min_10:
            setFrontMatrixWord(1, 7, 10);
            break;

        case FrontWord::min_20:
            setFrontMatrixWord(1, 0, 6);
            break;

        case FrontWord::halb:
            setFrontMatrixWord(4, 7, 10);
            break;

        case FrontWord::eins:
            setFrontMatrixWord(7, 5, 8);
            break;

        case FrontWord::uhr:
            setFrontMatrixWord(9, 2, 4);
            break;

        case FrontWord::happy_birthday:
            // happy
            setFrontMatrixWord(3, 3, 7);
            // happy
            setFrontMatrixWord(4, 0, 7);
            break;

        case FrontWord::hour_1:
            setFrontMatrixWord(7, 6, 8);
            break;

        case FrontWord::hour_2:
            setFrontMatrixWord(7, 7, 10);
            break;

        case FrontWord::hour_3:
            setFrontMatrixWord(6, 0, 3);
            break;

        case FrontWord::hour_4:
            setFrontMatrixWord(8, 4, 7);
            break;

        case FrontWord::hour_5:
            setFrontMatrixWord(5, 1, 4);
            break;

        case FrontWord::hour_6:
            setFrontMatrixWord(9, 6, 10);
            break;

        case FrontWord::hour_7:
            setFrontMatrixWord(7, 0, 5);
            break;

        case FrontWord::hour_8:
            setFrontMatrixWord(8, 0, 3);
            break;

        case FrontWord::hour_9:
            setFrontMatrixWord(6, 4, 7);
            break;

        case FrontWord::hour_10:
            setFrontMatrixWord(6, 7, 10);
            break;

        case FrontWord::hour_11:
            setFrontMatrixWord(8, 8, 10);
            break;

        case FrontWord::hour_12:
            setFrontMatrixWord(5, 4, 8);
            break;

        case FrontWord::h_droelf:
            setFrontMatrixWord(5, 4, 8);
            break;

        default:
            break;
        };
    };
};

De10x11Alternative_t _de10x11Alternative;
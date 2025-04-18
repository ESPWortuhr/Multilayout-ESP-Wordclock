#pragma once

#include "Uhrtype.hpp"

/*
 *                  Layout Front
 *                       COL
 *       7 6 5 4 3 2 1 X 9 8 7 6 5 4 3 2 1 0
 * ROW + - - - - - - - - - - - - - - - - - -
 *  0  | E S A I S T O V I E R T E L E I N S
 *  1  | D R E I N E R S E C H S I E B E N E
 *  2  | E L F Ü N F N E U N V I E R A C H T
 *  3  | N U L L Z W E I N Z W Ö L F Z E H N
 *  4  | U N D O Z W A N Z I G V I E R Z I G
 *  5  | D R E I S S I G F Ü N F Z I G U H R
 *  6  | M I N U T E N I V O R U N D N A C H
 *  7  | E I N D R E I V I E R T E L H A L B
 *  8  | S I E B E N E U N U L L Z W E I N E
 *  9  | F Ü N F S E C H S N A C H T V I E R
 *  X  | D R E I N S U N D A E L F E Z E H N
 *  1  | Z W A N Z I G G R A D R E I S S I G
 *  2  | V I E R Z I G Z W Ö L F Ü N F Z I G
 *  3  | M I N U T E N U H R E F R Ü H V O R
 *  4  | A B E N D S M I T T E R N A C H T S
 *  5  | M O R G E N S W A R M M I T T A G S
 *
 */

class De16x18_t : public iUhrType {
public:
    virtual LanguageAbbreviation usedLang() override {
        return LanguageAbbreviation::DE;
    };

    //------------------------------------------------------------------------------

    virtual const bool hasMitternacht() override { return true; }

    //------------------------------------------------------------------------------

    virtual const uint8_t rowsWordMatrix() override { return 16; };

    //------------------------------------------------------------------------------

    virtual const uint8_t colsWordMatrix() override { return 18; };

    //------------------------------------------------------------------------------

    virtual const bool hasDreiviertel() override { return true; }

    //------------------------------------------------------------------------------

    virtual const bool has24HourLayout() override { return true; }

    //------------------------------------------------------------------------------

    virtual const bool has60MinuteLayout() override { return true; }

    //------------------------------------------------------------------------------

    void show(FrontWord word) override {
        switch (word) {

        // Minutes
        case FrontWord::min_1:
            setFrontMatrixWord(8, 0, 3);
            break;

        case FrontWord::min_2:
            setFrontMatrixWord(8, 2, 5);
            break;

        case FrontWord::min_3:
            setFrontMatrixWord(10, 14, 17);
            break;

        case FrontWord::min_4:
            setFrontMatrixWord(9, 0, 3);
            break;

        case FrontWord::min_5:
            setFrontMatrixWord(9, 14, 17);
            break;

        case FrontWord::min_6:
            setFrontMatrixWord(9, 9, 13);
            break;

        case FrontWord::min_7:
            setFrontMatrixWord(8, 12, 17);
            break;

        case FrontWord::min_8:
            setFrontMatrixWord(9, 4, 7);
            break;

        case FrontWord::min_9:
            setFrontMatrixWord(8, 9, 12);
            break;

        case FrontWord::min_10:
            setFrontMatrixWord(10, 0, 3);
            break;

        case FrontWord::min_11:
            setFrontMatrixWord(10, 5, 7);
            break;

        case FrontWord::min_12:
            setFrontMatrixWord(12, 6, 10);
            break;

        case FrontWord::min_13:
            setFrontMatrixWord(10, 14, 17);
            setFrontMatrixWord(10, 0, 3);
            break;

        case FrontWord::min_14:
            setFrontMatrixWord(9, 0, 3);
            setFrontMatrixWord(10, 0, 3);
            break;

        case FrontWord::min_15:
            setFrontMatrixWord(9, 14, 17);
            setFrontMatrixWord(10, 0, 3);
            break;

        case FrontWord::min_16:
            setFrontMatrixWord(9, 10, 13);
            setFrontMatrixWord(10, 0, 3);
            break;

        case FrontWord::min_17:
            setFrontMatrixWord(8, 14, 17);
            setFrontMatrixWord(10, 0, 3);
            break;

        case FrontWord::min_18:
            setFrontMatrixWord(9, 4, 7);
            setFrontMatrixWord(10, 0, 3);
            break;

        case FrontWord::min_19:
            setFrontMatrixWord(8, 9, 12);
            setFrontMatrixWord(10, 0, 3);
            break;

        case FrontWord::min_20:
            setFrontMatrixWord(11, 11, 17);
            break;

        case FrontWord::min_21:
            setFrontMatrixWord(8, 1, 3);
            setFrontMatrixWord(10, 9, 11);
            setFrontMatrixWord(11, 11, 17);
            break;

        case FrontWord::min_22:
            setFrontMatrixWord(8, 2, 5);
            setFrontMatrixWord(10, 9, 11);
            setFrontMatrixWord(11, 11, 17);
            break;

        case FrontWord::min_23:
            setFrontMatrixWord(10, 14, 17);
            setFrontMatrixWord(10, 9, 11);
            setFrontMatrixWord(11, 11, 17);
            break;

        case FrontWord::min_24:
            setFrontMatrixWord(9, 0, 3);
            setFrontMatrixWord(10, 9, 11);
            setFrontMatrixWord(11, 11, 17);
            break;

        case FrontWord::min_25:
            setFrontMatrixWord(9, 14, 17);
            setFrontMatrixWord(10, 9, 11);
            setFrontMatrixWord(11, 11, 17);
            break;

        case FrontWord::min_26:
            setFrontMatrixWord(9, 9, 13);
            setFrontMatrixWord(10, 9, 11);
            setFrontMatrixWord(11, 11, 17);
            break;

        case FrontWord::min_27:
            setFrontMatrixWord(8, 12, 17);
            setFrontMatrixWord(10, 9, 11);
            setFrontMatrixWord(11, 11, 17);
            break;

        case FrontWord::min_28:
            setFrontMatrixWord(9, 4, 7);
            setFrontMatrixWord(10, 9, 11);
            setFrontMatrixWord(11, 11, 17);
            break;

        case FrontWord::min_29:
            setFrontMatrixWord(8, 9, 12);
            setFrontMatrixWord(10, 9, 11);
            setFrontMatrixWord(11, 11, 17);
            break;

        case FrontWord::min_30:
            setFrontMatrixWord(11, 0, 7);
            break;

        case FrontWord::min_31:
            setFrontMatrixWord(8, 1, 3);
            setFrontMatrixWord(10, 9, 11);
            setFrontMatrixWord(11, 0, 7);
            break;

        case FrontWord::min_32:
            setFrontMatrixWord(8, 2, 5);
            setFrontMatrixWord(10, 9, 11);
            setFrontMatrixWord(11, 0, 7);
            break;

        case FrontWord::min_33:
            setFrontMatrixWord(10, 14, 17);
            setFrontMatrixWord(10, 9, 11);
            setFrontMatrixWord(11, 0, 7);
            break;

        case FrontWord::min_34:
            setFrontMatrixWord(9, 0, 3);
            setFrontMatrixWord(10, 9, 11);
            setFrontMatrixWord(11, 0, 7);
            break;

        case FrontWord::min_35:
            setFrontMatrixWord(9, 14, 17);
            setFrontMatrixWord(10, 9, 11);
            setFrontMatrixWord(11, 0, 7);
            break;

        case FrontWord::min_36:
            setFrontMatrixWord(9, 9, 13);
            setFrontMatrixWord(10, 9, 11);
            setFrontMatrixWord(11, 0, 7);
            break;

        case FrontWord::min_37:
            setFrontMatrixWord(8, 12, 17);
            setFrontMatrixWord(10, 9, 11);
            setFrontMatrixWord(11, 0, 7);
            break;

        case FrontWord::min_38:
            setFrontMatrixWord(9, 4, 7);
            setFrontMatrixWord(10, 9, 11);
            setFrontMatrixWord(11, 0, 7);
            break;

        case FrontWord::min_39:
            setFrontMatrixWord(8, 9, 12);
            setFrontMatrixWord(10, 9, 11);
            setFrontMatrixWord(11, 0, 7);
            break;

        case FrontWord::min_40:
            setFrontMatrixWord(12, 11, 17);
            break;

        case FrontWord::min_41:
            setFrontMatrixWord(8, 1, 3);
            setFrontMatrixWord(10, 9, 11);
            setFrontMatrixWord(12, 11, 17);
            break;

        case FrontWord::min_42:
            setFrontMatrixWord(8, 2, 5);
            setFrontMatrixWord(10, 9, 11);
            setFrontMatrixWord(12, 11, 17);
            break;

        case FrontWord::min_43:
            setFrontMatrixWord(10, 14, 17);
            setFrontMatrixWord(10, 9, 11);
            setFrontMatrixWord(12, 11, 17);
            break;

        case FrontWord::min_44:
            setFrontMatrixWord(9, 0, 3);
            setFrontMatrixWord(10, 9, 11);
            setFrontMatrixWord(12, 11, 17);
            break;

        case FrontWord::min_45:
            setFrontMatrixWord(9, 14, 17);
            setFrontMatrixWord(10, 9, 11);
            setFrontMatrixWord(12, 11, 17);
            break;

        case FrontWord::min_46:
            setFrontMatrixWord(9, 9, 13);
            setFrontMatrixWord(10, 9, 11);
            setFrontMatrixWord(12, 11, 17);
            break;

        case FrontWord::min_47:
            setFrontMatrixWord(8, 12, 17);
            setFrontMatrixWord(10, 9, 11);
            setFrontMatrixWord(12, 11, 17);
            break;

        case FrontWord::min_48:
            setFrontMatrixWord(9, 4, 7);
            setFrontMatrixWord(10, 9, 11);
            setFrontMatrixWord(12, 11, 17);
            break;

        case FrontWord::min_49:
            setFrontMatrixWord(8, 9, 12);
            setFrontMatrixWord(10, 9, 11);
            setFrontMatrixWord(12, 11, 17);
            break;

        case FrontWord::min_50:
            setFrontMatrixWord(12, 0, 6);
            break;

        case FrontWord::min_51:
            setFrontMatrixWord(8, 1, 3);
            setFrontMatrixWord(10, 9, 11);
            setFrontMatrixWord(12, 0, 6);
            break;

        case FrontWord::min_52:
            setFrontMatrixWord(8, 2, 5);
            setFrontMatrixWord(10, 9, 11);
            setFrontMatrixWord(12, 0, 6);
            break;

        case FrontWord::min_53:
            setFrontMatrixWord(10, 14, 17);
            setFrontMatrixWord(10, 9, 11);
            setFrontMatrixWord(12, 0, 6);
            break;

        case FrontWord::min_54:
            setFrontMatrixWord(9, 0, 3);
            setFrontMatrixWord(10, 9, 11);
            setFrontMatrixWord(12, 0, 6);
            break;

        case FrontWord::min_55:
            setFrontMatrixWord(9, 14, 17);
            setFrontMatrixWord(10, 9, 11);
            setFrontMatrixWord(12, 0, 6);
            break;

        case FrontWord::min_56:
            setFrontMatrixWord(9, 9, 13);
            setFrontMatrixWord(10, 9, 11);
            setFrontMatrixWord(12, 0, 6);
            break;

        case FrontWord::min_57:
            setFrontMatrixWord(8, 12, 17);
            setFrontMatrixWord(10, 9, 11);
            setFrontMatrixWord(12, 0, 6);
            break;

        case FrontWord::min_58:
            setFrontMatrixWord(9, 4, 7);
            setFrontMatrixWord(10, 9, 11);
            setFrontMatrixWord(12, 0, 6);
            break;

        case FrontWord::min_59:
            setFrontMatrixWord(8, 9, 12);
            setFrontMatrixWord(10, 9, 11);
            setFrontMatrixWord(12, 0, 6);
            break;

        // Hour
        case FrontWord::hour_0:
            setFrontMatrixWord(3, 14, 17);
            break;

        case FrontWord::hour_1:
        case FrontWord::eins:
            setFrontMatrixWord(0, 1, 3);
            break;

        case FrontWord::hour_2:
            setFrontMatrixWord(3, 10, 13);
            break;

        case FrontWord::hour_3:
            setFrontMatrixWord(1, 14, 17);
            break;

        case FrontWord::hour_4:
            setFrontMatrixWord(2, 4, 7);
            break;

        case FrontWord::hour_5:
            setFrontMatrixWord(2, 12, 15);
            break;

        case FrontWord::hour_6:
            setFrontMatrixWord(1, 6, 10);
            break;

        case FrontWord::hour_7:
            setFrontMatrixWord(1, 1, 6);
            break;

        case FrontWord::hour_8:
            setFrontMatrixWord(2, 0, 3);
            break;

        case FrontWord::hour_9:
            setFrontMatrixWord(2, 8, 11);
            break;

        case FrontWord::hour_10:
            setFrontMatrixWord(3, 0, 3);
            break;

        case FrontWord::hour_11:
            setFrontMatrixWord(2, 15, 17);
            break;

        case FrontWord::hour_12:
            setFrontMatrixWord(3, 4, 8);
            break;

        case FrontWord::hour_13:
            setFrontMatrixWord(1, 14, 17);
            setFrontMatrixWord(3, 0, 3);
            break;

        case FrontWord::hour_14:
            setFrontMatrixWord(2, 4, 7);
            setFrontMatrixWord(3, 0, 3);
            break;

        case FrontWord::hour_15:
            setFrontMatrixWord(2, 12, 15);
            setFrontMatrixWord(3, 0, 3);
            break;

        case FrontWord::hour_16:
            setFrontMatrixWord(1, 7, 10);
            setFrontMatrixWord(3, 0, 3);
            break;

        case FrontWord::hour_17:
            setFrontMatrixWord(1, 3, 6);
            setFrontMatrixWord(3, 0, 3);
            break;

        case FrontWord::hour_18:
            setFrontMatrixWord(2, 0, 3);
            setFrontMatrixWord(3, 0, 3);
            break;

        case FrontWord::hour_19:
            setFrontMatrixWord(2, 8, 11);
            setFrontMatrixWord(3, 0, 3);
            break;

        case FrontWord::hour_20:
            setFrontMatrixWord(4, 7, 13);
            break;

        case FrontWord::hour_21:
            setFrontMatrixWord(0, 1, 3);
            setFrontMatrixWord(4, 15, 17);
            setFrontMatrixWord(4, 7, 13);
            break;

        case FrontWord::hour_22:
            setFrontMatrixWord(3, 10, 13);
            setFrontMatrixWord(4, 15, 17);
            setFrontMatrixWord(4, 7, 13);
            break;

        case FrontWord::hour_23:
            setFrontMatrixWord(1, 14, 17);
            setFrontMatrixWord(4, 15, 17);
            setFrontMatrixWord(4, 7, 13);
            break;

        // Words
        case FrontWord::minute:
            setFrontMatrixWord(13, 12, 17);
            break;

        case FrontWord::minuten:
            setFrontMatrixWord(13, 11, 17);
            break;

        case FrontWord::es_ist:
            setFrontMatrixWord(0, 16, 17);
            setFrontMatrixWord(0, 12, 14);
            break;

        case FrontWord::uhr:
            setFrontMatrixWord(5, 0, 2);
            break;

        case FrontWord::und:
            setFrontMatrixWord(6, 4, 6);
            break;

        default:
            break;
        };
    };
};

De16x18_t _de16x18;
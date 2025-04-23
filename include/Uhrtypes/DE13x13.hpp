#pragma once

#include "Uhrtype.hpp"

/*
 *             Layout Front
 *                  COL
 *       2 1 X 9 8 7 6 5 4 3 2 1 0
 * ROW + - - - - - - - - - - - - -
 *  0  | F E S A I S T B S E C H S
 *  1  | Z E H N N U L L Z W Ö L F
 *  2  | E L F Z W E I S I E B E N
 *  3  | V I E R L E I N A N E U N
 *  4  | D R E I A C H T B F Ü N F
 *  5  | U H R N S E C H S V I E R
 *  6  | E I N S F Ü N F Z W E I B
 *  7  | N E U N A C H T G D R E I
 *  8  | S I E B E N F A B L A B N
 *  9  | U N D B G Z W A N Z I G F
 *  X  | Z W Ö L F A F Ü N F Z I G
 *  1  | V I E R Z I G B Z E H N L
 *  2  | A D R E I S S I G E L F B
 *
 */

class De13x13_t : public iUhrType {
public:
    virtual LanguageAbbreviation usedLang() override {
        return LanguageAbbreviation::DE;
    };

    //------------------------------------------------------------------------------

    virtual const bool hasMitternacht() override { return true; }

    //------------------------------------------------------------------------------

    virtual const uint8_t rowsWordMatrix() override { return 13; };

    //------------------------------------------------------------------------------

    virtual const uint8_t colsWordMatrix() override { return 13; };

    //------------------------------------------------------------------------------

    virtual const bool has60MinuteLayout() override { return true; }

    //------------------------------------------------------------------------------

    void show(FrontWord word) override {
        switch (word) {

        case FrontWord::es_ist:
            setFrontMatrixWord(0, 10, 11);
            setFrontMatrixWord(0, 6, 8);
            break;

        // Hour
        case FrontWord::hour_0:
            setFrontMatrixWord(1, 5, 8);
            break;

        case FrontWord::hour_1:
        case FrontWord::eins:
            setFrontMatrixWord(3, 5, 7);
            break;

        case FrontWord::hour_2:
            setFrontMatrixWord(2, 6, 9);
            break;

        case FrontWord::hour_3:
            setFrontMatrixWord(4, 9, 12);
            break;

        case FrontWord::hour_4:
            setFrontMatrixWord(3, 9, 12);
            break;

        case FrontWord::hour_5:
            setFrontMatrixWord(4, 0, 3);
            break;

        case FrontWord::hour_6:
            setFrontMatrixWord(0, 0, 4);
            break;

        case FrontWord::hour_7:
            setFrontMatrixWord(2, 0, 5);
            break;

        case FrontWord::hour_8:
            setFrontMatrixWord(4, 5, 8);
            break;

        case FrontWord::hour_9:
            setFrontMatrixWord(3, 0, 3);
            break;

        case FrontWord::hour_10:
            setFrontMatrixWord(1, 9, 12);
            break;

        case FrontWord::hour_11:
            setFrontMatrixWord(2, 10, 12);
            break;

        case FrontWord::hour_12:
            setFrontMatrixWord(1, 0, 4);
            break;

        case FrontWord::uhr:
            setFrontMatrixWord(5, 10, 12);
            break;

        // Minutes
        case FrontWord::min_1:
            setFrontMatrixWord(6, 9, 12);
            break;

        case FrontWord::min_2:
            setFrontMatrixWord(6, 1, 4);
            break;

        case FrontWord::min_3:
            setFrontMatrixWord(7, 0, 3);
            break;

        case FrontWord::min_4:
            setFrontMatrixWord(5, 0, 3);
            break;

        case FrontWord::min_5:
            setFrontMatrixWord(6, 5, 8);
            break;

        case FrontWord::min_6:
            setFrontMatrixWord(5, 4, 8);
            break;

        case FrontWord::min_7:
            setFrontMatrixWord(8, 7, 12);
            break;

        case FrontWord::min_8:
            setFrontMatrixWord(7, 5, 8);
            break;

        case FrontWord::min_9:
            setFrontMatrixWord(7, 9, 12);
            break;

        case FrontWord::min_10:
            setFrontMatrixWord(11, 1, 4);
            break;

        case FrontWord::min_11:
            setFrontMatrixWord(12, 1, 3);
            break;

        case FrontWord::min_12:
            setFrontMatrixWord(10, 8, 12);
            break;

        case FrontWord::min_13:
            show(FrontWord::min_3);
            show(FrontWord::min_10);
            break;

        case FrontWord::min_14:
            show(FrontWord::min_4);
            show(FrontWord::min_10);
            break;

        case FrontWord::min_15:
            show(FrontWord::min_5);
            show(FrontWord::min_10);
            break;

        case FrontWord::min_16:
            setFrontMatrixWord(5, 5, 8);
            show(FrontWord::min_10);
            break;

        case FrontWord::min_17:
            setFrontMatrixWord(8, 9, 12);
            show(FrontWord::min_10);
            break;

        case FrontWord::min_18:
            show(FrontWord::min_8);
            show(FrontWord::min_10);
            break;

        case FrontWord::min_19:
            show(FrontWord::min_9);
            show(FrontWord::min_10);
            break;

        case FrontWord::min_20:
            setFrontMatrixWord(9, 1, 7);
            break;

        case FrontWord::min_21:
            setFrontMatrixWord(6, 10, 12);
            setFrontMatrixWord(9, 10, 12); // und
            show(FrontWord::min_20);
            break;

        case FrontWord::min_22:
            show(FrontWord::min_2);
            setFrontMatrixWord(9, 10, 12); // und
            show(FrontWord::min_20);
            break;

        case FrontWord::min_23:
            show(FrontWord::min_3);
            setFrontMatrixWord(9, 10, 12); // und
            show(FrontWord::min_20);
            break;

        case FrontWord::min_24:
            show(FrontWord::min_4);
            setFrontMatrixWord(9, 10, 12); // und
            show(FrontWord::min_20);
            break;

        case FrontWord::min_25:
            show(FrontWord::min_5);
            setFrontMatrixWord(9, 10, 12); // und
            show(FrontWord::min_20);
            break;

        case FrontWord::min_26:
            show(FrontWord::min_6);
            setFrontMatrixWord(9, 10, 12); // und
            show(FrontWord::min_20);
            break;

        case FrontWord::min_27:
            show(FrontWord::min_7);
            setFrontMatrixWord(9, 10, 12); // und
            show(FrontWord::min_20);
            break;

        case FrontWord::min_28:
            show(FrontWord::min_8);
            setFrontMatrixWord(9, 10, 12); // und
            show(FrontWord::min_20);
            break;

        case FrontWord::min_29:
            show(FrontWord::min_9);
            setFrontMatrixWord(9, 10, 12); // und
            show(FrontWord::min_20);
            break;

        case FrontWord::min_30:
            setFrontMatrixWord(12, 4, 11);
            break;

        case FrontWord::min_31:
            setFrontMatrixWord(6, 10, 12);
            setFrontMatrixWord(9, 10, 12); // und
            show(FrontWord::min_30);
            break;

        case FrontWord::min_32:
            show(FrontWord::min_2);
            setFrontMatrixWord(9, 10, 12); // und
            show(FrontWord::min_30);
            break;

        case FrontWord::min_33:
            show(FrontWord::min_3);
            setFrontMatrixWord(9, 10, 12); // und
            show(FrontWord::min_30);
            break;

        case FrontWord::min_34:
            show(FrontWord::min_4);
            setFrontMatrixWord(9, 10, 12); // und
            show(FrontWord::min_30);
            break;

        case FrontWord::min_35:
            show(FrontWord::min_5);
            setFrontMatrixWord(9, 10, 12); // und
            show(FrontWord::min_30);
            break;

        case FrontWord::min_36:
            show(FrontWord::min_6);
            setFrontMatrixWord(9, 10, 12); // und
            show(FrontWord::min_30);
            break;

        case FrontWord::min_37:
            show(FrontWord::min_7);
            setFrontMatrixWord(9, 10, 12); // und
            show(FrontWord::min_30);
            break;

        case FrontWord::min_38:
            show(FrontWord::min_8);
            setFrontMatrixWord(9, 10, 12); // und
            show(FrontWord::min_30);
            break;

        case FrontWord::min_39:
            show(FrontWord::min_9);
            setFrontMatrixWord(9, 10, 12); // und
            show(FrontWord::min_30);
            break;

        case FrontWord::min_40:
            setFrontMatrixWord(11, 6, 12);
            break;

        case FrontWord::min_41:
            setFrontMatrixWord(6, 10, 12);
            setFrontMatrixWord(9, 10, 12); // und
            show(FrontWord::min_40);
            break;

        case FrontWord::min_42:
            show(FrontWord::min_2);
            setFrontMatrixWord(9, 10, 12); // und
            show(FrontWord::min_40);
            break;

        case FrontWord::min_43:
            show(FrontWord::min_3);
            setFrontMatrixWord(9, 10, 12); // und
            show(FrontWord::min_40);
            break;

        case FrontWord::min_44:
            show(FrontWord::min_4);
            setFrontMatrixWord(9, 10, 12); // und
            show(FrontWord::min_40);
            break;

        case FrontWord::min_45:
            show(FrontWord::min_5);
            setFrontMatrixWord(9, 10, 12); // und
            show(FrontWord::min_40);
            break;

        case FrontWord::min_46:
            show(FrontWord::min_6);
            setFrontMatrixWord(9, 10, 12); // und
            show(FrontWord::min_40);
            break;

        case FrontWord::min_47:
            show(FrontWord::min_7);
            setFrontMatrixWord(9, 10, 12); // und
            show(FrontWord::min_40);
            break;

        case FrontWord::min_48:
            show(FrontWord::min_8);
            setFrontMatrixWord(9, 10, 12); // und
            show(FrontWord::min_40);
            break;

        case FrontWord::min_49:
            show(FrontWord::min_9);
            setFrontMatrixWord(9, 10, 12); // und
            show(FrontWord::min_40);
            break;

        case FrontWord::min_50:
            setFrontMatrixWord(10, 0, 6);
            break;

        case FrontWord::min_51:
            setFrontMatrixWord(6, 10, 12);
            setFrontMatrixWord(9, 10, 12); // und
            show(FrontWord::min_50);
            break;

        case FrontWord::min_52:
            show(FrontWord::min_2);
            setFrontMatrixWord(9, 10, 12); // und
            show(FrontWord::min_50);
            break;

        case FrontWord::min_53:
            show(FrontWord::min_3);
            setFrontMatrixWord(9, 10, 12); // und
            show(FrontWord::min_50);
            break;

        case FrontWord::min_54:
            show(FrontWord::min_4);
            setFrontMatrixWord(9, 10, 12); // und
            show(FrontWord::min_50);
            break;

        case FrontWord::min_55:
            show(FrontWord::min_5);
            setFrontMatrixWord(9, 10, 12); // und
            show(FrontWord::min_50);
            break;

        case FrontWord::min_56:
            show(FrontWord::min_6);
            setFrontMatrixWord(9, 10, 12); // und
            show(FrontWord::min_50);
            break;

        case FrontWord::min_57:
            show(FrontWord::min_7);
            setFrontMatrixWord(9, 10, 12); // und
            show(FrontWord::min_50);
            break;

        case FrontWord::min_58:
            show(FrontWord::min_8);
            setFrontMatrixWord(9, 10, 12); // und
            show(FrontWord::min_50);
            break;

        case FrontWord::min_59:
            show(FrontWord::min_9);
            setFrontMatrixWord(9, 10, 12); // und
            show(FrontWord::min_50);
            break;

        default:
            break;
        };
    };
};

De13x13_t _de13x13;
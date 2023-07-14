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

    virtual const uint8_t rowsWordMatrix() override { return 16; };

    //------------------------------------------------------------------------------

    virtual const uint8_t colsWordMatrix() override { return 18; };

    //------------------------------------------------------------------------------

    virtual const bool hasDreiviertel() override { return true; }

    //------------------------------------------------------------------------------

    virtual const bool has24HourLayout() override { return true; }

    //------------------------------------------------------------------------------

    void show(FrontWord word) override {
        switch (word) {

        case FrontWord::m_eine:
            setFrontMatrixWord(1, 12, 15);
            break;

        case FrontWord::m_zwei:
            setFrontMatrixWord(3, 10, 13);
            break;

        case FrontWord::m_drei:
            setFrontMatrixWord(1, 14, 17);
            break;

        case FrontWord::m_vier:
            setFrontMatrixWord(2, 4, 7);
            break;

        case FrontWord::m_fuenf:
            setFrontMatrixWord(2, 12, 15);
            break;

        case FrontWord::m_sechs:
            setFrontMatrixWord(1, 6, 10);
            break;

        case FrontWord::m_sieben:
            setFrontMatrixWord(1, 1, 6);
            break;

        case FrontWord::m_acht:
            setFrontMatrixWord(2, 0, 3);
            break;

        case FrontWord::m_neun:
            setFrontMatrixWord(2, 8, 11);
            break;

        case FrontWord::m_zehn:
            setFrontMatrixWord(3, 0, 3);
            break;

        case FrontWord::m_elf:
            setFrontMatrixWord(2, 15, 17);
            break;

        case FrontWord::m_zwoelf:
            setFrontMatrixWord(3, 4, 8);
            break;

        case FrontWord::m_dreizehn:
            setFrontMatrixWord(1, 14, 17);
            setFrontMatrixWord(3, 0, 3);
            break;

        case FrontWord::m_vierzehn:
            setFrontMatrixWord(2, 4, 7);
            setFrontMatrixWord(3, 0, 3);
            break;

            // viertel nach
        case FrontWord::m_sechzehn:
            setFrontMatrixWord(1, 7, 10);
            setFrontMatrixWord(3, 0, 3);
            break;

        case FrontWord::m_siebzehn:
            setFrontMatrixWord(1, 3, 6);
            setFrontMatrixWord(3, 0, 3);
            break;

        case FrontWord::m_achtzehn:
            setFrontMatrixWord(2, 0, 3);
            setFrontMatrixWord(3, 0, 3);
            break;

        case FrontWord::m_neunzehn:
            setFrontMatrixWord(2, 8, 11);
            setFrontMatrixWord(3, 0, 3);
            break;

        case FrontWord::m_zwanzig:
            setFrontMatrixWord(4, 7, 13);
            break;

        case FrontWord::minute:
            setFrontMatrixWord(6, 12, 17);
            break;

        case FrontWord::minuten:
            setFrontMatrixWord(6, 11, 17);

        default:
            break;
        }

        switch (word) {
        case FrontWord::es_ist:
            setFrontMatrixWord(0, 16, 17);
            setFrontMatrixWord(0, 12, 14);
            break;

        case FrontWord::nach:
            break;

        case FrontWord::v_nach:
            setFrontMatrixWord(6, 0, 3);
            break;

        case FrontWord::vor:
            break;

        case FrontWord::v_vor:
            setFrontMatrixWord(6, 7, 9);
            break;

        case FrontWord::viertel:
            setFrontMatrixWord(0, 4, 10);
            break;

        case FrontWord::dreiviertel:
            setFrontMatrixWord(7, 4, 14);
            break;

        case FrontWord::halb:
            setFrontMatrixWord(7, 0, 3);
            break;

        case FrontWord::frueh:
            setFrontMatrixWord(13, 3, 6);
            break;

        case FrontWord::minuten_uhr:
            setFrontMatrixWord(13, 8, 11);
            break;

        case FrontWord::minuten_extra:
            setFrontMatrixWord(13, 11, 17);
            break;

        case FrontWord::abends:
            setFrontMatrixWord(14, 12, 17);
            break;

        case FrontWord::mitternacht:
            setFrontMatrixWord(14, 1, 11);
            break;

        case FrontWord::mittags:
            setFrontMatrixWord(15, 0, 6);
            break;

        case FrontWord::warm:
            setFrontMatrixWord(14, 7, 10);
            break;

        case FrontWord::morgens:
            setFrontMatrixWord(15, 11, 17);
            break;

        case FrontWord::uhr:
            setFrontMatrixWord(5, 0, 2);
            break;

        case FrontWord::und:
            setFrontMatrixWord(6, 4, 6);

            // Stunden
            break;

        case FrontWord::h_ein:
            setFrontMatrixWord(0, 0, 3);
            break;

        case FrontWord::h_zwei:
            setFrontMatrixWord(3, 10, 13);
            break;

        case FrontWord::h_drei:
            setFrontMatrixWord(1, 14, 17);
            break;

        case FrontWord::h_vier:
            setFrontMatrixWord(2, 4, 7);
            break;

        case FrontWord::h_fuenf:
            setFrontMatrixWord(2, 12, 15);
            break;

        case FrontWord::h_sechs:
            setFrontMatrixWord(1, 6, 10);
            break;

        case FrontWord::h_sieben:
            setFrontMatrixWord(1, 1, 6);
            break;

        case FrontWord::h_acht:
            setFrontMatrixWord(2, 0, 3);
            break;

        case FrontWord::h_neun:
            setFrontMatrixWord(2, 8, 11);
            break;

        case FrontWord::h_zehn:
            setFrontMatrixWord(3, 0, 3);
            break;

        case FrontWord::h_elf:
            setFrontMatrixWord(2, 15, 17);
            break;

        case FrontWord::h_zwoelf:
            setFrontMatrixWord(3, 4, 8);
            break;

        default:
            break;
        };
    };
};

De16x18_t _de16x18;
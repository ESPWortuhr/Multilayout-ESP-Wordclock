#pragma once

#include "Uhrtype.hpp"

/*
 * Vertical Layout Front
 * Minutes 110 111 112 113
 * 
 * E S K I S T A F Ü N F    009 010 029 030 049 050 069 070 089 090 109 
 * Z E H N Z W A N Z I G    008 011 028 031 048 051 068 071 088 091 108
 * D R E I V I E R T E L    007 012 027 032 047 052 067 072 087 092 107
 * V O R F U N K N A C H    006 013 026 033 046 053 066 073 086 093 106
 * H A L B A E L F Ü N F    005 014 025 034 045 054 065 074 085 094 105
 * E I N S X A M Z W E I    004 015 024 035 044 055 064 075 084 095 104
 * D R E I A U J V I E R    003 016 023 036 043 056 063 076 083 096 103
 * S E C H S N L A C H T    002 017 022 037 042 057 062 077 082 097 102
 * S I E B E N Z W Ö L F    001 018 021 038 041 058 061 078 081 098 101
 * Z E H N E U N K U H R    000 019 020 039 040 059 060 079 080 099 100
*/

class De10x11Vertical_t : public iUhrType {
public: 
    //------------------------------------------------------------------------------
    virtual LanguageAbbreviation usedLang() override {
        return LanguageAbbreviation::DE;
    };
    //------------------------------------------------------------------------------
    virtual const bool hasDreiviertel() override { return true; }
    //------------------------------------------------------------------------------
    virtual const void getFrontMatrixColRow(uint8_t &row, uint8_t &col,
                                            const uint16 index) {
        row = index % rowsWordMatrix();
        col = colsWordMatrix() -1 - ( index / rowsWordMatrix());
        if (col % 2 == 0) {
            row = rowsWordMatrix() - 1 - row;
        }
    };
    //------------------------------------------------------------------------------
    void show(FrontWord word) override {
        switch (word) {

        case FrontWord::es_ist:
            // Es
            setFrontMatrixPixel(9);
            setFrontMatrixPixel(10);

            // Ist
            setFrontMatrixPixel(30);
            setFrontMatrixPixel(49);
            setFrontMatrixPixel(50);
            break;

            //------------------------------------------------------------------------------

        case FrontWord::nach:
        case FrontWord::v_nach:
            setFrontMatrixPixel(73);
            setFrontMatrixPixel(86);
            setFrontMatrixPixel(93);
            setFrontMatrixPixel(106);
            break;

            //------------------------------------------------------------------------------

        case FrontWord::vor:
        case FrontWord::v_vor:
            setFrontMatrixPixel(6);
            setFrontMatrixPixel(13);
            setFrontMatrixPixel(26);
            break;
            //------------------------------------------------------------------------------

        case FrontWord::viertel:
            setFrontMatrixPixel(47);
            setFrontMatrixPixel(52);
            setFrontMatrixPixel(67);
            setFrontMatrixPixel(72);
            setFrontMatrixPixel(87);
            setFrontMatrixPixel(92);
            setFrontMatrixPixel(107);
            break;

            //------------------------------------------------------------------------------

        case FrontWord::dreiviertel:
            setFrontMatrixPixel(07);
            setFrontMatrixPixel(12);
            setFrontMatrixPixel(27);
            setFrontMatrixPixel(32);
            setFrontMatrixPixel(47);
            setFrontMatrixPixel(52);
            setFrontMatrixPixel(67);
            setFrontMatrixPixel(72);
            setFrontMatrixPixel(87);
            setFrontMatrixPixel(92);
            setFrontMatrixPixel(107);
            break;

            //------------------------------------------------------------------------------

        case FrontWord::fuenf:
            setFrontMatrixPixel(70);
            setFrontMatrixPixel(89);
            setFrontMatrixPixel(90);
            setFrontMatrixPixel(109);
            break;

            //------------------------------------------------------------------------------

        case FrontWord::zehn:
            setFrontMatrixPixel(8);
            setFrontMatrixPixel(11);
            setFrontMatrixPixel(28);
            setFrontMatrixPixel(31);
            break;

            //------------------------------------------------------------------------------
        case FrontWord::zwanzig:
            setFrontMatrixPixel(48);
            setFrontMatrixPixel(51);
            setFrontMatrixPixel(68);
            setFrontMatrixPixel(71);
            setFrontMatrixPixel(88);
            setFrontMatrixPixel(91);
            setFrontMatrixPixel(108);
            break;

            //------------------------------------------------------------------------------

        case FrontWord::halb:
            setFrontMatrixPixel(5);
            setFrontMatrixPixel(14);
            setFrontMatrixPixel(25);
            setFrontMatrixPixel(34);
            break;

            //------------------------------------------------------------------------------

        case FrontWord::eins:
            setFrontMatrixPixel(4);
            setFrontMatrixPixel(15);
            setFrontMatrixPixel(24);
            setFrontMatrixPixel(34);
            break;

            //------------------------------------------------------------------------------

        case FrontWord::uhr:
            setFrontMatrixPixel(80);
            setFrontMatrixPixel(99);
            setFrontMatrixPixel(100);
            break;

            //------------------------------------------------------------------------------

        case FrontWord::h_ein:
            setFrontMatrixPixel(4);
            setFrontMatrixPixel(15);
            setFrontMatrixPixel(24);
            break;

            //------------------------------------------------------------------------------

        case FrontWord::h_zwei:
            setFrontMatrixPixel(75);
            setFrontMatrixPixel(84);
            setFrontMatrixPixel(95);
            setFrontMatrixPixel(104);
            break;

            //------------------------------------------------------------------------------

        case FrontWord::h_drei:
            setFrontMatrixPixel(7);
            setFrontMatrixPixel(12);
            setFrontMatrixPixel(27);
            setFrontMatrixPixel(32);
            break;

            //------------------------------------------------------------------------------

        case FrontWord::h_vier:
            setFrontMatrixPixel(76);
            setFrontMatrixPixel(83);
            setFrontMatrixPixel(96);
            setFrontMatrixPixel(103);
            break;

            //------------------------------------------------------------------------------

        case FrontWord::h_fuenf:
            setFrontMatrixPixel(74);
            setFrontMatrixPixel(85);
            setFrontMatrixPixel(94);
            setFrontMatrixPixel(105);
            break;

            //------------------------------------------------------------------------------

        case FrontWord::h_sechs:
            setFrontMatrixPixel(2);
            setFrontMatrixPixel(17);
            setFrontMatrixPixel(22);
            setFrontMatrixPixel(37);
            setFrontMatrixPixel(42);
            break;

            //------------------------------------------------------------------------------

        case FrontWord::h_sieben:
            setFrontMatrixPixel(1);
            setFrontMatrixPixel(18);
            setFrontMatrixPixel(21);
            setFrontMatrixPixel(38);
            setFrontMatrixPixel(41);
            setFrontMatrixPixel(58);
            break;

            //------------------------------------------------------------------------------

        case FrontWord::h_acht:
            setFrontMatrixPixel(77);
            setFrontMatrixPixel(82);
            setFrontMatrixPixel(97);
            setFrontMatrixPixel(102);
            break;

            //------------------------------------------------------------------------------

        case FrontWord::h_neun:
            setFrontMatrixPixel(39);
            setFrontMatrixPixel(40);
            setFrontMatrixPixel(59);
            setFrontMatrixPixel(60);
            break;

            //------------------------------------------------------------------------------

        case FrontWord::h_zehn:
            setFrontMatrixPixel(0);
            setFrontMatrixPixel(19);
            setFrontMatrixPixel(20);
            setFrontMatrixPixel(39);
            break;

            //------------------------------------------------------------------------------

        case FrontWord::h_elf:
            setFrontMatrixPixel(54);
            setFrontMatrixPixel(65);
            setFrontMatrixPixel(74);
            break;

            //------------------------------------------------------------------------------

        case FrontWord::h_zwoelf:

            setFrontMatrixPixel(61);
            setFrontMatrixPixel(78);
            setFrontMatrixPixel(81);
            setFrontMatrixPixel(98);
            setFrontMatrixPixel(101);
            break;

        default:
            break;
            //------------------------------------------------------------------------------
        };
    };
};

De10x11Vertical_t _de10x11Vertical;
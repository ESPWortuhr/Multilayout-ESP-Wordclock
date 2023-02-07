#pragma once

#include "Uhrtype.hpp"

/*
 * Layout Front
 *
 * E S K I S T R F Ü N F
 * Z E H N Z W A N Z I G
 * D R E I V I E R T E L
 * T G N A C H V O R U M
 * H A L B G Z W Ö L F J
 * Z W E I N S I E B E N
 * K D R E I R H F Ü N F
 * E L F N E U N V I E R
 * N A C H T Z E H N B X
 * U S E C H S F U E R Y
 * W A S D F U N K U H R
 * + + + +
 */

/*
 * LED Layout
 *
 * 0  ,1  ,2  ,3  ,4  ,5  ,6  ,7  ,8  ,9  ,10 ,11 ,12
 * 25 ,24 ,23 ,22 ,21 ,20 ,19 ,18 ,17 ,16 ,15 ,14 ,13
 * 26 ,27 ,28 ,29 ,30 ,31 ,32 ,33 ,34 ,35 ,36 ,37 ,38
 * 51 ,50 ,49 ,48 ,47 ,46 ,45 ,44 ,43 ,42 ,41 ,40 ,39
 * 52 ,53 ,54 ,55 ,56 ,57 ,58 ,59 ,60 ,61 ,62 ,63 ,64
 * 77 ,76 ,75 ,74 ,73 ,72 ,71 ,70 ,69 ,68 ,67 ,66 ,65
 * 78 ,79 ,80 ,81 ,82 ,83 ,84 ,85 ,86 ,87 ,88 ,89 ,90
 * 103,102,101,100,99 ,98 ,97 ,96 ,95 ,94 ,93 ,92 ,91
 * 104,105,106,107,108,109,110,111,112,113,114,115,116
 * 129,128,127,126,125,124,123,122,121,120,119,118,117
 * 130,131,132,133,134,135,136,137,138,139,140,141,142
 * 155,154,153,152,151,150,149,148,147,146,145,144,143
 * 156,157,158,159,160,161,162,163,164,165,166,167,168
 */

class De11x11frame_t : public iUhrType {
public:
    virtual LanguageAbbreviation usedLang() override {
        return LanguageAbbreviation::DE;
    };

    //------------------------------------------------------------------------------

    const uint16_t rmatrix[48] = {
        0,   1,   2,   3,   4,   5,   6,   7,   8,   9,   10,  11,
        12,  13,  38,  39,  64,  65,  90,  91,  116, 117, 142, 143,
        168, 167, 166, 165, 164, 163, 162, 161, 160, 159, 158, 157,
        156, 155, 130, 129, 104, 103, 78,  77,  52,  51,  26,  25};

    //------------------------------------------------------------------------------

    const uint16_t minArr[2][4] = {
        {0, 12, 168, 156},   // LEDs for "Normal" minute display
        {152, 150, 148, 146} // LEDs für "Row" type minute display
    };

    //------------------------------------------------------------------------------

    virtual const void getMinArr(uint16_t *returnArr, uint8_t col) {
        for (uint8_t i = 0; i < 4; i++) {
            returnArr[i] = minArr[col][i];
        }
    };

    //------------------------------------------------------------------------------

    virtual const uint16_t getFrontMatrix(uint8_t row, uint8_t col) override {
        row++;
        if (row % 2 != 0) {
            col = COLS_MATRIX() - col - 1;
        }
        uint16_t returnValue = col + 1 + (row * COLS_MATRIX() + 2);
        if (returnValue > NUM_PIXELS()) {
            Serial.println("[ERROR] getMatrix() ReturnValue out of Bounds");
        }
        return returnValue;
    };
    //------------------------------------------------------------------------------

    virtual const uint16_t getRMatrix(uint16_t index) override {
        return rmatrix[index];
    };

    //------------------------------------------------------------------------------

    virtual const uint16_t getSMatrix(uint16_t index) override {
        uint8_t row = index / COLS_MATRIX();
        uint8_t col = index % COLS_MATRIX();
        if (row % 2 == 0) {
            col = COLS_MATRIX() - 1 - col;
        }
        return getFrontMatrix(row, col);
    };

    //------------------------------------------------------------------------------

    virtual const uint16_t NUM_PIXELS() override { return 169; };

    //------------------------------------------------------------------------------

    virtual const uint16_t NUM_SMATRIX() override { return 121; };

    //------------------------------------------------------------------------------

    virtual const uint16_t NUM_RMATRIX() override { return 48; };

    //------------------------------------------------------------------------------

    virtual const bool hasSecondsFrame() override { return true; }

    //------------------------------------------------------------------------------

    void show(FrontWord word) override {
        switch (word) {

        case FrontWord::es_ist:
            // Es
            setLetter(24);
            setLetter(23);

            // Ist
            setLetter(21);
            setLetter(20);
            setLetter(19);
            break;

        case FrontWord::nach:
        case FrontWord::v_nach:
            setLetter(55);
            setLetter(56);
            setLetter(57);
            setLetter(58);
            break;

        case FrontWord::vor:
        case FrontWord::v_vor:
            setLetter(59);
            setLetter(60);
            setLetter(61);

        case FrontWord::viertel:
            setLetter(40);
            setLetter(41);
            setLetter(42);
            setLetter(43);
            setLetter(44);
            setLetter(45);
            setLetter(46);
            break;

        case FrontWord::fuenf:
            setLetter(14);
            setLetter(15);
            setLetter(16);
            setLetter(17);
            break;

        case FrontWord::zehn:
            setLetter(27);
            setLetter(28);
            setLetter(29);
            setLetter(30);
            break;

        case FrontWord::zwanzig:
            setLetter(31);
            setLetter(32);
            setLetter(33);
            setLetter(34);
            setLetter(35);
            setLetter(36);
            setLetter(37);
            break;

        case FrontWord::halb:
            setLetter(73);
            setLetter(74);
            setLetter(75);
            setLetter(76);
            break;

        case FrontWord::eins:
            setLetter(81);
            setLetter(82);
            setLetter(83);
            setLetter(84);
            break;

        case FrontWord::uhr:
            setLetter(146);
            setLetter(145);
            setLetter(144);
            break;

        case FrontWord::h_ein:
            setLetter(81);
            setLetter(82);
            setLetter(83);
            break;

        case FrontWord::h_zwei:
            setLetter(79);
            setLetter(80);
            setLetter(81);
            setLetter(82);
            break;

        case FrontWord::h_drei:
            setLetter(98);
            setLetter(99);
            setLetter(100);
            setLetter(101);
            break;

        case FrontWord::h_vier:
            setLetter(112);
            setLetter(113);
            setLetter(114);
            setLetter(115);
            break;

        case FrontWord::h_fuenf:
            setLetter(92);
            setLetter(93);
            setLetter(94);
            setLetter(95);
            break;

        case FrontWord::h_sechs:
            setLetter(132);
            setLetter(133);
            setLetter(134);
            setLetter(135);
            setLetter(136);
            break;

        case FrontWord::h_sieben:
            setLetter(84);
            setLetter(85);
            setLetter(86);
            setLetter(87);
            setLetter(88);
            setLetter(89);
            break;

        case FrontWord::h_acht:
            setLetter(124);
            setLetter(125);
            setLetter(126);
            setLetter(127);
            break;

        case FrontWord::h_neun:
            setLetter(108);
            setLetter(109);
            setLetter(110);
            setLetter(111);
            break;

        case FrontWord::h_zehn:
            setLetter(120);
            setLetter(121);
            setLetter(122);
            setLetter(123);
            break;

        case FrontWord::h_elf:
            setLetter(105);
            setLetter(106);
            setLetter(107);
            break;

        case FrontWord::h_zwoelf:
            setLetter(67);
            setLetter(68);
            setLetter(69);
            setLetter(70);
            setLetter(71);
            break;

        case FrontWord::funk:
            setLetter(150);
            setLetter(149);
            setLetter(148);
            setLetter(147);
            break;

        default:
            break;
        };
    };
};

De11x11frame_t _de11x11frame;
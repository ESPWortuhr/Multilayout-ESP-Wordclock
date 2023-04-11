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

    virtual const void getMinuteArray(uint16_t *returnArr, uint8_t col) {
        const uint8_t corner[4] = {152, 150, 148, 146};

        for (uint8_t i = 0; i < 4; i++) {
            switch (col) {
            case 0:
                // LEDs for "LED4x" minute display
                // {150, 149, 148, 147}
                returnArr[i] = 150 - i;
                break;
            case 1:
                // LEDs for "LED7x" minute display
                // {152, 150, 148, 146}
                returnArr[i] = 152 - (i * 2);
                break;
            case 2:
                // LEDs for "Corners" minute display
                // {0, 12, 168, 156}
                returnArr[i] = corner[i];
                break;

            default:
                break;
            }
        }
    };

    //------------------------------------------------------------------------------

    virtual const uint16_t getFrontMatrixIndex(uint8_t row,
                                               uint8_t col) override {
        row++;
        if (row % 2 != 0) {
            col = colsWordMatrix() - col - 1;
        }
        uint16_t returnValue = col + 1 + (row * colsWordMatrix() + 2);
        if (returnValue > numPixels()) {
            Serial.println("[ERROR] getMatrix() ReturnValue out of Bounds");
        }
        return returnValue;
    };
    //------------------------------------------------------------------------------

    virtual const uint16_t getFrameMatrixIndex(uint16_t index) override {
        return rmatrix[index];
    };

    //------------------------------------------------------------------------------

    virtual const uint16_t getWordMatrixIndex(uint16_t index) override {
        uint8_t row = index / colsWordMatrix();
        uint8_t col = index % colsWordMatrix();
        if (row % 2 == 0) {
            col = colsWordMatrix() - 1 - col;
        }
        return getFrontMatrixIndex(row, col);
    };

    //------------------------------------------------------------------------------

    virtual const uint16_t numPixels() override { return 169; };

    //------------------------------------------------------------------------------

    virtual const uint8_t rowsWordMatrix() override { return 11; };

    //------------------------------------------------------------------------------

    virtual const uint16_t numPixelsWordMatrix() override { return 121; };

    //------------------------------------------------------------------------------

    virtual const uint8_t numPixelsFrameMatrix() override { return 48; };

    //------------------------------------------------------------------------------

    virtual const bool hasSecondsFrame() override { return true; }

    //------------------------------------------------------------------------------

    void show(FrontWord word) override {
        switch (word) {

        case FrontWord::es_ist:
            // Es
            setFrontMatrixPixel(24);
            setFrontMatrixPixel(23);

            // Ist
            setFrontMatrixPixel(21);
            setFrontMatrixPixel(20);
            setFrontMatrixPixel(19);
            break;

        case FrontWord::nach:
        case FrontWord::v_nach:
            setFrontMatrixPixel(55);
            setFrontMatrixPixel(56);
            setFrontMatrixPixel(57);
            setFrontMatrixPixel(58);
            break;

        case FrontWord::vor:
        case FrontWord::v_vor:
            setFrontMatrixPixel(59);
            setFrontMatrixPixel(60);
            setFrontMatrixPixel(61);

        case FrontWord::viertel:
            setFrontMatrixPixel(40);
            setFrontMatrixPixel(41);
            setFrontMatrixPixel(42);
            setFrontMatrixPixel(43);
            setFrontMatrixPixel(44);
            setFrontMatrixPixel(45);
            setFrontMatrixPixel(46);
            break;

        case FrontWord::fuenf:
            setFrontMatrixPixel(14);
            setFrontMatrixPixel(15);
            setFrontMatrixPixel(16);
            setFrontMatrixPixel(17);
            break;

        case FrontWord::zehn:
            setFrontMatrixPixel(27);
            setFrontMatrixPixel(28);
            setFrontMatrixPixel(29);
            setFrontMatrixPixel(30);
            break;

        case FrontWord::zwanzig:
            setFrontMatrixPixel(31);
            setFrontMatrixPixel(32);
            setFrontMatrixPixel(33);
            setFrontMatrixPixel(34);
            setFrontMatrixPixel(35);
            setFrontMatrixPixel(36);
            setFrontMatrixPixel(37);
            break;

        case FrontWord::halb:
            setFrontMatrixPixel(73);
            setFrontMatrixPixel(74);
            setFrontMatrixPixel(75);
            setFrontMatrixPixel(76);
            break;

        case FrontWord::eins:
            setFrontMatrixPixel(81);
            setFrontMatrixPixel(82);
            setFrontMatrixPixel(83);
            setFrontMatrixPixel(84);
            break;

        case FrontWord::uhr:
            setFrontMatrixPixel(146);
            setFrontMatrixPixel(145);
            setFrontMatrixPixel(144);
            break;

        case FrontWord::h_ein:
            setFrontMatrixPixel(81);
            setFrontMatrixPixel(82);
            setFrontMatrixPixel(83);
            break;

        case FrontWord::h_zwei:
            setFrontMatrixPixel(79);
            setFrontMatrixPixel(80);
            setFrontMatrixPixel(81);
            setFrontMatrixPixel(82);
            break;

        case FrontWord::h_drei:
            setFrontMatrixPixel(98);
            setFrontMatrixPixel(99);
            setFrontMatrixPixel(100);
            setFrontMatrixPixel(101);
            break;

        case FrontWord::h_vier:
            setFrontMatrixPixel(112);
            setFrontMatrixPixel(113);
            setFrontMatrixPixel(114);
            setFrontMatrixPixel(115);
            break;

        case FrontWord::h_fuenf:
            setFrontMatrixPixel(92);
            setFrontMatrixPixel(93);
            setFrontMatrixPixel(94);
            setFrontMatrixPixel(95);
            break;

        case FrontWord::h_sechs:
            setFrontMatrixPixel(132);
            setFrontMatrixPixel(133);
            setFrontMatrixPixel(134);
            setFrontMatrixPixel(135);
            setFrontMatrixPixel(136);
            break;

        case FrontWord::h_sieben:
            setFrontMatrixPixel(84);
            setFrontMatrixPixel(85);
            setFrontMatrixPixel(86);
            setFrontMatrixPixel(87);
            setFrontMatrixPixel(88);
            setFrontMatrixPixel(89);
            break;

        case FrontWord::h_acht:
            setFrontMatrixPixel(124);
            setFrontMatrixPixel(125);
            setFrontMatrixPixel(126);
            setFrontMatrixPixel(127);
            break;

        case FrontWord::h_neun:
            setFrontMatrixPixel(108);
            setFrontMatrixPixel(109);
            setFrontMatrixPixel(110);
            setFrontMatrixPixel(111);
            break;

        case FrontWord::h_zehn:
            setFrontMatrixPixel(120);
            setFrontMatrixPixel(121);
            setFrontMatrixPixel(122);
            setFrontMatrixPixel(123);
            break;

        case FrontWord::h_elf:
            setFrontMatrixPixel(105);
            setFrontMatrixPixel(106);
            setFrontMatrixPixel(107);
            break;

        case FrontWord::h_zwoelf:
            setFrontMatrixPixel(67);
            setFrontMatrixPixel(68);
            setFrontMatrixPixel(69);
            setFrontMatrixPixel(70);
            setFrontMatrixPixel(71);
            break;

        case FrontWord::funk:
            setFrontMatrixPixel(150);
            setFrontMatrixPixel(149);
            setFrontMatrixPixel(148);
            setFrontMatrixPixel(147);
            break;

        default:
            break;
        };
    };
};

De11x11frame_t _de11x11frame;
#include "Uhrtype.hpp"

/*
0  ,1  ,2  ,3  ,4  ,5  ,6  ,7  ,8  ,9  ,10 ,11 ,12
25 ,24 ,23 ,22 ,21 ,20 ,19 ,18 ,17 ,16 ,15 ,14 ,13
26 ,27 ,28 ,29 ,30 ,31 ,32 ,33 ,34 ,35 ,36 ,37 ,38
51 ,50 ,49 ,48 ,47 ,46 ,45 ,44 ,43 ,42 ,41 ,40 ,39
52 ,53 ,54 ,55 ,56 ,57 ,58 ,59 ,60 ,61 ,62 ,63 ,64
77 ,76 ,75 ,74 ,73 ,72 ,71 ,70 ,69 ,68 ,67 ,66 ,65
78 ,79 ,80 ,81 ,82 ,83 ,84 ,85 ,86 ,87 ,88 ,89 ,90
103,102,101,100,99 ,98 ,97 ,96 ,95 ,94 ,93 ,92 ,91
104,105,106,107,108,109,110,111,112,113,114,115,116
129,128,127,126,125,124,123,122,121,120,119,118,117
130,131,132,133,134,135,136,137,138,139,140,141,142
155,154,153,152,151,150,149,148,147,146,145,144,143
156,157,158,159,160,161,162,163,164,165,166,167,168
*/

class De10x11frame_t : public iUhrType {
public:
    const uint16_t rmatrix[48] = {
        0,   1,   2,   3,   4,   5,   6,   7,   8,   9,   10,  11,
        12,  13,  38,  39,  64,  65,  90,  91,  116, 117, 142, 143,
        168, 167, 166, 165, 164, 163, 162, 161, 160, 159, 158, 157,
        156, 155, 130, 129, 104, 103, 78,  77,  52,  51,  26,  25};

    //------------------------------------------------------------------------------

    const uint16_t min_arr[2][4] = {

        {152, 150, 148, 146}, {0, 12, 168, 156}};

    //------------------------------------------------------------------------------

    virtual const uint16_t getMinArr(uint8_t col, uint8_t row) override {
        return min_arr[col][row];
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

    void show(uint8_t text) override {
        switch (text) {
        case es_ist:
            // Es
            Letter_set(24);
            Letter_set(23);

            // Ist
            Letter_set(21);
            Letter_set(20);
            Letter_set(19);
            break;

        case nach:
        case v_nach:
            Letter_set(55);
            Letter_set(56);
            Letter_set(57);
            Letter_set(58);
            break;

        case vor:
        case v_vor:
            Letter_set(59);
            Letter_set(60);
            Letter_set(61);

        case viertel:
            Letter_set(40);
            Letter_set(41);
            Letter_set(42);
            Letter_set(43);
            Letter_set(44);
            Letter_set(45);
            Letter_set(46);
            break;

        case fuenf:
            Letter_set(14);
            Letter_set(15);
            Letter_set(16);
            Letter_set(17);
            break;

        case zehn:
            Letter_set(27);
            Letter_set(28);
            Letter_set(29);
            Letter_set(30);
            break;

        case zwanzig:
            Letter_set(31);
            Letter_set(32);
            Letter_set(33);
            Letter_set(34);
            Letter_set(35);
            Letter_set(36);
            Letter_set(37);
            break;

        case halb:
            Letter_set(73);
            Letter_set(74);
            Letter_set(75);
            Letter_set(76);
            break;

        case eins:
            Letter_set(81);
            Letter_set(82);
            Letter_set(83);
            Letter_set(84);
            break;

        case uhr:
            Letter_set(138);
            Letter_set(139);
            Letter_set(140);
            break;

        case h_ein:
            Letter_set(81);
            Letter_set(82);
            Letter_set(83);
            break;

        case h_zwei:
            Letter_set(79);
            Letter_set(80);
            Letter_set(81);
            Letter_set(82);
            break;

        case h_drei:
            Letter_set(98);
            Letter_set(99);
            Letter_set(100);
            Letter_set(101);
            break;

        case h_vier:
            Letter_set(112);
            Letter_set(113);
            Letter_set(114);
            Letter_set(115);
            break;

        case h_fuenf:
            Letter_set(92);
            Letter_set(93);
            Letter_set(94);
            Letter_set(95);
            break;

        case h_sechs:
            Letter_set(132);
            Letter_set(133);
            Letter_set(134);
            Letter_set(135);
            Letter_set(136);
            break;

        case h_sieben:
            Letter_set(84);
            Letter_set(85);
            Letter_set(86);
            Letter_set(87);
            Letter_set(88);
            Letter_set(89);
            break;

        case h_acht:
            Letter_set(124);
            Letter_set(125);
            Letter_set(126);
            Letter_set(127);
            break;

        case h_neun:
            Letter_set(108);
            Letter_set(109);
            Letter_set(110);
            Letter_set(111);
            break;

        case h_zehn:
            Letter_set(120);
            Letter_set(121);
            Letter_set(122);
            Letter_set(123);
            break;

        case h_elf:
            Letter_set(105);
            Letter_set(106);
            Letter_set(107);
            break;

        case h_zwoelf:
            Letter_set(67);
            Letter_set(68);
            Letter_set(69);
            Letter_set(70);
            Letter_set(71);
            break;

        default:
            break;
        };
    };
};

De10x11frame_t _de10x11frame;
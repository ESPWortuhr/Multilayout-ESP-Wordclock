#include "Uhrtype.hpp"

/*
 * Layout Front
 *
 * E S A I S T O V I E R T E L E I N S
 * D R E I N E R S E C H S I E B E N E
 * E L F Ü N F N E U N V I E R A C H T
 * N U L L Z W E I N Z W Ö L F Z E H N
 * U N D O Z W A N Z I G V I E R Z I G
 * D R E I S S I G F Ü N F Z I G U H R
 * M I N U T E N I V O R U N D N A C H
 * E I N D R E I V I E R T E L H A L B
 * S I E B E N E U N U L L Z W E I N E
 * F Ü N F S E C H S N A C H T V I E R
 * D R E I N S U N D A E L F E Z E H N
 * Z W A N Z I G G R A D R E I S S I G
 * V I E R Z I G Z W Ö L F Ü N F Z I G
 * M I N U T E N U H R E F R Ü H V O R
 * A B E N D S M I T T E R N A C H T S
 * M O R G E N S W A R M M I T T A G S
 *
 */

class UHR_291_t : public iUhrType {
public:

    const uint16_t min_arr[2][4] = {{288, 289, 290, 290}, {288, 289, 290, 290}};

    //------------------------------------------------------------------------------

    virtual const uint16_t getMinArr(uint8_t col, uint8_t row) override {
        return min_arr[col][row];
    };

    //------------------------------------------------------------------------------

    virtual const uint16_t getRMatrix(uint16_t index) override { return 0; };

    //------------------------------------------------------------------------------

    virtual const uint16_t getSMatrix(uint16_t index) override {
        return index;
    };

    //------------------------------------------------------------------------------

    virtual const uint16_t NUM_PIXELS() override { return 291; };

    //------------------------------------------------------------------------------

    virtual const uint16_t NUM_SMATRIX() override { return 291; };

    //------------------------------------------------------------------------------

    virtual const uint16_t ROWS_MATRIX() override { return 17; };

    //------------------------------------------------------------------------------

    virtual const uint16_t COLS_MATRIX() override { return 18; };

    //------------------------------------------------------------------------------

    virtual const uint16_t NUM_RMATRIX() override { return 0; };

    //------------------------------------------------------------------------------

    void show(uint8_t displayingText) override {

        switch (displayingText) // Minuten
        {
        case 1:
            Letter_set(33);
            Letter_set(32);
            Letter_set(31);
            Letter_set(30); // eine minute nach

            displayingText = minute;
            break;

        case 2:
            Letter_set(67);
            Letter_set(66);
            Letter_set(65);
            Letter_set(64); // zwei minuten nach

            displayingText = minuten;
            break;

        case 3:
            Letter_set(35);
            Letter_set(34);
            Letter_set(33);
            Letter_set(32); // drei minuten nach

            displayingText = minuten;
            break;

        case 4:
            Letter_set(49);
            Letter_set(48);
            Letter_set(47);
            Letter_set(46); // vier minuten nach

            displayingText = minuten;
            break;

        case 5:
            Letter_set(38);
            Letter_set(39);
            Letter_set(40);
            Letter_set(41); // fünf minuten nach

            displayingText = minuten;
            break;

        case 6:
            Letter_set(24);
            Letter_set(25);
            Letter_set(26);
            Letter_set(27);
            Letter_set(28); // sechs minuten nach

            displayingText = minuten;
            break;

        case 7:
            Letter_set(19);
            Letter_set(20);
            Letter_set(21);
            Letter_set(22);
            Letter_set(23);
            Letter_set(24); // sieben minuten nach

            displayingText = minuten;
            break;

        case 8:
            Letter_set(53);
            Letter_set(52);
            Letter_set(51);
            Letter_set(50); // acht minuten nach

            displayingText = minuten;
            break;

        case 9:
            Letter_set(42);
            Letter_set(43);
            Letter_set(44);
            Letter_set(45); // neun minuten nach

            displayingText = minuten;
            break;

        case 10:
            Letter_set(54);
            Letter_set(55);
            Letter_set(56);
            Letter_set(57); // zehn minuten nach

            displayingText = minuten;
            break;

        case 11:
            Letter_set(36);
            Letter_set(37);
            Letter_set(37); // elf minuten nach

            displayingText = minuten;
            break;

        case 12:
            Letter_set(58);
            Letter_set(59);
            Letter_set(60);
            Letter_set(61);
            Letter_set(62); // zwölf minuten nach

            displayingText = minuten;
            break;

        case 13:
            Letter_set(35);
            Letter_set(34);
            Letter_set(33);
            Letter_set(32);
            Letter_set(54);
            Letter_set(55);
            Letter_set(56);
            Letter_set(57); // dreizehn minuten nach

            displayingText = minuten;
            break;

        case 14:
            Letter_set(49);
            Letter_set(48);
            Letter_set(47);
            Letter_set(46);
            Letter_set(54);
            Letter_set(55);
            Letter_set(56);
            Letter_set(57); // vierzehn minuten nach

            displayingText = minuten;
            break;

        case 15:
            Letter_set(7);
            Letter_set(8);
            Letter_set(9);
            Letter_set(10);
            Letter_set(11);
            Letter_set(12);
            Letter_set(13); // viertel nach
            break;

        case 16:
            Letter_set(24);
            Letter_set(25);
            Letter_set(26);
            Letter_set(54);
            Letter_set(55);
            Letter_set(56);
            Letter_set(57); // sechzehn minuten nach

            displayingText = minuten;
            break;

        case 17:
            Letter_set(19);
            Letter_set(20);
            Letter_set(21);
            Letter_set(22);
            Letter_set(54);
            Letter_set(55);
            Letter_set(56);
            Letter_set(57); // siebzehn nach
            break;

        case 18:
            Letter_set(53);
            Letter_set(52);
            Letter_set(51);
            Letter_set(50);
            Letter_set(54);
            Letter_set(55);
            Letter_set(56);
            Letter_set(57); // achtzehn nach
            break;

        case 19:
            Letter_set(42);
            Letter_set(43);
            Letter_set(44);
            Letter_set(45);
            Letter_set(53);
            Letter_set(52);
            Letter_set(51);
            Letter_set(50); // neunzehn nach
            break;

        case 20:
            Letter_set(76);
            Letter_set(77);
            Letter_set(78);
            Letter_set(79);
            Letter_set(80);
            Letter_set(81);
            Letter_set(82); // zwanzig nach
            break;

        case 21:
            Letter_set(42);
            Letter_set(43);
            Letter_set(44);
            Letter_set(45); // neun vor halb
            break;

        case 22:
            Letter_set(53);
            Letter_set(52);
            Letter_set(51);
            Letter_set(50); // acht vor halb
            break;

        case 23:
            Letter_set(19);
            Letter_set(20);
            Letter_set(21);
            Letter_set(22);
            Letter_set(23);
            Letter_set(24); // sieben vor halb
            break;

        case 24:
            Letter_set(24);
            Letter_set(25);
            Letter_set(26);
            Letter_set(27);
            Letter_set(28); // sechs vor halb
            break;

        case 25:
            Letter_set(38);
            Letter_set(39);
            Letter_set(40);
            Letter_set(41); // fünf vor halb
            break;

        case 26:
            Letter_set(49);
            Letter_set(48);
            Letter_set(47);
            Letter_set(46); // vier vor halb
            break;

        case 27:
            Letter_set(35);
            Letter_set(34);
            Letter_set(33);
            Letter_set(32); // drei vor halb
            break;

        case 28:
            Letter_set(67);
            Letter_set(66);
            Letter_set(65);
            Letter_set(64); // zwei vor halb
            break;

        case 29:
            Letter_set(33);
            Letter_set(32);
            Letter_set(31);
            Letter_set(30); // eine minute vor halb
            break;

        case 30:
            Letter_set(126);
            Letter_set(127);
            Letter_set(128);
            Letter_set(129); // halb
            break;

        case 31:
            Letter_set(33);
            Letter_set(32);
            Letter_set(31);
            Letter_set(30); // eine minute nach halb
            break;

        case 32:
            Letter_set(67);
            Letter_set(66);
            Letter_set(65);
            Letter_set(64); // zwei minuten nach halb
            break;

        case 33:
            Letter_set(35);
            Letter_set(34);
            Letter_set(33);
            Letter_set(32); // drei minuten nach halb
            break;

        case 34:
            Letter_set(49);
            Letter_set(48);
            Letter_set(47);
            Letter_set(46);
            break;

        case 35:
            Letter_set(38);
            Letter_set(39);
            Letter_set(40);
            Letter_set(41);
            break;

        case 36:
            Letter_set(24);
            Letter_set(25);
            Letter_set(26);
            Letter_set(27);
            Letter_set(28);
            break;

        case 37:
            Letter_set(19);
            Letter_set(20);
            Letter_set(21);
            Letter_set(22);
            Letter_set(23);
            Letter_set(24);
            break;

        case 38:
            Letter_set(53);
            Letter_set(52);
            Letter_set(51);
            Letter_set(50);
            break;

        case 39:
            Letter_set(42);
            Letter_set(43);
            Letter_set(44);
            Letter_set(45);
            break;

        case 40:
            Letter_set(54);
            Letter_set(55);
            Letter_set(56);
            Letter_set(57);
            break;

        case 41:
            Letter_set(36);
            Letter_set(37);
            Letter_set(37);
            break;

        case 42:
            Letter_set(58);
            Letter_set(59);
            Letter_set(60);
            Letter_set(61);
            Letter_set(62);
            break;

        case 43:
            Letter_set(35);
            Letter_set(34);
            Letter_set(33);
            Letter_set(32);
            Letter_set(54);
            Letter_set(55);
            Letter_set(56);
            Letter_set(57);
            break;

        case 44:
            Letter_set(49);
            Letter_set(48);
            Letter_set(47);
            Letter_set(46);
            Letter_set(54);
            Letter_set(55);
            Letter_set(56);
            Letter_set(57);
            break;

        case 45:
            // viertel vor

        case 46:
            Letter_set(49);
            Letter_set(48);
            Letter_set(47);
            Letter_set(46);
            Letter_set(54);
            Letter_set(55);
            Letter_set(56);
            Letter_set(57); // vierzehn vor
            break;

        case 47:
            Letter_set(35);
            Letter_set(34);
            Letter_set(33);
            Letter_set(32);
            Letter_set(54);
            Letter_set(55);
            Letter_set(56);
            Letter_set(57);
            break;

        case 48:
            Letter_set(58);
            Letter_set(59);
            Letter_set(60);
            Letter_set(61);
            Letter_set(62);
            break;

        case 49:
            Letter_set(36);
            Letter_set(37);
            Letter_set(37);
            break;

        case 50:
            Letter_set(54);
            Letter_set(55);
            Letter_set(56);
            Letter_set(57);
            break;

        case 51:
            Letter_set(42);
            Letter_set(43);
            Letter_set(44);
            Letter_set(45);
            break;

        case 52:
            Letter_set(53);
            Letter_set(52);
            Letter_set(51);
            Letter_set(50);
            break;

        case 53:
            Letter_set(19);
            Letter_set(20);
            Letter_set(21);
            Letter_set(22);
            Letter_set(23);
            Letter_set(24);
            break;

        case 54:
            Letter_set(24);
            Letter_set(25);
            Letter_set(26);
            Letter_set(27);
            Letter_set(28); // sechs minuten vor
            break;

        case 55:
            Letter_set(38);
            Letter_set(39);
            Letter_set(40);
            Letter_set(41); // fünf minuten vor
            break;

        case 56:
            Letter_set(49);
            Letter_set(48);
            Letter_set(47);
            Letter_set(46); // vier minuten vor
            break;

        case 57:
            Letter_set(35);
            Letter_set(34);
            Letter_set(33);
            Letter_set(32); // drei minuten vor
            break;

        case 58:
            Letter_set(67);
            Letter_set(66);
            Letter_set(65);
            Letter_set(64); // zwei minuten vor
            break;

        case 59:
            Letter_set(33);
            Letter_set(32);
            Letter_set(31);
            Letter_set(30); // eine minute vor

            displayingText = minute;
            break;

        default:
            break;
        }

        switch (displayingText) {
        case minute:
            Letter_set(108);
            Letter_set(109);
            Letter_set(110);
            Letter_set(111);
            Letter_set(112);
            Letter_set(113);
            break;

        case minuten:
            Letter_set(108);
            Letter_set(109);
            Letter_set(110);
            Letter_set(111);
            Letter_set(112);
            Letter_set(113);
            Letter_set(114);
            break;
        }

        switch (displayingText) {

        case es_ist:
            Letter_set(0);
            Letter_set(1);

            Letter_set(3);
            Letter_set(4);
            Letter_set(5);
            break;

        case nach:
            Letter_set(122);
            Letter_set(123);
            Letter_set(124);
            Letter_set(125);
            break;

        case vor:
            Letter_set(116);
            Letter_set(117);
            Letter_set(118);
            break;

        case viertel:
            Letter_set(7);
            Letter_set(8);
            Letter_set(9);
            Letter_set(10);
            Letter_set(11);
            Letter_set(12);
            Letter_set(13);
            break;

        case dreiviertel:
            Letter_set(140);
            Letter_set(139);
            Letter_set(138);
            Letter_set(137);
            Letter_set(136);
            Letter_set(135);
            Letter_set(134);
            Letter_set(133);
            Letter_set(132);
            Letter_set(131);
            Letter_set(130);
            break;

        case halb:
            Letter_set(126);
            Letter_set(127);
            Letter_set(128);
            Letter_set(129);
            break;

        case frueh:
            Letter_set(227);
            Letter_set(228);
            Letter_set(229);
            Letter_set(230);
            break;

        case minuten_uhr:
            Letter_set(232);
            Letter_set(233);
            Letter_set(234);
            break;

        case minuten_extra:
            Letter_set(235);
            Letter_set(236);
            Letter_set(237);
            Letter_set(238);
            Letter_set(239);
            Letter_set(240);
            Letter_set(241);
            break;

        case abends:
            Letter_set(242);
            Letter_set(243);
            Letter_set(244);
            Letter_set(245);
            Letter_set(246);
            Letter_set(247);
            break;

        case mitternacht:
            Letter_set(248);
            Letter_set(249);
            Letter_set(250);
            Letter_set(251);
            Letter_set(252);
            Letter_set(253);
            Letter_set(254);
            Letter_set(255);
            Letter_set(256);
            Letter_set(257);
            Letter_set(258);
            break;

        case mittags:
            Letter_set(260);
            Letter_set(261);
            Letter_set(262);
            Letter_set(263);
            Letter_set(264);
            Letter_set(265);
            Letter_set(266);
            break;

        case warm:
            Letter_set(267);
            Letter_set(268);
            Letter_set(269);
            Letter_set(270);
            break;

        case morgens:
            Letter_set(271);
            Letter_set(272);
            Letter_set(273);
            Letter_set(274);
            Letter_set(275);
            Letter_set(276);
            Letter_set(277);
            break;

        case uhr:
            Letter_set(87);
            Letter_set(88);
            Letter_set(89);
            break;

        case und:
            Letter_set(101);
            Letter_set(102);
            Letter_set(103);
            break;

            // Stunden

        case h_ein:
            Letter_set(14);
            Letter_set(15);
            Letter_set(16);
            Letter_set(17);
            break;

        case h_zwei:
            Letter_set(64);
            Letter_set(65);
            Letter_set(66);
            Letter_set(67);
            break;

        case h_drei:
            Letter_set(32);
            Letter_set(33);
            Letter_set(34);
            Letter_set(35);
            break;

        case h_vier:
            Letter_set(46);
            Letter_set(47);
            Letter_set(48);
            Letter_set(49);
            break;

        case h_fuenf:
            Letter_set(38);
            Letter_set(39);
            Letter_set(40);
            Letter_set(41);
            break;

        case h_sechs:
            Letter_set(24);
            Letter_set(25);
            Letter_set(26);
            Letter_set(27);
            Letter_set(28);
            break;

        case h_sieben:
            Letter_set(19);
            Letter_set(20);
            Letter_set(21);
            Letter_set(22);
            Letter_set(23);
            Letter_set(24);
            break;

        case h_acht:
            Letter_set(50);
            Letter_set(51);
            Letter_set(52);
            Letter_set(53);
            break;

        case h_neun:
            Letter_set(42);
            Letter_set(43);
            Letter_set(44);
            Letter_set(45);
            break;

        case h_zehn:
            Letter_set(54);
            Letter_set(55);
            Letter_set(56);
            Letter_set(57);
            break;

        case h_elf:
            Letter_set(36);
            Letter_set(37);
            Letter_set(38);
            break;

        case h_zwoelf:
            Letter_set(58);
            Letter_set(59);
            Letter_set(60);
            Letter_set(61);
            Letter_set(62);
            break;

        case h_dreizehn:
            Letter_set(14);
            Letter_set(15);
            Letter_set(16);
            Letter_set(17);
            break;

        default:
            break;
        };
    };
};
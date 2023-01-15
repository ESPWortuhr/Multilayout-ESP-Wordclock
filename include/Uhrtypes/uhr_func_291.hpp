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
    virtual const uint16_t NUM_PIXELS() override { return 291; };

    //------------------------------------------------------------------------------

    virtual const uint16_t NUM_SMATRIX() override { return 291; };

    //------------------------------------------------------------------------------

    virtual const uint16_t ROWS_MATRIX() override { return 17; };

    //------------------------------------------------------------------------------

    virtual const uint16_t COLS_MATRIX() override { return 18; };

    //------------------------------------------------------------------------------

    virtual const bool hasDreiviertel() override { return true; }

    //------------------------------------------------------------------------------

    virtual const bool has24HourLayout() override { return true; }

    //------------------------------------------------------------------------------

    void show(uint8_t displayingText) override {

        switch (displayingText) // Minuten
        {
        case m_eine:
            Letter_set(33);
            Letter_set(32);
            Letter_set(31);
            Letter_set(30); // eine minute nach
            break;

        case m_zwei:
            Letter_set(67);
            Letter_set(66);
            Letter_set(65);
            Letter_set(64); // zwei minuten nach
            break;

        case m_drei:
            Letter_set(35);
            Letter_set(34);
            Letter_set(33);
            Letter_set(32); // drei minuten nach
            break;

        case m_vier:
            Letter_set(49);
            Letter_set(48);
            Letter_set(47);
            Letter_set(46); // vier minuten nach
            break;

        case fuenf:
            Letter_set(38);
            Letter_set(39);
            Letter_set(40);
            Letter_set(41); // fünf minuten nach
            break;

        case m_sechs:
            Letter_set(24);
            Letter_set(25);
            Letter_set(26);
            Letter_set(27);
            Letter_set(28); // sechs minuten nach
            break;

        case m_sieben:
            Letter_set(19);
            Letter_set(20);
            Letter_set(21);
            Letter_set(22);
            Letter_set(23);
            Letter_set(24); // sieben minuten nach
            break;

        case m_acht:
            Letter_set(53);
            Letter_set(52);
            Letter_set(51);
            Letter_set(50); // acht minuten nach
            break;

        case m_neun:
            Letter_set(42);
            Letter_set(43);
            Letter_set(44);
            Letter_set(45); // neun minuten nach
            break;

        case zehn:
            Letter_set(54);
            Letter_set(55);
            Letter_set(56);
            Letter_set(57); // zehn minuten nach
            break;

        case m_elf:
            Letter_set(36);
            Letter_set(37);
            Letter_set(37); // elf minuten nach
            break;

        case m_zwoelf:
            Letter_set(58);
            Letter_set(59);
            Letter_set(60);
            Letter_set(61);
            Letter_set(62); // zwölf minuten nach
            break;

        case m_dreizehn:
            Letter_set(35);
            Letter_set(34);
            Letter_set(33);
            Letter_set(32);
            Letter_set(54);
            Letter_set(55);
            Letter_set(56);
            Letter_set(57); // dreizehn minuten nach
            break;

        case m_vierzehn:
            Letter_set(49);
            Letter_set(48);
            Letter_set(47);
            Letter_set(46);
            Letter_set(54);
            Letter_set(55);
            Letter_set(56);
            Letter_set(57); // vierzehn minuten nach
            break;

            // viertel nach

        case m_sechzehn:
            Letter_set(24);
            Letter_set(25);
            Letter_set(26);
            Letter_set(54);
            Letter_set(55);
            Letter_set(56);
            Letter_set(57); // sechzehn nach
            break;

        case m_siebzehn:
            Letter_set(19);
            Letter_set(20);
            Letter_set(21);
            Letter_set(22);
            Letter_set(54);
            Letter_set(55);
            Letter_set(56);
            Letter_set(57); // siebzehn nach
            break;

        case m_achtzehn:
            Letter_set(53);
            Letter_set(52);
            Letter_set(51);
            Letter_set(50);
            Letter_set(54);
            Letter_set(55);
            Letter_set(56);
            Letter_set(57); // achtzehn nach
            break;

        case m_neunzehn:
            Letter_set(42);
            Letter_set(43);
            Letter_set(44);
            Letter_set(45);
            Letter_set(53);
            Letter_set(52);
            Letter_set(51);
            Letter_set(50); // neunzehn nach
            break;

        case zwanzig:
            Letter_set(76);
            Letter_set(77);
            Letter_set(78);
            Letter_set(79);
            Letter_set(80);
            Letter_set(81);
            Letter_set(82); // zwanzig nach
            break;

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
        case v_nach:
            Letter_set(122);
            Letter_set(123);
            Letter_set(124);
            Letter_set(125);
            break;

        case vor:
        case v_vor:
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

UHR_291_t Uhr_291_type;
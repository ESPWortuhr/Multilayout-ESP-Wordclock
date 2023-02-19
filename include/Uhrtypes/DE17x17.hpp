#pragma once

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

class De17x17_t : public iUhrType {
public:
    virtual LanguageAbbreviation usedLang() override {
        return LanguageAbbreviation::DE;
    };

    //------------------------------------------------------------------------------

    virtual const uint16_t numPixels() override { return 291; };

    //------------------------------------------------------------------------------

    virtual const uint16_t numPixelsWordMatrix() override { return 291; };

    //------------------------------------------------------------------------------

    virtual const uint16_t ROWS_MATRIX() override { return 17; };

    //------------------------------------------------------------------------------

    virtual const uint16_t COLS_MATRIX() override { return 18; };

    //------------------------------------------------------------------------------

    virtual const bool hasDreiviertel() override { return true; }

    //------------------------------------------------------------------------------

    virtual const bool has24HourLayout() override { return true; }

    //------------------------------------------------------------------------------

    void show(FrontWord word) override {
        switch (word) {

        case FrontWord::m_eine:
            setLetter(33);
            setLetter(32);
            setLetter(31);
            setLetter(30); // eine minute nach
            break;

        case FrontWord::m_zwei:
            setLetter(67);
            setLetter(66);
            setLetter(65);
            setLetter(64); // zwei minuten nach
            break;

        case FrontWord::m_drei:
            setLetter(35);
            setLetter(34);
            setLetter(33);
            setLetter(32); // drei minuten nach
            break;

        case FrontWord::m_vier:
            setLetter(49);
            setLetter(48);
            setLetter(47);
            setLetter(46); // vier minuten nach
            break;

        case FrontWord::fuenf:
            setLetter(38);
            setLetter(39);
            setLetter(40);
            setLetter(41); // fünf minuten nach
            break;

        case FrontWord::m_sechs:
            setLetter(24);
            setLetter(25);
            setLetter(26);
            setLetter(27);
            setLetter(28); // sechs minuten nach
            break;

        case FrontWord::m_sieben:
            setLetter(19);
            setLetter(20);
            setLetter(21);
            setLetter(22);
            setLetter(23);
            setLetter(24); // sieben minuten nach
            break;

        case FrontWord::m_acht:
            setLetter(53);
            setLetter(52);
            setLetter(51);
            setLetter(50); // acht minuten nach
            break;

        case FrontWord::m_neun:
            setLetter(42);
            setLetter(43);
            setLetter(44);
            setLetter(45); // neun minuten nach
            break;

        case FrontWord::zehn:
            setLetter(54);
            setLetter(55);
            setLetter(56);
            setLetter(57); // zehn minuten nach
            break;

        case FrontWord::m_elf:
            setLetter(36);
            setLetter(37);
            setLetter(37); // elf minuten nach
            break;

        case FrontWord::m_zwoelf:
            setLetter(58);
            setLetter(59);
            setLetter(60);
            setLetter(61);
            setLetter(62); // zwölf minuten nach
            break;

        case FrontWord::m_dreizehn:
            setLetter(35);
            setLetter(34);
            setLetter(33);
            setLetter(32);
            setLetter(54);
            setLetter(55);
            setLetter(56);
            setLetter(57); // dreizehn minuten nach
            break;

        case FrontWord::m_vierzehn:
            setLetter(49);
            setLetter(48);
            setLetter(47);
            setLetter(46);
            setLetter(54);
            setLetter(55);
            setLetter(56);
            setLetter(57); // vierzehn minuten nach
            break;

            // viertel nach

        case FrontWord::m_sechzehn:
            setLetter(24);
            setLetter(25);
            setLetter(26);
            setLetter(54);
            setLetter(55);
            setLetter(56);
            setLetter(57); // sechzehn nach
            break;

        case FrontWord::m_siebzehn:
            setLetter(19);
            setLetter(20);
            setLetter(21);
            setLetter(22);
            setLetter(54);
            setLetter(55);
            setLetter(56);
            setLetter(57); // siebzehn nach
            break;

        case FrontWord::m_achtzehn:
            setLetter(53);
            setLetter(52);
            setLetter(51);
            setLetter(50);
            setLetter(54);
            setLetter(55);
            setLetter(56);
            setLetter(57); // achtzehn nach
            break;

        case FrontWord::m_neunzehn:
            setLetter(42);
            setLetter(43);
            setLetter(44);
            setLetter(45);
            setLetter(53);
            setLetter(52);
            setLetter(51);
            setLetter(50); // neunzehn nach
            break;

        case FrontWord::zwanzig:
            setLetter(76);
            setLetter(77);
            setLetter(78);
            setLetter(79);
            setLetter(80);
            setLetter(81);
            setLetter(82); // zwanzig nach
            break;

        case FrontWord::minute:
            setLetter(108);
            setLetter(109);
            setLetter(110);
            setLetter(111);
            setLetter(112);
            setLetter(113);
            break;

        case FrontWord::minuten:
            setLetter(108);
            setLetter(109);
            setLetter(110);
            setLetter(111);
            setLetter(112);
            setLetter(113);
            setLetter(114);
            break;

        default:
            break;
        }

        switch (word) {

        case FrontWord::es_ist:
            setLetter(0);
            setLetter(1);

            setLetter(3);
            setLetter(4);
            setLetter(5);
            break;

        case FrontWord::nach:
        case FrontWord::v_nach:
            setLetter(122);
            setLetter(123);
            setLetter(124);
            setLetter(125);
            break;

        case FrontWord::vor:
        case FrontWord::v_vor:
            setLetter(116);
            setLetter(117);
            setLetter(118);
            break;

        case FrontWord::viertel:
            setLetter(7);
            setLetter(8);
            setLetter(9);
            setLetter(10);
            setLetter(11);
            setLetter(12);
            setLetter(13);
            break;

        case FrontWord::dreiviertel:
            setLetter(140);
            setLetter(139);
            setLetter(138);
            setLetter(137);
            setLetter(136);
            setLetter(135);
            setLetter(134);
            setLetter(133);
            setLetter(132);
            setLetter(131);
            setLetter(130);
            break;

        case FrontWord::halb:
            setLetter(126);
            setLetter(127);
            setLetter(128);
            setLetter(129);
            break;

        case FrontWord::frueh:
            setLetter(227);
            setLetter(228);
            setLetter(229);
            setLetter(230);
            break;

        case FrontWord::minuten_uhr:
            setLetter(232);
            setLetter(233);
            setLetter(234);
            break;

        case FrontWord::minuten_extra:
            setLetter(235);
            setLetter(236);
            setLetter(237);
            setLetter(238);
            setLetter(239);
            setLetter(240);
            setLetter(241);
            break;

        case FrontWord::abends:
            setLetter(242);
            setLetter(243);
            setLetter(244);
            setLetter(245);
            setLetter(246);
            setLetter(247);
            break;

        case FrontWord::mitternacht:
            setLetter(248);
            setLetter(249);
            setLetter(250);
            setLetter(251);
            setLetter(252);
            setLetter(253);
            setLetter(254);
            setLetter(255);
            setLetter(256);
            setLetter(257);
            setLetter(258);
            break;

        case FrontWord::mittags:
            setLetter(260);
            setLetter(261);
            setLetter(262);
            setLetter(263);
            setLetter(264);
            setLetter(265);
            setLetter(266);
            break;

        case FrontWord::warm:
            setLetter(267);
            setLetter(268);
            setLetter(269);
            setLetter(270);
            break;

        case FrontWord::morgens:
            setLetter(271);
            setLetter(272);
            setLetter(273);
            setLetter(274);
            setLetter(275);
            setLetter(276);
            setLetter(277);
            break;

        case FrontWord::uhr:
            setLetter(87);
            setLetter(88);
            setLetter(89);
            break;

        case FrontWord::und:
            setLetter(101);
            setLetter(102);
            setLetter(103);
            break;

            // Stunden

        case FrontWord::h_ein:
            setLetter(14);
            setLetter(15);
            setLetter(16);
            setLetter(17);
            break;

        case FrontWord::h_zwei:
            setLetter(64);
            setLetter(65);
            setLetter(66);
            setLetter(67);
            break;

        case FrontWord::h_drei:
            setLetter(32);
            setLetter(33);
            setLetter(34);
            setLetter(35);
            break;

        case FrontWord::h_vier:
            setLetter(46);
            setLetter(47);
            setLetter(48);
            setLetter(49);
            break;

        case FrontWord::h_fuenf:
            setLetter(38);
            setLetter(39);
            setLetter(40);
            setLetter(41);
            break;

        case FrontWord::h_sechs:
            setLetter(24);
            setLetter(25);
            setLetter(26);
            setLetter(27);
            setLetter(28);
            break;

        case FrontWord::h_sieben:
            setLetter(19);
            setLetter(20);
            setLetter(21);
            setLetter(22);
            setLetter(23);
            setLetter(24);
            break;

        case FrontWord::h_acht:
            setLetter(50);
            setLetter(51);
            setLetter(52);
            setLetter(53);
            break;

        case FrontWord::h_neun:
            setLetter(42);
            setLetter(43);
            setLetter(44);
            setLetter(45);
            break;

        case FrontWord::h_zehn:
            setLetter(54);
            setLetter(55);
            setLetter(56);
            setLetter(57);
            break;

        case FrontWord::h_elf:
            setLetter(36);
            setLetter(37);
            setLetter(38);
            break;

        case FrontWord::h_zwoelf:
            setLetter(58);
            setLetter(59);
            setLetter(60);
            setLetter(61);
            setLetter(62);
            break;

        case FrontWord::h_dreizehn:
            setLetter(14);
            setLetter(15);
            setLetter(16);
            setLetter(17);
            break;

        default:
            break;
        };
    };
};

De17x17_t _de17x17;
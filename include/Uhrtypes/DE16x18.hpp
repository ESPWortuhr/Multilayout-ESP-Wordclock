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
            setFrontMatrixPixel(33);
            setFrontMatrixPixel(32);
            setFrontMatrixPixel(31);
            setFrontMatrixPixel(30); // eine minute nach
            break;

        case FrontWord::m_zwei:
            setFrontMatrixPixel(67);
            setFrontMatrixPixel(66);
            setFrontMatrixPixel(65);
            setFrontMatrixPixel(64); // zwei minuten nach
            break;

        case FrontWord::m_drei:
            setFrontMatrixPixel(35);
            setFrontMatrixPixel(34);
            setFrontMatrixPixel(33);
            setFrontMatrixPixel(32); // drei minuten nach
            break;

        case FrontWord::m_vier:
            setFrontMatrixPixel(49);
            setFrontMatrixPixel(48);
            setFrontMatrixPixel(47);
            setFrontMatrixPixel(46); // vier minuten nach
            break;

        case FrontWord::fuenf:
            setFrontMatrixPixel(38);
            setFrontMatrixPixel(39);
            setFrontMatrixPixel(40);
            setFrontMatrixPixel(41); // fünf minuten nach
            break;

        case FrontWord::m_sechs:
            setFrontMatrixPixel(24);
            setFrontMatrixPixel(25);
            setFrontMatrixPixel(26);
            setFrontMatrixPixel(27);
            setFrontMatrixPixel(28); // sechs minuten nach
            break;

        case FrontWord::m_sieben:
            setFrontMatrixPixel(19);
            setFrontMatrixPixel(20);
            setFrontMatrixPixel(21);
            setFrontMatrixPixel(22);
            setFrontMatrixPixel(23);
            setFrontMatrixPixel(24); // sieben minuten nach
            break;

        case FrontWord::m_acht:
            setFrontMatrixPixel(53);
            setFrontMatrixPixel(52);
            setFrontMatrixPixel(51);
            setFrontMatrixPixel(50); // acht minuten nach
            break;

        case FrontWord::m_neun:
            setFrontMatrixPixel(42);
            setFrontMatrixPixel(43);
            setFrontMatrixPixel(44);
            setFrontMatrixPixel(45); // neun minuten nach
            break;

        case FrontWord::zehn:
            setFrontMatrixPixel(54);
            setFrontMatrixPixel(55);
            setFrontMatrixPixel(56);
            setFrontMatrixPixel(57); // zehn minuten nach
            break;

        case FrontWord::m_elf:
            setFrontMatrixPixel(36);
            setFrontMatrixPixel(37);
            setFrontMatrixPixel(37); // elf minuten nach
            break;

        case FrontWord::m_zwoelf:
            setFrontMatrixPixel(58);
            setFrontMatrixPixel(59);
            setFrontMatrixPixel(60);
            setFrontMatrixPixel(61);
            setFrontMatrixPixel(62); // zwölf minuten nach
            break;

        case FrontWord::m_dreizehn:
            setFrontMatrixPixel(35);
            setFrontMatrixPixel(34);
            setFrontMatrixPixel(33);
            setFrontMatrixPixel(32);
            setFrontMatrixPixel(54);
            setFrontMatrixPixel(55);
            setFrontMatrixPixel(56);
            setFrontMatrixPixel(57); // dreizehn minuten nach
            break;

        case FrontWord::m_vierzehn:
            setFrontMatrixPixel(49);
            setFrontMatrixPixel(48);
            setFrontMatrixPixel(47);
            setFrontMatrixPixel(46);
            setFrontMatrixPixel(54);
            setFrontMatrixPixel(55);
            setFrontMatrixPixel(56);
            setFrontMatrixPixel(57); // vierzehn minuten nach
            break;

            // viertel nach

        case FrontWord::m_sechzehn:
            setFrontMatrixPixel(24);
            setFrontMatrixPixel(25);
            setFrontMatrixPixel(26);
            setFrontMatrixPixel(54);
            setFrontMatrixPixel(55);
            setFrontMatrixPixel(56);
            setFrontMatrixPixel(57); // sechzehn nach
            break;

        case FrontWord::m_siebzehn:
            setFrontMatrixPixel(19);
            setFrontMatrixPixel(20);
            setFrontMatrixPixel(21);
            setFrontMatrixPixel(22);
            setFrontMatrixPixel(54);
            setFrontMatrixPixel(55);
            setFrontMatrixPixel(56);
            setFrontMatrixPixel(57); // siebzehn nach
            break;

        case FrontWord::m_achtzehn:
            setFrontMatrixPixel(53);
            setFrontMatrixPixel(52);
            setFrontMatrixPixel(51);
            setFrontMatrixPixel(50);
            setFrontMatrixPixel(54);
            setFrontMatrixPixel(55);
            setFrontMatrixPixel(56);
            setFrontMatrixPixel(57); // achtzehn nach
            break;

        case FrontWord::m_neunzehn:
            setFrontMatrixPixel(42);
            setFrontMatrixPixel(43);
            setFrontMatrixPixel(44);
            setFrontMatrixPixel(45);
            setFrontMatrixPixel(53);
            setFrontMatrixPixel(52);
            setFrontMatrixPixel(51);
            setFrontMatrixPixel(50); // neunzehn nach
            break;

        case FrontWord::zwanzig:
            setFrontMatrixPixel(76);
            setFrontMatrixPixel(77);
            setFrontMatrixPixel(78);
            setFrontMatrixPixel(79);
            setFrontMatrixPixel(80);
            setFrontMatrixPixel(81);
            setFrontMatrixPixel(82); // zwanzig nach
            break;

        case FrontWord::minute:
            setFrontMatrixPixel(108);
            setFrontMatrixPixel(109);
            setFrontMatrixPixel(110);
            setFrontMatrixPixel(111);
            setFrontMatrixPixel(112);
            setFrontMatrixPixel(113);
            break;

        case FrontWord::minuten:
            setFrontMatrixPixel(108);
            setFrontMatrixPixel(109);
            setFrontMatrixPixel(110);
            setFrontMatrixPixel(111);
            setFrontMatrixPixel(112);
            setFrontMatrixPixel(113);
            setFrontMatrixPixel(114);
            break;

        default:
            break;
        }

        switch (word) {

        case FrontWord::es_ist:
            setFrontMatrixPixel(0);
            setFrontMatrixPixel(1);

            setFrontMatrixPixel(3);
            setFrontMatrixPixel(4);
            setFrontMatrixPixel(5);
            break;

        case FrontWord::nach:
        case FrontWord::v_nach:
            setFrontMatrixPixel(122);
            setFrontMatrixPixel(123);
            setFrontMatrixPixel(124);
            setFrontMatrixPixel(125);
            break;

        case FrontWord::vor:
        case FrontWord::v_vor:
            setFrontMatrixPixel(116);
            setFrontMatrixPixel(117);
            setFrontMatrixPixel(118);
            break;

        case FrontWord::viertel:
            setFrontMatrixPixel(7);
            setFrontMatrixPixel(8);
            setFrontMatrixPixel(9);
            setFrontMatrixPixel(10);
            setFrontMatrixPixel(11);
            setFrontMatrixPixel(12);
            setFrontMatrixPixel(13);
            break;

        case FrontWord::dreiviertel:
            setFrontMatrixPixel(140);
            setFrontMatrixPixel(139);
            setFrontMatrixPixel(138);
            setFrontMatrixPixel(137);
            setFrontMatrixPixel(136);
            setFrontMatrixPixel(135);
            setFrontMatrixPixel(134);
            setFrontMatrixPixel(133);
            setFrontMatrixPixel(132);
            setFrontMatrixPixel(131);
            setFrontMatrixPixel(130);
            break;

        case FrontWord::halb:
            setFrontMatrixPixel(126);
            setFrontMatrixPixel(127);
            setFrontMatrixPixel(128);
            setFrontMatrixPixel(129);
            break;

        case FrontWord::frueh:
            setFrontMatrixPixel(227);
            setFrontMatrixPixel(228);
            setFrontMatrixPixel(229);
            setFrontMatrixPixel(230);
            break;

        case FrontWord::minuten_uhr:
            setFrontMatrixPixel(232);
            setFrontMatrixPixel(233);
            setFrontMatrixPixel(234);
            break;

        case FrontWord::minuten_extra:
            setFrontMatrixPixel(235);
            setFrontMatrixPixel(236);
            setFrontMatrixPixel(237);
            setFrontMatrixPixel(238);
            setFrontMatrixPixel(239);
            setFrontMatrixPixel(240);
            setFrontMatrixPixel(241);
            break;

        case FrontWord::abends:
            setFrontMatrixPixel(242);
            setFrontMatrixPixel(243);
            setFrontMatrixPixel(244);
            setFrontMatrixPixel(245);
            setFrontMatrixPixel(246);
            setFrontMatrixPixel(247);
            break;

        case FrontWord::mitternacht:
            setFrontMatrixPixel(248);
            setFrontMatrixPixel(249);
            setFrontMatrixPixel(250);
            setFrontMatrixPixel(251);
            setFrontMatrixPixel(252);
            setFrontMatrixPixel(253);
            setFrontMatrixPixel(254);
            setFrontMatrixPixel(255);
            setFrontMatrixPixel(256);
            setFrontMatrixPixel(257);
            setFrontMatrixPixel(258);
            break;

        case FrontWord::mittags:
            setFrontMatrixPixel(260);
            setFrontMatrixPixel(261);
            setFrontMatrixPixel(262);
            setFrontMatrixPixel(263);
            setFrontMatrixPixel(264);
            setFrontMatrixPixel(265);
            setFrontMatrixPixel(266);
            break;

        case FrontWord::warm:
            setFrontMatrixPixel(267);
            setFrontMatrixPixel(268);
            setFrontMatrixPixel(269);
            setFrontMatrixPixel(270);
            break;

        case FrontWord::morgens:
            setFrontMatrixPixel(271);
            setFrontMatrixPixel(272);
            setFrontMatrixPixel(273);
            setFrontMatrixPixel(274);
            setFrontMatrixPixel(275);
            setFrontMatrixPixel(276);
            setFrontMatrixPixel(277);
            break;

        case FrontWord::uhr:
            setFrontMatrixPixel(87);
            setFrontMatrixPixel(88);
            setFrontMatrixPixel(89);
            break;

        case FrontWord::und:
            setFrontMatrixPixel(101);
            setFrontMatrixPixel(102);
            setFrontMatrixPixel(103);
            break;

            // Stunden

        case FrontWord::h_ein:
            setFrontMatrixPixel(14);
            setFrontMatrixPixel(15);
            setFrontMatrixPixel(16);
            setFrontMatrixPixel(17);
            break;

        case FrontWord::h_zwei:
            setFrontMatrixPixel(64);
            setFrontMatrixPixel(65);
            setFrontMatrixPixel(66);
            setFrontMatrixPixel(67);
            break;

        case FrontWord::h_drei:
            setFrontMatrixPixel(32);
            setFrontMatrixPixel(33);
            setFrontMatrixPixel(34);
            setFrontMatrixPixel(35);
            break;

        case FrontWord::h_vier:
            setFrontMatrixPixel(46);
            setFrontMatrixPixel(47);
            setFrontMatrixPixel(48);
            setFrontMatrixPixel(49);
            break;

        case FrontWord::h_fuenf:
            setFrontMatrixPixel(38);
            setFrontMatrixPixel(39);
            setFrontMatrixPixel(40);
            setFrontMatrixPixel(41);
            break;

        case FrontWord::h_sechs:
            setFrontMatrixPixel(24);
            setFrontMatrixPixel(25);
            setFrontMatrixPixel(26);
            setFrontMatrixPixel(27);
            setFrontMatrixPixel(28);
            break;

        case FrontWord::h_sieben:
            setFrontMatrixPixel(19);
            setFrontMatrixPixel(20);
            setFrontMatrixPixel(21);
            setFrontMatrixPixel(22);
            setFrontMatrixPixel(23);
            setFrontMatrixPixel(24);
            break;

        case FrontWord::h_acht:
            setFrontMatrixPixel(50);
            setFrontMatrixPixel(51);
            setFrontMatrixPixel(52);
            setFrontMatrixPixel(53);
            break;

        case FrontWord::h_neun:
            setFrontMatrixPixel(42);
            setFrontMatrixPixel(43);
            setFrontMatrixPixel(44);
            setFrontMatrixPixel(45);
            break;

        case FrontWord::h_zehn:
            setFrontMatrixPixel(54);
            setFrontMatrixPixel(55);
            setFrontMatrixPixel(56);
            setFrontMatrixPixel(57);
            break;

        case FrontWord::h_elf:
            setFrontMatrixPixel(36);
            setFrontMatrixPixel(37);
            setFrontMatrixPixel(38);
            break;

        case FrontWord::h_zwoelf:
            setFrontMatrixPixel(58);
            setFrontMatrixPixel(59);
            setFrontMatrixPixel(60);
            setFrontMatrixPixel(61);
            setFrontMatrixPixel(62);
            break;

        case FrontWord::h_dreizehn:
            setFrontMatrixPixel(14);
            setFrontMatrixPixel(15);
            setFrontMatrixPixel(16);
            setFrontMatrixPixel(17);
            break;

        default:
            break;
        };
    };
};

De16x18_t _de16x18;
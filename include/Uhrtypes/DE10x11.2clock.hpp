#pragma once

#include "Uhrtype.hpp"

/*
 * Layout Front
 *
 * E S K I S T A F Ü N F
 * Z E H N Z W A N Z I G
 * D R E I V I E R T E L
 * V O R F U N K N A C H
 * H A L B A E L F Ü N F
 * E I N S X A M Z W E I
 * D R E I A U J V I E R
 * S E C H S N L A C H T
 * S I E B E N Z W Ö L F
 * Z E H N E U N K U H R
 *
 */

class De10x11Clock_t : public iUhrType {
public:
    virtual LanguageAbbreviation usedLang() override {
        return LanguageAbbreviation::DE;
    };

    //------------------------------------------------------------------------------

    virtual const bool hasDreiviertel() override { return true; }

    //------------------------------------------------------------------------------

    void show(FrontWord word) override {
        switch (word) {

        case FrontWord::es_ist:
            // Es
            setFrontMatrixPixel(0);
            setFrontMatrixPixel(1);

            // Ist
            setFrontMatrixPixel(3);
            setFrontMatrixPixel(4);
            setFrontMatrixPixel(5);
            break;

        case FrontWord::nach:
        case FrontWord::v_nach:
            // NACH
            setFrontMatrixPixel(36);
            setFrontMatrixPixel(35);
            setFrontMatrixPixel(34);
            setFrontMatrixPixel(33);
            break;

        case FrontWord::vor:
        case FrontWord::v_vor:
            // Vor
            setFrontMatrixPixel(41);
            setFrontMatrixPixel(42);
            setFrontMatrixPixel(43);
            break;

        case FrontWord::viertel:
            // Viertel
            setFrontMatrixPixel(32);
            setFrontMatrixPixel(31);
            setFrontMatrixPixel(30);
            setFrontMatrixPixel(29);
            setFrontMatrixPixel(28);
            setFrontMatrixPixel(27);
            setFrontMatrixPixel(26);
            break;

        case FrontWord::dreiviertel:
            setFrontMatrixPixel(32);
            setFrontMatrixPixel(31);
            setFrontMatrixPixel(30);
            setFrontMatrixPixel(29);
            setFrontMatrixPixel(28);
            setFrontMatrixPixel(27);
            setFrontMatrixPixel(26);
            setFrontMatrixPixel(25);
            setFrontMatrixPixel(24);
            setFrontMatrixPixel(23);
            setFrontMatrixPixel(22);
            break;

        case FrontWord::fuenf:
            setFrontMatrixPixel(7);
            setFrontMatrixPixel(8);
            setFrontMatrixPixel(9);
            setFrontMatrixPixel(10);
            break;

        case FrontWord::zehn:
            setFrontMatrixPixel(21);
            setFrontMatrixPixel(20);
            setFrontMatrixPixel(19);
            setFrontMatrixPixel(18);
            break;

        case FrontWord::zwanzig:
            setFrontMatrixPixel(17);
            setFrontMatrixPixel(16);
            setFrontMatrixPixel(15);
            setFrontMatrixPixel(14);
            setFrontMatrixPixel(13);
            setFrontMatrixPixel(12);
            setFrontMatrixPixel(11);
            break;

        case FrontWord::halb:
            setFrontMatrixPixel(44);
            setFrontMatrixPixel(45);
            setFrontMatrixPixel(46);
            setFrontMatrixPixel(47);
            break;

        case FrontWord::eins:
            setFrontMatrixPixel(65);
            setFrontMatrixPixel(64);
            setFrontMatrixPixel(63);
            setFrontMatrixPixel(62);
            break;

        case FrontWord::uhr:
            setFrontMatrixPixel(99);
            setFrontMatrixPixel(100);
            setFrontMatrixPixel(101);
            break;

        case FrontWord::h_ein:
            setFrontMatrixPixel(63);
            setFrontMatrixPixel(64);
            setFrontMatrixPixel(65);
            break;

        case FrontWord::h_zwei:
            setFrontMatrixPixel(55);
            setFrontMatrixPixel(56);
            setFrontMatrixPixel(57);
            setFrontMatrixPixel(58);
            break;

        case FrontWord::h_drei:
            setFrontMatrixPixel(66);
            setFrontMatrixPixel(67);
            setFrontMatrixPixel(68);
            setFrontMatrixPixel(69);
            break;

        case FrontWord::h_vier:
            setFrontMatrixPixel(73);
            setFrontMatrixPixel(74);
            setFrontMatrixPixel(75);
            setFrontMatrixPixel(76);
            break;

        case FrontWord::h_fuenf:
            setFrontMatrixPixel(51);
            setFrontMatrixPixel(52);
            setFrontMatrixPixel(53);
            setFrontMatrixPixel(54);
            break;

        case FrontWord::h_sechs:
            setFrontMatrixPixel(83);
            setFrontMatrixPixel(84);
            setFrontMatrixPixel(85);
            setFrontMatrixPixel(86);
            setFrontMatrixPixel(87);
            break;

        case FrontWord::h_sieben:
            setFrontMatrixPixel(88);
            setFrontMatrixPixel(89);
            setFrontMatrixPixel(90);
            setFrontMatrixPixel(91);
            setFrontMatrixPixel(92);
            setFrontMatrixPixel(93);
            break;

        case FrontWord::h_acht:
            setFrontMatrixPixel(77);
            setFrontMatrixPixel(78);
            setFrontMatrixPixel(79);
            setFrontMatrixPixel(80);
            break;

        case FrontWord::h_neun:
            setFrontMatrixPixel(103);
            setFrontMatrixPixel(104);
            setFrontMatrixPixel(105);
            setFrontMatrixPixel(106);
            break;

        case FrontWord::h_zehn:
            setFrontMatrixPixel(106);
            setFrontMatrixPixel(107);
            setFrontMatrixPixel(108);
            setFrontMatrixPixel(109);
            break;

        case FrontWord::h_elf:
            setFrontMatrixPixel(49);
            setFrontMatrixPixel(50);
            setFrontMatrixPixel(51);
            break;

        case FrontWord::h_zwoelf:
            setFrontMatrixPixel(94);
            setFrontMatrixPixel(95);
            setFrontMatrixPixel(96);
            setFrontMatrixPixel(97);
            setFrontMatrixPixel(98);
            break;

        case FrontWord::funk:
            setFrontMatrixPixel(40);
            setFrontMatrixPixel(39);
            setFrontMatrixPixel(38);
            setFrontMatrixPixel(37);
            break;

        default:
            break;
        };
    };
};

De10x11Clock_t _de10x11Clock;
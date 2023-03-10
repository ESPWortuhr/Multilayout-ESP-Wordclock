#pragma once

#include "Uhrtype.hpp"

/*
 * Layout Front
 *
 * E S J I S T L F Ü N F
 * Z E H N Z W A N Z I G
 * D R E I V I E R T E L
 * T F N A C H V O R J M
 * H A L B X Z W Ö L F T
 * Z W E I N S I E B E N
 * L D R E I C I F Ü N F
 * E L F N E U N V I E R
 * U A C H T Z E H N S I
 * C S E C H S T L U H R
 */

class De10x11Nero_t : public iUhrType {
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
            setFrontMatrixPixel(38);
            setFrontMatrixPixel(39);
            setFrontMatrixPixel(40);
            setFrontMatrixPixel(41);
            break;

        case FrontWord::vor:
        case FrontWord::v_vor:
            setFrontMatrixPixel(35);
            setFrontMatrixPixel(36);
            setFrontMatrixPixel(37);
            break;

        case FrontWord::viertel:
            setFrontMatrixPixel(26);
            setFrontMatrixPixel(27);
            setFrontMatrixPixel(28);
            setFrontMatrixPixel(29);
            setFrontMatrixPixel(30);
            setFrontMatrixPixel(31);
            setFrontMatrixPixel(32);
            break;

        case FrontWord::dreiviertel:
            setFrontMatrixPixel(22);
            setFrontMatrixPixel(23);
            setFrontMatrixPixel(24);
            setFrontMatrixPixel(25);
            setFrontMatrixPixel(26);
            setFrontMatrixPixel(27);
            setFrontMatrixPixel(28);
            setFrontMatrixPixel(29);
            setFrontMatrixPixel(30);
            setFrontMatrixPixel(31);
            setFrontMatrixPixel(32);
            break;

        case FrontWord::fuenf:
            setFrontMatrixPixel(7);
            setFrontMatrixPixel(8);
            setFrontMatrixPixel(9);
            setFrontMatrixPixel(10);
            break;

        case FrontWord::zehn:
            setFrontMatrixPixel(18);
            setFrontMatrixPixel(19);
            setFrontMatrixPixel(20);
            setFrontMatrixPixel(21);
            break;

        case FrontWord::zwanzig:
            setFrontMatrixPixel(11);
            setFrontMatrixPixel(12);
            setFrontMatrixPixel(13);
            setFrontMatrixPixel(14);
            setFrontMatrixPixel(15);
            setFrontMatrixPixel(16);
            setFrontMatrixPixel(17);
            break;

        case FrontWord::halb:
            setFrontMatrixPixel(44);
            setFrontMatrixPixel(45);
            setFrontMatrixPixel(46);
            setFrontMatrixPixel(47);
            break;

        case FrontWord::eins:
            setFrontMatrixPixel(60);
            setFrontMatrixPixel(61);
            setFrontMatrixPixel(62);
            setFrontMatrixPixel(63);
            break;

        case FrontWord::uhr:
            setFrontMatrixPixel(99);
            setFrontMatrixPixel(100);
            setFrontMatrixPixel(101);
            break;

        case FrontWord::h_ein:
            setFrontMatrixPixel(61);
            setFrontMatrixPixel(62);
            setFrontMatrixPixel(63);
            break;

        case FrontWord::h_zwei:
            setFrontMatrixPixel(62);
            setFrontMatrixPixel(63);
            setFrontMatrixPixel(64);
            setFrontMatrixPixel(65);
            break;

        case FrontWord::h_drei:
            setFrontMatrixPixel(67);
            setFrontMatrixPixel(68);
            setFrontMatrixPixel(69);
            setFrontMatrixPixel(70);
            break;

        case FrontWord::h_vier:
            setFrontMatrixPixel(77);
            setFrontMatrixPixel(78);
            setFrontMatrixPixel(79);
            setFrontMatrixPixel(80);
            break;

        case FrontWord::h_fuenf:
            setFrontMatrixPixel(73);
            setFrontMatrixPixel(74);
            setFrontMatrixPixel(75);
            setFrontMatrixPixel(76);
            break;

        case FrontWord::h_sechs:
            setFrontMatrixPixel(104);
            setFrontMatrixPixel(105);
            setFrontMatrixPixel(106);
            setFrontMatrixPixel(107);
            setFrontMatrixPixel(108);
            break;

        case FrontWord::h_sieben:
            setFrontMatrixPixel(55);
            setFrontMatrixPixel(56);
            setFrontMatrixPixel(57);
            setFrontMatrixPixel(58);
            setFrontMatrixPixel(59);
            setFrontMatrixPixel(60);
            break;

        case FrontWord::h_acht:
            setFrontMatrixPixel(89);
            setFrontMatrixPixel(90);
            setFrontMatrixPixel(91);
            setFrontMatrixPixel(92);
            break;

        case FrontWord::h_neun:
            setFrontMatrixPixel(81);
            setFrontMatrixPixel(82);
            setFrontMatrixPixel(83);
            setFrontMatrixPixel(84);
            break;

        case FrontWord::h_zehn:
            setFrontMatrixPixel(93);
            setFrontMatrixPixel(94);
            setFrontMatrixPixel(95);
            setFrontMatrixPixel(96);
            break;

        case FrontWord::h_elf:
            setFrontMatrixPixel(85);
            setFrontMatrixPixel(86);
            setFrontMatrixPixel(87);
            break;

        case FrontWord::h_zwoelf:

            setFrontMatrixPixel(49);
            setFrontMatrixPixel(50);
            setFrontMatrixPixel(51);
            setFrontMatrixPixel(52);
            setFrontMatrixPixel(53);
            break;

        default:
            break;
        };
    };
};

De10x11Nero_t _de10x11Nero;
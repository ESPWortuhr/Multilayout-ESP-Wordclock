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
            setLetter(0);
            setLetter(1);

            // Ist
            setLetter(3);
            setLetter(4);
            setLetter(5);
            break;

        case FrontWord::nach:
        case FrontWord::v_nach:
            setLetter(38);
            setLetter(39);
            setLetter(40);
            setLetter(41);
            break;

        case FrontWord::vor:
        case FrontWord::v_vor:
            setLetter(35);
            setLetter(36);
            setLetter(37);
            break;

        case FrontWord::viertel:
            setLetter(26);
            setLetter(27);
            setLetter(28);
            setLetter(29);
            setLetter(30);
            setLetter(31);
            setLetter(32);
            break;

        case FrontWord::dreiviertel:
            setLetter(22);
            setLetter(23);
            setLetter(24);
            setLetter(25);
            setLetter(26);
            setLetter(27);
            setLetter(28);
            setLetter(29);
            setLetter(30);
            setLetter(31);
            setLetter(32);
            break;

        case FrontWord::fuenf:
            setLetter(7);
            setLetter(8);
            setLetter(9);
            setLetter(10);
            break;

        case FrontWord::zehn:
            setLetter(18);
            setLetter(19);
            setLetter(20);
            setLetter(21);
            break;

        case FrontWord::zwanzig:
            setLetter(11);
            setLetter(12);
            setLetter(13);
            setLetter(14);
            setLetter(15);
            setLetter(16);
            setLetter(17);
            break;

        case FrontWord::halb:
            setLetter(44);
            setLetter(45);
            setLetter(46);
            setLetter(47);
            break;

        case FrontWord::eins:
            setLetter(60);
            setLetter(61);
            setLetter(62);
            setLetter(63);
            break;

        case FrontWord::uhr:
            setLetter(99);
            setLetter(100);
            setLetter(101);
            break;

        case FrontWord::h_ein:
            setLetter(61);
            setLetter(62);
            setLetter(63);
            break;

        case FrontWord::h_zwei:
            setLetter(62);
            setLetter(63);
            setLetter(64);
            setLetter(65);
            break;

        case FrontWord::h_drei:
            setLetter(67);
            setLetter(68);
            setLetter(69);
            setLetter(70);
            break;

        case FrontWord::h_vier:
            setLetter(77);
            setLetter(78);
            setLetter(79);
            setLetter(80);
            break;

        case FrontWord::h_fuenf:
            setLetter(73);
            setLetter(74);
            setLetter(75);
            setLetter(76);
            break;

        case FrontWord::h_sechs:
            setLetter(104);
            setLetter(105);
            setLetter(106);
            setLetter(107);
            setLetter(108);
            break;

        case FrontWord::h_sieben:
            setLetter(55);
            setLetter(56);
            setLetter(57);
            setLetter(58);
            setLetter(59);
            setLetter(60);
            break;

        case FrontWord::h_acht:
            setLetter(89);
            setLetter(90);
            setLetter(91);
            setLetter(92);
            break;

        case FrontWord::h_neun:
            setLetter(81);
            setLetter(82);
            setLetter(83);
            setLetter(84);
            break;

        case FrontWord::h_zehn:
            setLetter(93);
            setLetter(94);
            setLetter(95);
            setLetter(96);
            break;

        case FrontWord::h_elf:
            setLetter(85);
            setLetter(86);
            setLetter(87);
            break;

        case FrontWord::h_zwoelf:

            setLetter(49);
            setLetter(50);
            setLetter(51);
            setLetter(52);
            setLetter(53);
            break;

        default:
            break;
        };
    };
};

De10x11Nero_t _de10x11Nero;
#pragma once

#include "Uhrtype.hpp"

/*
 * Layout Front
 *
 * E S K I S T L F Ü N F
 * Z E H N Z W A N Z I G
 * D R E I V I E R T E L
 * N A C H A P P Y V O R
 * H A L B I R T H D A Y
 * D R Z W Ö L F Ü N F X
 * Z E H N E U N D R E I
 * Z W E I N S I E B E N
 * E L F V I E R A C H T
 * S E C H S I U H R Y E
 *
 */

class De10x11Alternative_t : public iUhrType {
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
            setLetter(43);
            setLetter(42);
            setLetter(41);
            setLetter(40);
            break;

        case FrontWord::vor:
        case FrontWord::v_vor:
            setLetter(33);
            setLetter(34);
            setLetter(35);
            break;

        case FrontWord::viertel:
            setLetter(32);
            setLetter(31);
            setLetter(30);
            setLetter(29);
            setLetter(28);
            setLetter(27);
            setLetter(26);
            break;

        case FrontWord::dreiviertel:
            setLetter(32);
            setLetter(31);
            setLetter(30);
            setLetter(29);
            setLetter(28);
            setLetter(27);
            setLetter(26);
            setLetter(25);
            setLetter(24);
            setLetter(23);
            setLetter(22);
            break;

        case FrontWord::fuenf:
            setLetter(7);
            setLetter(8);
            setLetter(9);
            setLetter(10);
            break;

        case FrontWord::zehn:
            setLetter(21);
            setLetter(20);
            setLetter(19);
            setLetter(18);
            break;

        case FrontWord::zwanzig:
            setLetter(17);
            setLetter(16);
            setLetter(15);
            setLetter(14);
            setLetter(13);
            setLetter(12);
            setLetter(11);
            break;

        case FrontWord::halb:
            setLetter(44);
            setLetter(45);
            setLetter(46);
            setLetter(47);
            break;

        case FrontWord::eins:
            setLetter(85);
            setLetter(84);
            setLetter(83);
            setLetter(82);
            break;

        case FrontWord::uhr:
            setLetter(101);
            setLetter(102);
            setLetter(103);
            break;

        case FrontWord::happy_birthday:
            // happy
            setLetter(40);
            setLetter(39);
            setLetter(38);
            setLetter(37);
            setLetter(36);

            // happy
            setLetter(47);
            setLetter(48);
            setLetter(49);
            setLetter(50);
            setLetter(51);
            setLetter(52);
            setLetter(53);
            setLetter(54);
            break;

        case FrontWord::h_ein:
            setLetter(85);
            setLetter(84);
            setLetter(83);
            break;

        case FrontWord::h_zwei:
            setLetter(87);
            setLetter(86);
            setLetter(85);
            setLetter(84);
            break;

        case FrontWord::h_drei:
            setLetter(73);
            setLetter(74);
            setLetter(75);
            setLetter(76);
            break;

        case FrontWord::h_vier:
            setLetter(91);
            setLetter(92);
            setLetter(93);
            setLetter(94);
            break;

        case FrontWord::h_fuenf:
            setLetter(59);
            setLetter(58);
            setLetter(57);
            setLetter(56);
            break;

        case FrontWord::h_sechs:
            setLetter(109);
            setLetter(108);
            setLetter(107);
            setLetter(106);
            setLetter(105);
            break;

        case FrontWord::h_sieben:
            setLetter(82);
            setLetter(81);
            setLetter(80);
            setLetter(79);
            setLetter(78);
            setLetter(77);
            break;

        case FrontWord::h_acht:
            setLetter(95);
            setLetter(96);
            setLetter(97);
            setLetter(98);
            break;

        case FrontWord::h_neun:
            setLetter(69);
            setLetter(70);
            setLetter(71);
            setLetter(72);
            break;

        case FrontWord::h_zehn:
            setLetter(66);
            setLetter(67);
            setLetter(68);
            setLetter(69);
            break;

        case FrontWord::h_elf:
            setLetter(88);
            setLetter(89);
            setLetter(90);
            break;

        case FrontWord::h_zwoelf:
            setLetter(63);
            setLetter(62);
            setLetter(61);
            setLetter(60);
            setLetter(59);
            break;

        case FrontWord::h_droelf:
            setLetter(65);
            setLetter(64);
            setLetter(61);
            setLetter(60);
            setLetter(59);
            break;

        default:
            break;
        };
    };
};

De10x11Alternative_t _de10x11Alternative;
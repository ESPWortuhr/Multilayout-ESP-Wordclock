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
            setFrontMatrixPixel(0);
            setFrontMatrixPixel(1);

            // Ist
            setFrontMatrixPixel(3);
            setFrontMatrixPixel(4);
            setFrontMatrixPixel(5);
            break;

        case FrontWord::nach:
        case FrontWord::v_nach:
            setFrontMatrixPixel(43);
            setFrontMatrixPixel(42);
            setFrontMatrixPixel(41);
            setFrontMatrixPixel(40);
            break;

        case FrontWord::vor:
        case FrontWord::v_vor:
            setFrontMatrixPixel(33);
            setFrontMatrixPixel(34);
            setFrontMatrixPixel(35);
            break;

        case FrontWord::viertel:
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
            setFrontMatrixPixel(85);
            setFrontMatrixPixel(84);
            setFrontMatrixPixel(83);
            setFrontMatrixPixel(82);
            break;

        case FrontWord::uhr:
            setFrontMatrixPixel(101);
            setFrontMatrixPixel(102);
            setFrontMatrixPixel(103);
            break;

        case FrontWord::happy_birthday:
            // happy
            setFrontMatrixPixel(40);
            setFrontMatrixPixel(39);
            setFrontMatrixPixel(38);
            setFrontMatrixPixel(37);
            setFrontMatrixPixel(36);

            // happy
            setFrontMatrixPixel(47);
            setFrontMatrixPixel(48);
            setFrontMatrixPixel(49);
            setFrontMatrixPixel(50);
            setFrontMatrixPixel(51);
            setFrontMatrixPixel(52);
            setFrontMatrixPixel(53);
            setFrontMatrixPixel(54);
            break;

        case FrontWord::h_ein:
            setFrontMatrixPixel(85);
            setFrontMatrixPixel(84);
            setFrontMatrixPixel(83);
            break;

        case FrontWord::h_zwei:
            setFrontMatrixPixel(87);
            setFrontMatrixPixel(86);
            setFrontMatrixPixel(85);
            setFrontMatrixPixel(84);
            break;

        case FrontWord::h_drei:
            setFrontMatrixPixel(73);
            setFrontMatrixPixel(74);
            setFrontMatrixPixel(75);
            setFrontMatrixPixel(76);
            break;

        case FrontWord::h_vier:
            setFrontMatrixPixel(91);
            setFrontMatrixPixel(92);
            setFrontMatrixPixel(93);
            setFrontMatrixPixel(94);
            break;

        case FrontWord::h_fuenf:
            setFrontMatrixPixel(59);
            setFrontMatrixPixel(58);
            setFrontMatrixPixel(57);
            setFrontMatrixPixel(56);
            break;

        case FrontWord::h_sechs:
            setFrontMatrixPixel(109);
            setFrontMatrixPixel(108);
            setFrontMatrixPixel(107);
            setFrontMatrixPixel(106);
            setFrontMatrixPixel(105);
            break;

        case FrontWord::h_sieben:
            setFrontMatrixPixel(82);
            setFrontMatrixPixel(81);
            setFrontMatrixPixel(80);
            setFrontMatrixPixel(79);
            setFrontMatrixPixel(78);
            setFrontMatrixPixel(77);
            break;

        case FrontWord::h_acht:
            setFrontMatrixPixel(95);
            setFrontMatrixPixel(96);
            setFrontMatrixPixel(97);
            setFrontMatrixPixel(98);
            break;

        case FrontWord::h_neun:
            setFrontMatrixPixel(69);
            setFrontMatrixPixel(70);
            setFrontMatrixPixel(71);
            setFrontMatrixPixel(72);
            break;

        case FrontWord::h_zehn:
            setFrontMatrixPixel(66);
            setFrontMatrixPixel(67);
            setFrontMatrixPixel(68);
            setFrontMatrixPixel(69);
            break;

        case FrontWord::h_elf:
            setFrontMatrixPixel(88);
            setFrontMatrixPixel(89);
            setFrontMatrixPixel(90);
            break;

        case FrontWord::h_zwoelf:
            setFrontMatrixPixel(63);
            setFrontMatrixPixel(62);
            setFrontMatrixPixel(61);
            setFrontMatrixPixel(60);
            setFrontMatrixPixel(59);
            break;

        case FrontWord::h_droelf:
            setFrontMatrixPixel(65);
            setFrontMatrixPixel(64);
            setFrontMatrixPixel(61);
            setFrontMatrixPixel(60);
            setFrontMatrixPixel(59);
            break;

        default:
            break;
        };
    };
};

De10x11Alternative_t _de10x11Alternative;
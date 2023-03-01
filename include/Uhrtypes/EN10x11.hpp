#pragma once

#include "Uhrtype.hpp"

/*
 * Layout Front
 *
 * I T L I S A S A M P M
 * A C Q U A R T E R D C
 * T W E N T Y F I V E X
 * H A L F S T E N F T O
 * P A S T E R U N I N E
 * O N E S I X T H R E E
 * F O U R F I V E T W O
 * E I G H T E L E V E N
 * S E V E N T W E L V E
 * T E N S E O'C L O C K
 *
 */

class En10x11_t : public iUhrType {
public:
    virtual LanguageAbbreviation usedLang() override {
        return LanguageAbbreviation::EN;
    };

    //------------------------------------------------------------------------------

    virtual const bool hasZwanzig() override { return true; }

    //------------------------------------------------------------------------------

    virtual const bool hasTwentyfive() override { return true; }

    //------------------------------------------------------------------------------

    void show(FrontWord word) override {
        switch (word) {

        case FrontWord::es_ist:
            // It
            setFrontMatrixPixel(0);
            setFrontMatrixPixel(1);
            // is
            setFrontMatrixPixel(3);
            setFrontMatrixPixel(4);
            break;

        case FrontWord::h_ein:
            // One
            setFrontMatrixPixel(65);
            setFrontMatrixPixel(64);
            setFrontMatrixPixel(63);
            break;

        case FrontWord::h_zwei:
            // Two
            setFrontMatrixPixel(74);
            setFrontMatrixPixel(75);
            setFrontMatrixPixel(76);
            break;

        case FrontWord::h_drei:
            // Three
            setFrontMatrixPixel(59);
            setFrontMatrixPixel(58);
            setFrontMatrixPixel(57);
            setFrontMatrixPixel(56);
            setFrontMatrixPixel(55);
            break;

        case FrontWord::h_vier:
            // Four
            setFrontMatrixPixel(66);
            setFrontMatrixPixel(67);
            setFrontMatrixPixel(68);
            setFrontMatrixPixel(69);
            break;

        case FrontWord::h_fuenf:
            // Five
            setFrontMatrixPixel(70);
            setFrontMatrixPixel(71);
            setFrontMatrixPixel(72);
            setFrontMatrixPixel(73);
            break;

        case FrontWord::h_sechs:
            // Six
            setFrontMatrixPixel(62);
            setFrontMatrixPixel(61);
            setFrontMatrixPixel(60);
            break;

        case FrontWord::h_sieben:
            // Seven
            setFrontMatrixPixel(88);
            setFrontMatrixPixel(89);
            setFrontMatrixPixel(90);
            setFrontMatrixPixel(91);
            setFrontMatrixPixel(92);
            break;

        case FrontWord::h_acht:
            // Eight
            setFrontMatrixPixel(87);
            setFrontMatrixPixel(86);
            setFrontMatrixPixel(85);
            setFrontMatrixPixel(84);
            setFrontMatrixPixel(83);
            break;

        case FrontWord::h_neun:
            // Nine
            setFrontMatrixPixel(51);
            setFrontMatrixPixel(52);
            setFrontMatrixPixel(53);
            setFrontMatrixPixel(54);
            break;

        case FrontWord::h_zehn:
            // Ten
            setFrontMatrixPixel(109);
            setFrontMatrixPixel(108);
            setFrontMatrixPixel(107);
            break;

        case FrontWord::h_elf:
            // Eleven
            setFrontMatrixPixel(82);
            setFrontMatrixPixel(81);
            setFrontMatrixPixel(80);
            setFrontMatrixPixel(79);
            setFrontMatrixPixel(78);
            setFrontMatrixPixel(77);
            break;

        case FrontWord::h_zwoelf:
            // Twelve
            setFrontMatrixPixel(93);
            setFrontMatrixPixel(94);
            setFrontMatrixPixel(95);
            setFrontMatrixPixel(96);
            setFrontMatrixPixel(97);
            setFrontMatrixPixel(98);
            break;

        case FrontWord::fuenf:
            // Five
            setFrontMatrixPixel(28);
            setFrontMatrixPixel(29);
            setFrontMatrixPixel(30);
            setFrontMatrixPixel(31);
            break;

        case FrontWord::zehn:
            // Ten
            setFrontMatrixPixel(38);
            setFrontMatrixPixel(37);
            setFrontMatrixPixel(36);
            break;

        case FrontWord::a_quarter:
            // A Quater
            setFrontMatrixPixel(21);
            setFrontMatrixPixel(13);
            setFrontMatrixPixel(14);
            setFrontMatrixPixel(15);
            setFrontMatrixPixel(16);
            setFrontMatrixPixel(17);
            setFrontMatrixPixel(18);
            setFrontMatrixPixel(19);
            break;

        case FrontWord::viertel:
            // Quater
            setFrontMatrixPixel(13);
            setFrontMatrixPixel(14);
            setFrontMatrixPixel(15);
            setFrontMatrixPixel(16);
            setFrontMatrixPixel(17);
            setFrontMatrixPixel(18);
            setFrontMatrixPixel(19);
            break;

        case FrontWord::zwanzig:
            // Twenty
            setFrontMatrixPixel(22);
            setFrontMatrixPixel(23);
            setFrontMatrixPixel(24);
            setFrontMatrixPixel(25);
            setFrontMatrixPixel(26);
            setFrontMatrixPixel(27);
            break;

        case FrontWord::twentyfive:
            // Twentyfive
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
            break;

        case FrontWord::halb:
            // Half
            setFrontMatrixPixel(43);
            setFrontMatrixPixel(42);
            setFrontMatrixPixel(41);
            setFrontMatrixPixel(40);
            break;

        case FrontWord::nach:
        case FrontWord::v_nach:
            // Past
            setFrontMatrixPixel(44);
            setFrontMatrixPixel(45);
            setFrontMatrixPixel(46);
            setFrontMatrixPixel(47);
            break;

        case FrontWord::vor:
        case FrontWord::v_vor:
            // To
            setFrontMatrixPixel(34);
            setFrontMatrixPixel(33);
            break;

        case FrontWord::uhr:
            // O'Clock
            setFrontMatrixPixel(104);
            setFrontMatrixPixel(103);
            setFrontMatrixPixel(102);
            setFrontMatrixPixel(101);
            setFrontMatrixPixel(100);
            setFrontMatrixPixel(99);
            break;

        default:
            break;
        };
    };
};

En10x11_t _en10x11;
#pragma once

#include "Uhrtype.hpp"

/*
 * Layout Front
 *
 * H E T L I S H N U W S
 * T W I N T I G T I E N
 * V I J F B Y K W A R T
 * V O O R P M O V E R U
 * H A L F I T W E E N N
 * A C H T D R I E Z E S
 * Z E V E N E G E N T O
 * T W A A L F A T I E N
 * V I E R V I J F E L F
 * U U R A G E W E E S T
 *
 */

class NL10x11_t : public iUhrType {
public:
    virtual LanguageAbbreviation usedLang() override {
        return LanguageAbbreviation::NL;
    };

    //------------------------------------------------------------------------------

    void show(FrontWord word) override {
        switch (word) {

        case FrontWord::es_ist:
            // Es
            setFrontMatrixPixel(0);
            setFrontMatrixPixel(1);
            setFrontMatrixPixel(2);

            // Ist
            setFrontMatrixPixel(4);
            setFrontMatrixPixel(5);
            break;

        case FrontWord::nach:
        case FrontWord::v_nach:
            setFrontMatrixPixel(37);
            setFrontMatrixPixel(36);
            setFrontMatrixPixel(35);
            setFrontMatrixPixel(34);
            break;

        case FrontWord::vor:
        case FrontWord::v_vor:
            setFrontMatrixPixel(43);
            setFrontMatrixPixel(42);
            setFrontMatrixPixel(41);
            setFrontMatrixPixel(40);
            break;

        case FrontWord::viertel:
            setFrontMatrixPixel(28);
            setFrontMatrixPixel(29);
            setFrontMatrixPixel(30);
            setFrontMatrixPixel(31);
            setFrontMatrixPixel(32);
            break;

        case FrontWord::fuenf:
            setFrontMatrixPixel(22);
            setFrontMatrixPixel(23);
            setFrontMatrixPixel(24);
            setFrontMatrixPixel(25);
            break;

        case FrontWord::zehn:
            setFrontMatrixPixel(14);
            setFrontMatrixPixel(13);
            setFrontMatrixPixel(12);
            setFrontMatrixPixel(11);
            break;

        case FrontWord::zwanzig:
            setFrontMatrixPixel(21);
            setFrontMatrixPixel(20);
            setFrontMatrixPixel(19);
            setFrontMatrixPixel(18);
            setFrontMatrixPixel(17);
            setFrontMatrixPixel(16);
            setFrontMatrixPixel(15);
            break;

        case FrontWord::halb:
            setFrontMatrixPixel(44);
            setFrontMatrixPixel(45);
            setFrontMatrixPixel(46);
            setFrontMatrixPixel(47);
            break;

        case FrontWord::h_ein:
            setFrontMatrixPixel(51);
            setFrontMatrixPixel(52);
            setFrontMatrixPixel(53);
            break;

        case FrontWord::uhr:
            setFrontMatrixPixel(109);
            setFrontMatrixPixel(108);
            setFrontMatrixPixel(107);
            break;

        case FrontWord::h_zwei:
            setFrontMatrixPixel(49);
            setFrontMatrixPixel(50);
            setFrontMatrixPixel(51);
            setFrontMatrixPixel(52);
            break;

        case FrontWord::h_drei:
            setFrontMatrixPixel(61);
            setFrontMatrixPixel(60);
            setFrontMatrixPixel(59);
            setFrontMatrixPixel(58);
            break;

        case FrontWord::h_vier:
            setFrontMatrixPixel(88);
            setFrontMatrixPixel(89);
            setFrontMatrixPixel(90);
            setFrontMatrixPixel(91);
            break;

        case FrontWord::h_fuenf:
            setFrontMatrixPixel(92);
            setFrontMatrixPixel(93);
            setFrontMatrixPixel(94);
            setFrontMatrixPixel(95);
            break;

        case FrontWord::h_sechs:
            setFrontMatrixPixel(57);
            setFrontMatrixPixel(56);
            setFrontMatrixPixel(55);
            break;

        case FrontWord::h_sieben:
            setFrontMatrixPixel(66);
            setFrontMatrixPixel(67);
            setFrontMatrixPixel(68);
            setFrontMatrixPixel(69);
            setFrontMatrixPixel(70);
            break;

        case FrontWord::h_acht:
            setFrontMatrixPixel(65);
            setFrontMatrixPixel(64);
            setFrontMatrixPixel(63);
            setFrontMatrixPixel(62);
            break;

        case FrontWord::h_neun:
            setFrontMatrixPixel(70);
            setFrontMatrixPixel(71);
            setFrontMatrixPixel(72);
            setFrontMatrixPixel(73);
            setFrontMatrixPixel(74);
            break;

        case FrontWord::h_zehn:
            setFrontMatrixPixel(80);
            setFrontMatrixPixel(79);
            setFrontMatrixPixel(78);
            setFrontMatrixPixel(77);
            break;

        case FrontWord::h_elf:
            setFrontMatrixPixel(96);
            setFrontMatrixPixel(97);
            setFrontMatrixPixel(98);
            break;

        case FrontWord::h_zwoelf:
            setFrontMatrixPixel(87);
            setFrontMatrixPixel(86);
            setFrontMatrixPixel(85);
            setFrontMatrixPixel(84);
            setFrontMatrixPixel(83);
            setFrontMatrixPixel(82);
            break;

        case FrontWord::nur:
            // Nu
            setFrontMatrixPixel(7);
            setFrontMatrixPixel(8);
            break;

        case FrontWord::gewesen:
            // Geweest
            setFrontMatrixPixel(105);
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

NL10x11_t _nl10x11;
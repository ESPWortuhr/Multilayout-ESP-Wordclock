#pragma once

#include "DE11x11.hpp"

/*
 * Layout Front
 *
 * E S K I S T A F Ü N F
 * Z E H N D A U V O R G
 * N A C H V I E R T E L
 * H A L B V O R N A C H
 * E I N S K U R Z W E I
 * D R E I A U J V I E R
 * F Ü N F T O S E C H S
 * S I E B E N L A C H T
 * A N E U N M H Z E H N
 * Z W Ö L F D T F E L F
 * W A S D F U N K U H R
 * + + + +
 */

class De11x11V2_t : public De11x11_t {
public:
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
            setFrontMatrixPixel(22);
            setFrontMatrixPixel(23);
            setFrontMatrixPixel(24);
            setFrontMatrixPixel(25);
            break;

        case FrontWord::vor:
            setFrontMatrixPixel(12);
            setFrontMatrixPixel(13);
            setFrontMatrixPixel(14);
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

        case FrontWord::halb:
            setFrontMatrixPixel(43);
            setFrontMatrixPixel(42);
            setFrontMatrixPixel(41);
            setFrontMatrixPixel(40);
            break;

        case FrontWord::eins:
            setFrontMatrixPixel(44);
            setFrontMatrixPixel(45);
            setFrontMatrixPixel(46);
            setFrontMatrixPixel(47);
            break;

        case FrontWord::uhr:
            setFrontMatrixPixel(120);
            setFrontMatrixPixel(119);
            setFrontMatrixPixel(118);
            break;

        case FrontWord::v_nach:
            setFrontMatrixPixel(36);
            setFrontMatrixPixel(35);
            setFrontMatrixPixel(34);
            setFrontMatrixPixel(33);
            break;

        case FrontWord::v_vor:
            setFrontMatrixPixel(39);
            setFrontMatrixPixel(38);
            setFrontMatrixPixel(37);
            break;

        case FrontWord::h_ein:
            setFrontMatrixPixel(44);
            setFrontMatrixPixel(45);
            setFrontMatrixPixel(46);
            break;

        case FrontWord::h_zwei:
            setFrontMatrixPixel(51);
            setFrontMatrixPixel(52);
            setFrontMatrixPixel(53);
            setFrontMatrixPixel(54);
            break;

        case FrontWord::h_drei:
            setFrontMatrixPixel(62);
            setFrontMatrixPixel(63);
            setFrontMatrixPixel(64);
            setFrontMatrixPixel(65);
            break;

        case FrontWord::h_vier:
            setFrontMatrixPixel(55);
            setFrontMatrixPixel(56);
            setFrontMatrixPixel(57);
            setFrontMatrixPixel(58);
            break;

        case FrontWord::h_fuenf:
            setFrontMatrixPixel(66);
            setFrontMatrixPixel(67);
            setFrontMatrixPixel(68);
            setFrontMatrixPixel(69);
            break;

        case FrontWord::h_sechs:
            setFrontMatrixPixel(72);
            setFrontMatrixPixel(73);
            setFrontMatrixPixel(74);
            setFrontMatrixPixel(75);
            setFrontMatrixPixel(76);
            break;

        case FrontWord::h_sieben:
            setFrontMatrixPixel(87);
            setFrontMatrixPixel(86);
            setFrontMatrixPixel(85);
            setFrontMatrixPixel(84);
            setFrontMatrixPixel(83);
            setFrontMatrixPixel(82);
            break;

        case FrontWord::h_acht:
            setFrontMatrixPixel(77);
            setFrontMatrixPixel(78);
            setFrontMatrixPixel(79);
            setFrontMatrixPixel(80);
            break;

        case FrontWord::h_neun:
            setFrontMatrixPixel(89);
            setFrontMatrixPixel(90);
            setFrontMatrixPixel(91);
            setFrontMatrixPixel(92);
            break;

        case FrontWord::h_zehn:
            setFrontMatrixPixel(95);
            setFrontMatrixPixel(96);
            setFrontMatrixPixel(97);
            setFrontMatrixPixel(98);
            break;

        case FrontWord::h_elf:
            setFrontMatrixPixel(99);
            setFrontMatrixPixel(100);
            setFrontMatrixPixel(101);
            break;

        case FrontWord::h_zwoelf:
            setFrontMatrixPixel(105);
            setFrontMatrixPixel(106);
            setFrontMatrixPixel(107);
            setFrontMatrixPixel(108);
            setFrontMatrixPixel(109);
            break;

        case FrontWord::funk:
            setFrontMatrixPixel(114);
            setFrontMatrixPixel(115);
            setFrontMatrixPixel(116);
            setFrontMatrixPixel(117);
            break;

        default:
            break;
        };
    };
};

De11x11V2_t _de11x11V2;
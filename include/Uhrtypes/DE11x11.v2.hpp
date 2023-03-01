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
            setLetter(0);
            setLetter(1);

            // Ist
            setLetter(3);
            setLetter(4);
            setLetter(5);
            break;

        case FrontWord::nach:
            setLetter(22);
            setLetter(23);
            setLetter(24);
            setLetter(25);
            break;

        case FrontWord::vor:
            setLetter(12);
            setLetter(13);
            setLetter(14);
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

        case FrontWord::halb:
            setLetter(43);
            setLetter(42);
            setLetter(41);
            setLetter(40);
            break;

        case FrontWord::eins:
            setLetter(44);
            setLetter(45);
            setLetter(46);
            setLetter(47);
            break;

        case FrontWord::uhr:
            setLetter(120);
            setLetter(119);
            setLetter(118);
            break;

        case FrontWord::v_nach:
            setLetter(36);
            setLetter(35);
            setLetter(34);
            setLetter(33);
            break;

        case FrontWord::v_vor:
            setLetter(39);
            setLetter(38);
            setLetter(37);
            break;

        case FrontWord::h_ein:
            setLetter(44);
            setLetter(45);
            setLetter(46);
            break;

        case FrontWord::h_zwei:
            setLetter(51);
            setLetter(52);
            setLetter(53);
            setLetter(54);
            break;

        case FrontWord::h_drei:
            setLetter(62);
            setLetter(63);
            setLetter(64);
            setLetter(65);
            break;

        case FrontWord::h_vier:
            setLetter(55);
            setLetter(56);
            setLetter(57);
            setLetter(58);
            break;

        case FrontWord::h_fuenf:
            setLetter(66);
            setLetter(67);
            setLetter(68);
            setLetter(69);
            break;

        case FrontWord::h_sechs:
            setLetter(72);
            setLetter(73);
            setLetter(74);
            setLetter(75);
            setLetter(76);
            break;

        case FrontWord::h_sieben:
            setLetter(87);
            setLetter(86);
            setLetter(85);
            setLetter(84);
            setLetter(83);
            setLetter(82);
            break;

        case FrontWord::h_acht:
            setLetter(77);
            setLetter(78);
            setLetter(79);
            setLetter(80);
            break;

        case FrontWord::h_neun:
            setLetter(89);
            setLetter(90);
            setLetter(91);
            setLetter(92);
            break;

        case FrontWord::h_zehn:
            setLetter(95);
            setLetter(96);
            setLetter(97);
            setLetter(98);
            break;

        case FrontWord::h_elf:
            setLetter(99);
            setLetter(100);
            setLetter(101);
            break;

        case FrontWord::h_zwoelf:
            setLetter(105);
            setLetter(106);
            setLetter(107);
            setLetter(108);
            setLetter(109);
            break;

        case FrontWord::funk:
            setLetter(114);
            setLetter(115);
            setLetter(116);
            setLetter(117);
            break;

        default:
            break;
        };
    };
};

De11x11V2_t _de11x11V2;
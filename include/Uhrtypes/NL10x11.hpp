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
    void show(uint8_t text) override {
        switch (text) {

        case es_ist:
            // Es
            Letter_set(0);
            Letter_set(1);
            Letter_set(2);

            // Ist
            Letter_set(4);
            Letter_set(5);
            break;

        case nach:
        case v_nach:
            Letter_set(37);
            Letter_set(36);
            Letter_set(35);
            Letter_set(34);
            break;

        case vor:
        case v_vor:
            Letter_set(43);
            Letter_set(42);
            Letter_set(41);
            Letter_set(40);
            break;

        case viertel:
            Letter_set(28);
            Letter_set(29);
            Letter_set(30);
            Letter_set(31);
            Letter_set(32);
            break;

        case fuenf:
            Letter_set(22);
            Letter_set(23);
            Letter_set(24);
            Letter_set(25);
            break;

        case zehn:
            Letter_set(14);
            Letter_set(13);
            Letter_set(12);
            Letter_set(11);
            break;

        case halb:
            Letter_set(44);
            Letter_set(45);
            Letter_set(46);
            Letter_set(47);
            break;

        case h_ein:
            Letter_set(51);
            Letter_set(52);
            Letter_set(53);
            break;

        case uhr:
            Letter_set(109);
            Letter_set(108);
            Letter_set(107);
            break;

        case h_zwei:
            Letter_set(49);
            Letter_set(50);
            Letter_set(51);
            Letter_set(52);
            break;

        case h_drei:
            Letter_set(61);
            Letter_set(60);
            Letter_set(59);
            Letter_set(58);
            break;

        case h_vier:
            Letter_set(88);
            Letter_set(89);
            Letter_set(90);
            Letter_set(91);
            break;

        case h_fuenf:
            Letter_set(92);
            Letter_set(93);
            Letter_set(94);
            Letter_set(95);
            break;

        case h_sechs:
            Letter_set(57);
            Letter_set(56);
            Letter_set(55);
            break;

        case h_sieben:
            Letter_set(66);
            Letter_set(67);
            Letter_set(68);
            Letter_set(69);
            Letter_set(70);
            break;

        case h_acht:
            Letter_set(65);
            Letter_set(64);
            Letter_set(63);
            Letter_set(62);
            break;

        case h_neun:
            Letter_set(70);
            Letter_set(71);
            Letter_set(72);
            Letter_set(73);
            Letter_set(74);
            break;

        case h_zehn:
            Letter_set(80);
            Letter_set(79);
            Letter_set(78);
            Letter_set(77);
            break;

        case h_elf:
            Letter_set(96);
            Letter_set(97);
            Letter_set(98);
            break;

        case h_zwoelf:
            Letter_set(87);
            Letter_set(86);
            Letter_set(85);
            Letter_set(84);
            Letter_set(83);
            Letter_set(82);
            break;

        case nur:
            // Nu
            Letter_set(7);
            Letter_set(8);
            break;

        case gewesen:
            // Geweest
            Letter_set(105);
            Letter_set(104);
            Letter_set(103);
            Letter_set(102);
            Letter_set(101);
            Letter_set(100);
            Letter_set(99);
            break;

        default:
            break;
        };
    };
};

NL10x11_t _nl10x11;
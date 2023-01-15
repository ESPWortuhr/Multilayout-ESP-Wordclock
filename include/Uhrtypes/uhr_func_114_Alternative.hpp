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

#define USE_DREIVIERTEL

class UHR_114_Alternative_t : public iUhrType {
public:
    virtual const bool hasDreiviertel() override { return true; }

    //------------------------------------------------------------------------------

    void show(uint8_t text) override {
        switch (text) {

        case es_ist:
            // Es
            Letter_set(0);
            Letter_set(1);

            // Ist
            Letter_set(3);
            Letter_set(4);
            Letter_set(5);
            break;

        case nach:
        case v_nach:
            Letter_set(43);
            Letter_set(42);
            Letter_set(41);
            Letter_set(40);
            break;

        case vor:
        case v_vor:
            Letter_set(33);
            Letter_set(34);
            Letter_set(35);
            break;

        case viertel:
            Letter_set(32);
            Letter_set(31);
            Letter_set(30);
            Letter_set(29);
            Letter_set(28);
            Letter_set(27);
            Letter_set(26);
            break;

        case dreiviertel:
            Letter_set(32);
            Letter_set(31);
            Letter_set(30);
            Letter_set(29);
            Letter_set(28);
            Letter_set(27);
            Letter_set(26);
            Letter_set(25);
            Letter_set(24);
            Letter_set(23);
            Letter_set(22);
            break;

        case fuenf:
            Letter_set(7);
            Letter_set(8);
            Letter_set(9);
            Letter_set(10);
            break;

        case zehn:
            Letter_set(21);
            Letter_set(20);
            Letter_set(19);
            Letter_set(18);
            break;

        case zwanzig:
            Letter_set(17);
            Letter_set(16);
            Letter_set(15);
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

        case eins:
            Letter_set(85);
            Letter_set(84);
            Letter_set(83);
            Letter_set(82);
            break;

        case uhr:
            Letter_set(101);
            Letter_set(102);
            Letter_set(103);
            break;

        case happy_birthday:
            // happy
            Letter_set(40);
            Letter_set(39);
            Letter_set(38);
            Letter_set(37);
            Letter_set(36);

            // happy
            Letter_set(47);
            Letter_set(48);
            Letter_set(49);
            Letter_set(50);
            Letter_set(51);
            Letter_set(52);
            Letter_set(53);
            Letter_set(54);
            break;

        case h_ein:
            Letter_set(85);
            Letter_set(84);
            Letter_set(83);
            break;

        case h_zwei:
            Letter_set(87);
            Letter_set(86);
            Letter_set(85);
            Letter_set(84);
            break;

        case h_drei:
            Letter_set(73);
            Letter_set(74);
            Letter_set(75);
            Letter_set(76);
            break;

        case h_vier:
            Letter_set(91);
            Letter_set(92);
            Letter_set(93);
            Letter_set(94);
            break;

        case h_fuenf:
            Letter_set(59);
            Letter_set(58);
            Letter_set(57);
            Letter_set(56);
            break;

        case h_sechs:
            Letter_set(109);
            Letter_set(108);
            Letter_set(107);
            Letter_set(106);
            Letter_set(105);
            break;

        case h_sieben:
            Letter_set(82);
            Letter_set(81);
            Letter_set(80);
            Letter_set(79);
            Letter_set(78);
            Letter_set(77);
            break;

        case h_acht:
            Letter_set(95);
            Letter_set(96);
            Letter_set(97);
            Letter_set(98);
            break;

        case h_neun:
            Letter_set(69);
            Letter_set(70);
            Letter_set(71);
            Letter_set(72);
            break;

        case h_zehn:
            Letter_set(66);
            Letter_set(67);
            Letter_set(68);
            Letter_set(69);
            break;

        case h_elf:
            Letter_set(88);
            Letter_set(89);
            Letter_set(90);
            break;

        case h_zwoelf:
            Letter_set(63);
            Letter_set(62);
            Letter_set(61);
            Letter_set(60);
            Letter_set(59);
            break;

        case h_droelf:
            Letter_set(65);
            Letter_set(64);
            Letter_set(61);
            Letter_set(60);
            Letter_set(59);
            break;

        default:
            break;
        };
    };
};

UHR_114_Alternative_t Uhr_114_Alternative_type;
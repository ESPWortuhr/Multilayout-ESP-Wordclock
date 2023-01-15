#include "Uhrtype.hpp"

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

class UHR_125_Type2_t : public iUhrType {
public:
    virtual const uint16_t NUM_PIXELS() override { return 125; };

    //------------------------------------------------------------------------------

    virtual const uint16_t NUM_SMATRIX() override { return 125; };

    //------------------------------------------------------------------------------

    virtual const uint16_t ROWS_MATRIX() override { return 12; };

    //------------------------------------------------------------------------------

    virtual const bool hasZwanzig() override { return false; }

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

            //------------------------------------------------------------------------------

        case nach:
            Letter_set(22);
            Letter_set(23);
            Letter_set(24);
            Letter_set(25);
            break;

            //------------------------------------------------------------------------------

        case vor:
            Letter_set(12);
            Letter_set(13);
            Letter_set(14);
            break;

            //------------------------------------------------------------------------------

        case viertel:
            Letter_set(26);
            Letter_set(27);
            Letter_set(28);
            Letter_set(29);
            Letter_set(30);
            Letter_set(31);
            Letter_set(32);
            break;

            //------------------------------------------------------------------------------

        case fuenf:
            Letter_set(7);
            Letter_set(8);
            Letter_set(9);
            Letter_set(10);
            break;

            //------------------------------------------------------------------------------

        case zehn:
            Letter_set(18);
            Letter_set(19);
            Letter_set(20);
            Letter_set(21);
            break;

            //------------------------------------------------------------------------------
        case zwanzig:
            Letter_set(11);
            Letter_set(12);
            Letter_set(13);
            Letter_set(14);
            Letter_set(15);
            Letter_set(16);
            Letter_set(17);
            break;

            //------------------------------------------------------------------------------

        case halb:
            Letter_set(44);
            Letter_set(45);
            Letter_set(46);
            Letter_set(47);
            break;

            //------------------------------------------------------------------------------

        case eins:
            Letter_set(44);
            Letter_set(45);
            Letter_set(46);
            Letter_set(47);
            break;

            //------------------------------------------------------------------------------

        case uhr:
            Letter_set(120);
            Letter_set(119);
            Letter_set(118);
            break;

            //------------------------------------------------------------------------------

        case v_nach:
            Letter_set(36);
            Letter_set(35);
            Letter_set(34);
            Letter_set(33);
            break;

            //------------------------------------------------------------------------------

        case v_vor:
            Letter_set(39);
            Letter_set(38);
            Letter_set(37);
            break;

            //------------------------------------------------------------------------------

        case h_ein:
            Letter_set(44);
            Letter_set(45);
            Letter_set(46);
            break;

            //------------------------------------------------------------------------------

        case h_zwei:
            Letter_set(51);
            Letter_set(52);
            Letter_set(53);
            Letter_set(54);
            break;

            //------------------------------------------------------------------------------

        case h_drei:
            Letter_set(62);
            Letter_set(63);
            Letter_set(64);
            Letter_set(65);
            break;

            //------------------------------------------------------------------------------

        case h_vier:
            Letter_set(55);
            Letter_set(56);
            Letter_set(57);
            Letter_set(58);
            break;

            //------------------------------------------------------------------------------

        case h_fuenf:
            Letter_set(66);
            Letter_set(67);
            Letter_set(68);
            Letter_set(69);
            break;

            //------------------------------------------------------------------------------

        case h_sechs:
            Letter_set(72);
            Letter_set(73);
            Letter_set(74);
            Letter_set(75);
            Letter_set(76);
            break;

            //------------------------------------------------------------------------------

        case h_sieben:
            Letter_set(87);
            Letter_set(86);
            Letter_set(85);
            Letter_set(84);
            Letter_set(83);
            Letter_set(82);
            break;

            //------------------------------------------------------------------------------

        case h_acht:
            Letter_set(77);
            Letter_set(78);
            Letter_set(79);
            Letter_set(80);
            break;

            //------------------------------------------------------------------------------

        case h_neun:
            Letter_set(89);
            Letter_set(90);
            Letter_set(91);
            Letter_set(92);
            break;

            //------------------------------------------------------------------------------

        case h_zehn:
            Letter_set(95);
            Letter_set(96);
            Letter_set(97);
            Letter_set(98);
            break;

            //------------------------------------------------------------------------------

        case h_elf:
            Letter_set(99);
            Letter_set(100);
            Letter_set(101);
            break;

            //------------------------------------------------------------------------------

        case h_zwoelf:

            Letter_set(105);
            Letter_set(106);
            Letter_set(107);
            Letter_set(108);
            Letter_set(109);
            break;

        default:
            break;
        };
    };
};

UHR_125_Type2_t Uhr_125_type2_type;
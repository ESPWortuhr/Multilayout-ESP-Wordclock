#include "Uhrtype.hpp"

/*
 * Layout Front
 *
 * E S K I S T A F Ü N F
 * Z E H N Z W A N Z I G
 * D R E I V I E R T E L
 * V O R F U N K N A C H
 * H A L B A E L F Ü N F
 * E I N S X A M Z W E I
 * D R E I A U J V I E R
 * S E C H S N L A C H T
 * S I E B E N Z W Ö L F
 * Z E H N E U N K U H R
 *
 */

class De10x11Clock_t : public iUhrType {
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
            // NACH
            setLetter(36);
            setLetter(35);
            setLetter(34);
            setLetter(33);
            break;

        case FrontWord::vor:
        case FrontWord::v_vor:
            // Vor
            setLetter(41);
            setLetter(42);
            setLetter(43);
            break;

        case FrontWord::viertel:
            // Viertel
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
            setLetter(65);
            setLetter(64);
            setLetter(63);
            setLetter(62);
            break;

        case FrontWord::uhr:
            setLetter(99);
            setLetter(100);
            setLetter(101);
            break;

        case FrontWord::h_ein:
            setLetter(63);
            setLetter(64);
            setLetter(65);
            break;

        case FrontWord::h_zwei:
            setLetter(55);
            setLetter(56);
            setLetter(57);
            setLetter(58);
            break;

        case FrontWord::h_drei:
            setLetter(66);
            setLetter(67);
            setLetter(68);
            setLetter(69);
            break;

        case FrontWord::h_vier:
            setLetter(73);
            setLetter(74);
            setLetter(75);
            setLetter(76);
            break;

        case FrontWord::h_fuenf:
            setLetter(51);
            setLetter(52);
            setLetter(53);
            setLetter(54);
            break;

        case FrontWord::h_sechs:
            setLetter(83);
            setLetter(84);
            setLetter(85);
            setLetter(86);
            setLetter(87);
            break;

        case FrontWord::h_sieben:
            setLetter(88);
            setLetter(89);
            setLetter(90);
            setLetter(91);
            setLetter(92);
            setLetter(93);
            break;

        case FrontWord::h_acht:
            setLetter(77);
            setLetter(78);
            setLetter(79);
            setLetter(80);
            break;

        case FrontWord::h_neun:
            setLetter(103);
            setLetter(104);
            setLetter(105);
            setLetter(106);
            break;

        case FrontWord::h_zehn:
            setLetter(106);
            setLetter(107);
            setLetter(108);
            setLetter(109);
            break;

        case FrontWord::h_elf:
            setLetter(49);
            setLetter(50);
            setLetter(51);
            break;

        case FrontWord::h_zwoelf:
            setLetter(94);
            setLetter(95);
            setLetter(96);
            setLetter(97);
            setLetter(98);
            break;

        case FrontWord::funk:
            setLetter(40);
            setLetter(39);
            setLetter(38);
            setLetter(37);
            break;

        default:
            break;
        };
    };
};

De10x11Clock_t _de10x11Clock;
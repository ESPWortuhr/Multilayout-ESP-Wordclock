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
            setLetter(0);
            setLetter(1);
            setLetter(2);

            // Ist
            setLetter(4);
            setLetter(5);
            break;

        case FrontWord::nach:
        case FrontWord::v_nach:
            setLetter(37);
            setLetter(36);
            setLetter(35);
            setLetter(34);
            break;

        case FrontWord::vor:
        case FrontWord::v_vor:
            setLetter(43);
            setLetter(42);
            setLetter(41);
            setLetter(40);
            break;

        case FrontWord::viertel:
            setLetter(28);
            setLetter(29);
            setLetter(30);
            setLetter(31);
            setLetter(32);
            break;

        case FrontWord::fuenf:
            setLetter(22);
            setLetter(23);
            setLetter(24);
            setLetter(25);
            break;

        case FrontWord::zehn:
            setLetter(14);
            setLetter(13);
            setLetter(12);
            setLetter(11);
            break;

        case FrontWord::zwanzig:
            setLetter(21);
            setLetter(20);
            setLetter(19);
            setLetter(18);
            setLetter(17);
            setLetter(16);
            setLetter(15);
            break;

        case FrontWord::halb:
            setLetter(44);
            setLetter(45);
            setLetter(46);
            setLetter(47);
            break;

        case FrontWord::h_ein:
            setLetter(51);
            setLetter(52);
            setLetter(53);
            break;

        case FrontWord::uhr:
            setLetter(109);
            setLetter(108);
            setLetter(107);
            break;

        case FrontWord::h_zwei:
            setLetter(49);
            setLetter(50);
            setLetter(51);
            setLetter(52);
            break;

        case FrontWord::h_drei:
            setLetter(61);
            setLetter(60);
            setLetter(59);
            setLetter(58);
            break;

        case FrontWord::h_vier:
            setLetter(88);
            setLetter(89);
            setLetter(90);
            setLetter(91);
            break;

        case FrontWord::h_fuenf:
            setLetter(92);
            setLetter(93);
            setLetter(94);
            setLetter(95);
            break;

        case FrontWord::h_sechs:
            setLetter(57);
            setLetter(56);
            setLetter(55);
            break;

        case FrontWord::h_sieben:
            setLetter(66);
            setLetter(67);
            setLetter(68);
            setLetter(69);
            setLetter(70);
            break;

        case FrontWord::h_acht:
            setLetter(65);
            setLetter(64);
            setLetter(63);
            setLetter(62);
            break;

        case FrontWord::h_neun:
            setLetter(70);
            setLetter(71);
            setLetter(72);
            setLetter(73);
            setLetter(74);
            break;

        case FrontWord::h_zehn:
            setLetter(80);
            setLetter(79);
            setLetter(78);
            setLetter(77);
            break;

        case FrontWord::h_elf:
            setLetter(96);
            setLetter(97);
            setLetter(98);
            break;

        case FrontWord::h_zwoelf:
            setLetter(87);
            setLetter(86);
            setLetter(85);
            setLetter(84);
            setLetter(83);
            setLetter(82);
            break;

        case FrontWord::nur:
            // Nu
            setLetter(7);
            setLetter(8);
            break;

        case FrontWord::gewesen:
            // Geweest
            setLetter(105);
            setLetter(104);
            setLetter(103);
            setLetter(102);
            setLetter(101);
            setLetter(100);
            setLetter(99);
            break;

        default:
            break;
        };
    };
};

NL10x11_t _nl10x11;
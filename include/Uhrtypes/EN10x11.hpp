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

#define USE_DREIVIERTEL

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
            setLetter(0);
            setLetter(1);
            // is
            setLetter(3);
            setLetter(4);
            break;

        case FrontWord::h_ein:
            // One
            setLetter(65);
            setLetter(64);
            setLetter(63);
            break;

        case FrontWord::h_zwei:
            // Two
            setLetter(74);
            setLetter(75);
            setLetter(76);
            break;

        case FrontWord::h_drei:
            // Three
            setLetter(59);
            setLetter(58);
            setLetter(57);
            setLetter(56);
            setLetter(55);
            break;

        case FrontWord::h_vier:
            // Four
            setLetter(66);
            setLetter(67);
            setLetter(68);
            setLetter(69);
            break;

        case FrontWord::h_fuenf:
            // Five
            setLetter(70);
            setLetter(71);
            setLetter(72);
            setLetter(73);
            break;

        case FrontWord::h_sechs:
            // Six
            setLetter(62);
            setLetter(61);
            setLetter(60);
            break;

        case FrontWord::h_sieben:
            // Seven
            setLetter(88);
            setLetter(89);
            setLetter(90);
            setLetter(91);
            setLetter(92);
            break;

        case FrontWord::h_acht:
            // Eight
            setLetter(87);
            setLetter(86);
            setLetter(85);
            setLetter(84);
            setLetter(83);
            break;

        case FrontWord::h_neun:
            // Nine
            setLetter(51);
            setLetter(52);
            setLetter(53);
            setLetter(54);
            break;

        case FrontWord::h_zehn:
            // Ten
            setLetter(109);
            setLetter(108);
            setLetter(107);
            break;

        case FrontWord::h_elf:
            // Eleven
            setLetter(82);
            setLetter(81);
            setLetter(80);
            setLetter(79);
            setLetter(78);
            setLetter(77);
            break;

        case FrontWord::h_zwoelf:
            // Twelve
            setLetter(93);
            setLetter(94);
            setLetter(95);
            setLetter(96);
            setLetter(97);
            setLetter(98);
            break;

        case FrontWord::fuenf:
            // Five
            setLetter(28);
            setLetter(29);
            setLetter(30);
            setLetter(31);
            break;

        case FrontWord::zehn:
            // Ten
            setLetter(38);
            setLetter(37);
            setLetter(36);
            break;

        case FrontWord::a_quarter:
            // A Quater
            setLetter(21);
            setLetter(13);
            setLetter(14);
            setLetter(15);
            setLetter(16);
            setLetter(17);
            setLetter(18);
            setLetter(19);
            break;

        case FrontWord::viertel:
            // Quater
            setLetter(13);
            setLetter(14);
            setLetter(15);
            setLetter(16);
            setLetter(17);
            setLetter(18);
            setLetter(19);
            break;

        case FrontWord::zwanzig:
            // Twenty
            setLetter(22);
            setLetter(23);
            setLetter(24);
            setLetter(25);
            setLetter(26);
            setLetter(27);
            break;

        case FrontWord::twentyfive:
            // Twentyfive
            setLetter(22);
            setLetter(23);
            setLetter(24);
            setLetter(25);
            setLetter(26);
            setLetter(27);
            setLetter(28);
            setLetter(29);
            setLetter(30);
            setLetter(31);
            break;

        case FrontWord::halb:
            // Half
            setLetter(43);
            setLetter(42);
            setLetter(41);
            setLetter(40);
            break;

        case FrontWord::nach:
            // Past
            setLetter(44);
            setLetter(45);
            setLetter(46);
            setLetter(47);
            break;

        case FrontWord::vor:
            // To
            setLetter(34);
            setLetter(33);
            break;

        case FrontWord::uhr:
            // O'Clock
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

En10x11_t _en10x11;
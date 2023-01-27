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

class De11x11_t : public iUhrType {
public:
    virtual LanguageAbbreviation usedLang() override {
        return LanguageAbbreviation::DE;
    };

    //------------------------------------------------------------------------------

    const uint16_t minArr[2][4] = {
        {121, 122, 123, 124}, // LEDs for "Normal" minute display
        {110, 111, 112, 113}  // LEDs für "Row" type minute display
    };

    //------------------------------------------------------------------------------

    virtual const void getMinArr(uint16_t *returnArr, uint8_t col) {
        for (uint8_t i = 0; i < 4; i++) {
            returnArr[i] = minArr[col][i];
        }
    };

    //------------------------------------------------------------------------------

    virtual const uint16_t NUM_PIXELS() override { return 125; };

    //------------------------------------------------------------------------------

    virtual const uint16_t NUM_SMATRIX() override { return 125; };

    //------------------------------------------------------------------------------

    virtual const uint16_t ROWS_MATRIX() override { return 12; };

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

        case FrontWord::zwanzig:
            setLetter(11);
            setLetter(12);
            setLetter(13);
            setLetter(14);
            setLetter(15);
            setLetter(16);
            setLetter(17);
            break;

        case FrontWord::halb:
            setLetter(44);
            setLetter(45);
            setLetter(46);
            setLetter(47);
            break;

        case FrontWord::eins:
            setLetter(60);
            setLetter(61);
            setLetter(62);
            setLetter(63);
            break;

        case FrontWord::nach:
        case FrontWord::v_nach:
            setLetter(38);
            setLetter(39);
            setLetter(40);
            setLetter(41);
            break;

        case FrontWord::vor:
        case FrontWord::v_vor:
            setLetter(35);
            setLetter(36);
            setLetter(37);

        case FrontWord::uhr:
            setLetter(100);
            setLetter(101);
            setLetter(102);
            break;

        case FrontWord::h_ein:
            setLetter(61);
            setLetter(62);
            setLetter(63);
            break;

        case FrontWord::h_zwei:
            setLetter(62);
            setLetter(63);
            setLetter(64);
            setLetter(65);
            break;

        case FrontWord::h_drei:
            setLetter(67);
            setLetter(68);
            setLetter(69);
            setLetter(70);
            break;

        case FrontWord::h_vier:
            setLetter(77);
            setLetter(78);
            setLetter(79);
            setLetter(80);
            break;

        case FrontWord::h_fuenf:
            setLetter(73);
            setLetter(74);
            setLetter(75);
            setLetter(76);
            break;

        case FrontWord::h_sechs:
            setLetter(104);
            setLetter(105);
            setLetter(106);
            setLetter(107);
            setLetter(108);
            break;

        case FrontWord::h_sieben:
            setLetter(55);
            setLetter(56);
            setLetter(57);
            setLetter(58);
            setLetter(59);
            setLetter(60);
            break;

        case FrontWord::h_acht:
            setLetter(89);
            setLetter(90);
            setLetter(91);
            setLetter(92);
            break;

        case FrontWord::h_neun:
            setLetter(81);
            setLetter(82);
            setLetter(83);
            setLetter(84);
            break;

        case FrontWord::h_zehn:
            setLetter(93);
            setLetter(94);
            setLetter(95);
            setLetter(96);
            break;

        case FrontWord::h_elf:
            setLetter(85);
            setLetter(86);
            setLetter(87);
            break;

        case FrontWord::h_zwoelf:
            setLetter(49);
            setLetter(50);
            setLetter(51);
            setLetter(52);
            setLetter(53);
            break;

        default:
            break;
        };
    };
};

De11x11_t _de11x11;
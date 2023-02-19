#pragma once

#include "Uhrtype.hpp"

/*
 * Layout Front
 *
 * E S K I S T R F Ü N F
 * Z E H N Z W A N Z I G
 * D R E I V I E R T E L
 * T G N A C H V O R U M
 * H A L B G Z W Ö L F J
 * Z W E I N S I E B E N
 * K D R E I R H F Ü N F
 * E L F N E U N V I E R
 * N A C H T Z E H N B X
 * U S E C H S F U H R Y
 */

#define LED_LAYOUT_REVERSE 0

#define LED_MAP(x) (x)

class De10x11_t : public iUhrType {
public:
    virtual LanguageAbbreviation usedLang() override {
        return LanguageAbbreviation::DE;
    };

        //------------------------------------------------------------------------------

#if (LED_LAYOUT_REVERSE)

        /*
         * 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113,
         * 102, 101, 100,  99,  98,  97,  96,  95,  94,  93,  92,
         *  81,  82,  83,  84,  85,  86,  87,  88,  89,  90,  91,
         *  80,  79,  78,  77,  76,  75,  74,  73,  72,  71,  70,
         *  59,  60,  61,  62,  63,  64,  65,  66,  67,  68,  69,
         *  58,  57,  56,  55,  54,  53,  52,  51,  50,  49,  48,
         *  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,
         *  36,  35,  34,  33,  32,  31,  30,  29,  28,  27,  26,
         *  15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,
         *  14,  13,  12,  11,  10,   9,   8,   7,   6,   5,   4,
         *                      0, 1, 2, 3
         */
#undef LED_MAP
#define LED_MAP(x)                                                             \
    (x > 109 ? x - 110 : 113 - (((x / 11) * 11) + (10 - (x % 11))))

    //------------------------------------------------------------------------------

    virtual const uint16_t getFrontMatrix(uint8_t row, uint8_t col) override {
        if (row == rowsWordMatrix() - 1) {
            return col;
        }
        if (row % 2 == 0) {
            col = colsWordMatrix() - col - 1;
        }
        uint16_t returnValue =
            numPixels() - 1 - (col + (row * colsWordMatrix()));
        if (returnValue > numPixels()) {
            Serial.println("[ERROR] getMatrix() ReturnValue out of Bounds");
        }
        return returnValue;
    };

#endif

    //------------------------------------------------------------------------------

    uint16_t minArr[4] = {LED_MAP(110), LED_MAP(111), LED_MAP(112),
                          LED_MAP(113)};

    //------------------------------------------------------------------------------

    virtual const void getMinuteArray(uint16_t *returnArr, uint8_t col) {
        for (uint8_t i = 0; i < 4; i++) {
            returnArr[i] = minArr[i];
        }
    };

    //------------------------------------------------------------------------------

    virtual const bool hasDreiviertel() override { return true; }

    //------------------------------------------------------------------------------

    void show(FrontWord word) override {
        switch (word) {

        case FrontWord::es_ist:
            // Es
            setLetter(LED_MAP(0));
            setLetter(LED_MAP(1));

            // Ist
            setLetter(LED_MAP(3));
            setLetter(LED_MAP(4));
            setLetter(LED_MAP(5));
            break;

            //------------------------------------------------------------------------------

        case FrontWord::nach:
        case FrontWord::v_nach:
            setLetter(LED_MAP(38));
            setLetter(LED_MAP(39));
            setLetter(LED_MAP(40));
            setLetter(LED_MAP(41));
            break;

            //------------------------------------------------------------------------------

        case FrontWord::vor:
        case FrontWord::v_vor:
            setLetter(LED_MAP(35));
            setLetter(LED_MAP(36));
            setLetter(LED_MAP(37));
            break;
            //------------------------------------------------------------------------------

        case FrontWord::viertel:
            setLetter(LED_MAP(26));
            setLetter(LED_MAP(27));
            setLetter(LED_MAP(28));
            setLetter(LED_MAP(29));
            setLetter(LED_MAP(30));
            setLetter(LED_MAP(31));
            setLetter(LED_MAP(32));
            break;

            //------------------------------------------------------------------------------

        case FrontWord::dreiviertel:
            setLetter(LED_MAP(22));
            setLetter(LED_MAP(23));
            setLetter(LED_MAP(24));
            setLetter(LED_MAP(25));
            setLetter(LED_MAP(26));
            setLetter(LED_MAP(27));
            setLetter(LED_MAP(28));
            setLetter(LED_MAP(29));
            setLetter(LED_MAP(30));
            setLetter(LED_MAP(31));
            setLetter(LED_MAP(32));
            break;

            //------------------------------------------------------------------------------

        case FrontWord::fuenf:
            setLetter(LED_MAP(7));
            setLetter(LED_MAP(8));
            setLetter(LED_MAP(9));
            setLetter(LED_MAP(10));
            break;

            //------------------------------------------------------------------------------

        case FrontWord::zehn:
            setLetter(LED_MAP(18));
            setLetter(LED_MAP(19));
            setLetter(LED_MAP(20));
            setLetter(LED_MAP(21));
            break;

            //------------------------------------------------------------------------------
        case FrontWord::zwanzig:
            setLetter(LED_MAP(11));
            setLetter(LED_MAP(12));
            setLetter(LED_MAP(13));
            setLetter(LED_MAP(14));
            setLetter(LED_MAP(15));
            setLetter(LED_MAP(16));
            setLetter(LED_MAP(17));
            break;

            //------------------------------------------------------------------------------

        case FrontWord::halb:
            setLetter(LED_MAP(44));
            setLetter(LED_MAP(45));
            setLetter(LED_MAP(46));
            setLetter(LED_MAP(47));
            break;

            //------------------------------------------------------------------------------

        case FrontWord::eins:
            setLetter(LED_MAP(60));
            setLetter(LED_MAP(61));
            setLetter(LED_MAP(62));
            setLetter(LED_MAP(63));
            break;

            //------------------------------------------------------------------------------

        case FrontWord::uhr:
            setLetter(LED_MAP(100));
            setLetter(LED_MAP(101));
            setLetter(LED_MAP(102));
            break;

            //------------------------------------------------------------------------------

        case FrontWord::h_ein:
            setLetter(LED_MAP(61));
            setLetter(LED_MAP(62));
            setLetter(LED_MAP(63));
            break;

            //------------------------------------------------------------------------------

        case FrontWord::h_zwei:
            setLetter(LED_MAP(62));
            setLetter(LED_MAP(63));
            setLetter(LED_MAP(64));
            setLetter(LED_MAP(65));
            break;

            //------------------------------------------------------------------------------

        case FrontWord::h_drei:
            setLetter(LED_MAP(67));
            setLetter(LED_MAP(68));
            setLetter(LED_MAP(69));
            setLetter(LED_MAP(70));
            break;

            //------------------------------------------------------------------------------

        case FrontWord::h_vier:
            setLetter(LED_MAP(77));
            setLetter(LED_MAP(78));
            setLetter(LED_MAP(79));
            setLetter(LED_MAP(80));
            break;

            //------------------------------------------------------------------------------

        case FrontWord::h_fuenf:
            setLetter(LED_MAP(73));
            setLetter(LED_MAP(74));
            setLetter(LED_MAP(75));
            setLetter(LED_MAP(76));
            break;

            //------------------------------------------------------------------------------

        case FrontWord::h_sechs:
            setLetter(LED_MAP(104));
            setLetter(LED_MAP(105));
            setLetter(LED_MAP(106));
            setLetter(LED_MAP(107));
            setLetter(LED_MAP(108));
            break;

            //------------------------------------------------------------------------------

        case FrontWord::h_sieben:
            setLetter(LED_MAP(55));
            setLetter(LED_MAP(56));
            setLetter(LED_MAP(57));
            setLetter(LED_MAP(58));
            setLetter(LED_MAP(59));
            setLetter(LED_MAP(60));
            break;

            //------------------------------------------------------------------------------

        case FrontWord::h_acht:
            setLetter(LED_MAP(89));
            setLetter(LED_MAP(90));
            setLetter(LED_MAP(91));
            setLetter(LED_MAP(92));
            break;

            //------------------------------------------------------------------------------

        case FrontWord::h_neun:
            setLetter(LED_MAP(81));
            setLetter(LED_MAP(82));
            setLetter(LED_MAP(83));
            setLetter(LED_MAP(84));
            break;

            //------------------------------------------------------------------------------

        case FrontWord::h_zehn:
            setLetter(LED_MAP(93));
            setLetter(LED_MAP(94));
            setLetter(LED_MAP(95));
            setLetter(LED_MAP(96));
            break;

            //------------------------------------------------------------------------------

        case FrontWord::h_elf:
            setLetter(LED_MAP(85));
            setLetter(LED_MAP(86));
            setLetter(LED_MAP(87));
            break;

            //------------------------------------------------------------------------------

        case FrontWord::h_zwoelf:

            setLetter(LED_MAP(49));
            setLetter(LED_MAP(50));
            setLetter(LED_MAP(51));
            setLetter(LED_MAP(52));
            setLetter(LED_MAP(53));
            break;

        default:
            break;
            //------------------------------------------------------------------------------
        };
    };
};

De10x11_t _de10x11;
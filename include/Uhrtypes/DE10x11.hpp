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

    virtual const uint16_t getFrontMatrixIndex(uint8_t row,
                                               uint8_t col) override {
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
            setFrontMatrixPixel(LED_MAP(0));
            setFrontMatrixPixel(LED_MAP(1));

            // Ist
            setFrontMatrixPixel(LED_MAP(3));
            setFrontMatrixPixel(LED_MAP(4));
            setFrontMatrixPixel(LED_MAP(5));
            break;

            //------------------------------------------------------------------------------

        case FrontWord::nach:
        case FrontWord::v_nach:
            setFrontMatrixPixel(LED_MAP(38));
            setFrontMatrixPixel(LED_MAP(39));
            setFrontMatrixPixel(LED_MAP(40));
            setFrontMatrixPixel(LED_MAP(41));
            break;

            //------------------------------------------------------------------------------

        case FrontWord::vor:
        case FrontWord::v_vor:
            setFrontMatrixPixel(LED_MAP(35));
            setFrontMatrixPixel(LED_MAP(36));
            setFrontMatrixPixel(LED_MAP(37));
            break;
            //------------------------------------------------------------------------------

        case FrontWord::viertel:
            setFrontMatrixPixel(LED_MAP(26));
            setFrontMatrixPixel(LED_MAP(27));
            setFrontMatrixPixel(LED_MAP(28));
            setFrontMatrixPixel(LED_MAP(29));
            setFrontMatrixPixel(LED_MAP(30));
            setFrontMatrixPixel(LED_MAP(31));
            setFrontMatrixPixel(LED_MAP(32));
            break;

            //------------------------------------------------------------------------------

        case FrontWord::dreiviertel:
            setFrontMatrixPixel(LED_MAP(22));
            setFrontMatrixPixel(LED_MAP(23));
            setFrontMatrixPixel(LED_MAP(24));
            setFrontMatrixPixel(LED_MAP(25));
            setFrontMatrixPixel(LED_MAP(26));
            setFrontMatrixPixel(LED_MAP(27));
            setFrontMatrixPixel(LED_MAP(28));
            setFrontMatrixPixel(LED_MAP(29));
            setFrontMatrixPixel(LED_MAP(30));
            setFrontMatrixPixel(LED_MAP(31));
            setFrontMatrixPixel(LED_MAP(32));
            break;

            //------------------------------------------------------------------------------

        case FrontWord::fuenf:
            setFrontMatrixPixel(LED_MAP(7));
            setFrontMatrixPixel(LED_MAP(8));
            setFrontMatrixPixel(LED_MAP(9));
            setFrontMatrixPixel(LED_MAP(10));
            break;

            //------------------------------------------------------------------------------

        case FrontWord::zehn:
            setFrontMatrixPixel(LED_MAP(18));
            setFrontMatrixPixel(LED_MAP(19));
            setFrontMatrixPixel(LED_MAP(20));
            setFrontMatrixPixel(LED_MAP(21));
            break;

            //------------------------------------------------------------------------------
        case FrontWord::zwanzig:
            setFrontMatrixPixel(LED_MAP(11));
            setFrontMatrixPixel(LED_MAP(12));
            setFrontMatrixPixel(LED_MAP(13));
            setFrontMatrixPixel(LED_MAP(14));
            setFrontMatrixPixel(LED_MAP(15));
            setFrontMatrixPixel(LED_MAP(16));
            setFrontMatrixPixel(LED_MAP(17));
            break;

            //------------------------------------------------------------------------------

        case FrontWord::halb:
            setFrontMatrixPixel(LED_MAP(44));
            setFrontMatrixPixel(LED_MAP(45));
            setFrontMatrixPixel(LED_MAP(46));
            setFrontMatrixPixel(LED_MAP(47));
            break;

            //------------------------------------------------------------------------------

        case FrontWord::eins:
            setFrontMatrixPixel(LED_MAP(60));
            setFrontMatrixPixel(LED_MAP(61));
            setFrontMatrixPixel(LED_MAP(62));
            setFrontMatrixPixel(LED_MAP(63));
            break;

            //------------------------------------------------------------------------------

        case FrontWord::uhr:
            setFrontMatrixPixel(LED_MAP(100));
            setFrontMatrixPixel(LED_MAP(101));
            setFrontMatrixPixel(LED_MAP(102));
            break;

            //------------------------------------------------------------------------------

        case FrontWord::h_ein:
            setFrontMatrixPixel(LED_MAP(61));
            setFrontMatrixPixel(LED_MAP(62));
            setFrontMatrixPixel(LED_MAP(63));
            break;

            //------------------------------------------------------------------------------

        case FrontWord::h_zwei:
            setFrontMatrixPixel(LED_MAP(62));
            setFrontMatrixPixel(LED_MAP(63));
            setFrontMatrixPixel(LED_MAP(64));
            setFrontMatrixPixel(LED_MAP(65));
            break;

            //------------------------------------------------------------------------------

        case FrontWord::h_drei:
            setFrontMatrixPixel(LED_MAP(67));
            setFrontMatrixPixel(LED_MAP(68));
            setFrontMatrixPixel(LED_MAP(69));
            setFrontMatrixPixel(LED_MAP(70));
            break;

            //------------------------------------------------------------------------------

        case FrontWord::h_vier:
            setFrontMatrixPixel(LED_MAP(77));
            setFrontMatrixPixel(LED_MAP(78));
            setFrontMatrixPixel(LED_MAP(79));
            setFrontMatrixPixel(LED_MAP(80));
            break;

            //------------------------------------------------------------------------------

        case FrontWord::h_fuenf:
            setFrontMatrixPixel(LED_MAP(73));
            setFrontMatrixPixel(LED_MAP(74));
            setFrontMatrixPixel(LED_MAP(75));
            setFrontMatrixPixel(LED_MAP(76));
            break;

            //------------------------------------------------------------------------------

        case FrontWord::h_sechs:
            setFrontMatrixPixel(LED_MAP(104));
            setFrontMatrixPixel(LED_MAP(105));
            setFrontMatrixPixel(LED_MAP(106));
            setFrontMatrixPixel(LED_MAP(107));
            setFrontMatrixPixel(LED_MAP(108));
            break;

            //------------------------------------------------------------------------------

        case FrontWord::h_sieben:
            setFrontMatrixPixel(LED_MAP(55));
            setFrontMatrixPixel(LED_MAP(56));
            setFrontMatrixPixel(LED_MAP(57));
            setFrontMatrixPixel(LED_MAP(58));
            setFrontMatrixPixel(LED_MAP(59));
            setFrontMatrixPixel(LED_MAP(60));
            break;

            //------------------------------------------------------------------------------

        case FrontWord::h_acht:
            setFrontMatrixPixel(LED_MAP(89));
            setFrontMatrixPixel(LED_MAP(90));
            setFrontMatrixPixel(LED_MAP(91));
            setFrontMatrixPixel(LED_MAP(92));
            break;

            //------------------------------------------------------------------------------

        case FrontWord::h_neun:
            setFrontMatrixPixel(LED_MAP(81));
            setFrontMatrixPixel(LED_MAP(82));
            setFrontMatrixPixel(LED_MAP(83));
            setFrontMatrixPixel(LED_MAP(84));
            break;

            //------------------------------------------------------------------------------

        case FrontWord::h_zehn:
            setFrontMatrixPixel(LED_MAP(93));
            setFrontMatrixPixel(LED_MAP(94));
            setFrontMatrixPixel(LED_MAP(95));
            setFrontMatrixPixel(LED_MAP(96));
            break;

            //------------------------------------------------------------------------------

        case FrontWord::h_elf:
            setFrontMatrixPixel(LED_MAP(85));
            setFrontMatrixPixel(LED_MAP(86));
            setFrontMatrixPixel(LED_MAP(87));
            break;

            //------------------------------------------------------------------------------

        case FrontWord::h_zwoelf:

            setFrontMatrixPixel(LED_MAP(49));
            setFrontMatrixPixel(LED_MAP(50));
            setFrontMatrixPixel(LED_MAP(51));
            setFrontMatrixPixel(LED_MAP(52));
            setFrontMatrixPixel(LED_MAP(53));
            break;

        default:
            break;
            //------------------------------------------------------------------------------
        };
    };
};

De10x11_t _de10x11;
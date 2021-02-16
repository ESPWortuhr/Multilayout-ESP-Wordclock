#include "Uhrtype.hpp"

#define LED_LAYOUT_REVERSE 0

#define LED_MAP(x) (x)

class UHR_114_t : public iUhrType {
public:
#if (LED_LAYOUT_REVERSE)
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
     *
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
        if (row == ROWS_MATRIX() - 1) {
            return col;
        }
        if (row % 2 == 0) {
            col = COLS_MATRIX() - col - 1;
        }
        uint16_t returnValue = NUM_PIXELS() - 1 - (col + (row * COLS_MATRIX()));
        if (returnValue > NUM_PIXELS()) {
            Serial.println("[ERROR] getMatrix() ReturnValue out of Bounds");
        }
        return returnValue;
    };

#endif

    //------------------------------------------------------------------------------

    uint16_t min_arr[2][4] = {
        // ergänzt aus "Uhr func 169"-datei
        {LED_MAP(110), LED_MAP(111), LED_MAP(112),
         LED_MAP(113)}, // LED für Minuten Anzeige Zeile
        {LED_MAP(110), LED_MAP(111), LED_MAP(112),
         LED_MAP(113)} // LED für Minuten Anzeige Ecken
    };

    //------------------------------------------------------------------------------

    virtual const uint16_t getMinArr(uint8_t col, uint8_t row) override {
        return min_arr[col][row];
    };

    //------------------------------------------------------------------------------

    void show(uint8_t text) override {

        switch (text) {

        case es_ist:
            // Es
            Letter_set(LED_MAP(0));
            Letter_set(LED_MAP(1));

            // Ist
            Letter_set(LED_MAP(3));
            Letter_set(LED_MAP(4));
            Letter_set(LED_MAP(5));
            break;

            //------------------------------------------------------------------------------

        case nach:
            Letter_set(LED_MAP(38));
            Letter_set(LED_MAP(39));
            Letter_set(LED_MAP(40));
            Letter_set(LED_MAP(41));
            break;

            //------------------------------------------------------------------------------

        case vor:
            Letter_set(LED_MAP(35));
            Letter_set(LED_MAP(36));
            Letter_set(LED_MAP(37));
            break;
            //------------------------------------------------------------------------------

        case viertel:
            Letter_set(LED_MAP(26));
            Letter_set(LED_MAP(27));
            Letter_set(LED_MAP(28));
            Letter_set(LED_MAP(29));
            Letter_set(LED_MAP(30));
            Letter_set(LED_MAP(31));
            Letter_set(LED_MAP(32));
            break;

            //------------------------------------------------------------------------------

        case fuenf:
            Letter_set(LED_MAP(7));
            Letter_set(LED_MAP(8));
            Letter_set(LED_MAP(9));
            Letter_set(LED_MAP(10));
            break;

            //------------------------------------------------------------------------------

        case zehn:
            Letter_set(LED_MAP(18));
            Letter_set(LED_MAP(19));
            Letter_set(LED_MAP(20));
            Letter_set(LED_MAP(21));
            break;

            //------------------------------------------------------------------------------
        case zwanzig:
            Letter_set(LED_MAP(11));
            Letter_set(LED_MAP(12));
            Letter_set(LED_MAP(13));
            Letter_set(LED_MAP(14));
            Letter_set(LED_MAP(15));
            Letter_set(LED_MAP(16));
            Letter_set(LED_MAP(17));
            break;

            //------------------------------------------------------------------------------

        case halb:
            Letter_set(LED_MAP(44));
            Letter_set(LED_MAP(45));
            Letter_set(LED_MAP(46));
            Letter_set(LED_MAP(47));
            break;

            //------------------------------------------------------------------------------

        case eins:
            Letter_set(LED_MAP(60));
            Letter_set(LED_MAP(61));
            Letter_set(LED_MAP(62));
            Letter_set(LED_MAP(63));
            break;

            //------------------------------------------------------------------------------

        case uhr:
            Letter_set(LED_MAP(100));
            Letter_set(LED_MAP(101));
            Letter_set(LED_MAP(102));
            break;

            //------------------------------------------------------------------------------

        case h_ein:
            Letter_set(LED_MAP(61));
            Letter_set(LED_MAP(62));
            Letter_set(LED_MAP(63));
            break;

            //------------------------------------------------------------------------------

        case h_zwei:
            Letter_set(LED_MAP(62));
            Letter_set(LED_MAP(63));
            Letter_set(LED_MAP(64));
            Letter_set(LED_MAP(65));
            break;

            //------------------------------------------------------------------------------

        case h_drei:
            Letter_set(LED_MAP(67));
            Letter_set(LED_MAP(68));
            Letter_set(LED_MAP(69));
            Letter_set(LED_MAP(70));
            break;

            //------------------------------------------------------------------------------

        case h_vier:
            Letter_set(LED_MAP(77));
            Letter_set(LED_MAP(78));
            Letter_set(LED_MAP(79));
            Letter_set(LED_MAP(80));
            break;

            //------------------------------------------------------------------------------

        case h_fuenf:
            Letter_set(LED_MAP(73));
            Letter_set(LED_MAP(74));
            Letter_set(LED_MAP(75));
            Letter_set(LED_MAP(76));
            break;

            //------------------------------------------------------------------------------

        case h_sechs:
            Letter_set(LED_MAP(104));
            Letter_set(LED_MAP(105));
            Letter_set(LED_MAP(106));
            Letter_set(LED_MAP(107));
            Letter_set(LED_MAP(108));
            break;

            //------------------------------------------------------------------------------

        case h_sieben:
            Letter_set(LED_MAP(55));
            Letter_set(LED_MAP(56));
            Letter_set(LED_MAP(57));
            Letter_set(LED_MAP(58));
            Letter_set(LED_MAP(59));
            Letter_set(LED_MAP(60));
            break;

            //------------------------------------------------------------------------------

        case h_acht:
            Letter_set(LED_MAP(89));
            Letter_set(LED_MAP(90));
            Letter_set(LED_MAP(91));
            Letter_set(LED_MAP(92));
            break;

            //------------------------------------------------------------------------------

        case h_neun:
            Letter_set(LED_MAP(81));
            Letter_set(LED_MAP(82));
            Letter_set(LED_MAP(83));
            Letter_set(LED_MAP(84));
            break;

            //------------------------------------------------------------------------------

        case h_zehn:
            Letter_set(LED_MAP(93));
            Letter_set(LED_MAP(94));
            Letter_set(LED_MAP(95));
            Letter_set(LED_MAP(96));
            break;

            //------------------------------------------------------------------------------

        case h_elf:
            Letter_set(LED_MAP(85));
            Letter_set(LED_MAP(86));
            Letter_set(LED_MAP(87));
            break;

            //------------------------------------------------------------------------------

        case h_zwoelf:

            Letter_set(LED_MAP(49));
            Letter_set(LED_MAP(50));
            Letter_set(LED_MAP(51));
            Letter_set(LED_MAP(52));
            Letter_set(LED_MAP(53));
            break;

        default:
            break;
            //------------------------------------------------------------------------------
        };
    };
};
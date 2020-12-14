#include "Uhrtype.hpp"

class UHR_125_t : public iUhrType {
public:

		const uint8_t matrix[12][11] = {

            {0,   1,   2,   3,   4,   5,   6,   7,   8,   9,   10},
            {21,  20,  19,  18,  17,  16,  15,  14,  13,  12,  11},
            {22,  23,  24,  25,  26,  27,  28,  29,  30,  31,  32},
            {43,  42,  41,  40,  39,  38,  37,  36,  35,  34,  33},
            {44,  45,  46,  47,  48,  49,  50,  51,  52,  53,  54},
            {65,  64,  63,  62,  61,  60,  59,  58,  57,  56,  55},
            {66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76},
            {87,  86,  85,  84,  83,  82,  81,  80,  79,  78,  77},
            {88,  89,  90,  91,  92,  93,  94,  95,  96,  97,  98},
            {109, 108, 107, 106, 105, 104, 103, 102, 101, 100, 99},
            {110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120},
            {124, 123, 122, 121},};
//------------------------------------------------------------------------------

		const uint8_t smatrix[125] = {

            0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28,
            29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43,
            44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70,
            71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85,
            86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109,
            110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121,
            122, 123, 124};

//------------------------------------------------------------------------------

		const uint8_t min_arr[2][4] = {                       //ergänzt aus "Uhr func 169"-datei
            {110, 111, 112, 113},                           // LED für Minuten Anzeige Zeile
            {121, 122, 123, 124}                            // LED für Minuten Anzeige Ecken
    };

//------------------------------------------------------------------------------

    virtual const uint8_t getMinArr(uint8_t col, uint8_t row) override{
        return min_arr[col][row];
    };

//------------------------------------------------------------------------------

    virtual const uint8_t getMatrix(uint8_t col, uint8_t row) override{
        return matrix[col][row];
    };

//------------------------------------------------------------------------------

    virtual const uint8_t getRMatrix(uint8_t index) override{
        return 0;
    };

//------------------------------------------------------------------------------

    virtual const uint8_t getSMatrix(uint8_t index) override{
        return smatrix[index];
    };

//------------------------------------------------------------------------------

    virtual const uint8_t NUM_PIXELS() override {return 125;};

//------------------------------------------------------------------------------

    virtual const uint8_t NUM_SMATRIX() override {return 125;};

//------------------------------------------------------------------------------

    virtual const uint8_t ROWS_MATRIX() override {return 12;};

//------------------------------------------------------------------------------

    virtual const uint8_t NUM_RMATRIX() override {return 0;};

//------------------------------------------------------------------------------



    void show(uint8_t text) override {
        switch (text) {


            case es_ist:
                //Es
                Letter_set(0);
                Letter_set(1);

                //Ist
                Letter_set(3);
                Letter_set(4);
                Letter_set(5);
                break;

            case viertel:
                Letter_set(26);
                Letter_set(27);
                Letter_set(28);
                Letter_set(29);
                Letter_set(30);
                Letter_set(31);
                Letter_set(32);
                break;

            case fuenf:
                Letter_set(7);
                Letter_set(8);
                Letter_set(9);
                Letter_set(10);
                break;

            case zehn:
                Letter_set(18);
                Letter_set(19);
                Letter_set(20);
                Letter_set(21);
                break;
            case zwanzig:
                Letter_set(11);
                Letter_set(12);
                Letter_set(13);
                Letter_set(14);
                Letter_set(15);
                Letter_set(16);
                Letter_set(17);
                break;

            case halb:
                Letter_set(44);
                Letter_set(45);
                Letter_set(46);
                Letter_set(47);
                break;

            case eins:
                Letter_set(60);
                Letter_set(61);
                Letter_set(62);
                Letter_set(63);
                break;

            case nach:
                Letter_set(38);
                Letter_set(39);
                Letter_set(40);
                Letter_set(41);
                break;

            case vor:
                Letter_set(35);
                Letter_set(36);
                Letter_set(37);

            case uhr:
                Letter_set(100);
                Letter_set(101);
                Letter_set(102);
                break;

            case h_ein:
                Letter_set(61);
                Letter_set(62);
                Letter_set(63);
                break;

            case h_zwei:
                Letter_set(62);
                Letter_set(63);
                Letter_set(64);
                Letter_set(65);
                break;

            case h_drei:
                Letter_set(67);
                Letter_set(68);
                Letter_set(69);
                Letter_set(70);
                break;

            case h_vier:
                Letter_set(77);
                Letter_set(78);
                Letter_set(79);
                Letter_set(80);
                break;

            case h_fuenf:
                Letter_set(73);
                Letter_set(74);
                Letter_set(75);
                Letter_set(76);
                break;

            case h_sechs:
                Letter_set(104);
                Letter_set(105);
                Letter_set(106);
                Letter_set(107);
                Letter_set(108);
                break;

            case h_sieben:
                Letter_set(55);
                Letter_set(56);
                Letter_set(57);
                Letter_set(58);
                Letter_set(59);
                Letter_set(60);
                break;

            case h_acht:
                Letter_set(89);
                Letter_set(90);
                Letter_set(91);
                Letter_set(92);
                break;

            case h_neun:
                Letter_set(81);
                Letter_set(82);
                Letter_set(83);
                Letter_set(84);
                break;

            case h_zehn:
                Letter_set(93);
                Letter_set(94);
                Letter_set(95);
                Letter_set(96);
                break;

            case h_elf:
                Letter_set(85);
                Letter_set(86);
                Letter_set(87);
                break;

            case h_zwoelf:
                Letter_set(49);
                Letter_set(50);
                Letter_set(51);
                Letter_set(52);
                Letter_set(53);
                break;

            default:
                break;
        };
    };
};
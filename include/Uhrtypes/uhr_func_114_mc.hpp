#include "Uhrtype.hpp"


class UHR_114_mc_t : public iUhrType
{
public:
    const uint8_t matrix_top_left[11][11] = {
        //aus 10  11

        {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
        {21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11},
        {22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32},
        {43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33},
        {44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54},
        {65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55},
        {66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76},
        {87, 86, 85, 84, 83, 82, 81, 80, 79, 78, 77},
        {88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98},
        {109, 108, 107, 106, 105, 104, 103, 102, 101, 100, 99},
        {110, 111, 112, 113}, //ergänzt

    };

    const uint8_t matrix_button_right[11][11] = {
        {103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113},
        {102, 101, 100, 99, 98, 97, 96, 95, 94, 93, 92},
        {81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91},
        {80, 79, 78, 77, 76, 75, 74, 73, 72, 71, 70},
        {59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69},
        {58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48},
        {37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47},
        {36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26},
        {15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25},
        {14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4},
        {0, 1, 2, 3}};

    const uint8_t matrix_button_left[11][11] = {

        {109, 108, 107, 106, 105, 104, 103, 102, 101, 100, 99},
        {88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98},
        {87, 86, 85, 84, 83, 82, 81, 80, 79, 78, 77},
        {66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76},
        {65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55},
        {44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54},
        {43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33},
        {22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32},
        {21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11},
        {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
        {110, 111, 112, 113}, //ergänzt

    };

    const char front_layout_mc[11][11] = {
        {'E', 'S', 'K', 'I', 'S', 'T', 'L', 'F', 'U', 'N', 'F'},
        {'Z', 'E', 'H', 'N', 'Z', 'W', 'A', 'N', 'Z', 'I', 'G'},
        {'D', 'R', 'E', 'I', 'V', 'I', 'E', 'R', 'T', 'E', 'L'},
        {'T', 'G', 'N', 'A', 'C', 'H', 'V', 'O', 'R', 'J', 'M'},
        {'H', 'A', 'L', 'B', 'Q', 'Z', 'W', 'O', 'L', 'F', 'P'},
        {'Z', 'W', 'E', 'I', 'N', 'S', 'I', 'E', 'B', 'E', 'N'},
        {'K', 'D', 'R', 'E', 'I', 'R', 'H', 'F', 'U', 'N', 'F'},
        {'E', 'L', 'F', 'N', 'E', 'U', 'N', 'V', 'I', 'E', 'R'},
        {'W', 'A', 'C', 'H', 'T', 'Z', 'E', 'H', 'N', 'R', 'S'},
        {'B', 'S', 'E', 'C', 'H', 'S', 'F', 'M', 'U', 'H', 'R'},
        {'#', '#', '#', '#'},
    };

    //TODO das hier müsste getrennt werden und durch das Webfrontent konfiguriert werden
    const uint8_t *marix = &matrix_button_left[0][0];
    const char *charmarix = &front_layout_mc[0][0];
    //Eine LED zum level schiften vor der Matrix
    const uint8_t leds_bevor_matrix = 1;


    uint8_t min_arr[2][4] = {
        //ergänzt aus "Uhr func 169"-datei
        {*(marix + (10 * 11 + 0)), *(marix + (10 * 11 + 1)), *(marix + (10 * 11 + 2)), *(marix + (10 * 11 + 3))},
        {*(marix + (10 * 11 + 0)), *(marix + (10 * 11 + 1)), *(marix + (10 * 11 + 2)), *(marix + (10 * 11 + 3))},
    };

    //------------------------------------------------------------------------------

    virtual const uint16_t getMinArr(uint8_t col, uint8_t row) override
    {
        return min_arr[col][row] + leds_bevor_matrix;
    };

    //------------------------------------------------------------------------------

    virtual const uint16_t getSMatrix(uint16_t index) override
    {
        return (index + leds_bevor_matrix);
    };
    //------------------------------------------------------------------------------

    virtual const char getCharFrontplate(uint8_t row, uint8_t col) override
    {
        return *(charmarix + (row * 11 + col));
    };

    virtual const uint16_t getFrontMatrix(uint8_t row, uint8_t col) override
    {
        return *(marix + (row * 11 + col)) + leds_bevor_matrix;
    };


    //------------------------------------------------------------------------------

    virtual const uint16_t NUM_PIXELS() override { return 114 + leds_bevor_matrix; };

    //------------------------------------------------------------------------------

    virtual const uint16_t NUM_SMATRIX() override { return 114 + leds_bevor_matrix; };

    //------------------------------------------------------------------------------

    // virtual const uint16_t ROWS_MATRIX() override { return 11; };

    //------------------------------------------------------------------------------

    //virtual const uint16_t NUM_RMATRIX() override { return 0; };

    //------------------------------------------------------------------------------

    virtual const bool hasDreiviertel() override { return true; };

    //------------------------------------------------------------------------------
    virtual void Letter_set_Matrix(uint8_t col, uint8_t row) { 
       Letter_set(getFrontMatrix(row, col));         
    };

    //------------------------------------------------------------------------------
    //   0 1 2 3 4 5 6 7 8 9 10
    // 0 E S K I S T L F Ü N F    ==> ES IST FÜNF
    // 1 Z E H N Z W A N Z I G    ==> ZEHN ZWANZIG
    // 2 D R E I V I E R T E L    ==> DREI|VIERTEL
    // 3 T G N A C H V O R J M    ==> NACH VOR
    // 4 H A L B Q Z W Ö L F P    ==> HALB ZWÖLF
    // 5 Z W E I N S I E B E N    ==> ZW|EI|N|S|IEBEN
    // 6 K D R E I R H F Ü N F    ==> DREI FÜNF
    // 7 E L F N E U N V I E R    ==> ELF NEUN VIER
    // 8 W A C H T Z E H N R S    ==> ACHT ZEHN
    // 9 B S E C H S F M U H R    ==> SECHS UHR


/* TODO durch ein array ersetzen

*/
    void show(uint8_t text) override
    {

        switch (text)
        {

        case es_ist:
            //Es
            Letter_set_Matrix(0, 0);
            Letter_set_Matrix(1, 0);
            //ist
            Letter_set_Matrix(3,0);
            Letter_set_Matrix(4,0);
            Letter_set_Matrix(5,0);
            break;

            //------------------------------------------------------------------------------

        case nach:
        case v_nach:
            Letter_set_Matrix(2,3);
            Letter_set_Matrix(3,3);
            Letter_set_Matrix(4,3);
            Letter_set_Matrix(5,3);
            break;

            //------------------------------------------------------------------------------

        case vor:
        case v_vor:
            Letter_set_Matrix(6,3);
            Letter_set_Matrix(7,3);
            Letter_set_Matrix(8,3);
            break;
            //------------------------------------------------------------------------------

        case viertel:
            Letter_set_Matrix(4,2);
            Letter_set_Matrix(5,2);
            Letter_set_Matrix(6,2);
            Letter_set_Matrix(7,2);
            Letter_set_Matrix(8,2);
            Letter_set_Matrix(9,2);
            Letter_set_Matrix(10,2);
            break;

        case dreiviertel:
            Letter_set_Matrix(0,2);
            Letter_set_Matrix(1,2);
            Letter_set_Matrix(2,2);
            Letter_set_Matrix(3,2);
            Letter_set_Matrix(4,2);
            Letter_set_Matrix(5,2);
            Letter_set_Matrix(6,2);
            Letter_set_Matrix(7,2);
            Letter_set_Matrix(8,2);
            Letter_set_Matrix(9,2);
            Letter_set_Matrix(10,2);
            break;

            //------------------------------------------------------------------------------

        case fuenf:
            Letter_set_Matrix(7,0);
            Letter_set_Matrix(8,0);
            Letter_set_Matrix(9,0);
            Letter_set_Matrix(10,0);
            break;

            //------------------------------------------------------------------------------

        case zehn:
            Letter_set_Matrix(0,1);
            Letter_set_Matrix(1,1);
            Letter_set_Matrix(2,1);
            Letter_set_Matrix(3,1);
            break;

            //------------------------------------------------------------------------------
        case zwanzig:
            Letter_set_Matrix(4,1);
            Letter_set_Matrix(5,1);
            Letter_set_Matrix(6,1);
            Letter_set_Matrix(7,1);
            Letter_set_Matrix(8,1);
            Letter_set_Matrix(9,1);
            Letter_set_Matrix(10,1);
            break;

            //------------------------------------------------------------------------------

        case halb:
            Letter_set_Matrix(0,4);
            Letter_set_Matrix(1,4);
            Letter_set_Matrix(2,4);
            Letter_set_Matrix(3,4);
            break;

            //------------------------------------------------------------------------------

        case eins:
            Letter_set_Matrix(2,5);
            Letter_set_Matrix(3,5);
            Letter_set_Matrix(4,5);
            Letter_set_Matrix(5,5);
            break;

            //------------------------------------------------------------------------------

        case uhr:
            Letter_set_Matrix(10,9);
            Letter_set_Matrix(9,9);
            Letter_set_Matrix(8,9);
            break;

            //------------------------------------------------------------------------------

        case h_ein:
            Letter_set_Matrix(2,5);
            Letter_set_Matrix(3,5);
            Letter_set_Matrix(4,5);
            break;

            //------------------------------------------------------------------------------

        case h_zwei:
            Letter_set_Matrix(0,5);
            Letter_set_Matrix(1,5);
            Letter_set_Matrix(2,5);
            Letter_set_Matrix(3,5);
            break;

            //------------------------------------------------------------------------------

        case h_drei:
            Letter_set_Matrix(1,6);
            Letter_set_Matrix(2,6);
            Letter_set_Matrix(3,6);
            Letter_set_Matrix(4,6);
            break;

            //------------------------------------------------------------------------------

        case h_vier:
            Letter_set_Matrix(7,7);
            Letter_set_Matrix(8,7);
            Letter_set_Matrix(9,7);
            Letter_set_Matrix(10,7);
            break;

            //------------------------------------------------------------------------------

        case h_fuenf:
            //Letter_set_Matrix(6, 6);
            Letter_set_Matrix(7,6);
            Letter_set_Matrix(8,6);
            Letter_set_Matrix(9,6);
            Letter_set_Matrix(10,6);
            break;

            //------------------------------------------------------------------------------

        case h_sechs:
            Letter_set_Matrix(1,9);
            Letter_set_Matrix(2,9);
            Letter_set_Matrix(3,9);
            Letter_set_Matrix(4,9);
            Letter_set_Matrix(5,9);
            break;

            //------------------------------------------------------------------------------

        case h_sieben:
            Letter_set_Matrix(5,5);
            Letter_set_Matrix(6,5);
            Letter_set_Matrix(7,5);
            Letter_set_Matrix(8,5);
            Letter_set_Matrix(9,5);
            Letter_set_Matrix(10,5);
            break;

            //------------------------------------------------------------------------------

        case h_acht:
            Letter_set_Matrix(1,8);
            Letter_set_Matrix(2,8);
            Letter_set_Matrix(3,8);
            Letter_set_Matrix(4,8);
            break;

            //------------------------------------------------------------------------------

        case h_neun:
            Letter_set_Matrix(3,7);
            Letter_set_Matrix(4,7);
            Letter_set_Matrix(5,7);
            Letter_set_Matrix(6,7);
            break;

            //------------------------------------------------------------------------------

        case h_zehn:
            Letter_set_Matrix(5,8);
            Letter_set_Matrix(6,8);
            Letter_set_Matrix(7,8);
            Letter_set_Matrix(8,8);
            break;

            //------------------------------------------------------------------------------

        case h_elf:
            Letter_set_Matrix(0,7);
            Letter_set_Matrix(1,7);
            Letter_set_Matrix(2,7);
            break;

            //------------------------------------------------------------------------------

        case h_zwoelf:
            Letter_set_Matrix(5,4);
            Letter_set_Matrix(6,4);
            Letter_set_Matrix(7,4);
            Letter_set_Matrix(8,4);
            Letter_set_Matrix(9,4);
            break;

        default:
            break;
            //------------------------------------------------------------------------------
        };
    };
};


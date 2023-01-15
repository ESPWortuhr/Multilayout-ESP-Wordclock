#include "Uhrtype.hpp"

/*
 * Layout Front
 *
 * H E T R I S L V I J F
 * T I E N Z J A R U A G
 * D R I E S K W A R T N
 * F T O V E R V O O R M
 * H A L F X T W A A L F
 * T W E E N Z E V E N P
 * L D R I E B Y V I J F
 * E L F A C H T V I E R
 * N E G E N P M T I E N
 * O S Z E S T L P U U R
 *
 */

class UHR_114_dutch_t : public iUhrType {
public:
    const uint16_t min_arr[2][4] = {
        // ergänzt aus "Uhr func 169"-datei
        {110, 111, 112, 113}, // LED für Minuten Anzeige Zeile
        {110, 111, 112, 113}  // LED für Minuten Anzeige Ecken
    };

    //------------------------------------------------------------------------------

    virtual const uint16_t getMinArr(uint8_t col, uint8_t row) override {
        return min_arr[col][row];
    };

    //------------------------------------------------------------------------------

    virtual const bool hasZwanzig() override { return false; }

    //------------------------------------------------------------------------------

    void show(uint8_t text) override {
        switch (text) {

        case es_ist:
            // Es
            Letter_set(0);
            Letter_set(1);
            Letter_set(2);

            // Ist
            Letter_set(4);
            Letter_set(5);
            break;

        case nach:
        case v_nach:
            Letter_set(41);
            Letter_set(40);
            Letter_set(39);
            Letter_set(38);
            break;

        case vor:
        case v_vor:
            Letter_set(37);
            Letter_set(36);
            Letter_set(35);
            Letter_set(34);
            break;

        case viertel:
            Letter_set(27);
            Letter_set(28);
            Letter_set(29);
            Letter_set(30);
            break;

        case fuenf:
            Letter_set(7);
            Letter_set(8);
            Letter_set(9);
            Letter_set(10);
            break;

        case zehn:
            Letter_set(21);
            Letter_set(20);
            Letter_set(19);
            Letter_set(18);
            break;

        case halb:
            Letter_set(44);
            Letter_set(45);
            Letter_set(46);
            Letter_set(47);
            break;

        case h_ein:
            Letter_set(63);
            Letter_set(62);
            Letter_set(61);
            break;

        case uhr:
            Letter_set(101);
            Letter_set(100);
            Letter_set(99);
            break;

        case h_zwei:
            Letter_set(65);
            Letter_set(64);
            Letter_set(63);
            Letter_set(62);
            break;

        case h_drei:
            Letter_set(67);
            Letter_set(68);
            Letter_set(69);
            Letter_set(70);
            break;

        case h_vier:
            Letter_set(80);
            Letter_set(79);
            Letter_set(78);
            Letter_set(77);
            break;

        case h_fuenf:
            Letter_set(73);
            Letter_set(74);
            Letter_set(75);
            Letter_set(76);
            break;

        case h_sechs:
            Letter_set(107);
            Letter_set(106);
            Letter_set(105);
            break;

        case h_sieben:
            Letter_set(60);
            Letter_set(59);
            Letter_set(58);
            Letter_set(57);
            Letter_set(56);
            break;

        case h_acht:
            Letter_set(84);
            Letter_set(83);
            Letter_set(82);
            Letter_set(81);
            break;

        case h_neun:
            Letter_set(88);
            Letter_set(89);
            Letter_set(90);
            Letter_set(91);
            Letter_set(92);
            break;

        case h_zehn:
            Letter_set(95);
            Letter_set(96);
            Letter_set(97);
            Letter_set(98);
            break;

        case h_elf:
            Letter_set(87);
            Letter_set(86);
            Letter_set(85);
            break;

        case h_zwoelf:
            Letter_set(49);
            Letter_set(50);
            Letter_set(51);
            Letter_set(52);
            Letter_set(53);
            Letter_set(54);
            break;

        default:
            break;
        };
    };
};

UHR_114_dutch_t Uhr_114_dutch_type;
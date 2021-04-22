#include "Uhrtype.hpp"

class UHR_242_t : public iUhrType {
public:
    const uint16_t min_arr[2][4] = {
        {112, 114, 116, 118}, // Minuten LED´s für Zeile
        {112, 114, 116, 118}  // Minuten LED´s für Ecken
    };

    //------------------------------------------------------------------------------

    virtual const uint16_t getMinArr(uint8_t col, uint8_t row) override {
        return min_arr[col][row];
    };

    //------------------------------------------------------------------------------

    virtual const uint16_t NUM_PIXELS() override { return 242; };

    //------------------------------------------------------------------------------

    virtual const uint16_t NUM_SMATRIX() override { return 242; };

    //------------------------------------------------------------------------------

    virtual const uint16_t ROWS_MATRIX() override { return 22; };

    //------------------------------------------------------------------------------

    virtual const bool hasWeatherLayout() override { return true; }

    //------------------------------------------------------------------------------

    void show(uint8_t text) override {
        switch (text) {
        case es_ist:
            // Es
            Letter_set(0);
            Letter_set(1);

            // Ist
            Letter_set(3);
            Letter_set(4);
            Letter_set(5);

#ifdef VERBOSE
            Serial.println("");
            Serial.print("Es ist ");
#endif
            break;

        case viertel:
            Letter_set(26);
            Letter_set(27);
            Letter_set(28);
            Letter_set(29);
            Letter_set(30);
            Letter_set(31);
            Letter_set(32);

#ifdef VERBOSE
            Serial.print("viertel ");
#endif
            break;

        case fuenf:
            Letter_set(7);
            Letter_set(8);
            Letter_set(9);
            Letter_set(10);

#ifdef VERBOSE
            Serial.print("Fünf ");
#endif
            break;

        case zehn:
            Letter_set(18);
            Letter_set(19);
            Letter_set(20);
            Letter_set(21);

#ifdef VERBOSE
            Serial.print("zehn ");
#endif
            break;

        case zwanzig:
            Letter_set(11);
            Letter_set(12);
            Letter_set(13);
            Letter_set(14);
            Letter_set(15);
            Letter_set(16);
            Letter_set(17);

#ifdef VERBOSE
            Serial.print("zwanzig ");
#endif
            break;

        case halb:
            Letter_set(44);
            Letter_set(45);
            Letter_set(46);
            Letter_set(47);

#ifdef VERBOSE
            Serial.print("halb ");
#endif
            break;

        case eins:
            Letter_set(60);
            Letter_set(61);
            Letter_set(62);
            Letter_set(63);

#ifdef VERBOSE
            Serial.print("Eins ");
#endif
            break;

        case nach:
        case v_nach:
            Letter_set(38);
            Letter_set(39);
            Letter_set(40);
            Letter_set(41);

#ifdef VERBOSE
            Serial.print("nach ");
#endif
            break;

        case vor:
        case v_vor:
            Letter_set(35);
            Letter_set(36);
            Letter_set(37);

#ifdef VERBOSE
            Serial.print("vor ");
#endif

        case uhr:
            Letter_set(100);
            Letter_set(101);
            Letter_set(102);

#ifdef VERBOSE
            Serial.println("Uhr ");
#endif
            break;

        case h_ein:
            Letter_set(61);
            Letter_set(62);
            Letter_set(63);

#ifdef VERBOSE
            Serial.println("Eins ");
#endif
            break;

        case h_zwei:
            Letter_set(62);
            Letter_set(63);
            Letter_set(64);
            Letter_set(65);

#ifdef VERBOSE
            Serial.println("Zwei ");
#endif
            break;

        case h_drei:
            Letter_set(67);
            Letter_set(68);
            Letter_set(69);
            Letter_set(70);

#ifdef VERBOSE
            Serial.println("Drei ");
#endif
            break;

        case h_vier:
            Letter_set(77);
            Letter_set(78);
            Letter_set(79);
            Letter_set(80);

#ifdef VERBOSE
            Serial.println("Vier ");
#endif
            break;

        case h_fuenf:
            Letter_set(73);
            Letter_set(74);
            Letter_set(75);
            Letter_set(76);

#ifdef VERBOSE
            Serial.println("Fünf ");
#endif
            break;

        case h_sechs:
            Letter_set(104);
            Letter_set(105);
            Letter_set(106);
            Letter_set(107);
            Letter_set(108);

#ifdef VERBOSE
            Serial.println("Sechs ");
#endif
            break;

        case h_sieben:
            Letter_set(55);
            Letter_set(56);
            Letter_set(57);
            Letter_set(58);
            Letter_set(59);
            Letter_set(60);

#ifdef VERBOSE
            Serial.println("Sieben ");
#endif
            break;

        case h_acht:
            Letter_set(89);
            Letter_set(90);
            Letter_set(91);
            Letter_set(92);

#ifdef VERBOSE
            Serial.println("Acht ");
#endif
            break;

        case h_neun:
            Letter_set(81);
            Letter_set(82);
            Letter_set(83);
            Letter_set(84);

#ifdef VERBOSE
            Serial.println("Neun ");
#endif
            break;

        case h_zehn:
            Letter_set(93);
            Letter_set(94);
            Letter_set(95);
            Letter_set(96);

#ifdef VERBOSE
            Serial.println("Zehn ");
#endif
            break;

        case h_elf:
            Letter_set(85);
            Letter_set(86);
            Letter_set(87);

#ifdef VERBOSE
            Serial.println("Elf ");
#endif
            break;

        case h_zwoelf:
            Letter_set(49);
            Letter_set(50);
            Letter_set(51);
            Letter_set(52);
            Letter_set(53);

#ifdef VERBOSE
            Serial.println("Zwölf ");
#endif
            break;

            //------------------------------------------------------------------------------
            // Wetter
            //------------------------------------------------------------------------------

        case w_morgen:
            Letter_set(131);
            Letter_set(130);
            Letter_set(129);
            Letter_set(128);
            Letter_set(127);
            Letter_set(126);

#ifdef VERBOSE
            Serial.print("Morgen ");
#endif
            break;

        case w_frueh:
            Letter_set(124);
            Letter_set(123);
            Letter_set(122);
            Letter_set(121);

#ifdef VERBOSE
            Serial.print("Früh ");
#endif
            break;

        case w_abend:
            Letter_set(132);
            Letter_set(133);
            Letter_set(134);
            Letter_set(135);
            Letter_set(136);

#ifdef VERBOSE
            Serial.print("Abend ");
#endif
            break;

        case w_mittag:
            Letter_set(137);
            Letter_set(138);
            Letter_set(139);
            Letter_set(140);
            Letter_set(141);
            Letter_set(142);

#ifdef VERBOSE
            Serial.print("Mittag ");
#endif
            break;

        case w_nacht:
            Letter_set(153);
            Letter_set(152);
            Letter_set(151);
            Letter_set(150);
            Letter_set(149);

#ifdef VERBOSE
            Serial.print("Nacht ");
#endif
            break;

        case w_schnee:
            Letter_set(148);
            Letter_set(147);
            Letter_set(146);
            Letter_set(145);
            Letter_set(144);
            Letter_set(143);

#ifdef VERBOSE
            Serial.print("Schnee ");
#endif
            break;

        case w_klar:
            Letter_set(154);
            Letter_set(155);
            Letter_set(156);
            Letter_set(157);

#ifdef VERBOSE
            Serial.print("klar ");
#endif
            break;

        case w_warnung:
            Letter_set(158);
            Letter_set(159);
            Letter_set(160);
            Letter_set(161);
            Letter_set(162);
            Letter_set(163);
            Letter_set(164);

#ifdef VERBOSE
            Serial.print("Warnung ");
#endif
            break;

        case w_regen:
            Letter_set(175);
            Letter_set(174);
            Letter_set(173);
            Letter_set(172);
            Letter_set(171);

#ifdef VERBOSE
            Serial.print("Regen ");
#endif
            break;

        case w_wolken:
            Letter_set(170);
            Letter_set(169);
            Letter_set(168);
            Letter_set(167);
            Letter_set(166);
            Letter_set(165);

#ifdef VERBOSE
            Serial.print("Wolken ");
#endif
            break;

        case w_gewitter:
            Letter_set(177);
            Letter_set(178);
            Letter_set(179);
            Letter_set(180);
            Letter_set(181);
            Letter_set(182);
            Letter_set(183);
            Letter_set(184);

#ifdef VERBOSE
            Serial.print("Gewitter ");
#endif
            break;

        case w_unter:
            Letter_set(197);
            Letter_set(196);
            Letter_set(195);
            Letter_set(194);
            Letter_set(193);

#ifdef VERBOSE
            Serial.print("unter ");
#endif
            break;

        case w_ueber:
            Letter_set(191);
            Letter_set(190);
            Letter_set(189);
            Letter_set(188);

#ifdef VERBOSE
            Serial.print("über ");
#endif
            break;

        case w_minus:
            Letter_set(199);
            Letter_set(200);
            Letter_set(201);
            Letter_set(202);
            Letter_set(203);

#ifdef VERBOSE
            Serial.print("minus ");
#endif
            break;

        case w_null:
            Letter_set(205);
            Letter_set(206);
            Letter_set(207);
            Letter_set(208);

#ifdef VERBOSE
            Serial.print("Null ");
#endif
            break;

        case w_fuenf:
            Letter_set(219);
            Letter_set(218);
            Letter_set(217);
            Letter_set(216);

#ifdef VERBOSE
            Serial.print("Fünf ");
#endif
            break;

        case w_zehn:
            Letter_set(215);
            Letter_set(214);
            Letter_set(213);
            Letter_set(212);

#ifdef VERBOSE
            Serial.print("Zehn ");
#endif
            break;

        case w_und:
            Letter_set(211);
            Letter_set(210);
            Letter_set(209);

#ifdef VERBOSE
            Serial.print("und ");
#endif
            break;

        case w_zwanzig:
            Letter_set(222);
            Letter_set(223);
            Letter_set(224);
            Letter_set(225);
            Letter_set(226);
            Letter_set(227);
            Letter_set(228);

#ifdef VERBOSE
            Serial.print("Zwanzig ");
#endif
            break;

        case w_dreissig:
            Letter_set(241);
            Letter_set(240);
            Letter_set(239);
            Letter_set(238);
            Letter_set(237);
            Letter_set(236);
            Letter_set(235);

#ifdef VERBOSE
            Serial.print("Dreißig ");
#endif
            break;

        case w_grad:
            Letter_set(233);
            Letter_set(232);

#ifdef VERBOSE
            Serial.println("°C ");
#endif
            break;

        default:
            break;
        }
    };
};

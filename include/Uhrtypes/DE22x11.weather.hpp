#pragma once

#include "Uhrtype.hpp"

/*
 * Layout Front
 *
 * E S K I S T L F Ü N F
 * Z E H N Z W A N Z I G
 * D R E I V I E R T E L
 * T G N A C H V O R J M
 * H A L B Q Z W Ö L F P
 * Z W E I N S I E B E N
 * K D R E I R H F Ü N F
 * E L F N E U N V I E R
 * W A C H T Z E H N B X
 * B S E C H S F U H R M
 *
 *
 *
 * M O R G E N X F R Ü H
 * A B E N D M I T T A G
 * N A C H T S C H N E E
 * K L A R W A R N U N G
 * R E G E N W O L K E N
 * O G E W I T T E R B N
 * U N T E R Z Ü B E R K
 * Y M I N U S A N U L L
 * H N Z W A N Z I G J T
 * D R E I ẞ I G O ° C X
 */

class De22x11Weather_t : public iUhrType {
public:
    virtual LanguageAbbreviation usedLang() override {
        return LanguageAbbreviation::DE;
    };

    //------------------------------------------------------------------------------

    virtual const void getMinuteArray(uint16_t *returnArr, uint8_t col) {
        for (uint8_t i = 0; i < 4; i++) {
            switch (col) {
            case 0:
                // LEDs for "LED4x" minute display
                // {113, 114, 115, 116}
                returnArr[i] = 113 + i;
                break;
            case 1:
                // LEDs for "LED7x" minute display
                // {112, 114, 116, 118}
                returnArr[i] = 112 + (i * 2);
                break;

            default:
                break;
            }
        }
    };

    //------------------------------------------------------------------------------

    virtual const uint8_t rowsWordMatrix() override { return 22; };

    //------------------------------------------------------------------------------

    virtual const bool hasWeatherLayout() override { return true; }

    //------------------------------------------------------------------------------

    void show(FrontWord word) override {
        switch (word) {

        case FrontWord::es_ist:
            // Es
            setFrontMatrixPixel(0);
            setFrontMatrixPixel(1);

            // Ist
            setFrontMatrixPixel(3);
            setFrontMatrixPixel(4);
            setFrontMatrixPixel(5);

#if WEATHER_VERBOSE
            Serial.println("");
            Serial.print("Es ist ");
#endif
            break;

        case FrontWord::viertel:
            setFrontMatrixPixel(26);
            setFrontMatrixPixel(27);
            setFrontMatrixPixel(28);
            setFrontMatrixPixel(29);
            setFrontMatrixPixel(30);
            setFrontMatrixPixel(31);
            setFrontMatrixPixel(32);

#if WEATHER_VERBOSE
            Serial.print("viertel ");
#endif
            break;

        case FrontWord::fuenf:
            setFrontMatrixPixel(7);
            setFrontMatrixPixel(8);
            setFrontMatrixPixel(9);
            setFrontMatrixPixel(10);

#if WEATHER_VERBOSE
            Serial.print("Fünf ");
#endif
            break;

        case FrontWord::zehn:
            setFrontMatrixPixel(18);
            setFrontMatrixPixel(19);
            setFrontMatrixPixel(20);
            setFrontMatrixPixel(21);

#if WEATHER_VERBOSE
            Serial.print("zehn ");
#endif
            break;

        case FrontWord::zwanzig:
            setFrontMatrixPixel(11);
            setFrontMatrixPixel(12);
            setFrontMatrixPixel(13);
            setFrontMatrixPixel(14);
            setFrontMatrixPixel(15);
            setFrontMatrixPixel(16);
            setFrontMatrixPixel(17);

#if WEATHER_VERBOSE
            Serial.print("zwanzig ");
#endif
            break;

        case FrontWord::halb:
            setFrontMatrixPixel(44);
            setFrontMatrixPixel(45);
            setFrontMatrixPixel(46);
            setFrontMatrixPixel(47);

#if WEATHER_VERBOSE
            Serial.print("halb ");
#endif
            break;

        case FrontWord::eins:
            setFrontMatrixPixel(60);
            setFrontMatrixPixel(61);
            setFrontMatrixPixel(62);
            setFrontMatrixPixel(63);

#if WEATHER_VERBOSE
            Serial.print("Eins ");
#endif
            break;

        case FrontWord::nach:
        case FrontWord::v_nach:
            setFrontMatrixPixel(38);
            setFrontMatrixPixel(39);
            setFrontMatrixPixel(40);
            setFrontMatrixPixel(41);

#if WEATHER_VERBOSE
            Serial.print("nach ");
#endif
            break;

        case FrontWord::vor:
        case FrontWord::v_vor:
            setFrontMatrixPixel(35);
            setFrontMatrixPixel(36);
            setFrontMatrixPixel(37);

#if WEATHER_VERBOSE
            Serial.print("vor ");
#endif

        case FrontWord::uhr:
            setFrontMatrixPixel(100);
            setFrontMatrixPixel(101);
            setFrontMatrixPixel(102);

#if WEATHER_VERBOSE
            Serial.println("Uhr ");
#endif
            break;

        case FrontWord::h_ein:
            setFrontMatrixPixel(61);
            setFrontMatrixPixel(62);
            setFrontMatrixPixel(63);

#if WEATHER_VERBOSE
            Serial.println("Eins ");
#endif
            break;

        case FrontWord::h_zwei:
            setFrontMatrixPixel(62);
            setFrontMatrixPixel(63);
            setFrontMatrixPixel(64);
            setFrontMatrixPixel(65);

#if WEATHER_VERBOSE
            Serial.println("Zwei ");
#endif
            break;

        case FrontWord::h_drei:
            setFrontMatrixPixel(67);
            setFrontMatrixPixel(68);
            setFrontMatrixPixel(69);
            setFrontMatrixPixel(70);

#if WEATHER_VERBOSE
            Serial.println("Drei ");
#endif
            break;

        case FrontWord::h_vier:
            setFrontMatrixPixel(77);
            setFrontMatrixPixel(78);
            setFrontMatrixPixel(79);
            setFrontMatrixPixel(80);

#if WEATHER_VERBOSE
            Serial.println("Vier ");
#endif
            break;

        case FrontWord::h_fuenf:
            setFrontMatrixPixel(73);
            setFrontMatrixPixel(74);
            setFrontMatrixPixel(75);
            setFrontMatrixPixel(76);

#if WEATHER_VERBOSE
            Serial.println("Fünf ");
#endif
            break;

        case FrontWord::h_sechs:
            setFrontMatrixPixel(104);
            setFrontMatrixPixel(105);
            setFrontMatrixPixel(106);
            setFrontMatrixPixel(107);
            setFrontMatrixPixel(108);

#if WEATHER_VERBOSE
            Serial.println("Sechs ");
#endif
            break;

        case FrontWord::h_sieben:
            setFrontMatrixPixel(55);
            setFrontMatrixPixel(56);
            setFrontMatrixPixel(57);
            setFrontMatrixPixel(58);
            setFrontMatrixPixel(59);
            setFrontMatrixPixel(60);

#if WEATHER_VERBOSE
            Serial.println("Sieben ");
#endif
            break;

        case FrontWord::h_acht:
            setFrontMatrixPixel(89);
            setFrontMatrixPixel(90);
            setFrontMatrixPixel(91);
            setFrontMatrixPixel(92);

#if WEATHER_VERBOSE
            Serial.println("Acht ");
#endif
            break;

        case FrontWord::h_neun:
            setFrontMatrixPixel(81);
            setFrontMatrixPixel(82);
            setFrontMatrixPixel(83);
            setFrontMatrixPixel(84);

#if WEATHER_VERBOSE
            Serial.println("Neun ");
#endif
            break;

        case FrontWord::h_zehn:
            setFrontMatrixPixel(93);
            setFrontMatrixPixel(94);
            setFrontMatrixPixel(95);
            setFrontMatrixPixel(96);

#if WEATHER_VERBOSE
            Serial.println("Zehn ");
#endif
            break;

        case FrontWord::h_elf:
            setFrontMatrixPixel(85);
            setFrontMatrixPixel(86);
            setFrontMatrixPixel(87);

#if WEATHER_VERBOSE
            Serial.println("Elf ");
#endif
            break;

        case FrontWord::h_zwoelf:
            setFrontMatrixPixel(49);
            setFrontMatrixPixel(50);
            setFrontMatrixPixel(51);
            setFrontMatrixPixel(52);
            setFrontMatrixPixel(53);

#if WEATHER_VERBOSE
            Serial.println("Zwölf ");
#endif
            break;

            //------------------------------------------------------------------------------
            // Wetter
            //------------------------------------------------------------------------------

        case FrontWord::w_morgen:
            setFrontMatrixPixel(131);
            setFrontMatrixPixel(130);
            setFrontMatrixPixel(129);
            setFrontMatrixPixel(128);
            setFrontMatrixPixel(127);
            setFrontMatrixPixel(126);

#if WEATHER_VERBOSE
            Serial.print("Morgen ");
#endif
            break;

        case FrontWord::w_frueh:
            setFrontMatrixPixel(124);
            setFrontMatrixPixel(123);
            setFrontMatrixPixel(122);
            setFrontMatrixPixel(121);

#if WEATHER_VERBOSE
            Serial.print("Früh ");
#endif
            break;

        case FrontWord::w_abend:
            setFrontMatrixPixel(132);
            setFrontMatrixPixel(133);
            setFrontMatrixPixel(134);
            setFrontMatrixPixel(135);
            setFrontMatrixPixel(136);

#if WEATHER_VERBOSE
            Serial.print("Abend ");
#endif
            break;

        case FrontWord::w_mittag:
            setFrontMatrixPixel(137);
            setFrontMatrixPixel(138);
            setFrontMatrixPixel(139);
            setFrontMatrixPixel(140);
            setFrontMatrixPixel(141);
            setFrontMatrixPixel(142);

#if WEATHER_VERBOSE
            Serial.print("Mittag ");
#endif
            break;

        case FrontWord::w_nacht:
            setFrontMatrixPixel(153);
            setFrontMatrixPixel(152);
            setFrontMatrixPixel(151);
            setFrontMatrixPixel(150);
            setFrontMatrixPixel(149);

#if WEATHER_VERBOSE
            Serial.print("Nacht ");
#endif
            break;

        case FrontWord::w_schnee:
            setFrontMatrixPixel(148);
            setFrontMatrixPixel(147);
            setFrontMatrixPixel(146);
            setFrontMatrixPixel(145);
            setFrontMatrixPixel(144);
            setFrontMatrixPixel(143);

#if WEATHER_VERBOSE
            Serial.print("Schnee ");
#endif
            break;

        case FrontWord::w_klar:
            setFrontMatrixPixel(154);
            setFrontMatrixPixel(155);
            setFrontMatrixPixel(156);
            setFrontMatrixPixel(157);

#if WEATHER_VERBOSE
            Serial.print("klar ");
#endif
            break;

        case FrontWord::w_warnung:
            setFrontMatrixPixel(158);
            setFrontMatrixPixel(159);
            setFrontMatrixPixel(160);
            setFrontMatrixPixel(161);
            setFrontMatrixPixel(162);
            setFrontMatrixPixel(163);
            setFrontMatrixPixel(164);

#if WEATHER_VERBOSE
            Serial.print("Warnung ");
#endif
            break;

        case FrontWord::w_regen:
            setFrontMatrixPixel(175);
            setFrontMatrixPixel(174);
            setFrontMatrixPixel(173);
            setFrontMatrixPixel(172);
            setFrontMatrixPixel(171);

#if WEATHER_VERBOSE
            Serial.print("Regen ");
#endif
            break;

        case FrontWord::w_wolken:
            setFrontMatrixPixel(170);
            setFrontMatrixPixel(169);
            setFrontMatrixPixel(168);
            setFrontMatrixPixel(167);
            setFrontMatrixPixel(166);
            setFrontMatrixPixel(165);

#if WEATHER_VERBOSE
            Serial.print("Wolken ");
#endif
            break;

        case FrontWord::w_gewitter:
            setFrontMatrixPixel(177);
            setFrontMatrixPixel(178);
            setFrontMatrixPixel(179);
            setFrontMatrixPixel(180);
            setFrontMatrixPixel(181);
            setFrontMatrixPixel(182);
            setFrontMatrixPixel(183);
            setFrontMatrixPixel(184);

#if WEATHER_VERBOSE
            Serial.print("Gewitter ");
#endif
            break;

        case FrontWord::w_unter:
            setFrontMatrixPixel(197);
            setFrontMatrixPixel(196);
            setFrontMatrixPixel(195);
            setFrontMatrixPixel(194);
            setFrontMatrixPixel(193);

#if WEATHER_VERBOSE
            Serial.print("unter ");
#endif
            break;

        case FrontWord::w_ueber:
            setFrontMatrixPixel(191);
            setFrontMatrixPixel(190);
            setFrontMatrixPixel(189);
            setFrontMatrixPixel(188);

#if WEATHER_VERBOSE
            Serial.print("über ");
#endif
            break;

        case FrontWord::w_minus:
            setFrontMatrixPixel(199);
            setFrontMatrixPixel(200);
            setFrontMatrixPixel(201);
            setFrontMatrixPixel(202);
            setFrontMatrixPixel(203);

#if WEATHER_VERBOSE
            Serial.print("minus ");
#endif
            break;

        case FrontWord::w_null:
            setFrontMatrixPixel(205);
            setFrontMatrixPixel(206);
            setFrontMatrixPixel(207);
            setFrontMatrixPixel(208);

#if WEATHER_VERBOSE
            Serial.print("Null ");
#endif
            break;

        case FrontWord::w_fuenf:
            setFrontMatrixPixel(219);
            setFrontMatrixPixel(218);
            setFrontMatrixPixel(217);
            setFrontMatrixPixel(216);

#if WEATHER_VERBOSE
            Serial.print("Fünf ");
#endif
            break;

        case FrontWord::w_zehn:
            setFrontMatrixPixel(215);
            setFrontMatrixPixel(214);
            setFrontMatrixPixel(213);
            setFrontMatrixPixel(212);

#if WEATHER_VERBOSE
            Serial.print("Zehn ");
#endif
            break;

        case FrontWord::w_und:
            setFrontMatrixPixel(211);
            setFrontMatrixPixel(210);
            setFrontMatrixPixel(209);

#if WEATHER_VERBOSE
            Serial.print("und ");
#endif
            break;

        case FrontWord::w_zwanzig:
            setFrontMatrixPixel(222);
            setFrontMatrixPixel(223);
            setFrontMatrixPixel(224);
            setFrontMatrixPixel(225);
            setFrontMatrixPixel(226);
            setFrontMatrixPixel(227);
            setFrontMatrixPixel(228);

#if WEATHER_VERBOSE
            Serial.print("Zwanzig ");
#endif
            break;

        case FrontWord::w_dreissig:
            setFrontMatrixPixel(241);
            setFrontMatrixPixel(240);
            setFrontMatrixPixel(239);
            setFrontMatrixPixel(238);
            setFrontMatrixPixel(237);
            setFrontMatrixPixel(236);
            setFrontMatrixPixel(235);

#if WEATHER_VERBOSE
            Serial.print("Dreißig ");
#endif
            break;

        case FrontWord::w_grad:
            setFrontMatrixPixel(233);
            setFrontMatrixPixel(232);

#if WEATHER_VERBOSE
            Serial.println("°C ");
#endif
            break;

        default:
            break;
        }
    };
};

De22x11Weather_t _de22x11Weather;
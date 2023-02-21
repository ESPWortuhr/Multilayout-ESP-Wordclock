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

class De21x11Weather_t : public iUhrType {
public:
    virtual LanguageAbbreviation usedLang() override {
        return LanguageAbbreviation::DE;
    };

    //------------------------------------------------------------------------------

    const uint16_t minArr[4] = {112, 114, 116, 118};

    //------------------------------------------------------------------------------

    virtual const void getMinuteArray(uint16_t *returnArr, uint8_t col) {
        for (uint8_t i = 0; i < 4; i++) {
            returnArr[i] = minArr[i];
        }
    };

    //------------------------------------------------------------------------------

    virtual const uint16_t numPixels() override { return 242; };

    //------------------------------------------------------------------------------

    virtual const uint16_t rowsWordMatrix() override { return 22; };

    //------------------------------------------------------------------------------

    virtual const bool hasWeatherLayout() override { return true; }

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

#if WEATHER_VERBOSE
            Serial.println("");
            Serial.print("Es ist ");
#endif
            break;

        case FrontWord::viertel:
            setLetter(26);
            setLetter(27);
            setLetter(28);
            setLetter(29);
            setLetter(30);
            setLetter(31);
            setLetter(32);

#if WEATHER_VERBOSE
            Serial.print("viertel ");
#endif
            break;

        case FrontWord::fuenf:
            setLetter(7);
            setLetter(8);
            setLetter(9);
            setLetter(10);

#if WEATHER_VERBOSE
            Serial.print("Fünf ");
#endif
            break;

        case FrontWord::zehn:
            setLetter(18);
            setLetter(19);
            setLetter(20);
            setLetter(21);

#if WEATHER_VERBOSE
            Serial.print("zehn ");
#endif
            break;

        case FrontWord::zwanzig:
            setLetter(11);
            setLetter(12);
            setLetter(13);
            setLetter(14);
            setLetter(15);
            setLetter(16);
            setLetter(17);

#if WEATHER_VERBOSE
            Serial.print("zwanzig ");
#endif
            break;

        case FrontWord::halb:
            setLetter(44);
            setLetter(45);
            setLetter(46);
            setLetter(47);

#if WEATHER_VERBOSE
            Serial.print("halb ");
#endif
            break;

        case FrontWord::eins:
            setLetter(60);
            setLetter(61);
            setLetter(62);
            setLetter(63);

#if WEATHER_VERBOSE
            Serial.print("Eins ");
#endif
            break;

        case FrontWord::nach:
        case FrontWord::v_nach:
            setLetter(38);
            setLetter(39);
            setLetter(40);
            setLetter(41);

#if WEATHER_VERBOSE
            Serial.print("nach ");
#endif
            break;

        case FrontWord::vor:
        case FrontWord::v_vor:
            setLetter(35);
            setLetter(36);
            setLetter(37);

#if WEATHER_VERBOSE
            Serial.print("vor ");
#endif

        case FrontWord::uhr:
            setLetter(100);
            setLetter(101);
            setLetter(102);

#if WEATHER_VERBOSE
            Serial.println("Uhr ");
#endif
            break;

        case FrontWord::h_ein:
            setLetter(61);
            setLetter(62);
            setLetter(63);

#if WEATHER_VERBOSE
            Serial.println("Eins ");
#endif
            break;

        case FrontWord::h_zwei:
            setLetter(62);
            setLetter(63);
            setLetter(64);
            setLetter(65);

#if WEATHER_VERBOSE
            Serial.println("Zwei ");
#endif
            break;

        case FrontWord::h_drei:
            setLetter(67);
            setLetter(68);
            setLetter(69);
            setLetter(70);

#if WEATHER_VERBOSE
            Serial.println("Drei ");
#endif
            break;

        case FrontWord::h_vier:
            setLetter(77);
            setLetter(78);
            setLetter(79);
            setLetter(80);

#if WEATHER_VERBOSE
            Serial.println("Vier ");
#endif
            break;

        case FrontWord::h_fuenf:
            setLetter(73);
            setLetter(74);
            setLetter(75);
            setLetter(76);

#if WEATHER_VERBOSE
            Serial.println("Fünf ");
#endif
            break;

        case FrontWord::h_sechs:
            setLetter(104);
            setLetter(105);
            setLetter(106);
            setLetter(107);
            setLetter(108);

#if WEATHER_VERBOSE
            Serial.println("Sechs ");
#endif
            break;

        case FrontWord::h_sieben:
            setLetter(55);
            setLetter(56);
            setLetter(57);
            setLetter(58);
            setLetter(59);
            setLetter(60);

#if WEATHER_VERBOSE
            Serial.println("Sieben ");
#endif
            break;

        case FrontWord::h_acht:
            setLetter(89);
            setLetter(90);
            setLetter(91);
            setLetter(92);

#if WEATHER_VERBOSE
            Serial.println("Acht ");
#endif
            break;

        case FrontWord::h_neun:
            setLetter(81);
            setLetter(82);
            setLetter(83);
            setLetter(84);

#if WEATHER_VERBOSE
            Serial.println("Neun ");
#endif
            break;

        case FrontWord::h_zehn:
            setLetter(93);
            setLetter(94);
            setLetter(95);
            setLetter(96);

#if WEATHER_VERBOSE
            Serial.println("Zehn ");
#endif
            break;

        case FrontWord::h_elf:
            setLetter(85);
            setLetter(86);
            setLetter(87);

#if WEATHER_VERBOSE
            Serial.println("Elf ");
#endif
            break;

        case FrontWord::h_zwoelf:
            setLetter(49);
            setLetter(50);
            setLetter(51);
            setLetter(52);
            setLetter(53);

#if WEATHER_VERBOSE
            Serial.println("Zwölf ");
#endif
            break;

            //------------------------------------------------------------------------------
            // Wetter
            //------------------------------------------------------------------------------

        case FrontWord::w_morgen:
            setLetter(131);
            setLetter(130);
            setLetter(129);
            setLetter(128);
            setLetter(127);
            setLetter(126);

#if WEATHER_VERBOSE
            Serial.print("Morgen ");
#endif
            break;

        case FrontWord::w_frueh:
            setLetter(124);
            setLetter(123);
            setLetter(122);
            setLetter(121);

#if WEATHER_VERBOSE
            Serial.print("Früh ");
#endif
            break;

        case FrontWord::w_abend:
            setLetter(132);
            setLetter(133);
            setLetter(134);
            setLetter(135);
            setLetter(136);

#if WEATHER_VERBOSE
            Serial.print("Abend ");
#endif
            break;

        case FrontWord::w_mittag:
            setLetter(137);
            setLetter(138);
            setLetter(139);
            setLetter(140);
            setLetter(141);
            setLetter(142);

#if WEATHER_VERBOSE
            Serial.print("Mittag ");
#endif
            break;

        case FrontWord::w_nacht:
            setLetter(153);
            setLetter(152);
            setLetter(151);
            setLetter(150);
            setLetter(149);

#if WEATHER_VERBOSE
            Serial.print("Nacht ");
#endif
            break;

        case FrontWord::w_schnee:
            setLetter(148);
            setLetter(147);
            setLetter(146);
            setLetter(145);
            setLetter(144);
            setLetter(143);

#if WEATHER_VERBOSE
            Serial.print("Schnee ");
#endif
            break;

        case FrontWord::w_klar:
            setLetter(154);
            setLetter(155);
            setLetter(156);
            setLetter(157);

#if WEATHER_VERBOSE
            Serial.print("klar ");
#endif
            break;

        case FrontWord::w_warnung:
            setLetter(158);
            setLetter(159);
            setLetter(160);
            setLetter(161);
            setLetter(162);
            setLetter(163);
            setLetter(164);

#if WEATHER_VERBOSE
            Serial.print("Warnung ");
#endif
            break;

        case FrontWord::w_regen:
            setLetter(175);
            setLetter(174);
            setLetter(173);
            setLetter(172);
            setLetter(171);

#if WEATHER_VERBOSE
            Serial.print("Regen ");
#endif
            break;

        case FrontWord::w_wolken:
            setLetter(170);
            setLetter(169);
            setLetter(168);
            setLetter(167);
            setLetter(166);
            setLetter(165);

#if WEATHER_VERBOSE
            Serial.print("Wolken ");
#endif
            break;

        case FrontWord::w_gewitter:
            setLetter(177);
            setLetter(178);
            setLetter(179);
            setLetter(180);
            setLetter(181);
            setLetter(182);
            setLetter(183);
            setLetter(184);

#if WEATHER_VERBOSE
            Serial.print("Gewitter ");
#endif
            break;

        case FrontWord::w_unter:
            setLetter(197);
            setLetter(196);
            setLetter(195);
            setLetter(194);
            setLetter(193);

#if WEATHER_VERBOSE
            Serial.print("unter ");
#endif
            break;

        case FrontWord::w_ueber:
            setLetter(191);
            setLetter(190);
            setLetter(189);
            setLetter(188);

#if WEATHER_VERBOSE
            Serial.print("über ");
#endif
            break;

        case FrontWord::w_minus:
            setLetter(199);
            setLetter(200);
            setLetter(201);
            setLetter(202);
            setLetter(203);

#if WEATHER_VERBOSE
            Serial.print("minus ");
#endif
            break;

        case FrontWord::w_null:
            setLetter(205);
            setLetter(206);
            setLetter(207);
            setLetter(208);

#if WEATHER_VERBOSE
            Serial.print("Null ");
#endif
            break;

        case FrontWord::w_fuenf:
            setLetter(219);
            setLetter(218);
            setLetter(217);
            setLetter(216);

#if WEATHER_VERBOSE
            Serial.print("Fünf ");
#endif
            break;

        case FrontWord::w_zehn:
            setLetter(215);
            setLetter(214);
            setLetter(213);
            setLetter(212);

#if WEATHER_VERBOSE
            Serial.print("Zehn ");
#endif
            break;

        case FrontWord::w_und:
            setLetter(211);
            setLetter(210);
            setLetter(209);

#if WEATHER_VERBOSE
            Serial.print("und ");
#endif
            break;

        case FrontWord::w_zwanzig:
            setLetter(222);
            setLetter(223);
            setLetter(224);
            setLetter(225);
            setLetter(226);
            setLetter(227);
            setLetter(228);

#if WEATHER_VERBOSE
            Serial.print("Zwanzig ");
#endif
            break;

        case FrontWord::w_dreissig:
            setLetter(241);
            setLetter(240);
            setLetter(239);
            setLetter(238);
            setLetter(237);
            setLetter(236);
            setLetter(235);

#if WEATHER_VERBOSE
            Serial.print("Dreißig ");
#endif
            break;

        case FrontWord::w_grad:
            setLetter(233);
            setLetter(232);

#if WEATHER_VERBOSE
            Serial.println("°C ");
#endif
            break;

        default:
            break;
        }
    };
};

De21x11Weather_t _de21x11Weather;
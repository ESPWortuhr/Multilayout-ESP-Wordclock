#pragma once

#include "Uhrtype.hpp"

/*
 *           Layout Front
 *                COL
 *       X 9 8 7 6 5 4 3 2 1 0
 * ROW + - - - - - - - - - - -
 *  0  | E S K I S T L F Ü N F
 *  1  | Z E H N Z W A N Z I G
 *  2  | D R E I V I E R T E L
 *  3  | T G N A C H V O R J M
 *  4  | H A L B Q Z W Ö L F P
 *  5  | Z W E I N S I E B E N
 *  6  | K D R E I R H F Ü N F
 *  7  | E L F N E U N V I E R
 *  8  | W A C H T Z E H N B X
 *  9  | B S E C H S F U H R M
 *
 *  X  |        X X X X
 *
 *  1  | M O R G E N X F R Ü H
 *  2  | A B E N D M I T T A G
 *  3  | N A C H T S C H N E E
 *  4  | K L A R W A R N U N G
 *  5  | R E G E N W O L K E N
 *  6  | O G E W I T T E R B N
 *  7  | U N T E R Z Ü B E R K
 *  8  | Y M I N U S A N U L L
 *  9  | H N Z W A N Z I G J T
 *  X  | D R E I ẞ I G O ° C X
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
            setFrontMatrixWord(0, 9, 10);
            // Ist
            setFrontMatrixWord(0, 5, 7);
#if WEATHER_VERBOSE
            Serial.println("");
            Serial.print("Es ist ");
#endif
            break;

        case FrontWord::viertel:
            setFrontMatrixWord(2, 0, 6);
#if WEATHER_VERBOSE
            Serial.print("viertel ");
#endif
            break;

        case FrontWord::m_fuenf:
            setFrontMatrixWord(0, 0, 3);
#if WEATHER_VERBOSE
            Serial.print("Fünf ");
#endif
            break;

        case FrontWord::m_zehn:
            setFrontMatrixWord(1, 7, 10);
#if WEATHER_VERBOSE
            Serial.print("zehn ");
#endif
            break;

        case FrontWord::m_zwanzig:
            setFrontMatrixWord(1, 0, 6);
#if WEATHER_VERBOSE
            Serial.print("zwanzig ");
#endif
            break;

        case FrontWord::halb:
            setFrontMatrixWord(4, 7, 10);
#if WEATHER_VERBOSE
            Serial.print("halb ");
#endif
            break;

        case FrontWord::eins:
            setFrontMatrixWord(5, 5, 8);
#if WEATHER_VERBOSE
            Serial.print("Eins ");
#endif
            break;

        case FrontWord::nach:
        case FrontWord::v_nach:
            setFrontMatrixWord(3, 5, 8);
#if WEATHER_VERBOSE
            Serial.print("nach ");
#endif
            break;

        case FrontWord::vor:
        case FrontWord::v_vor:
            setFrontMatrixWord(3, 2, 4);
#if WEATHER_VERBOSE
            Serial.print("vor ");
#endif

        case FrontWord::uhr:
            setFrontMatrixWord(9, 1, 3);
#if WEATHER_VERBOSE
            Serial.println("Uhr ");
#endif
            break;

        case FrontWord::h_ein:
            setFrontMatrixWord(5, 6, 8);
#if WEATHER_VERBOSE
            Serial.println("Eins ");
#endif
            break;

        case FrontWord::h_zwei:
            setFrontMatrixWord(5, 7, 10);
#if WEATHER_VERBOSE
            Serial.println("Zwei ");
#endif
            break;

        case FrontWord::h_drei:
            setFrontMatrixWord(6, 6, 9);
#if WEATHER_VERBOSE
            Serial.println("Drei ");
#endif
            break;

        case FrontWord::h_vier:
            setFrontMatrixWord(7, 0, 3);
#if WEATHER_VERBOSE
            Serial.println("Vier ");
#endif
            break;

        case FrontWord::h_fuenf:
            setFrontMatrixWord(6, 0, 3);
#if WEATHER_VERBOSE
            Serial.println("Fünf ");
#endif
            break;

        case FrontWord::h_sechs:
            setFrontMatrixWord(9, 5, 9);
#if WEATHER_VERBOSE
            Serial.println("Sechs ");
#endif
            break;

        case FrontWord::h_sieben:
            setFrontMatrixWord(5, 0, 5);
#if WEATHER_VERBOSE
            Serial.println("Sieben ");
#endif
            break;

        case FrontWord::h_acht:
            setFrontMatrixWord(8, 6, 9);
#if WEATHER_VERBOSE
            Serial.println("Acht ");
#endif
            break;

        case FrontWord::h_neun:
            setFrontMatrixWord(7, 4, 7);
#if WEATHER_VERBOSE
            Serial.println("Neun ");
#endif
            break;

        case FrontWord::h_zehn:
            setFrontMatrixWord(8, 2, 5);
#if WEATHER_VERBOSE
            Serial.println("Zehn ");
#endif
            break;

        case FrontWord::h_elf:
            setFrontMatrixWord(7, 8, 10);
#if WEATHER_VERBOSE
            Serial.println("Elf ");
#endif
            break;

        case FrontWord::h_zwoelf:
            setFrontMatrixWord(4, 1, 5);
#if WEATHER_VERBOSE
            Serial.println("Zwölf ");
#endif
            break;

            //------------------------------------------------------------------------------
            // Wetter
            //------------------------------------------------------------------------------

        case FrontWord::w_morgen:
            setFrontMatrixWord(11, 5, 10);
#if WEATHER_VERBOSE
            Serial.print("Morgen ");
#endif
            break;

        case FrontWord::w_frueh:
            setFrontMatrixWord(11, 0, 3);
#if WEATHER_VERBOSE
            Serial.print("Früh ");
#endif
            break;

        case FrontWord::w_abend:
            setFrontMatrixWord(12, 6, 10);
#if WEATHER_VERBOSE
            Serial.print("Abend ");
#endif
            break;

        case FrontWord::w_mittag:
            setFrontMatrixWord(12, 0, 5);
#if WEATHER_VERBOSE
            Serial.print("Mittag ");
#endif
            break;

        case FrontWord::w_nacht:
            setFrontMatrixWord(13, 6, 10);
#if WEATHER_VERBOSE
            Serial.print("Nacht ");
#endif
            break;

        case FrontWord::w_schnee:
            setFrontMatrixWord(13, 0, 5);
#if WEATHER_VERBOSE
            Serial.print("Schnee ");
#endif
            break;

        case FrontWord::w_klar:
            setFrontMatrixWord(14, 7, 10);
#if WEATHER_VERBOSE
            Serial.print("klar ");
#endif
            break;

        case FrontWord::w_warnung:
            setFrontMatrixWord(14, 0, 6);
#if WEATHER_VERBOSE
            Serial.print("Warnung ");
#endif
            break;

        case FrontWord::w_regen:
            setFrontMatrixWord(15, 6, 10);
#if WEATHER_VERBOSE
            Serial.print("Regen ");
#endif
            break;

        case FrontWord::w_wolken:
            setFrontMatrixWord(15, 0, 5);
#if WEATHER_VERBOSE
            Serial.print("Wolken ");
#endif
            break;

        case FrontWord::w_gewitter:
            setFrontMatrixWord(16, 2, 9);
#if WEATHER_VERBOSE
            Serial.print("Gewitter ");
#endif
            break;

        case FrontWord::w_unter:
            setFrontMatrixWord(17, 6, 10);
#if WEATHER_VERBOSE
            Serial.print("unter ");
#endif
            break;

        case FrontWord::w_ueber:
            setFrontMatrixWord(17, 1, 4);
#if WEATHER_VERBOSE
            Serial.print("über ");
#endif
            break;

        case FrontWord::w_minus:
            setFrontMatrixWord(18, 5, 9);
#if WEATHER_VERBOSE
            Serial.print("minus ");
#endif
            break;

        case FrontWord::w_null:
            setFrontMatrixWord(18, 0, 3);
#if WEATHER_VERBOSE
            Serial.print("Null ");
#endif
            break;

        case FrontWord::w_fuenf:
            setFrontMatrixWord(19, 7, 10);
#if WEATHER_VERBOSE
            Serial.print("Fünf ");
#endif
            break;

        case FrontWord::w_zehn:
            setFrontMatrixWord(19, 3, 6);
#if WEATHER_VERBOSE
            Serial.print("Zehn ");
#endif
            break;

        case FrontWord::w_und:
            setFrontMatrixWord(19, 0, 2);
#if WEATHER_VERBOSE
            Serial.print("und ");
#endif
            break;

        case FrontWord::w_zwanzig:
            setFrontMatrixWord(20, 2, 8);
#if WEATHER_VERBOSE
            Serial.print("Zwanzig ");
#endif
            break;

        case FrontWord::w_dreissig:
            setFrontMatrixWord(21, 4, 10);
#if WEATHER_VERBOSE
            Serial.print("Dreißig ");
#endif
            break;

        case FrontWord::w_grad:
            setFrontMatrixWord(21, 1, 2);
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
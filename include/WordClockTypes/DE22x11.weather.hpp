#pragma once

#include "ClockType.hpp"

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
 *  9  | F Ü N F Z E H N U N D
 *  X  | H N Z W A N Z I G J T
 *  1  | D R E I ẞ I G O ° C X
 */

class De22x11Weather_t : public ClockType {
public:
    virtual LanguageAbbreviation usedLang() override {
        return LanguageAbbreviation::DE;
    };

    //------------------------------------------------------------------------------

    virtual void getMinuteArray(uint16_t *returnArr,
                                MinuteVariant variant) override {
        for (uint8_t i = 0; i < 4; i++) {
            returnArr[i] = 113 + i;
        }
    };

    //------------------------------------------------------------------------------

    virtual uint8_t rowsWordMatrix() override { return 22; };

    //------------------------------------------------------------------------------

    virtual bool supportsSecondsFrame() override { return false; }

    //------------------------------------------------------------------------------

    virtual bool hasLed7x() override { return false; }

    //------------------------------------------------------------------------------

    virtual bool hasWeatherLayout() override { return true; }

    //------------------------------------------------------------------------------

    void drawWord(FrontWord word) override {
        switch (word) {

        case FrontWord::es_ist:
            // Es
            setFrontMatrixWord(0, 9, 10);
            // Ist
            setFrontMatrixWord(0, 5, 7);
            break;

        case FrontWord::viertel:
            setFrontMatrixWord(2, 0, 6);
            break;

        case FrontWord::min_5:
            setFrontMatrixWord(0, 0, 3);
            break;

        case FrontWord::min_10:
            setFrontMatrixWord(1, 7, 10);
            break;

        case FrontWord::min_20:
            setFrontMatrixWord(1, 0, 6);
            break;

        case FrontWord::halb:
            setFrontMatrixWord(4, 7, 10);
            break;

        case FrontWord::eins:
            setFrontMatrixWord(5, 5, 8);
            break;

        case FrontWord::nach:
        case FrontWord::v_nach:
            setFrontMatrixWord(3, 5, 8);
            break;

        case FrontWord::vor:
        case FrontWord::v_vor:
            setFrontMatrixWord(3, 2, 4);
            break;

        case FrontWord::uhr:
            setFrontMatrixWord(9, 1, 3);
            break;

        case FrontWord::hour_1:
            setFrontMatrixWord(5, 6, 8);
            break;

        case FrontWord::hour_2:
            setFrontMatrixWord(5, 7, 10);
            break;

        case FrontWord::hour_3:
            setFrontMatrixWord(6, 6, 9);
            break;

        case FrontWord::hour_4:
            setFrontMatrixWord(7, 0, 3);
            break;

        case FrontWord::hour_5:
            setFrontMatrixWord(6, 0, 3);
            break;

        case FrontWord::hour_6:
            setFrontMatrixWord(9, 5, 9);
            break;

        case FrontWord::hour_7:
            setFrontMatrixWord(5, 0, 5);
            break;

        case FrontWord::hour_8:
            setFrontMatrixWord(8, 6, 9);
            break;

        case FrontWord::hour_9:
            setFrontMatrixWord(7, 4, 7);
            break;

        case FrontWord::hour_10:
            setFrontMatrixWord(8, 2, 5);
            break;

        case FrontWord::hour_11:
            setFrontMatrixWord(7, 8, 10);
            break;

        case FrontWord::hour_12:
            setFrontMatrixWord(4, 1, 5);
            break;

            //------------------------------------------------------------------------------
            // Wetter
            //------------------------------------------------------------------------------

        case FrontWord::w_morgen:
            setFrontMatrixWord(11, 5, 10);
            break;

        case FrontWord::w_frueh:
            setFrontMatrixWord(11, 0, 3);
            break;

        case FrontWord::w_abend:
            setFrontMatrixWord(12, 6, 10);
            break;

        case FrontWord::w_mittag:
            setFrontMatrixWord(12, 0, 5);
            break;

        case FrontWord::w_nacht:
            setFrontMatrixWord(13, 6, 10);
            break;

        case FrontWord::w_schnee:
            setFrontMatrixWord(13, 0, 5);
            break;

        case FrontWord::w_klar:
            setFrontMatrixWord(14, 7, 10);
            break;

        case FrontWord::w_warnung:
            setFrontMatrixWord(14, 0, 6);
            break;

        case FrontWord::w_regen:
            setFrontMatrixWord(15, 6, 10);
            break;

        case FrontWord::w_wolken:
            setFrontMatrixWord(15, 0, 5);
            break;

        case FrontWord::w_gewitter:
            setFrontMatrixWord(16, 2, 9);
            break;

        case FrontWord::w_unter:
            setFrontMatrixWord(17, 6, 10);
            break;

        case FrontWord::w_ueber:
            setFrontMatrixWord(17, 1, 4);
            break;

        case FrontWord::w_minus:
            setFrontMatrixWord(18, 5, 9);
            break;

        case FrontWord::w_null:
            setFrontMatrixWord(18, 0, 3);
            break;

        case FrontWord::w_fuenf:
            setFrontMatrixWord(19, 7, 10);
            break;

        case FrontWord::w_zehn:
            setFrontMatrixWord(19, 3, 6);
            break;

        case FrontWord::w_und:
            setFrontMatrixWord(19, 0, 2);
            break;

        case FrontWord::w_zwanzig:
            setFrontMatrixWord(20, 2, 8);
            break;

        case FrontWord::w_dreissig:
            setFrontMatrixWord(21, 4, 10);
            break;

        case FrontWord::w_grad:
            setFrontMatrixWord(21, 1, 2);
            break;

        default:
            break;
        }
    };
};

De22x11Weather_t _de22x11Weather;
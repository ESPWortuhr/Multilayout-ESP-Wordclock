#pragma once

#include "ClockType.hpp"

/*
 *           Layout Front – Züridütsch
 *                COL
 *      10  9  8  7  6  5  4  3  2  1  0
 * ROW + - - - - - - - - - - -
 *  0  | E  S  K  I  S  C  H  A  Z  Ä  Ä
 *  1  | Z  W  Ä  N  Z  G  W  F  O  I  F
 *  2  | V  I  E  R  T  E  L  M  A  B  N
 *  3  | V  O  R  P  H  A  L  B  I  E  D
 *  4  | Ä  I  S  Ü  S  Ä  C  H  S  I  G
 *  5  | Z  W  Ä  I  L  F  Ö  I  F  I  Ä
 *  6  | D  R  Ü  Ü  C  Z  W  Ö  L  F  I
 *  7  | S  I  B  N  I  B  E  L  F  I  M
 *  8  | A  C  H  T  I  H  N  Ü  Ü  N  I
 *  9  | Z  Ä  Ä  N  I  X  V  I  E  R  I
 */

class Ch10x11ZH_t : public ClockType {
public:
    virtual LanguageAbbreviation usedLang() override {
        return LanguageAbbreviation::DE;
    };

    //------------------------------------------------------------------------------

    virtual bool hasTwenty() override { return true; }

    //------------------------------------------------------------------------------

    void show(FrontWord word) override {
        switch (word) {

        case FrontWord::es_ist:
            // ES → row 0, display 0–1
            setFrontMatrixWord(0, 9, 10);
            // ISCH → row 0, display 3–6
            setFrontMatrixWord(0, 4, 7);
            break;

        case FrontWord::nach:
        case FrontWord::v_nach:
            // AB → row 2, display 8–9
            setFrontMatrixWord(2, 1, 2);
            break;

        case FrontWord::vor:
        case FrontWord::v_vor:
            // VOR → row 3, display 0–2
            setFrontMatrixWord(3, 8, 10);
            break;

        case FrontWord::viertel:
            // VIERTEL → row 2, display 0–6
            setFrontMatrixWord(2, 4, 10);
            break;

        case FrontWord::min_5:
            // FOIF → row 1, display 7–10
            setFrontMatrixWord(1, 0, 3);
            break;

        case FrontWord::min_10:
            // ZÄÄ → row 0, display 8–10
            setFrontMatrixWord(0, 0, 2);
            break;

        case FrontWord::min_20:
            // ZWÄNZG → row 1, display 0–5
            setFrontMatrixWord(1, 5, 10);
            break;

        case FrontWord::halb:
            // HALBI → row 3, display 4–8
            setFrontMatrixWord(3, 2, 6);
            break;

        case FrontWord::eins:
            // ÄIS → row 4, display 0–2
            setFrontMatrixWord(4, 8, 10);
            break;

        case FrontWord::hour_1:
            // ÄIS → row 4, display 0–2
            setFrontMatrixWord(4, 8, 10);
            break;

        case FrontWord::hour_2:
            // ZWÄI → row 5, display 0–3
            setFrontMatrixWord(5, 7, 10);
            break;

        case FrontWord::hour_3:
            // DRÜÜ → row 6, display 0–3
            setFrontMatrixWord(6, 7, 10);
            break;

        case FrontWord::hour_4:
            // VIERI → row 9, display 6–10
            setFrontMatrixWord(9, 0, 4);
            break;

        case FrontWord::hour_5:
            // FÖIFI → row 5, display 5–9
            setFrontMatrixWord(5, 1, 5);
            break;

        case FrontWord::hour_6:
            // SÄCHSI → row 4, display 4–9
            setFrontMatrixWord(4, 1, 6);
            break;

        case FrontWord::hour_7:
            // SIBNI → row 7, display 0–4
            setFrontMatrixWord(7, 6, 10);
            break;

        case FrontWord::hour_8:
            // ACHTI → row 8, display 0–4
            setFrontMatrixWord(8, 6, 10);
            break;

        case FrontWord::hour_9:
            // NÜÜNI → row 8, display 6–10
            setFrontMatrixWord(8, 0, 4);
            break;

        case FrontWord::hour_10:
            // ZÄÄNI → row 9, display 0–4
            setFrontMatrixWord(9, 6, 10);
            break;

        case FrontWord::hour_11:
            // ELFI → row 7, display 6–9
            setFrontMatrixWord(7, 1, 4);
            break;

        case FrontWord::hour_12:
            // ZWÖLFI → row 6, display 5–10
            setFrontMatrixWord(6, 0, 5);
            break;

        default:
            break;
        };
    };
};

Ch10x11ZH_t _ch10x11zh;

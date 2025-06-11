#pragma once

#include "Uhrtype.hpp"

/*
 *      Layout Front by Ragman
 *                COL
 *       X 9 8 7 6 5 4 3 2 1 0
 * ROW + - - - - - - - - - - -
 *  0  | О Д И Н П Я Т Ь Д В А
 *  1  | Д Е Ш Е С Т Ь В Я Т Ь
 *  2  | В О Ч Е C E M Ь Т Р И
 *  3  | Т Ы Д В Е Р Е С Я Т Ь
 *  4  | Н А Д Ц А Т Ь Ч А С А
 *  5  | Ч А С О В Д С О Р О К
 *  6  | Т Р И Д В А Д П Я Т Ь
 *  7  | П Я Т Н А Д Е Ц А Т Ь
 *  8  | A M Д Е С Я Т С Я Т Ь
 *  9  | П Я Т Ь Я Р М И Н У Т
 */

class Ru10x11_t : public iUhrType {
public:
    virtual LanguageAbbreviation usedLang() override {
        return LanguageAbbreviation::RU;
    };

    //------------------------------------------------------------------------------

    virtual const bool hasTwenty() override { return true; }

    //------------------------------------------------------------------------------

    virtual const bool hasTwentyfive() override { return true; }

    //------------------------------------------------------------------------------

    virtual const bool hasThirtyfive() override { return true; }

    //------------------------------------------------------------------------------

    virtual const bool hasForty() override { return true; }

    //------------------------------------------------------------------------------

    virtual const bool hasDreiviertel() override { return true; }

    //------------------------------------------------------------------------------

    virtual const bool hasFifty() override { return true; }

    //------------------------------------------------------------------------------

    virtual const bool hasFiftyFive() override { return true; }

    //------------------------------------------------------------------------------

    void show(FrontWord word) override {
        switch (word) {

        case FrontWord::es_ist:
            // Ч А С О В (5 ... 12)
            setFrontMatrixWord(5, 6, 10);
            break;

        case FrontWord::es_ist___plural___:
            // Ч А С A (2 ...4)
            setFrontMatrixWord(4, 0, 3);
            break;

        case FrontWord::es_ist__singular__:
            // Ч А С  (1)
            setFrontMatrixWord(4, 1, 3);
            break;

        case FrontWord::hour_1:
            // О Д И Н
            setFrontMatrixWord(0, 7, 10);
            break;

        case FrontWord::hour_2:
            // Д В А
            setFrontMatrixWord(0, 0, 2);
            break;

        case FrontWord::hour_3:
            // Т Р И
            setFrontMatrixWord(2, 0, 2);
            break;

        case FrontWord::hour_4:
            // Ч Е Т Ы Р Е
            setFrontMatrixWord(2, 7, 8);
            setFrontMatrixWord(3, 9, 10);
            setFrontMatrixWord(3, 4, 5);
            break;

        case FrontWord::hour_5:
            // П Я Т Ь
            setFrontMatrixWord(0, 3, 6);
            break;

        case FrontWord::hour_6:
            // Ш Е С Т Ь
            setFrontMatrixWord(1, 4, 8);
            break;

        case FrontWord::hour_7:
            // C E M Ь
            setFrontMatrixWord(2, 3, 6);
            break;

        case FrontWord::hour_8:
            // B O C E M Ь
            setFrontMatrixWord(2, 9, 10);
            setFrontMatrixWord(2, 3, 6);
            break;

        case FrontWord::hour_9:
            // Д Е В Я Т Ь
            setFrontMatrixWord(1, 9, 10);
            setFrontMatrixWord(1, 0, 3);
            break;

        case FrontWord::hour_10:
            // Д Е С Я Т Ь
            setFrontMatrixWord(1, 9, 10);
            setFrontMatrixWord(3, 0, 3);
            break;

        case FrontWord::hour_11:
            // О Д И Н Н А Д Ц А Т Ь
            setFrontMatrixWord(0, 7, 10);
            setFrontMatrixWord(4, 4, 10);
            break;

        case FrontWord::hour_12:
            // Д В Е Н А Д Ц А Т Ь
            setFrontMatrixWord(3, 6, 8);
            setFrontMatrixWord(4, 4, 10);
            break;

        case FrontWord::min_5:
            // П Я Т Ь   М И Н У Т
            setFrontMatrixWord(9, 7, 10);
            setFrontMatrixWord(9, 0, 4);
            break;

        case FrontWord::min_10:
            // Д Е С Я Т Ь   М И Н У Т
            setFrontMatrixWord(8, 7, 8);
            setFrontMatrixWord(8, 0, 3);
            setFrontMatrixWord(9, 0, 4);
            break;

        case FrontWord::viertel:
            // П Я Т Н А Д Ц А Т Ь   М И Н У Т
            setFrontMatrixWord(7, 5, 10);
            setFrontMatrixWord(7, 0, 3);
            setFrontMatrixWord(9, 0, 4);
            break;

        case FrontWord::min_20:
            // Д В А Д Ц А Т Ь   М И Н У Т
            setFrontMatrixWord(6, 4, 7);
            setFrontMatrixWord(7, 0, 3);
            setFrontMatrixWord(9, 0, 4);
            break;

        case FrontWord::min_25:
            // Д В А Д Ц А Т Ь   П Я Т Ь   М И Н У Т
            setFrontMatrixWord(6, 4, 7);
            setFrontMatrixWord(7, 0, 3);
            setFrontMatrixWord(9, 7, 10);
            setFrontMatrixWord(9, 0, 4);
            break;

        case FrontWord::halb:
            // Т Р И Д   Ц А Т Ь  М И Н У Т
            setFrontMatrixWord(6, 7, 10);
            setFrontMatrixWord(7, 0, 3);
            setFrontMatrixWord(9, 0, 4);
            break;

        case FrontWord::min_35:
            // Т Р И Д   Ц А Т Ь  П Я Т Ь  М И Н У Т
            setFrontMatrixWord(6, 7, 10);
            setFrontMatrixWord(7, 0, 3);
            setFrontMatrixWord(9, 7, 10);
            setFrontMatrixWord(9, 0, 4);
            break;

        case FrontWord::min_40:
            // C O P O K   М И Н У Т
            setFrontMatrixWord(5, 0, 4);
            setFrontMatrixWord(9, 0, 4);
            break;

        case FrontWord::dreiviertel:
            // C O P O K   П Я Т Ь  М И Н У Т
            setFrontMatrixWord(5, 0, 4);
            setFrontMatrixWord(9, 7, 10);
            setFrontMatrixWord(9, 0, 4);
            break;

        case FrontWord::min_50:
            // П Я Т Ь Д Е С Я Т    М И Н У Т
            setFrontMatrixWord(6, 0, 3);
            setFrontMatrixWord(8, 4, 8);
            setFrontMatrixWord(9, 0, 4);
            break;

        case FrontWord::min_55:
            // П Я Т Ь Д Е С Я Т   П Я Т Ь   М И Н У Т
            setFrontMatrixWord(6, 0, 3);
            setFrontMatrixWord(8, 4, 8);
            setFrontMatrixWord(9, 7, 10);
            setFrontMatrixWord(9, 0, 4);
            break;

        default:
            break;
        };
    };
};

Ru10x11_t _ru10x11;
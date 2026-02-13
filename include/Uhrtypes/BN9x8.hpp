#pragma once

#include "Uhrtype.hpp"

/*
 *      Layout Front by Cutyno
 *               COL
 *       8 7 6 5 4 3 2 1 0
 * ROW + - - - - - - - - -
 *  0  | এ খ ন ত এ ক দু ই টা
 *  1  | তি ন য় দ শ চা র পাঁ চ 
 *  2  | ছ য সা ত আ ট বা রো ফ 
 *  3  | এ গা রো বে জে প য় ত্রি শ
 *  4  | বি শ পঁ চি শ চৈ তি দ শ
 *  5  | পঁ য় তা ল্লি শ ফা বি পাঁ চ  
 *  6  | প ঞ্চা ন্ন চ ল্লি শ পো নে রো
 *  7  | বা জে ত প ঞ্চা শ মি নি ট
 */

class Bn9x8_t : public iUhrType {
public:
    virtual LanguageAbbreviation usedLang() override {
        return LanguageAbbreviation::BN;
    };

    //------------------------------------------------------------------------------

    virtual const uint8_t rowsWordMatrix() override { return 8; };

    //------------------------------------------------------------------------------

    virtual const uint8_t colsWordMatrix() override { return 9; };

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

    void ekhon() {
        setFrontMatrixWord(0, 6, 8);
    }
    void beje() {
        setFrontMatrixWord(3, 4, 5);
    }

    void baje() {
        setFrontMatrixWord(7, 7, 8);
    }

    void ta() {
        setFrontMatrixWord(0, 0, 0);
    }

    void minutes() {
        setFrontMatrixWord(7, 0, 2);
    }


    void show(FrontWord word) override {
        switch (word) {

        case FrontWord::es_ist:
            ekhon();
            ta();
            break;

        case FrontWord::hour_1:
            setFrontMatrixWord(0, 3, 4);
            break;

        case FrontWord::hour_2:
            setFrontMatrixWord(0, 1, 2);
            break;

        case FrontWord::hour_3:
            setFrontMatrixWord(1, 7, 8);
            break;

        case FrontWord::hour_4:
            setFrontMatrixWord(1, 2, 3);
            break;

        case FrontWord::hour_5:
            setFrontMatrixWord(1, 0, 1);
            break;

        case FrontWord::hour_6:
            setFrontMatrixWord(2, 7, 8);
            break;

        case FrontWord::hour_7:
            setFrontMatrixWord(2, 5, 6);
            break;

        case FrontWord::hour_8:
            setFrontMatrixWord(2, 3, 4);
            break;

        case FrontWord::hour_9:
            setFrontMatrixWord(1, 6, 7);
            break;

        case FrontWord::hour_10:
            setFrontMatrixWord(1, 4, 5);
            break;

        case FrontWord::hour_11:
            setFrontMatrixWord(3, 6, 8);
            break;

        case FrontWord::hour_12:
            setFrontMatrixWord(2, 1, 2);
            break;

        case FrontWord::min_5:
            setFrontMatrixWord(5, 0, 1);
            beje();
            minutes();
            break;

        case FrontWord::min_10:
            setFrontMatrixWord(4, 0, 1);
            beje();
            minutes();
            break;

        case FrontWord::viertel:
            setFrontMatrixWord(6, 0, 2);
            beje();
            minutes();
            break;

        case FrontWord::min_20:
            setFrontMatrixWord(4, 7, 8);
            beje();
            minutes();
            break;

        case FrontWord::min_25:
            setFrontMatrixWord(4, 4, 6);
            beje();
            minutes();
            break;

        case FrontWord::halb:
            setFrontMatrixWord(3, 0, 1);
            beje();
            minutes();
            break;

        case FrontWord::min_35:
            setFrontMatrixWord(3, 0, 3);
            beje();
            minutes();
            break;

        case FrontWord::min_40:
            setFrontMatrixWord(6, 3, 5);
            beje();
            minutes();
            break;

        case FrontWord::dreiviertel:
            setFrontMatrixWord(5, 4, 8);
            beje();
            minutes();
            break;

        case FrontWord::min_50:
            setFrontMatrixWord(7, 3, 5);
            beje();
            minutes();
            break;

        case FrontWord::min_55:
            setFrontMatrixWord(6, 6, 8);
            beje();
            minutes();
            break;

        case FrontWord::uhr:
            baje();
            break;

        default:
            break;
        };
    };
};

Bn9x8_t _bn9x8;

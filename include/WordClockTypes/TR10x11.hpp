#pragma once

#include "ClockType.hpp"

/*
 *           Layout Front
 *                COL
 *       X 9 8 7 6 5 4 3 2 1 0
 * ROW + - - - - - - - - - - -
 *  0  | S A A T R O N U Ü Ç Ü
 *  1  | B İ R İ A L T I Y I D
 *  2  | İ K İ Y İ D O K U Z U
 *  3  | D Ö R D Ü Y E D İ Y İ
 *  4  | S E K İ Z İ Y A R I M
 *  5  | D Ö R T A M S B E Ş İ
 *  6  | K P M O T U Z K I R K
 *  7  | E L L İ O N Y İ R M İ
 *  8  | B U Ç U K Ç E Y R E K
 *  9  | B E Ş M G E C İ Y O R
 */

class Tr10x11_t : public ClockType {
public:
    virtual LanguageAbbreviation usedLang() override {
        return LanguageAbbreviation::TR;
    };

    //------------------------------------------------------------------------------

    virtual const uint8_t rowsWordMatrix() override { return 10; };

    //------------------------------------------------------------------------------

    virtual const uint8_t colsWordMatrix() override { return 11; };

    //------------------------------------------------------------------------------

    virtual const bool hasTwenty() override { return true; }

    //------------------------------------------------------------------------------

    virtual const bool hasTwentyfive() override { return true; }

    //------------------------------------------------------------------------------

    virtual const bool hasThirtyfive() override { return true; }

    //------------------------------------------------------------------------------

    virtual const bool hasForty() override { return true; }

    //------------------------------------------------------------------------------

    virtual const bool hasFifty() override { return true; }

    //------------------------------------------------------------------------------

    virtual const bool hasFiftyFive() override { return true; }

    //------------------------------------------------------------------------------

    void show(FrontWord word) override {
        switch (word) {

        case FrontWord::es_ist:
            // SAAT
            setFrontMatrixWord(0, 7, 10);
            break;

        case FrontWord::hour_1:
            // BİRİ
            setFrontMatrixWord(1, 7, 10);
            break;

        case FrontWord::hour_2:
            // İKİYİ
            setFrontMatrixWord(2, 6, 10);
            break;

        case FrontWord::hour_3:
            // ÜÇÜ
            setFrontMatrixWord(0, 0, 2);
            break;

        case FrontWord::hour_4:
            // DÖRDÜ
            setFrontMatrixWord(3, 6, 10);
            break;

        case FrontWord::hour_5:
            // BEŞİ
            setFrontMatrixWord(5, 0, 3);
            break;

        case FrontWord::hour_6:
            // ALTIYI
            setFrontMatrixWord(1, 1, 6);
            break;

        case FrontWord::hour_7:
            // YEDİYİ
            setFrontMatrixWord(3, 0, 5);
            break;

        case FrontWord::hour_8:
            // SEKİZİ
            setFrontMatrixWord(4, 5, 10);
            break;

        case FrontWord::hour_9:
            // DOKUZU
            setFrontMatrixWord(2, 0, 5);
            break;

        case FrontWord::hour_10:
            // ONU
            setFrontMatrixWord(0, 3, 5);
            break;

        case FrontWord::hour_11:
            // ON BİRİ
            setFrontMatrixWord(7, 5, 6);
            setFrontMatrixWord(1, 7, 10);
            break;

        case FrontWord::hour_12:
            // ON İKİYİ
            setFrontMatrixWord(7, 5, 6);
            setFrontMatrixWord(2, 6, 10);
            break;

        case FrontWord::hour_13:
            // BİR
            setFrontMatrixWord(1, 8, 10);
            break;

        case FrontWord::hour_14:
            // İKİ
            setFrontMatrixWord(2, 8, 10);
            break;

        case FrontWord::hour_15:
            // ÜÇ
            setFrontMatrixWord(0, 1, 2);
            break;

        case FrontWord::hour_16:
            // DÖRT
            setFrontMatrixWord(5, 7, 10);
            break;

        case FrontWord::hour_17:
            // BEŞ
            setFrontMatrixWord(5, 1, 3);
            break;

        case FrontWord::hour_18:
            // ALTI
            setFrontMatrixWord(1, 3, 6);
            break;

        case FrontWord::hour_19:
            // YEDİ
            setFrontMatrixWord(3, 2, 5);
            break;

        case FrontWord::hour_20:
            // SEKİZ
            setFrontMatrixWord(4, 6, 10);
            break;

        case FrontWord::hour_21:
            // DOKUZ
            setFrontMatrixWord(2, 1, 5);
            break;

        case FrontWord::hour_22:
            // ON
            setFrontMatrixWord(0, 4, 5);
            break;

        case FrontWord::hour_23:
            // ON BİR
            setFrontMatrixWord(7, 5, 6);
            setFrontMatrixWord(1, 8, 10);
            break;

        case FrontWord::hour_0:
            // ON İKİ
            setFrontMatrixWord(7, 5, 6);
            setFrontMatrixWord(2, 8, 10);
            break;

        case FrontWord::min_5:
            // BEŞ
            setFrontMatrixWord(9, 8, 10);
            break;

        case FrontWord::min_10:
            // ON
            setFrontMatrixWord(7, 5, 6);
            break;

        case FrontWord::viertel:
            // ÇEYREK
            setFrontMatrixWord(8, 0, 5);
            break;

        case FrontWord::min_20:
            // YİRMİ
            setFrontMatrixWord(7, 0, 4);
            break;

        case FrontWord::min_25:
            // YİRMİ BEŞ
            setFrontMatrixWord(7, 0, 4);
            setFrontMatrixWord(9, 8, 10);
            break;

        case FrontWord::halb:
            // BUÇUK
            setFrontMatrixWord(8, 6, 10);
            break;

        case FrontWord::min_35:
            // OTUZ BEŞ
            setFrontMatrixWord(6, 4, 7);
            setFrontMatrixWord(9, 8, 10);
            break;

        case FrontWord::min_40:
            // KIRK
            setFrontMatrixWord(6, 0, 3);
            break;

        case FrontWord::min_45:
            // KIRK BEŞ
            setFrontMatrixWord(6, 0, 3);
            setFrontMatrixWord(9, 8, 10);
            break;

        case FrontWord::min_50:
            // ELLİ
            setFrontMatrixWord(7, 7, 10);
            break;

        case FrontWord::min_55:
            // ELLİ BEŞ
            setFrontMatrixWord(7, 7, 10);
            setFrontMatrixWord(9, 8, 10);
            break;

        case FrontWord::uhr:
            // TAM
            setFrontMatrixWord(5, 5, 7);
            break;

        case FrontWord::nach:
        case FrontWord::v_nach:
        case FrontWord::vor:
        case FrontWord::v_vor:
            // GEÇİYOR
            setFrontMatrixWord(9, 0, 6);
            break;

        default:
            break;
        };
    };
};

Tr10x11_t _tr10x11;

#pragma once

enum ledText {

    /* Numbers from 0 to 60 are reserved */

    es_ist = 101,
    nach = 102,
    vor = 103,
    viertel = 104,
    dreiviertel = 105,
    uhr = 106,
    halb = 107,
    fuenf = 108,
    zehn = 109,
    zwanzig = 110,
    eins = 111,
    frueh = 112,
    minute = 113,
    minuten = 114,
    minuten_extra = 115,
    abends = 116,
    mitternacht = 117,
    mittags = 118,
    warm = 119,
    morgens = 120,
    und = 121,
    minuten_uhr = 122,

    m_eine = 131,
    m_zwei = 132,
    m_drei = 133,
    m_vier = 134,
    m_sechs = 136,
    m_sieben = 137,
    m_acht = 138,
    m_neun = 139,
    m_elf = 141,
    m_zwoelf = 142,
    m_dreizehn = 143,
    m_vierzehn = 144,
    m_sechzehn = 146,
    m_siebzehn = 147,
    m_achtzehn = 148,
    m_neunzehn = 149,

    h_ein = 151,
    h_zwei = 152,
    h_drei = 153,
    h_vier = 154,
    h_fuenf = 155,
    h_sechs = 156,
    h_sieben = 157,
    h_acht = 158,
    h_neun = 159,
    h_zehn = 160,
    h_elf = 161,
    h_zwoelf = 162,
    h_dreizehn = 163,

    w_morgen = 200,
    w_frueh = 201,
    w_abend = 202,
    w_mittag = 203,
    w_nacht = 204,
    w_schnee = 205,
    w_klar = 206,
    w_warnung = 207,
    w_regen = 208,
    w_wolken = 209,
    w_gewitter = 210,
    w_unter = 211,
    w_ueber = 212,
    w_minus = 213,
    w_null = 214,
    w_fuenf = 215,
    w_zehn = 216,
    w_und = 217,
    w_zwanzig = 218,
    w_dreissig = 219,
    w_grad = 220,

    h_droelf = 240,
    happy_birthday = 241
};

class iUhrType {
public:
    virtual void Letter_set(const uint16_t index) { Word_array[index] = index; }

    virtual void show(uint8_t text) = 0;

    virtual inline const uint16_t NUM_PIXELS() = 0;

    virtual inline const uint16_t NUM_SMATRIX() = 0;

    virtual inline const uint16_t ROWS_MATRIX() = 0;

    virtual inline const uint16_t COLS_MATRIX() = 0;

    virtual inline const uint16_t NUM_RMATRIX() = 0;

    virtual const uint16_t getSMatrix(uint16_t index) = 0;

    virtual const uint16_t getRMatrix(uint16_t index) = 0;

    virtual const uint16_t getFrontMatrix(uint8_t row, uint8_t col) {
        if (row % 2 != 0) {
            col = COLS_MATRIX() - col - 1;
        }
        uint16_t returnValue = col + (row * COLS_MATRIX());
        if (returnValue > NUM_PIXELS()) {
            Serial.println("[ERROR] getMatrix() ReturnValue out of Bounds");
        }
        return returnValue;
    };

    virtual const uint16_t getMinArr(uint8_t col, uint8_t row) = 0;
};

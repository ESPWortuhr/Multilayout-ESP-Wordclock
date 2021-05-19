#pragma once

enum ledText {

    m_eine = 1,
    m_zwei = 2,
    m_drei = 3,
    m_vier = 4,
    fuenf = 5,
    m_sechs = 6,
    m_sieben = 7,
    m_acht = 8,
    m_neun = 9,
    zehn = 10,
    m_elf = 11,
    m_zwoelf = 12,
    m_dreizehn = 13,
    m_vierzehn = 14,
    m_sechzehn = 16,
    m_siebzehn = 17,
    m_achtzehn = 18,
    m_neunzehn = 19,
    zwanzig = 20,

    es_ist = 101,
    nach = 102,
    vor = 103,
    viertel = 104,
    dreiviertel = 105,
    uhr = 106,
    halb = 107,
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
    v_vor = 123,
    v_nach = 124,

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

    virtual inline const uint16_t NUM_PIXELS() { return 114; }

    virtual inline const uint16_t NUM_SMATRIX() { return 114; }

    virtual inline const uint16_t ROWS_MATRIX() { return 11; }

    virtual inline const uint16_t COLS_MATRIX() { return 11; }

    virtual inline const uint16_t NUM_RMATRIX() { return 0; }

    virtual const uint16_t getSMatrix(uint16_t index) { return index; }

    virtual const uint16_t getRMatrix(uint16_t index) { return 0; }

    virtual const bool hasDreiviertel() { return false; }

    virtual const bool hasZwanzig() { return true; }

    virtual const bool has24HourLayout() { return false; }

    virtual const bool hasWeatherLayout() { return false; }

    virtual const bool hasSecondsFrame() { return false; }

    virtual const char getCharFrontplate(uint8_t row, uint8_t col) { return '#'; }

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

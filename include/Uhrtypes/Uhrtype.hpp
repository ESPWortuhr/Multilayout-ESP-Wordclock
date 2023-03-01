#pragma once

enum class LanguageAbbreviation { EN, DE, NL };

enum class FrontWord {
    error,
    m_eine,
    m_zwei,
    m_drei,
    m_vier,
    fuenf,
    m_sechs,
    m_sieben,
    m_acht,
    m_neun,
    zehn,
    m_elf,
    m_zwoelf,
    m_dreizehn,
    m_vierzehn,
    m_sechzehn,
    m_siebzehn,
    m_achtzehn,
    m_neunzehn,
    zwanzig,
    twentyfive,

    es_ist,
    nach,
    vor,
    viertel,
    dreiviertel,
    uhr,
    halb,
    eins,
    frueh,
    minute,
    minuten,
    minuten_extra,
    abends,
    mitternacht,
    mittags,
    warm,
    morgens,
    und,
    minuten_uhr,
    v_vor,
    v_nach,
    a_quarter,
    nur,
    gewesen,

    h_ein,
    h_zwei,
    h_drei,
    h_vier,
    h_fuenf,
    h_sechs,
    h_sieben,
    h_acht,
    h_neun,
    h_zehn,
    h_elf,
    h_zwoelf,
    h_dreizehn,

    w_morgen,
    w_frueh,
    w_abend,
    w_mittag,
    w_nacht,
    w_schnee,
    w_klar,
    w_warnung,
    w_regen,
    w_wolken,
    w_gewitter,
    w_unter,
    w_ueber,
    w_minus,
    w_null,
    w_fuenf,
    w_zehn,
    w_und,
    w_zwanzig,
    w_dreissig,
    w_grad,

    funk,
    h_droelf,
    happy_birthday
};

class iUhrType {
protected:
    virtual void setFrontMatrixPixel(const uint16_t index) { frontMatrix[index] = true; }

public:
    virtual void show(FrontWord word) = 0;

    virtual LanguageAbbreviation usedLang() = 0;

    virtual inline const uint16_t numPixels() { return 117; }

    virtual inline const uint16_t numPixelsWordMatrix() {
        return rowsWordMatrix() * colsWordMatrix();
    }

    virtual inline const uint16_t numPixelsFrameMatrix() { return 0; }

    virtual inline const uint16_t rowsWordMatrix() { return 10; }

    virtual inline const uint16_t colsWordMatrix() { return 11; }

    virtual const uint16_t getWordMatrixIndex(uint16_t index) { return index; }

    virtual const uint16_t getFrameMatrixIndex(uint16_t index) { return 0; }

    virtual const bool hasDreiviertel() { return false; }

    virtual const bool hasZwanzig() { return true; }

    virtual const bool hasTwentyfive() { return false; }

    virtual const bool has24HourLayout() { return false; }

    virtual const bool hasWeatherLayout() { return false; }

    virtual const bool hasSecondsFrame() { return false; }

    virtual const uint16_t getFrontMatrix(uint8_t row, uint8_t col) {
        if (row % 2 != 0) {
            col = colsWordMatrix() - col - 1;
        }
        uint16_t returnValue = col + (row * colsWordMatrix());
        if (returnValue > numPixels()) {
            Serial.println("[ERROR] getMatrix() ReturnValue out of Bounds");
        }
        return returnValue;
    };

    virtual const void getMinuteArray(uint16_t *returnArr, uint8_t col) {
        for (uint8_t i = 0; i < 4; i++) {
            switch (col) {
            case 0: // LEDs for "LED4x" minute display
                returnArr[i] = numPixels() - (7 - i);
                break;
            case 1: // LEDs for "LED7x" minute display
                returnArr[i] = numPixels() - (7 - (i * 2));
                break;

            default:
                break;
            }
        }
    };
};

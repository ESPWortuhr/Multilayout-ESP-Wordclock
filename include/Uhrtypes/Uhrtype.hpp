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
public:
    virtual void setFrontMatrixWord(const uint8_t row, const uint8_t colStart,
                                    const uint8_t colEnd) {
        for (uint8_t i = colStart; i <= colEnd; i++) {
            setFrontMatrixPixel(row, i);
        }
    }

    virtual void setFrontMatrixPixel(const int row, const int col,
                                     bool state = true) {
        if (state) {
            frontMatrix[row] |= 1UL << col;
        } else {
            frontMatrix[row] &= ~(1UL << col);
        }
    }

    virtual bool getFrontMatrixPixel(const uint8_t row, const uint8_t col) {
        return (frontMatrix[row] >> (colsWordMatrix() - 1 - col)) & 1U;
    }

    virtual void show(FrontWord word) = 0;

    virtual LanguageAbbreviation usedLang() = 0;

    virtual inline const uint8_t numPixelsFrameMatrix() { return 0; }

    virtual inline const uint8_t rowsWordMatrix() { return 10; }

    virtual inline const uint8_t colsWordMatrix() { return 11; }

    virtual const uint16_t getFrameMatrixIndex(uint16_t index) { return 0; }

    virtual const bool hasDreiviertel() { return false; }

    virtual const bool hasZwanzig() { return true; }

    virtual const bool hasTwentyfive() { return false; }

    virtual const bool has24HourLayout() { return false; }

    virtual const bool hasWeatherLayout() { return false; }

    virtual const bool hasSecondsFrame() { return false; }

    virtual const uint16_t getFrontMatrixIndex(const uint8_t row, uint8_t col) {

        uint8_t newColsWordMatrix = colsWordMatrix();
        uint16_t numPixelsWordMatrix = rowsWordMatrix() * colsWordMatrix();

        if (G.buildTypeDef == BuildTypeDef::DoubleResM1) {
            newColsWordMatrix = 2 * colsWordMatrix() - 1;
            numPixelsWordMatrix = rowsWordMatrix() * newColsWordMatrix;
            col *= 2;
        }
        if (row % 2 != 0) {
            col = newColsWordMatrix - col - 1;
        }
        uint16_t returnValue = col + (row * newColsWordMatrix);

        if (returnValue > numPixelsWordMatrix) {
            Serial.println(
                "[ERROR] getFrontMatrixIndex() returnValue out of Bounds");
        }

        return returnValue;
    };

    virtual const void getMinuteArray(uint16_t *returnArr, uint8_t col) {
        uint16_t numPixelsWordMatrix = rowsWordMatrix() * colsWordMatrix();

        if (G.buildTypeDef == BuildTypeDef::DoubleResM1) {
            numPixelsWordMatrix = rowsWordMatrix() * (colsWordMatrix() * 2 - 1);
        }

        for (uint8_t i = 0; i < 4; i++) {
            switch (col) {
            case 0: // LEDs for "LED4x" minute display
                returnArr[i] = numPixelsWordMatrix + i;
                break;
            case 1: // LEDs for "LED7x" minute display
                returnArr[i] = numPixelsWordMatrix + i * 2;
                break;

            default:
                break;
            }
        }
    };
};

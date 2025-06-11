#pragma once

enum class LanguageAbbreviation { DE, EN, ES, IT, NL, HU, RO, FR, SE, RU };

enum class FrontWord {
    error,
    min_1,
    min_2,
    min_3,
    min_4,
    min_5,
    min_6,
    min_7,
    min_8,
    min_9,
    min_10,
    min_11,
    min_12,
    min_13,
    min_14,
    min_15,
    min_16,
    min_17,
    min_18,
    min_19,
    min_20,
    min_21,
    min_22,
    min_23,
    min_24,
    min_25,
    min_26,
    min_27,
    min_28,
    min_29,
    min_30,
    min_31,
    min_32,
    min_33,
    min_34,
    min_35,
    min_36,
    min_37,
    min_38,
    min_39,
    min_40,
    min_41,
    min_42,
    min_43,
    min_44,
    min_45,
    min_46,
    min_47,
    min_48,
    min_49,
    min_50,
    min_51,
    min_52,
    min_53,
    min_54,
    min_55,
    min_56,
    min_57,
    min_58,
    min_59,
    m_num1,
    m_num2,
    m_num3,
    m_num4,

    es_ist,
    es_ist__singular__,
    es_ist___plural___,
    nach,
    vor,
    viertel,
    dreiviertel,
    uhr,
    halb,
    eins,
    minute,
    minuten,
    und,
    v_vor,
    v_nach,
    a_quarter,
    nur,
    gewesen,

    hour_0,
    hour_1,
    hour_2,
    hour_3,
    hour_4,
    hour_5,
    hour_6,
    hour_7,
    hour_8,
    hour_9,
    hour_10,
    hour_11,
    hour_12,
    hour_13,
    hour_14,
    hour_15,
    hour_16,
    hour_17,
    hour_18,
    hour_19,
    hour_20,
    hour_21,
    hour_22,
    hour_23,

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

    plus,
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

    virtual const bool hasTwenty() { return true; }

    virtual const bool hasTwentyfive() { return false; }

    virtual const bool hasThirtyfive() { return false; }

    virtual const bool hasForty() { return false; }

    virtual const bool hasFifty() { return false; }

    virtual const bool hasFiftyFive() { return false; }

    virtual const bool hasMitternacht() { return false; }

    virtual const bool has24HourLayout() { return false; }

    virtual const bool has60MinuteLayout() { return false; }

    virtual const bool hasOnlyQuarterLayout() { return false; }

    virtual const bool hasWeatherLayout() { return false; }

    virtual const bool hasSecondsFrame() { return false; }

    virtual const bool hasMinuteInWords() { return false; }

    virtual const bool hasSpecialWordHappyBirthday() { return false; }

    virtual const bool hasSpecialWordBeen() { return false; }

    virtual const uint16_t getFrontMatrixIndex(uint8_t row, uint8_t col) {

        uint16_t returnValue;
        uint8_t newColsWordMatrix = colsWordMatrix();
        uint16_t numPixelsWordMatrix = rowsWordMatrix() * colsWordMatrix();

        if (G.buildTypeDef == BuildTypeDef::DoubleResM1) {
            newColsWordMatrix = 2 * colsWordMatrix() - 1;
            numPixelsWordMatrix = rowsWordMatrix() * newColsWordMatrix;
            col *= 2;
        }

        if (G.layoutVariant[FlipHorzVert] == false) {
            if (G.layoutVariant[MeanderRows] && (row % 2 != 0)) {
                col = newColsWordMatrix - col - 1;
            }
            returnValue = col + (row * newColsWordMatrix);
            if (G.layoutVariant[ExtraLedPerRow]) {
                returnValue += row;
                numPixelsWordMatrix += rowsWordMatrix() - 1;
            }
        } else {
            if (G.layoutVariant[MeanderRows] && (col % 2 == 0)) {
                row = rowsWordMatrix() - 1 - row;
            }
            returnValue =
                row + rowsWordMatrix() * (newColsWordMatrix - 1 - col);
            if (G.layoutVariant[ExtraLedPerRow]) {
                returnValue += colsWordMatrix() - 1 - col;
                numPixelsWordMatrix += colsWordMatrix() - 1;
            }
        }

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
        if (G.layoutVariant[ExtraLedPerRow]) {
            if (G.layoutVariant[FlipHorzVert] == false) {
                numPixelsWordMatrix += rowsWordMatrix() - 1;
            } else {
                numPixelsWordMatrix += colsWordMatrix() - 1;
            }
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

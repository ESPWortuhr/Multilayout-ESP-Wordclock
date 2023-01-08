#include "Animation.h"
#include "NeoMultiFeature.hpp"
#include "Uhr.h"
#include "Uhrtypes/Uhrtype.hpp"
#include "clockWork.h"
#include "openwmap.h"
#include <Arduino.h>

//------------------------------------------------------------------------------

void ClockWork::copyClockface(const uint16_t source[], uint16_t destination[]) {
    for (uint16_t i = 0; i < usedUhrType->NUM_PIXELS(); i++) {
        destination[i] = source[i];
    }
}

//------------------------------------------------------------------------------

bool ClockWork::changesInClockface() {
    for (uint16_t i = 0; i < usedUhrType->NUM_PIXELS(); i++) {
        if (Word_array[i] != Word_array_old[i]) {
            return true;
        }
    }
    return false;
}

//------------------------------------------------------------------------------

void ClockWork::loopLdrLogic() {
    int16_t lux = analogRead(A0); // Range 0-1023
    uint8_t ldrValOld = ldrVal;

    if (G.autoLdrEnabled) {
        lux /= 4;
        int minimum = min(G.autoLdrBright, G.autoLdrDark);
        int maximum = max(G.autoLdrBright, G.autoLdrDark);
        if (lux >= maximum)
            lux = maximum;
        if (lux <= minimum)
            lux = minimum;
        if (G.autoLdrDark == G.autoLdrBright) {
            // map() //Would crash with division by zero
            ldrVal = 100;
        } else {
            ldrVal = map(lux, G.autoLdrDark, G.autoLdrBright, 10, 100);
        }
    } else {
        if (G.ldr == 1) {
            lux = lux - (G.ldrCal * 20);
            if (lux >= 900) {
                lux = 900;
            } // Max Brightness
            if (lux <= 1) {
                lux = 1;
            } // Min Brightness
            ldrVal = map(lux, 1, 900, 1, 100);
        }
    }
    if (ldrValOld != ldrVal) {
        led.set();
    }
}

//------------------------------------------------------------------------------

void ClockWork::rainbow() {
    static float hue = 0;

    for (uint16_t i = 0; i < usedUhrType->NUM_PIXELS(); i++) {
        led.setPixelHsb(i, hue, 100, G.hell);
    }
    led.show();
    hue++;
    led.checkIfHueIsOutOfBound(hue);
}

//-----------------------------------------------------------------------------

void ClockWork::rainbowCycle() {
    static float hue = 0;
    float displayedHue;

    displayedHue = hue;
    for (uint16_t i = 0; i < usedUhrType->NUM_SMATRIX(); i++) {
        led.setPixelHsb(usedUhrType->getSMatrix(i), displayedHue, 100, G.hell);
        displayedHue = displayedHue + 360.0 / usedUhrType->NUM_SMATRIX();
        led.checkIfHueIsOutOfBound(displayedHue);
    }
    led.show();
    hue++;
    led.checkIfHueIsOutOfBound(hue);
}

//------------------------------------------------------------------------------

void ClockWork::scrollingText(const char *buf) {
    static uint8_t i = 0, ii = 0;
    uint8_t offsetRow = (usedUhrType->ROWS_MATRIX() - fontHeight - 1) / 2;
    uint8_t fontIndex = buf[ii];

    led.shiftColumnToRight();
    led.clearFrontExeptofFontspace(offsetRow);

    if (i < fontWidth) {
        for (uint8_t row = 0; row < fontHeight; row++) {
            if (pgm_read_byte(&(font_7x5[fontIndex][i])) & (1u << row)) {
                led.setPixel(
                    G.rgb[Effect][0], G.rgb[Effect][1], G.rgb[Effect][2],
                    G.rgb[Effect][3],
                    usedUhrType->getFrontMatrix(
                        row + offsetRow, usedUhrType->COLS_MATRIX() - 1));
            } else {
                led.clearPixel(usedUhrType->getFrontMatrix(
                    row + offsetRow, usedUhrType->COLS_MATRIX() - 1));
            }
        }
    } else {
        for (uint8_t row = 0; row < fontHeight; row++) {
            led.clearPixel(usedUhrType->getFrontMatrix(
                row + offsetRow, usedUhrType->COLS_MATRIX() - 1));
        }
    }
    led.show();

    i++;
    if (i > fontWidth) {
        i = 0;
        ii++;
        if (ii > strlen(buf)) {
            ii = 0;
        }
    }
}

//------------------------------------------------------------------------------

void ClockWork::initBootShowIp(const char *buf) {
    uint8_t StringLength = strlen(buf);
    StringLength = StringLength * 6; // Times 6, because thats the length of a
                                     // char in the 7x5 font plus spacing
    for (uint16_t i = 0; i <= StringLength; i++) {
        scrollingText(buf);
        delay(200);
    }
}

//------------------------------------------------------------------------------

void ClockWork::initBootWifiSignalStrength(int strength) {
    if (strength <= 100) {
        led.setIcon(WLAN100, 100);
    } else if (strength <= 60) {
        led.setIcon(WLAN60, 60);
    } else if (strength <= 30) {
        led.setIcon(WLAN30, 30);
    }
}

//------------------------------------------------------------------------------

void ClockWork::setHour(const uint8_t std, const uint8_t voll) {
    switch (std) {
    case 0:
        usedUhrType->show(h_zwoelf);
        break;
    case 1:
        if (voll == 1) {
            usedUhrType->show(h_ein);
        } else {
            usedUhrType->show(eins);
        }
        break;
    case 2:
        usedUhrType->show(h_zwei);
        break;
    case 3:
        usedUhrType->show(h_drei);
        break;
    case 4:
        usedUhrType->show(h_vier);
        break;
    case 5:
        usedUhrType->show(h_fuenf);
        break;
    case 6:
        usedUhrType->show(h_sechs);
        break;
    case 7:
        usedUhrType->show(h_sieben);
        break;
    case 8:
        usedUhrType->show(h_acht);
        break;
    case 9:
        usedUhrType->show(h_neun);
        break;
    case 10:
        usedUhrType->show(h_zehn);
        break;
    case 11:
        usedUhrType->show(h_elf);
        break;
    case 12:
        usedUhrType->show(h_zwoelf);
        break;
    case 13:
        if (voll == 1) {
            usedUhrType->show(h_ein);
        } else {
            usedUhrType->show(eins);
        }
        break;
    case 14:
        usedUhrType->show(h_zwei);
        break;
    case 15:
        usedUhrType->show(h_drei);
        break;
    case 16:
        usedUhrType->show(h_vier);
        break;
    case 17:
        usedUhrType->show(h_fuenf);
        break;
    case 18:
        usedUhrType->show(h_sechs);
        break;
    case 19:
        usedUhrType->show(h_sieben);
        break;
    case 20:
        usedUhrType->show(h_acht);
        break;
    case 21:
        usedUhrType->show(h_neun);
        break;
    case 22:
        usedUhrType->show(h_zehn);
        break;
    case 23:
        usedUhrType->show(h_elf);
        break;
    case 24:
        usedUhrType->show(h_zwoelf);
        break;
    default:
        break;
    }
}

//------------------------------------------------------------------------------

void ClockWork::showMinute(uint8_t min) {
    if (G.zeige_min > 0) {
        while (min > 4) {
            min -= 5;
        }

        if (min > 0) {
            Word_array[usedUhrType->getMinArr(G.zeige_min - 1, 0)] =
                usedUhrType->getMinArr(G.zeige_min - 1, 0);
        }
        if (min > 1) {
            Word_array[usedUhrType->getMinArr(G.zeige_min - 1, 1)] =
                usedUhrType->getMinArr(G.zeige_min - 1, 1);
        }
        if (min > 2) {
            Word_array[usedUhrType->getMinArr(G.zeige_min - 1, 2)] =
                usedUhrType->getMinArr(G.zeige_min - 1, 2);
        }
        if (min > 3) {
            Word_array[usedUhrType->getMinArr(G.zeige_min - 1, 3)] =
                usedUhrType->getMinArr(G.zeige_min - 1, 3);
        }
    }
}

//------------------------------------------------------------------------------

void ClockWork::setMinute(uint8_t min, uint8_t &offsetH, uint8_t &voll) {
    if (!usedUhrType->has24HourLayout()) {
        showMinute(min);
        min /= 5;
        min *= 5;
    }
    switch (min) {
    case 0: // full hour
        usedUhrType->show(uhr);
        voll = 1;
        break;
    case 1:
        usedUhrType->show(m_eine);
        usedUhrType->show(minute);
        usedUhrType->show(nach);
        break;
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
        usedUhrType->show(min);
        usedUhrType->show(minuten);
        usedUhrType->show(nach);
        break;
    case 15: // quarter past
        if (G.Sprachvariation[ItIs15]) {
            usedUhrType->show(viertel);
            offsetH = 1;
        } else {
            usedUhrType->show(viertel);
            usedUhrType->show(v_nach);
        }
        break;
    case 16:
    case 17:
    case 18:
    case 19:
        usedUhrType->show(min);
        usedUhrType->show(nach);
        break;
    case 20: // 20 past
        if (!usedUhrType->hasZwanzig() || G.Sprachvariation[ItIs20]) {
            usedUhrType->show(zehn);
            usedUhrType->show(vor);
            usedUhrType->show(halb);
            offsetH = 1;
        } else {
            usedUhrType->show(zwanzig);
            usedUhrType->show(nach);
        }
        break;
    case 21:
    case 22:
    case 23:
    case 24:
    case 25:
    case 26:
    case 27:
    case 28:
        usedUhrType->show(30 - min);
        usedUhrType->show(vor);
        usedUhrType->show(halb);
        offsetH = 1;
        break;
    case 29:
        usedUhrType->show(m_eine);
        usedUhrType->show(minute);
        usedUhrType->show(vor);
        usedUhrType->show(halb);
        offsetH = 1;
        break;
    case 30: // half
        usedUhrType->show(halb);
        offsetH = 1;
        break;
    case 31:
        usedUhrType->show(m_eine);
        usedUhrType->show(minute);
        usedUhrType->show(nach);
        usedUhrType->show(halb);
        offsetH = 1;
        break;
    case 32:
    case 33:
    case 34:
    case 35:
    case 36:
    case 37:
    case 38:
    case 39:
        usedUhrType->show(min - 30);
        usedUhrType->show(nach);
        usedUhrType->show(halb);
        offsetH = 1;
        break;
    case 40: // 20 to
        if (!usedUhrType->hasZwanzig() || G.Sprachvariation[ItIs40]) {
            usedUhrType->show(zehn);
            usedUhrType->show(nach);
            usedUhrType->show(halb);
        } else {
            usedUhrType->show(zwanzig);
            usedUhrType->show(vor);
        }
        offsetH = 1;
        break;
    case 41:
    case 42:
    case 43:
    case 44:
        usedUhrType->show(60 - min);
        usedUhrType->show(vor);
        offsetH = 1;
        break;
    case 45: // quarter to
        if (usedUhrType->hasDreiviertel() && G.Sprachvariation[ItIs45]) {
            usedUhrType->show(dreiviertel);
        } else {
            usedUhrType->show(viertel);
            usedUhrType->show(v_vor);
        }
        offsetH = 1;
        break;
    case 46:
    case 47:
    case 48:
    case 49:
    case 50:
    case 51:
    case 52:
    case 53:
    case 54:
    case 55:
        usedUhrType->show(60 - min);
        usedUhrType->show(vor);
        offsetH = 1;
        break;
    case 56:
    case 57:
    case 58:
        usedUhrType->show(60 - min);
        usedUhrType->show(minuten);
        usedUhrType->show(vor);
        offsetH = 1;
        break;
    case 59:
        usedUhrType->show(m_eine);
        usedUhrType->show(minute);
        usedUhrType->show(vor);
        offsetH = 1;
        break;
    default:
        break;
    }
}

//------------------------------------------------------------------------------

static void countdownToMidnight() {
    Serial.printf("Count down: %d\n", 60 - _sekunde);
    switch (_sekunde) {
    case 50:
        usedUhrType->show(zehn);
        break;
    case 51:
        usedUhrType->show(h_neun);
        break;
    case 52:
        usedUhrType->show(h_acht);
        break;
    case 53:
        usedUhrType->show(h_sieben);
        break;
    case 54:
        usedUhrType->show(h_sechs);
        break;
    case 55:
        usedUhrType->show(fuenf);
        break;
    case 56:
        usedUhrType->show(h_vier);
        break;
    case 57:
        usedUhrType->show(h_drei);
        break;
    case 58:
        usedUhrType->show(h_zwei);
        break;
    case 59:
        usedUhrType->show(eins);
        break;
    default:
        break;
    }
}

//------------------------------------------------------------------------------

void ClockWork::setClock() {
    uhrzeit = 0;

    if (!G.Sprachvariation[NotShowItIs]) {
        usedUhrType->show(es_ist);
    }

    uint8_t offsetH = 0;
    uint8_t voll = 0;

    setMinute(_minute, offsetH, voll);
    setHour(_stunde + offsetH, voll);
}

//------------------------------------------------------------------------------

void ClockWork::showWeather() {

    switch (wetterswitch) {
        // +6h
    case 1: {
        switch (wstunde) {
        case 1:
            usedUhrType->show(w_mittag);
            break;
        case 2:
            usedUhrType->show(w_abend);
            break;
        case 3:
            usedUhrType->show(w_nacht);
            break;
        case 4: {
            usedUhrType->show(w_morgen);
            usedUhrType->show(w_frueh);
        } break;
        }
        switch (wtemp_6) {
        case 30: {
            usedUhrType->show(w_ueber);
            usedUhrType->show(w_dreissig);
            usedUhrType->show(w_grad);
        } break;
        case 25: {
            usedUhrType->show(w_ueber);
            usedUhrType->show(w_fuenf);
            usedUhrType->show(w_und);
            usedUhrType->show(w_zwanzig);
            usedUhrType->show(w_grad);
        } break;
        case 20: {
            usedUhrType->show(w_ueber);
            usedUhrType->show(w_zwanzig);
            usedUhrType->show(w_grad);
        } break;
        case 15: {
            usedUhrType->show(w_ueber);
            usedUhrType->show(w_fuenf);
            usedUhrType->show(w_zehn);
            usedUhrType->show(w_grad);
        } break;
        case 10: {
            usedUhrType->show(w_ueber);
            usedUhrType->show(w_zehn);
            usedUhrType->show(w_grad);
        } break;
        case 5: {
            usedUhrType->show(w_ueber);
            usedUhrType->show(w_fuenf);
            usedUhrType->show(w_grad);
        } break;
        case 1: {
            usedUhrType->show(w_ueber);
            usedUhrType->show(w_null);
            usedUhrType->show(w_grad);
        } break;
        case -1: {
            usedUhrType->show(w_unter);
            usedUhrType->show(w_minus);
            usedUhrType->show(w_null);
            usedUhrType->show(w_grad);
        } break;
        case -5: {
            usedUhrType->show(w_unter);
            usedUhrType->show(w_minus);
            usedUhrType->show(w_fuenf);
            usedUhrType->show(w_grad);
        } break;
        case -10: {
            usedUhrType->show(w_unter);
            usedUhrType->show(w_minus);
            usedUhrType->show(w_zehn);
            usedUhrType->show(w_grad);
        } break;
        case -15: {
            usedUhrType->show(w_unter);
            usedUhrType->show(w_minus);
            usedUhrType->show(w_fuenf);
            usedUhrType->show(w_zehn);
            usedUhrType->show(w_grad);
        } break;
        case -20: {
            usedUhrType->show(w_unter);
            usedUhrType->show(w_minus);
            usedUhrType->show(w_zwanzig);
            usedUhrType->show(w_grad);
        } break;
        case -25: {
            usedUhrType->show(w_unter);
            usedUhrType->show(w_minus);
            usedUhrType->show(w_fuenf);
            usedUhrType->show(w_und);
            usedUhrType->show(w_zwanzig);
            usedUhrType->show(w_grad);
        } break;
        }
        switch (wwetter_6) {
        case 200:
            usedUhrType->show(w_gewitter);
            break;
        case 300:
            usedUhrType->show(w_regen);
            break;
        case 500:
            usedUhrType->show(w_regen);
            break;
        case 600:
            usedUhrType->show(w_schnee);
            break;
        case 700:
            usedUhrType->show(w_warnung);
            break;
        case 800:
            usedUhrType->show(w_klar);
            break;
        case 801:
            usedUhrType->show(w_wolken);
            break;
        }
    } break;
        // +12h
    case 2: {
        switch (wstunde) {
        case 1:
            usedUhrType->show(w_abend);
            break;
        case 2:
            usedUhrType->show(w_nacht);
            break;
        case 3: {
            usedUhrType->show(w_morgen);
            usedUhrType->show(w_frueh);
        } break;
        case 4: {
            usedUhrType->show(w_morgen);
            usedUhrType->show(w_mittag);
        } break;
        }
        switch (wtemp_12) {
        case 30: {
            usedUhrType->show(w_ueber);
            usedUhrType->show(w_dreissig);
            usedUhrType->show(w_grad);
        } break;
        case 25: {
            usedUhrType->show(w_ueber);
            usedUhrType->show(w_fuenf);
            usedUhrType->show(w_und);
            usedUhrType->show(w_zwanzig);
            usedUhrType->show(w_grad);
        } break;
        case 20: {
            usedUhrType->show(w_ueber);
            usedUhrType->show(w_zwanzig);
            usedUhrType->show(w_grad);
        } break;
        case 15: {
            usedUhrType->show(w_ueber);
            usedUhrType->show(w_fuenf);
            usedUhrType->show(w_zehn);
            usedUhrType->show(w_grad);
        } break;
        case 10: {
            usedUhrType->show(w_ueber);
            usedUhrType->show(w_zehn);
            usedUhrType->show(w_grad);
        } break;
        case 5: {
            usedUhrType->show(w_ueber);
            usedUhrType->show(w_fuenf);
            usedUhrType->show(w_grad);
        } break;
        case 1: {
            usedUhrType->show(w_ueber);
            usedUhrType->show(w_null);
            usedUhrType->show(w_grad);
        } break;
        case -1: {
            usedUhrType->show(w_unter);
            usedUhrType->show(w_minus);
            usedUhrType->show(w_null);
            usedUhrType->show(w_grad);
        } break;
        case -5: {
            usedUhrType->show(w_unter);
            usedUhrType->show(w_minus);
            usedUhrType->show(w_fuenf);
            usedUhrType->show(w_grad);
        } break;
        case -10: {
            usedUhrType->show(w_unter);
            usedUhrType->show(w_minus);
            usedUhrType->show(w_zehn);
            usedUhrType->show(w_grad);
        } break;
        case -15: {
            usedUhrType->show(w_unter);
            usedUhrType->show(w_minus);
            usedUhrType->show(w_fuenf);
            usedUhrType->show(w_zehn);
            usedUhrType->show(w_grad);
        } break;
        case -20: {
            usedUhrType->show(w_unter);
            usedUhrType->show(w_minus);
            usedUhrType->show(w_zwanzig);
            usedUhrType->show(w_grad);
        } break;
        case -25: {
            usedUhrType->show(w_unter);
            usedUhrType->show(w_minus);
            usedUhrType->show(w_fuenf);
            usedUhrType->show(w_und);
            usedUhrType->show(w_zwanzig);
            usedUhrType->show(w_grad);
        } break;
        }
        switch (wwetter_12) {
        case 200:
            usedUhrType->show(w_gewitter);
            break;
        case 300:
            usedUhrType->show(w_regen);
            break;
        case 500:
            usedUhrType->show(w_regen);
            break;
        case 600:
            usedUhrType->show(w_schnee);
            break;
        case 700:
            usedUhrType->show(w_warnung);
            break;
        case 800:
            usedUhrType->show(w_klar);
            break;
        case 801:
            usedUhrType->show(w_wolken);
            break;
        }

    } break;
        // +18h
    case 3: {
        switch (wstunde) {
        case 1:
            usedUhrType->show(w_nacht);
            break;
        case 2: {
            usedUhrType->show(w_morgen);
            usedUhrType->show(w_frueh);
        } break;
        case 3: {
            usedUhrType->show(w_morgen);
            usedUhrType->show(w_mittag);
        } break;
        case 4: {
            usedUhrType->show(w_morgen);
            usedUhrType->show(w_abend);
        } break;
        }
        switch (wtemp_18) {
        case 30: {
            usedUhrType->show(w_ueber);
            usedUhrType->show(w_dreissig);
            usedUhrType->show(w_grad);
        } break;
        case 25: {
            usedUhrType->show(w_ueber);
            usedUhrType->show(w_fuenf);
            usedUhrType->show(w_und);
            usedUhrType->show(w_zwanzig);
            usedUhrType->show(w_grad);
        } break;
        case 20: {
            usedUhrType->show(w_ueber);
            usedUhrType->show(w_zwanzig);
            usedUhrType->show(w_grad);
        } break;
        case 15: {
            usedUhrType->show(w_ueber);
            usedUhrType->show(w_fuenf);
            usedUhrType->show(w_zehn);
            usedUhrType->show(w_grad);
        } break;
        case 10: {
            usedUhrType->show(w_ueber);
            usedUhrType->show(w_zehn);
            usedUhrType->show(w_grad);
        } break;
        case 5: {
            usedUhrType->show(w_ueber);
            usedUhrType->show(w_fuenf);
            usedUhrType->show(w_grad);
        } break;
        case 1: {
            usedUhrType->show(w_ueber);
            usedUhrType->show(w_null);
            usedUhrType->show(w_grad);
        } break;
        case -1: {
            usedUhrType->show(w_unter);
            usedUhrType->show(w_minus);
            usedUhrType->show(w_null);
            usedUhrType->show(w_grad);
        } break;
        case -5: {
            usedUhrType->show(w_unter);
            usedUhrType->show(w_minus);
            usedUhrType->show(w_fuenf);
            usedUhrType->show(w_grad);
        } break;
        case -10: {
            usedUhrType->show(w_unter);
            usedUhrType->show(w_minus);
            usedUhrType->show(w_zehn);
            usedUhrType->show(w_grad);
        } break;
        case -15: {
            usedUhrType->show(w_unter);
            usedUhrType->show(w_minus);
            usedUhrType->show(w_fuenf);
            usedUhrType->show(w_zehn);
            usedUhrType->show(w_grad);
        } break;
        case -20: {
            usedUhrType->show(w_unter);
            usedUhrType->show(w_minus);
            usedUhrType->show(w_zwanzig);
            usedUhrType->show(w_grad);
        } break;
        case -25: {
            usedUhrType->show(w_unter);
            usedUhrType->show(w_minus);
            usedUhrType->show(w_fuenf);
            usedUhrType->show(w_und);
            usedUhrType->show(w_zwanzig);
            usedUhrType->show(w_grad);
        } break;
        }
        switch (wwetter_18) {
        case 200:
            usedUhrType->show(w_gewitter);
            break;
        case 300:
            usedUhrType->show(w_regen);
            break;
        case 500:
            usedUhrType->show(w_regen);
            break;
        case 600:
            usedUhrType->show(w_schnee);
            break;
        case 700:
            usedUhrType->show(w_warnung);
            break;
        case 800:
            usedUhrType->show(w_klar);
            break;
        case 801:
            usedUhrType->show(w_wolken);
            break;
        }

    } break;
        // +24h
    case 4: {
        switch (wstunde) {
        case 1: {
            usedUhrType->show(w_morgen);
            usedUhrType->show(w_frueh);
        } break;
        case 2: {
            usedUhrType->show(w_morgen);
            usedUhrType->show(w_mittag);
        } break;
        case 3: {
            usedUhrType->show(w_morgen);
            usedUhrType->show(w_abend);
        } break;
        case 4: {
            usedUhrType->show(w_morgen);
            usedUhrType->show(w_nacht);
        } break;
        }
        switch (wtemp_24) {
        case 30: {
            usedUhrType->show(w_ueber);
            usedUhrType->show(w_dreissig);
            usedUhrType->show(w_grad);
        } break;
        case 25: {
            usedUhrType->show(w_ueber);
            usedUhrType->show(w_fuenf);
            usedUhrType->show(w_und);
            usedUhrType->show(w_zwanzig);
            usedUhrType->show(w_grad);
        } break;
        case 20: {
            usedUhrType->show(w_ueber);
            usedUhrType->show(w_zwanzig);
            usedUhrType->show(w_grad);
        } break;
        case 15: {
            usedUhrType->show(w_ueber);
            usedUhrType->show(w_fuenf);
            usedUhrType->show(w_zehn);
            usedUhrType->show(w_grad);
        } break;
        case 10: {
            usedUhrType->show(w_ueber);
            usedUhrType->show(w_zehn);
            usedUhrType->show(w_grad);
        } break;
        case 5: {
            usedUhrType->show(w_ueber);
            usedUhrType->show(w_fuenf);
            usedUhrType->show(w_grad);
        } break;
        case 1: {
            usedUhrType->show(w_ueber);
            usedUhrType->show(w_null);
            usedUhrType->show(w_grad);
        } break;
        case -1: {
            usedUhrType->show(w_unter);
            usedUhrType->show(w_minus);
            usedUhrType->show(w_null);
            usedUhrType->show(w_grad);
        } break;
        case -5: {
            usedUhrType->show(w_unter);
            usedUhrType->show(w_minus);
            usedUhrType->show(w_fuenf);
            usedUhrType->show(w_grad);
        } break;
        case -10: {
            usedUhrType->show(w_unter);
            usedUhrType->show(w_minus);
            usedUhrType->show(w_zehn);
            usedUhrType->show(w_grad);
        } break;
        case -15: {
            usedUhrType->show(w_unter);
            usedUhrType->show(w_minus);
            usedUhrType->show(w_fuenf);
            usedUhrType->show(w_zehn);
            usedUhrType->show(w_grad);
        } break;
        case -20: {
            usedUhrType->show(w_unter);
            usedUhrType->show(w_minus);
            usedUhrType->show(w_zwanzig);
            usedUhrType->show(w_grad);
        } break;
        case -25: {
            usedUhrType->show(w_unter);
            usedUhrType->show(w_minus);
            usedUhrType->show(w_fuenf);
            usedUhrType->show(w_und);
            usedUhrType->show(w_zwanzig);
            usedUhrType->show(w_grad);
        } break;
        }
        switch (wwetter_24) {
        case 200:
            usedUhrType->show(w_gewitter);
            break;
        case 300:
            usedUhrType->show(w_regen);
            break;
        case 500:
            usedUhrType->show(w_regen);
            break;
        case 600:
            usedUhrType->show(w_schnee);
            break;
        case 700:
            usedUhrType->show(w_warnung);
            break;
        case 800:
            usedUhrType->show(w_klar);
            break;
        case 801:
            usedUhrType->show(w_wolken);
            break;
        }

    } break;
    }
}

//------------------------------------------------------------------------------

void ClockWork::calcClockface() {
    uint8_t rr, gg, bb, ww;

    if (_stunde == 23 && _minute == 59 && _sekunde >= 50) {
        countdownToMidnight();
    } else {
        setClock();
    }

    // Set Brighness hour dependent
    if (_stunde < 6) {
        G.hh = G.h24;
    } else if (_stunde < 8) {
        G.hh = G.h6;
    } else if (_stunde < 12) {
        G.hh = G.h8;
    } else if (_stunde < 16) {
        G.hh = G.h12;
    } else if (_stunde < 18) {
        G.hh = G.h16;
    } else if (_stunde < 20) {
        G.hh = G.h18;
    } else if (_stunde < 22) {
        G.hh = G.h20;
    } else if (_stunde < 24) {
        G.hh = G.h22;
    }

    led.setBrightnessLdr(rr, gg, bb, ww, Background);

    // set Background color
    for (uint16_t i = 0; i < usedUhrType->NUM_PIXELS(); i++) {
        led.setPixel(rr, gg, bb, ww, i);
    }

    if (usedUhrType->hasWeatherLayout()) {
        showWeather();
    }
}

//------------------------------------------------------------------------------

iUhrType *ClockWork::getPointer(uint8_t type) {
    switch (type) {
    case Uhr_114:
        return &Uhr_114_type;
    case Uhr_114_Alternative:
        return &Uhr_114_Alternative_type;
    case Uhr_114_2Clock:
        return &Uhr_114_2Clock_type;
    case Uhr_114_Dutch:
        return &Uhr_114_dutch_type;
    case Uhr_125:
        return &Uhr_125_type;
    case Uhr_125_Type2:
        return &Uhr_125_type2_type;
    case Uhr_169:
        return &Uhr_169_type;
    case Uhr_242:
        return &Uhr_242_type;
    case Uhr_291:
        return &Uhr_291_type;
    default:
        return nullptr;
    }
}

//------------------------------------------------------------------------------

void ClockWork::initLedStrip(uint8_t num) {
    NeoMultiFeature::setColortype(num);
    if (num == Grbw) {
        if (strip_RGB != NULL) {
            delete strip_RGB; // delete the previous dynamically created strip
            strip_RGB = NULL;
        }
        if (strip_RGBW == NULL) {
            strip_RGBW = new NeoPixelBus<NeoGrbwFeature, Neo800KbpsMethod>(
                usedUhrType->NUM_PIXELS());
            strip_RGBW->Begin();
        }
    } else {
        if (strip_RGBW != NULL) {
            delete strip_RGBW; // delete the previous dynamically created strip
            strip_RGBW = NULL;
        }
        if (strip_RGB == NULL) {
            strip_RGB = new NeoPixelBus<NeoMultiFeature, Neo800KbpsMethod>(
                usedUhrType->NUM_PIXELS());
            strip_RGB->Begin();
        }
    }
}

//------------------------------------------------------------------------------

void ClockWork::loop(struct tm &tm) {
    unsigned long currentMillis = millis();
    count_delay += currentMillis - previousMillis;
    if (usedUhrType->hasSecondsFrame()) {
        count_millis48 += currentMillis - previousMillis;
    }
    previousMillis = currentMillis;

    // Faster runtime for demo
    animation->demoMode(_minute, _sekunde);

    //------------------------------------------------
    // SecondsFrame
    //------------------------------------------------
    if (usedUhrType->hasSecondsFrame()) {
        loopSecondsFrame();
    }

    //------------------------------------------------
    // Secounds and LDR Routine
    //------------------------------------------------
    if (last_sekunde != _sekunde) {

        if (DEBUG == true) {
            char currentTime[80];
            strftime(currentTime, sizeof(currentTime), "%F %T (%z)\n", &tm);
            Serial.printf(currentTime);
        }

        // Weather Clock
        if (usedUhrType->hasWeatherLayout()) {
            weather_tag++;
        }

        // LDR Routine
        if ((G.ldr == 1) || G.autoLdrEnabled) {
            loopLdrLogic();
        }

        if (G.prog == 0 && G.conf == 0) {
            led.clear();
            G.prog = COMMAND_MODE_WORD_CLOCK;
        }

        last_sekunde = _sekunde;

        if (usedUhrType->hasWeatherLayout()) {
            loopWeather();
        }
    }

    //------------------------------------------------
    // Minute
    //------------------------------------------------
    if (last_minute != _minute) {
        _sekunde48 = 0;
        last_minute = _minute;
    }

    //------------------------------------------------
    // Pull Waether Data
    //------------------------------------------------
    if (usedUhrType->hasWeatherLayout() &&
        weather_tag >= 600) { // @Eisbaeeer changed for Debug (should be 600)
        weather_tag = 0;
        if (WiFi.status() == WL_CONNECTED) {
            getweather();
        }
    }

    switch (G.conf) {

    case COMMAND_RESET: {
        delay(500);
        ESP.reset();
        ESP.restart();
        while (true) {
        }
        break;
    }

    case COMMAND_REQUEST_MQTT_VALUES: {
        DynamicJsonDocument config(1024);
        config["command"] = "mqtt";
        config["MQTT_State"] = G.mqtt.state;
        config["MQTT_Port"] = G.mqtt.port;
        config["MQTT_Server"] = G.mqtt.serverAdress;
        config["MQTT_User"] = G.mqtt.user;
        config["MQTT_Pass"] = G.mqtt.password;
        config["MQTT_ClientId"] = G.mqtt.clientId;
        config["MQTT_Topic"] = G.mqtt.topic;
        serializeJson(config, str);
        Serial.print("Sending Payload:");
        Serial.println(str);
        webSocket.sendTXT(G.client_nr, str, strlen(str));
        G.conf = COMMAND_IDLE;
        break;
    }

    case COMMAND_REQUEST_CONFIG_VALUES: {
        DynamicJsonDocument config(1024);
        config["command"] = "config";
        config["ssid"] = Network_getSSID();
        config["zeitserver"] = G.zeitserver;
        config["hostname"] = G.hostname;
        config["ltext"] = G.ltext;
        config["h6"] = G.h6;
        config["h8"] = G.h8;
        config["h12"] = G.h12;
        config["h16"] = G.h16;
        config["h18"] = G.h18;
        config["h20"] = G.h20;
        config["h22"] = G.h22;
        config["h24"] = G.h24;
        for (uint8_t i = 0; i < 5; i++) {
            sprintf(s, "spv%d", i);
            config[s] = static_cast<uint8_t>(G.Sprachvariation[i]);
        }
        config["hell"] = G.hell;
        config["zeige_sek"] = G.zeige_sek;
        config["zeige_min"] = G.zeige_min;
        config["ldr"] = G.ldr;
        config["ldrCal"] = G.ldrCal;
        config["cityid"] = G.openWeatherMap.cityid;
        config["apikey"] = G.openWeatherMap.apikey;
        config["colortype"] = G.Colortype;
        config["UhrtypeDef"] = G.UhrtypeDef;
        config["bootLedBlink"] = G.bootLedBlink;
        config["bootLedSweep"] = G.bootLedSweep;
        config["bootShowWifi"] = G.bootShowWifi;
        config["bootShowIP"] = G.bootShowIP;
        config["autoLdrEnabled"] = G.autoLdrEnabled;
        config["hasDreiviertel"] = usedUhrType->hasDreiviertel();
        config["hasZwanzig"] = usedUhrType->hasZwanzig();
        config["hasWeatherLayout"] = usedUhrType->hasWeatherLayout();
        config["hasSecondsFrame"] = usedUhrType->hasSecondsFrame();
        serializeJson(config, str);
        Serial.print("Sending Payload:");
        Serial.println(str);
        webSocket.sendTXT(G.client_nr, str, strlen(str));
        G.conf = COMMAND_IDLE;
        break;
    }

    case COMMAND_REQUEST_COLOR_VALUES: {
        DynamicJsonDocument config(1024);
        config["command"] = "set";
        for (uint8_t i = 0; i < 4; i++) {
            for (uint8_t ii = 0; ii < 4; ii++) {
                sprintf(s, "rgb%d%d", i, ii);
                config[s] = G.rgb[i][ii];
            }
        }
        config["hell"] = G.hell;
        config["geschw"] = G.geschw;
        config["colortype"] = G.Colortype;
        config["prog"] = G.prog;
        serializeJson(config, str);
        webSocket.sendTXT(G.client_nr, str, strlen(str));
        G.conf = COMMAND_IDLE;
        break;
    }

    case COMMAND_REQUEST_AUTO_LDR: {
        DynamicJsonDocument config(1024);
        config["command"] = "autoLdr";
        if (G.param1 == 0) {
            config["autoLdrEnabled"] = G.autoLdrEnabled;
            config["autoLdrBright"] = G.autoLdrBright;
            config["autoLdrDark"] = G.autoLdrDark;
        }
        config["autoLdrValue"] = map(analogRead(A0), 0, 1023, 0, 255);
        serializeJson(config, str);
        webSocket.sendTXT(G.client_nr, str, strlen(str));
        G.conf = COMMAND_IDLE;
        break;
    }

    case COMMAND_REQUEST_ANIMATION: {
        DynamicJsonDocument config(1024);
        config["command"] = "animation";
        config["animType"] = G.animType;
        config["animDuration"] = G.animDuration;
        config["animSpeed"] = G.animSpeed;
        config["animDemo"] = G.animDemo;
        config["animColorize"] = G.animColorize;
        JsonArray types = config.createNestedArray("animTypes");
        // Sequence must match to 'enum Anim'
        types.add("keine");
        types.add("Hoch rollen");
        types.add("Runter rollen");
        types.add("Links schieben");
        types.add("Rechts schieben");
        types.add("Überblenden");
        types.add("Laser");
        types.add("Matrix");
        types.add("Baelle");
        types.add("Feuerwerk");
        types.add("Schlange");
        types.add("zufällig");
        serializeJson(config, str);
        webSocket.sendTXT(G.client_nr, str, strlen(str));
        G.conf = COMMAND_IDLE;
        break;
    }

    case COMMAND_SET_TIME:
    case COMMAND_SET_INITIAL_VALUES:
    case COMMAND_SET_WEATHER_DATA:
    case COMMAND_SET_MARQUEE_TEXT:
    case COMMAND_SET_BOOT: {
        eeprom_write();
        delay(100);
        G.conf = COMMAND_IDLE;
        break;
    }

    case COMMAND_SET_MINUTE:
    case COMMAND_SET_BRIGHTNESS:
    case COMMAND_SET_LDR:
    case COMMAND_SET_AUTO_LDR:
    case COMMAND_SET_LANGUAGE_VARIANT:
    case COMMAND_SET_SETTING_SECOND:
    case COMMAND_SET_TIME_MANUAL: {
        eeprom_write();
        led.clear();
        parameters_changed = true;
        G.conf = COMMAND_IDLE;
        break;
    }

    case COMMAND_SET_MQTT: {
        if (G.mqtt.state && !mqtt.getConnected()) {
            mqtt.reInit();
        }

        eeprom_write();
        G.conf = COMMAND_IDLE;
        break;
    }

    case COMMAND_SET_COLORTYPE: {
        // G.param1 sets new Colortype
        Serial.printf("LED Colortype: %u\n", G.param1);

        // if ((G.param1 != G.Colortype) && ((G.param1 == Grbw) ||
        //    (G.Colortype == Grbw))) {
        //    G.conf = COMMAND_RESET;
        // } else {
        G.conf = COMMAND_IDLE;
        // }

        // the G.Colortype must be called at the same time as initLedStrip,
        // otherwise it is referenced via a null-pointer.
        G.Colortype = G.param1;
        eeprom_write();
        initLedStrip(G.Colortype);
        led.setIcon(RGB_I, 100, true);
        break;
    }

    case COMMAND_SET_UHRTYPE: {
        eeprom_write();
        Serial.printf("Uhrtype: %u\n", G.UhrtypeDef);
        usedUhrType = getPointer(G.UhrtypeDef);
        G.conf = COMMAND_IDLE;
        break;
    }

    case COMMAND_SET_HOSTNAME: {
        Serial.print("Hostname: ");
        Serial.println(G.hostname);
        eeprom_write();
        Network_reboot();
        G.conf = COMMAND_IDLE;
        break;
    }

    case COMMAND_SET_WIFI_DISABLED: {
        eeprom_write();
        delay(100);
        Serial.println("Conf: WLAN off");
        Network_disable();
        G.conf = COMMAND_IDLE;
        break;
    }

    case COMMAND_SET_WIFI_AND_RESTART: {
        Serial.println("Conf: new Wifi Config");
        Network_resetSettings();
        G.conf = COMMAND_IDLE;
        break;
    }

    default:
        break;
    }

    switch (G.prog) {

    case COMMAND_MODE_SECONDS: {
        if (G.prog_init == 1) {
            led.clear();
            G.prog_init = 0;
        }
        char d1[5];
        char d2[5];
        sprintf(d1, "%d", (int)(_sekunde / 10));
        sprintf(d2, "%d", (int)(_sekunde % 10));
        led.showNumbers(d1[0], d2[0]);
        break;
    }

    case COMMAND_MODE_SCROLLINGTEXT:
    case COMMAND_MODE_RAINBOWCYCLE:
    case COMMAND_MODE_RAINBOW: {
        if (G.prog_init == 1) {
            G.prog_init = 0;
            led.clear();
            count_delay = (11u - G.geschw) * 30u;
        }
        if (count_delay >= (11u - G.geschw) * 30u) {
            switch (G.prog) {
            case COMMAND_MODE_SCROLLINGTEXT: {
                scrollingText(G.ltext);
                break;
            }
            case COMMAND_MODE_RAINBOWCYCLE: {
                rainbowCycle();
                break;
            }
            case COMMAND_MODE_RAINBOW: {
                rainbow();
                break;
            }
            default:
                break;
            }
            count_delay = 0;
        }
        break;
    }

    case COMMAND_MODE_COLOR: {
        if (G.prog_init == 1) {
            G.prog_init = 0;
            led.setColor();
            led.show();
        }
        break;
    }

    case COMMAND_MODE_ANIMATION: {
        if (G.prog_init == 1) {
            G.prog_init = 0;
            eeprom_write();
            delay(100);
        }
        // There is no break here on purpose, directly after the call
        // COMMAND_MODE_ANIMATION, COMMAND_MODE_WORD_CLOCK must be called
    }

    case COMMAND_MODE_WORD_CLOCK: {
        calcClockface();

        if (changesInClockface()) {
            copyClockface(Word_array, Word_array_old);
            led.set(true);
        } else if (parameters_changed) {
            led.set();
        }
        parameters_changed = false;

        if (usedUhrType->hasSecondsFrame() && G.zeige_sek < 1 &&
            G.zeige_min < 2) {
            led.setFrameColor();
        }
        G.prog = COMMAND_IDLE;
    }
    default:
        break;
    }

    if (count_delay > 10000) {
        count_delay = 0;
    }
}

//------------------------------------------------------------------------------

void ClockWork::loopSecondsFrame() {
    if (count_millis48 >= interval48) {
        count_millis48 = 0;
        _sekunde48++;
        if (_sekunde48 > 47) {
            _sekunde48 = 0;
        }
    }
    if (last_sekunde48 != _sekunde48) {
        if (G.prog == 0 && G.conf == 0) {
            if (G.zeige_sek == 1 || G.zeige_min == 2) {
                led.clearFrame();
            }
            if (G.zeige_sek > 0) {
                led.showSeconds();
            }
            led.show();
        }
        last_sekunde48 = _sekunde48;
    }
}

//------------------------------------------------------------------------------

void ClockWork::loopWeather() {
    if ((_sekunde == 0) | (_sekunde == 10) | (_sekunde == 20) |
        (_sekunde == 30) | (_sekunde == 40) | (_sekunde == 50)) {
        wetterswitch++;
        led.clear();
        if (wetterswitch > 4) {
            wetterswitch = 1;
        }
        Serial.print("Wetterswitch: ");
        Serial.println(wetterswitch);
        Serial.print("WStunde: ");
        Serial.println(wstunde);
    }
}

//------------------------------------------------------------------------------

void ClockWork::initBootLedBlink() {
    led.setAllPixels(50, 50, 50, 50);
    led.show();
}

//------------------------------------------------------------------------------

void ClockWork::initBootLedSweep(uint32_t delay) { led.setSingle(delay); }

//------------------------------------------------------------------------------

void ClockWork::initBootLed() {
    led.clear();
    led.show();
}

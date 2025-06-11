#include "NeoMultiFeature.hpp"
#include "Transitiontypes/Transition.h"
#include "Uhr.h"
#include "Uhrtypes/Uhrtype.hpp"
#include "clockWork.h"
#include "openwmap.h"
#include <Arduino.h>
#if AUTOBRIGHT_USE_BH1750
#include <BH1750.h>
BH1750 lightMeter(0x23);
bool bh1750Initialized;
#endif

OpenWMap weather;

//------------------------------------------------------------------------------
// Helper Functions
//------------------------------------------------------------------------------

// Automatic Brightness Control (enabled/disabled by G.autoBrightEnabled)
// 1. Measure ambient light with high resolution sensor BH1750, if available.
//    If not, use legacy LDR.
// 2. Derive the ledGain for the LEDs 0.0 - 100.0%
// 3. When ledGain changed, set parametersChanged = true
// Inputs:
//   G.autoBrightEnabled, G.autoBrightMin, G.autoBrightMax, G.autoBrightPeak
//   AUTOBRIGHT_LDR_RESBRIGHT, AUTOBRIGHT_LDR_RESDARK, AUTOBRIGHT_LDR_RESDIVIDER
// Outputs:
//  lux = Ambient light [LUX]
//  ledGain = gain for the the LEDs: 0.0-100.0% (Gain means n % of the
//    configured brightness: effectBri)
void ClockWork::loopAutoBrightLogic() {
    if (G.autoBrightEnabled != 1)
        return;

    if (G.autoBrightMin == G.autoBrightMax) {
        // If min and max are identical, nothing needs to be measured...
        // Besides: map() would crash with division by zero in this case
        ledGain = G.autoBrightMax;
        return;
    }

    float ledGainOld = ledGain;
    float luxNow = -1.0;

#if AUTOBRIGHT_USE_BH1750
    if (bh1750Initialized && lightMeter.measurementReady()) {
        luxNow = lightMeter.readLightLevel(); // 0.0-54612.5 LUX
    }
#endif

#if AUTOBRIGHT_USE_LDR
    // only use LDR, if no value from BH1750 is available
    if (luxNow < 0) {
        uint16_t adcValue = analogRead(A0); // Range 0-1023 (1024 on overflow)

        // The lux value is considerably misrepresented upwards at ADC values
        // above 980. As 980 with an LDR5528 corresponds to approx. 1500 lux,
        // but usually no more than 1000 lux are reached indoors even on sunny
        // days, a fixed upper measurement limit of 980 is set here.
        // Regardless of this, a maximum value of 1023 has to be applied in
        // any case in order to avoid division by zero (analogRead is 0-1024)!
        if (adcValue > 980) {
            adcValue = 980;
        }

        luxNow = (adcValue * AUTOBRIGHT_LDR_RESDARK * 10) /
                 (AUTOBRIGHT_LDR_RESBRIGHT * AUTOBRIGHT_LDR_RESDIVIDER *
                  (1024 - adcValue));
    }
#endif

    // If luxNow is still negative, no data could be retrieved from BH1750 or
    // LDR. We return to preserve the previous ledGain (potentially default).
    // Otherwise we may end up in a blinking light...
    if (luxNow < 0) {
        return;
    }

    // Even in HIGH_RES_MODE, the BH1750 only has a precision of 1 lux. The
    // decimal point values supplied by the library should therefore be
    // considered as 'noise' and their use makes little to no sense.
    // Also: in order to not trigger parametersChanged more often than
    // necessary, the value gets rounded.
    lux = round(luxNow);

    ledGain = map(lux, 0, G.autoBrightPeak, G.autoBrightMin, G.autoBrightMax);

    // Ensure ledGain meets the defined limits.
    uint16_t minimum = min(G.autoBrightMin, G.autoBrightMax);
    uint16_t maximum = max(G.autoBrightMin, G.autoBrightMax);

    if (ledGain >= maximum) {
        ledGain = maximum;
    }
    if (ledGain <= minimum) {
        ledGain = minimum;
    }

    if (ledGainOld != ledGain) {
        parametersChanged = true;
    }
}

//------------------------------------------------------------------------------

iUhrType *ClockWork::getPointer(uint8_t type) {
    switch (type) {
    case Ger10x11:
        return &_de10x11;
    case Ger10x11Alternative:
        return &_de10x11Alternative;
    case Ger10x11AlternativeFrame:
        return &_de10x11AlternativeFrame;
    case Ger10x11Clock:
        return &_de10x11Clock;
    case Ger10x11Nero:
        return &_de10x11Nero;
    case Ger10x11NeroFrame:
        return &_de10x11NeroFrame;
    case Ger10x11bayerisch:
        return &_de10x11bayerisch;
    case Nl10x11:
        return &_nl10x11;
    case Ger11x11:
        return &_de11x11;
    case Ger11x11V2:
        return &_de11x11V2;
    case Ger11x11V3:
        return &_de11x11V3;
    case Ger13x13:
        return &_de13x13;
    case Ger22x11Weather:
        return &_de22x11Weather;
    case Ger16x8:
        return &_de16x8;
    case Ger16x18:
        return &_de16x18;
    case Ger08x08:
        return &_de08x08;
    case Ger08x08Viertel:
        return &_de08x08Viertel;
    case Eng08x08:
        return &_en08x08;
    case Eng10x11:
        return &_en10x11;
    case Eng11x12:
        return &_en11x12;
    case Es10x11:
        return &_es10x11;
    case Es08x08Cuarto:
        return &_es08x08Cuarto;
    case It10x11:
        return &_it10x11;
    case Hu10x10:
        return &_hu10x10;
    case Ch10x11:
        return &_ch10x11;
    case Ro10x11:
        return &_ro10x11;
    case Ger10x11schwaebisch:
        return &_de10x11schwaebisch;
    case Fr10x11:
        return &_fr10x11;
    case Se10x11:
        return &_se10x11;
    case Ru10x11:
        return &_ru10x11;
    case Ch10x11V2:
        return &_ch10x11v2;
    default:
        return nullptr;
    }
}

//------------------------------------------------------------------------------

void ClockWork::initLedStrip(uint8_t num) {
    NeoMultiFeature::setColortype(num);
    if (num == Grbw) {
        if (strip_RGB != NULL) {
            delete strip_RGB; // delete the previous dynamically created
                              // strip
            strip_RGB = NULL;
        }
        if (strip_RGBW == NULL) {
#ifdef ESP8266
            strip_RGBW = new NeoPixelBus<NeoGrbwFeature, Neo800KbpsMethod>(500);
#elif defined(ESP32)
            pinMode(LED_PIN, OUTPUT);
            strip_RGBW =
                new NeoPixelBus<NeoGrbwFeature, NeoSk6812Method>(500, LED_PIN);
#endif
            strip_RGBW->Begin();
        }
    } else {
        if (strip_RGBW != NULL) {
            delete strip_RGBW; // delete the previous dynamically created
                               // strip
            strip_RGBW = NULL;
        }
        if (strip_RGB == NULL) {
#ifdef ESP8266
            strip_RGB = new NeoPixelBus<NeoMultiFeature, Neo800KbpsMethod>(500);
#elif defined(ESP32)
            pinMode(LED_PIN, OUTPUT);
            strip_RGB = new NeoPixelBus<NeoMultiFeature, NeoWs2812xMethod>(
                500, LED_PIN);
#endif
            strip_RGB->Begin();
        }
    }
}

//------------------------------------------------------------------------------

uint32_t ClockWork::num32BitWithOnesAccordingToColumns() {
    return pow(2, usedUhrType->colsWordMatrix()) - 1;
}

//------------------------------------------------------------------------------

bool ClockWork::isRomanLanguage() {
    return usedUhrType->usedLang() == LanguageAbbreviation::ES ||
           usedUhrType->usedLang() == LanguageAbbreviation::IT;
}

//------------------------------------------------------------------------------
// Front Effect Functions
//------------------------------------------------------------------------------

void ClockWork::rainbow() {
    static uint16_t hue = 0;

    for (uint8_t row = 0; row < usedUhrType->rowsWordMatrix(); row++) {
        for (uint8_t col = 0; col < usedUhrType->colsWordMatrix(); col++) {
            led.setPixel(row, col,
                         HsbColor(hue / 360.f, 1.f, G.effectBri / 100.f));
        }
    }
    led.show();
    hue++;
    led.checkIfHueIsOutOfBound(hue);
}

//-----------------------------------------------------------------------------

void ClockWork::rainbowCycle() {
    static uint16_t hue = 0;
    uint16_t numPixelsWordMatrix =
        usedUhrType->rowsWordMatrix() * usedUhrType->colsWordMatrix();
    uint16_t displayedHue;

    displayedHue = hue;
    for (uint8_t row = 0; row < usedUhrType->rowsWordMatrix(); row++) {
        for (uint8_t col = 0; col < usedUhrType->colsWordMatrix(); col++) {
            led.setPixel(
                row, col,
                HsbColor(displayedHue / 360.f, 1.f, G.effectBri / 100.f));
            displayedHue = displayedHue + 360.f / numPixelsWordMatrix;
            led.checkIfHueIsOutOfBound(displayedHue);
        }
    }
    led.show();
    hue++;
    led.checkIfHueIsOutOfBound(hue);
}

//------------------------------------------------------------------------------

void ClockWork::scrollingText(const char *buf) {
    static uint8_t i = 0, ii = 0;
    uint8_t offsetRow = (usedUhrType->rowsWordMatrix() -
                         pgm_read_byte(&(fontHeight[normalSizeASCII]))) /
                        2;
    uint8_t fontIndex = buf[ii];

    led.setbyFrontMatrix(Foreground); // Needed for Mirrored Display
    led.shiftColumnToRight();
    led.clearFrontExeptofFontspace(offsetRow);

    if (i < pgm_read_byte(&(fontWidth[normalSizeASCII]))) {
        for (uint8_t row = 0;
             row < pgm_read_byte(&(fontHeight[normalSizeASCII])); row++) {
            usedUhrType->setFrontMatrixPixel(
                row + offsetRow, 0,
                pgm_read_byte(&(font_7x5[fontIndex][i])) & (1u << row));
        }
    } else {
        for (uint8_t row = 0;
             row < pgm_read_byte(&(fontHeight[normalSizeASCII])); row++) {
            usedUhrType->setFrontMatrixPixel(row + offsetRow, 0, false);
        }
    }

    led.setbyFrontMatrix(Foreground);
    led.show();

    i++;
    if (i >= pgm_read_byte(&(fontWidth[normalSizeASCII])) + 1) // +1 for spacing
    {
        i = 0;
        ii++;
        if (ii >= strlen(buf)) {
            ii = 0;
        }
    }
}

//------------------------------------------------------------------------------

void ClockWork::displaySymbols(uint8_t iconNum) {
    static uint8_t count = 0;

    switch (iconNum) {
    case HEART:
        /* Heartbeat begin */
        if (count < 10) {
            G.color[Foreground].B += 0.03;
            if (G.color[Foreground].B > 1) {
                G.color[Foreground].B = 1;
            }
            count++;
        } else if (count < 20) {
            G.color[Foreground].B -= 0.03;
            if (G.color[Foreground].B <= 0) {
                G.color[Foreground].B = 0;
            }
            count++;
        } else {
            count = 0;
        }
        /* Heartbeat end */
        led.setIcon(iconNum);
        break;

    case SMILEY:
        led.setIcon(iconNum);
        break;

    case NOTE:
        led.setIcon(iconNum);
        break;

    case SNOW:
        led.setIcon(iconNum);
        break;

    default:
        break;
    }
}

//------------------------------------------------------------------------------

void ClockWork::countdownToMidnight() {
    Serial.printf("Count down: %d\n", 60 - _second);
    switch (_second) {
    case 50:
        if (!usedUhrType->hasOnlyQuarterLayout()) {
            usedUhrType->show(FrontWord::min_10);
        } else {
            usedUhrType->show(FrontWord::hour_10);
        }
        break;
    case 51:
        usedUhrType->show(FrontWord::hour_9);
        break;
    case 52:
        usedUhrType->show(FrontWord::hour_8);
        break;
    case 53:
        usedUhrType->show(FrontWord::hour_7);
        break;
    case 54:
        usedUhrType->show(FrontWord::hour_6);
        break;
    case 55:
        if (!usedUhrType->hasOnlyQuarterLayout()) {
            usedUhrType->show(FrontWord::min_5);
        } else {
            usedUhrType->show(FrontWord::hour_5);
        }
        break;
    case 56:
        usedUhrType->show(FrontWord::hour_4);
        break;
    case 57:
        usedUhrType->show(FrontWord::hour_3);
        break;
    case 58:
        usedUhrType->show(FrontWord::hour_2);
        break;
    case 59:
        if (usedUhrType->usedLang() == LanguageAbbreviation::DE) {
            usedUhrType->show(FrontWord::eins);
        } else {
            usedUhrType->show(FrontWord::hour_1);
        }
        break;
    default:
        break;
    }
}

//------------------------------------------------------------------------------
// Boot Functions
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

void ClockWork::initBootLedBlink() {
    for (uint8_t row = 0; row < usedUhrType->rowsWordMatrix(); row++) {
        frontMatrix[row] ^= num32BitWithOnesAccordingToColumns();
    }
    led.setbyFrontMatrix(Foreground);
    led.show();
}

//------------------------------------------------------------------------------

void ClockWork::initBootLedSweep(uint32_t delay) { led.setSingle(delay); }

//------------------------------------------------------------------------------

void ClockWork::initBootLed() {
    led.clear();
    led.show();
}

//------------------------------------------------------------------------------
// Minute Functions
//------------------------------------------------------------------------------

uint8_t ClockWork::determineWhichMinuteVariant() {
    switch (G.minuteVariant) {
    case MinuteVariant::Off:
        return 0;
        break;
    case MinuteVariant::LED4x:
        return 0;
        break;
    case MinuteVariant::LED7x:
        return 1;
        break;
    case MinuteVariant::Corners:
        return 2;
        break;
    case MinuteVariant::InWords:
        return 0;
        break;
    default:
        Serial.println("[ERROR] G.minuteVariant undefined");
        return 0;
        break;
    }
}

//------------------------------------------------------------------------------

void ClockWork::showSpecialWordBeen(const uint8_t min) {
    if (usedUhrType->hasSpecialWordBeen()) {
        if (min == 0) {
            usedUhrType->show(FrontWord::nur);
        } else {
            usedUhrType->show(FrontWord::gewesen);
        }
    }
}

//------------------------------------------------------------------------------

bool ClockWork::checkIfClockHasMinuteInWordsAndItIsSet() {
    return usedUhrType->hasMinuteInWords() &&
           G.minuteVariant == MinuteVariant::InWords;
}

//------------------------------------------------------------------------------

void ClockWork::showMinuteInWords(const uint8_t min) {
    if (checkIfClockHasMinuteInWordsAndItIsSet()) {
        if (min > 0) {
            usedUhrType->show(FrontWord::plus);
            usedUhrType->show(FrontWord::minute);
            if (min % 5 > 1) {
                usedUhrType->show(FrontWord::minuten);
            }
        }
        switch (min) {
        case 0:
            break;
        case 1:
            usedUhrType->show(FrontWord::m_num1);
            break;
        case 2:
            usedUhrType->show(FrontWord::m_num2);
            break;
        case 3:
            usedUhrType->show(FrontWord::m_num3);
            break;
        case 4:
            usedUhrType->show(FrontWord::m_num4);
            break;

        default:
            break;
        }
    }
}

//------------------------------------------------------------------------------

void ClockWork::showMinute(uint8_t min) {
    /* To prevent errors if min is not in Range of [0-4] */
    min %= 5;

    /* saving corosponding minutePixelArray */
    usedUhrType->getMinuteArray(minutePixelArray,
                                determineWhichMinuteVariant());
    /* Reseting minute byte */
    minuteArray = 0;
    if (usedUhrType->hasMinuteInWords() && min > 0) {
        minuteArray |= 1UL << (min - 1);
    } else {
        for (uint8_t i = 0; i < min; i++) {
            /* Shifting a 1 Bit for every pixel starting at LSB */
            minuteArray |= 1UL << i;
        }
    }
    /* Show "Been" according to Uhrtypedef */
    showSpecialWordBeen(min);

    /* Show Minute "In Words" according to Uhrtypedef */
    showMinuteInWords(min);
}

//------------------------------------------------------------------------------

void ClockWork::resetMinVariantIfNotAvailable() {
    if (G.UhrtypeDef != Ger16x8 && G.minuteVariant == MinuteVariant::InWords) {
        G.minuteVariant = MinuteVariant::Off;
    } else if (usedUhrType->rowsWordMatrix() != 11 &&
               G.minuteVariant == MinuteVariant::Corners) {
        G.minuteVariant = MinuteVariant::LED4x;
    }
}

//------------------------------------------------------------------------------

FrontWord ClockWork::getFrontWordForNum(uint8_t min) {
    if (min == 0 || min > 59) {
        Serial.println("[Error] getFrontWordForNum() out of Bounds");
        return FrontWord::error;
    } else {
        return static_cast<FrontWord>(min);
    }
}

//------------------------------------------------------------------------------

bool ClockWork::checkTwentyUsage() {
    return G.languageVariant[ItIs20] || G.languageVariant[ItIs40] ||
           usedUhrType->usedLang() == LanguageAbbreviation::ES;
}

//------------------------------------------------------------------------------

bool ClockWork::hasTwentyAndCheckForUsage() {
    // ToDo: Is this true for every supoorted language variant?
    return usedUhrType->hasTwenty() && checkTwentyUsage();
}

//------------------------------------------------------------------------------

bool ClockWork::hasDreiviertelAndCheckForUsage() {
    if (usedUhrType->hasDreiviertel()) {
        if (usedUhrType->usedLang() != LanguageAbbreviation::DE) {
            return true;
        } else if (G.languageVariant[ItIs45]) {
            return true;
        }
    }
    return false;
}

//------------------------------------------------------------------------------

void ClockWork::setMinute(uint8_t min, uint8_t &offsetHour, bool &fullHour) {
    if (usedUhrType->has60MinuteLayout()) {
        usedUhrType->show(FrontWord::uhr);

        if (min == 0) {
            fullHour = true;
        } else if (min > 0) {
            usedUhrType->show(getFrontWordForNum(min));
            usedUhrType->show(FrontWord::und);
        }

        if (min == 1) {
            usedUhrType->show(FrontWord::minute);
        } else if (min > 1) {
            usedUhrType->show(FrontWord::minuten);
        }
    } else if (usedUhrType->hasOnlyQuarterLayout()) {

        if (8 <= min && min <= 22) {
            if (G.languageVariant[ItIs15]) {
                usedUhrType->show(FrontWord::viertel);
                offsetHour = 1;
            } else {
                // A Quarter past
                if (G.languageVariant[EN_ShowAQuarter]) {
                    usedUhrType->show(FrontWord::a_quarter);
                }
                usedUhrType->show(FrontWord::viertel);
                usedUhrType->show(FrontWord::v_nach);
            }
        } else if (23 <= min && min <= 37) { // half
            if (G.UhrtypeDef == Eng10x11 || G.UhrtypeDef == It10x11 ||
                G.UhrtypeDef == Es10x11 || G.UhrtypeDef == Ro10x11) {
                usedUhrType->show(FrontWord::halb);
                usedUhrType->show(FrontWord::nach);
            } else {
                if (G.UhrtypeDef == Fr10x11 || G.UhrtypeDef == Ru10x11) {
                    usedUhrType->show(FrontWord::halb);
                } else {
                    usedUhrType->show(FrontWord::halb);
                    offsetHour = 1;
                }
            }
        } else if (38 <= min && min <= 52) { // quarter to
            if (hasDreiviertelAndCheckForUsage()) {
                usedUhrType->show(FrontWord::dreiviertel);
            } else {
                // A Quarter to
                if (G.languageVariant[EN_ShowAQuarter]) {
                    usedUhrType->show(FrontWord::a_quarter);
                }
                usedUhrType->show(FrontWord::viertel);
                usedUhrType->show(FrontWord::v_vor);
            }
            offsetHour = 1;
        } else if (53 <= min && min <= 59) { // almost full hour
            offsetHour = 1;
        }
    } else {
        showMinute(min);
        min /= 5;
        min *= 5;

        switch (min) {
        case 0: // full hour
            usedUhrType->show(FrontWord::uhr);
            fullHour = true;
            break;
        case 1:
            usedUhrType->show(FrontWord::min_1);
            usedUhrType->show(FrontWord::minute);
            usedUhrType->show(FrontWord::nach);
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
            usedUhrType->show(getFrontWordForNum(min));
            if (G.UhrtypeDef != Fr10x11) {
                usedUhrType->show(FrontWord::nach);
            }
            break;
        case 11:
        case 12:
        case 13:
        case 14:
            usedUhrType->show(getFrontWordForNum(min));
            usedUhrType->show(FrontWord::nach);
            break;
        case 15: // quarter past
            if (G.languageVariant[ItIs15]) {
                usedUhrType->show(FrontWord::viertel);
                offsetHour = 1;
            } else {
                // A Quarter past
                if (G.languageVariant[EN_ShowAQuarter]) {
                    usedUhrType->show(FrontWord::a_quarter);
                }
                usedUhrType->show(FrontWord::viertel);
                usedUhrType->show(FrontWord::v_nach);
            }
            break;
        case 16:
        case 17:
        case 18:
        case 19:
            usedUhrType->show(getFrontWordForNum(min));
            usedUhrType->show(FrontWord::nach);
            break;
        case 20:
            if (hasTwentyAndCheckForUsage()) {
                usedUhrType->show(FrontWord::min_20);
                if (G.UhrtypeDef != Fr10x11) {
                    usedUhrType->show(FrontWord::nach);
                }
            } else {
                usedUhrType->show(FrontWord::min_10);
                usedUhrType->show(FrontWord::vor);
                usedUhrType->show(FrontWord::halb);
                offsetHour = 1;
            }
            break;
        case 21:
        case 22:
        case 23:
        case 24:
        case 25:
            if (usedUhrType->hasTwentyfive()) {
                usedUhrType->show(FrontWord::min_25);
                if (G.UhrtypeDef != Fr10x11) {
                    usedUhrType->show(FrontWord::nach);
                }
            } else {
                usedUhrType->show(FrontWord::min_5);
                usedUhrType->show(FrontWord::vor);
                usedUhrType->show(FrontWord::halb);
                offsetHour = 1;
            }
            break;
        case 26:
        case 27:
        case 28:
            usedUhrType->show(getFrontWordForNum(30 - min));
            usedUhrType->show(FrontWord::vor);
            usedUhrType->show(FrontWord::halb);
            offsetHour = 1;
            break;
        case 29:
            usedUhrType->show(FrontWord::min_1);
            usedUhrType->show(FrontWord::minute);
            usedUhrType->show(FrontWord::vor);
            usedUhrType->show(FrontWord::halb);
            offsetHour = 1;
            break;
        case 30: // half
            if (G.UhrtypeDef == Eng10x11 || G.UhrtypeDef == Eng08x08 ||
                G.UhrtypeDef == It10x11 || G.UhrtypeDef == Ro10x11 ||
                G.UhrtypeDef == Es10x11 || G.UhrtypeDef == Es08x08Cuarto) {
                usedUhrType->show(FrontWord::halb);
                usedUhrType->show(FrontWord::nach);
            } else {
                if (G.UhrtypeDef == Fr10x11 || G.UhrtypeDef == Ru10x11) {
                    usedUhrType->show(FrontWord::halb);
                } else {
                    usedUhrType->show(FrontWord::halb);
                    offsetHour = 1;
                }
            }
            break;
        case 31:
            usedUhrType->show(FrontWord::min_1);
            usedUhrType->show(FrontWord::minute);
            usedUhrType->show(FrontWord::nach);
            usedUhrType->show(FrontWord::halb);
            offsetHour = 1;
            break;
        case 32:
        case 33:
        case 34:
        case 35:
            if (usedUhrType->hasThirtyfive()) {
                usedUhrType->show(FrontWord::min_35);
                usedUhrType->show(FrontWord::nach);
            } else if (usedUhrType->hasTwentyfive()) {
                usedUhrType->show(FrontWord::min_25);
                usedUhrType->show(FrontWord::vor);
                offsetHour = 1;
            } else {
                usedUhrType->show(FrontWord::min_5);
                usedUhrType->show(FrontWord::nach);
                usedUhrType->show(FrontWord::halb);
                offsetHour = 1;
            }
            break;
        case 36:
        case 37:
        case 38:
        case 39:
            usedUhrType->show(getFrontWordForNum(min - 30));
            usedUhrType->show(FrontWord::nach);
            usedUhrType->show(FrontWord::halb);
            offsetHour = 1;
            break;
        case 40:
            if (usedUhrType->hasForty()) {
                usedUhrType->show(FrontWord::min_40);
            } else if (hasTwentyAndCheckForUsage()) {
                usedUhrType->show(FrontWord::min_20);
                usedUhrType->show(FrontWord::vor);
            } else {
                usedUhrType->show(FrontWord::min_10);
                usedUhrType->show(FrontWord::nach);
                usedUhrType->show(FrontWord::halb);
            }
            offsetHour = 1;
            break;
        case 41:
        case 42:
        case 43:
        case 44:
            usedUhrType->show(getFrontWordForNum(60 - min));
            usedUhrType->show(FrontWord::vor);
            offsetHour = 1;
            break;
        case 45: // quarter to
            if (hasDreiviertelAndCheckForUsage()) {
                usedUhrType->show(FrontWord::dreiviertel);
            } else {
                // A Quarter to
                if (G.languageVariant[EN_ShowAQuarter]) {
                    usedUhrType->show(FrontWord::a_quarter);
                }
                usedUhrType->show(FrontWord::viertel);
                usedUhrType->show(FrontWord::v_vor);
            }
            offsetHour = 1;
            break;
        case 46:
        case 47:
        case 48:
        case 49:
        case 50:
            if (usedUhrType->hasFifty()) {
                usedUhrType->show(FrontWord::min_50);
            } else {
                usedUhrType->show(getFrontWordForNum(60 - min));
                usedUhrType->show(FrontWord::vor);
            }
            offsetHour = 1;
            break;
        case 51:
        case 52:
        case 53:
        case 54:
        case 55:
            if (usedUhrType->hasFiftyFive()) {
                usedUhrType->show(FrontWord::min_55);
            } else {
                usedUhrType->show(getFrontWordForNum(60 - min));
                usedUhrType->show(FrontWord::vor);
            }
            offsetHour = 1;
            break;
        case 56:
        case 57:
        case 58:
            usedUhrType->show(getFrontWordForNum(60 - min));
            usedUhrType->show(FrontWord::minuten);
            usedUhrType->show(FrontWord::vor);
            offsetHour = 1;
            break;
        case 59:
            usedUhrType->show(FrontWord::min_1);
            usedUhrType->show(FrontWord::minute);
            usedUhrType->show(FrontWord::vor);
            offsetHour = 1;
            break;
        default:
            break;
        }

        if (G.UhrtypeDef == Ru10x11) {
            offsetHour = 0;
        }
    }
}

//------------------------------------------------------------------------------
// Hour Functions
//------------------------------------------------------------------------------

void ClockWork::setHour(uint8_t hour, const bool fullHour) {
    bool midnight = isMidnight(hour);

    if (!usedUhrType->has24HourLayout()) {
        hour %= 12;
    }

    switch (hour) {
    case 0:
        if (midnight) {
            usedUhrType->show(FrontWord::hour_0);
        } else {
            usedUhrType->show(FrontWord::hour_12);
        }
        break;
    case 1:
        if (fullHour || usedUhrType->usedLang() != LanguageAbbreviation::DE) {
            usedUhrType->show(FrontWord::hour_1);
        } else {
            usedUhrType->show(FrontWord::eins);
        }
        break;
    case 2:
        usedUhrType->show(FrontWord::hour_2);
        break;
    case 3:
        usedUhrType->show(FrontWord::hour_3);
        break;
    case 4:
        usedUhrType->show(FrontWord::hour_4);
        break;
    case 5:
        usedUhrType->show(FrontWord::hour_5);
        break;
    case 6:
        usedUhrType->show(FrontWord::hour_6);
        break;
    case 7:
        usedUhrType->show(FrontWord::hour_7);
        break;
    case 8:
        usedUhrType->show(FrontWord::hour_8);
        break;
    case 9:
        usedUhrType->show(FrontWord::hour_9);
        break;
    case 10:
        usedUhrType->show(FrontWord::hour_10);
        break;
    case 11:
        usedUhrType->show(FrontWord::hour_11);
        break;
    case 12:
        usedUhrType->show(FrontWord::hour_12);
        break;
    case 13:
        usedUhrType->show(FrontWord::hour_13);
        break;
    case 14:
        usedUhrType->show(FrontWord::hour_14);
        break;
    case 15:
        usedUhrType->show(FrontWord::hour_15);
        break;
    case 16:
        usedUhrType->show(FrontWord::hour_16);
        break;
    case 17:
        usedUhrType->show(FrontWord::hour_17);
        break;
    case 18:
        usedUhrType->show(FrontWord::hour_18);
        break;
    case 19:
        usedUhrType->show(FrontWord::hour_19);
        break;
    case 20:
        usedUhrType->show(FrontWord::hour_20);
        break;
    case 21:
        usedUhrType->show(FrontWord::hour_21);
        break;
    case 22:
        usedUhrType->show(FrontWord::hour_22);
        break;
    case 23:
        usedUhrType->show(FrontWord::hour_23);
        break;

    default:
        break;
    }
}

//------------------------------------------------------------------------------
// Loop Helper Functions
//------------------------------------------------------------------------------

bool ClockWork::isMidnight(const uint8_t hour) {
    return usedUhrType->hasMitternacht() && hour == 0;
}

//------------------------------------------------------------------------------

WordclockChanges ClockWork::changesInClockface() {
    for (uint16_t i = 0; i < MAX_ROW_SIZE; i++) {
        if (frontMatrix[i] != lastFrontMatrix[i]) {
            return WordclockChanges::Words;
        }
    }
    if (parametersChanged) {
        parametersChanged = false;
        return WordclockChanges::Parameters;
    } else if (layoutChanged) {
        layoutChanged = false;
        return WordclockChanges::Layout;
    } else if (lastMinuteArray != minuteArray) {
        return WordclockChanges::Minute;
    }
    return WordclockChanges::Null;
}

//------------------------------------------------------------------------------

bool ClockWork::DetermineIfItIsIsShown(const uint8_t min) {
    switch (G.itIsVariant) {
    case ItIsVariant::Permanent:
        return true;
        break;
    case ItIsVariant::Hourly:
        return !min;
        break;
    case ItIsVariant::HalfHourly:
        return !(min % 30);
        break;
    case ItIsVariant::Quarterly:
        return !(min % 15);
        break;
    case ItIsVariant::Off:
    default:
        return false;
        break;
    }
}

//------------------------------------------------------------------------------
void ClockWork::setItIs(uint8_t min, const uint8_t offsetHour) {
    min /= 5;
    min *= 5;

    if (DetermineIfItIsIsShown(min)) {
        DetermineWhichItIsToShow(_hour + offsetHour, min);
    }
}

//------------------------------------------------------------------------------

void ClockWork::setClock() {
    uint8_t offsetHour = 0;
    bool fullHour = 0;

    setMinute(_minute, offsetHour, fullHour);
    setHour(_hour + offsetHour, fullHour);
    setItIs(_minute, offsetHour);
}

//------------------------------------------------------------------------------

void ClockWork::DetermineWhichItIsToShow(uint8_t hour, uint8_t min) {
    hour %= 12;
    if (G.UhrtypeDef == Ru10x11) {
        switch (hour) {
        case 1:
            usedUhrType->show(FrontWord::es_ist__singular__);
            break;
        case 2:
        case 3:
        case 4:
            usedUhrType->show(FrontWord::es_ist___plural___);
            break;
        default:
            usedUhrType->show(FrontWord::es_ist);
            break;
        }
    } else if (G.UhrtypeDef == Es10x11 && hour == 1 && min < 35) {
        usedUhrType->show(FrontWord::es_ist___plural___);
    } else if (G.UhrtypeDef == Es10x11 && hour == 0 && min > 34) {
        usedUhrType->show(FrontWord::es_ist___plural___);
    } else {
        usedUhrType->show(FrontWord::es_ist);
    }
}

//------------------------------------------------------------------------------

void ClockWork::calcClockface() {
    led.resetFrontMatrixBuffer();

    if (_hour == 23 && _minute == 59 && _second >= 50) {
        countdownToMidnight();
    } else {
        setClock();
    }

    if (usedUhrType->hasWeatherLayout()) {
        weather.calcWeatherClockface();
    }
}

//------------------------------------------------------------------------------

void ClockWork::clearClockByProgInit() {
    if (G.progInit) {
        G.progInit = false;
        led.clear();
        led.show();
    }
}

//------------------------------------------------------------------------------
// Loop Functions
//------------------------------------------------------------------------------

void ClockWork::loop(struct tm &tm) {
    unsigned long currentMillis = millis();
    countMillisSpeed += currentMillis - previousMillis;
    previousMillis = currentMillis;

    // Faster runtime for demo
    transition->demoMode(_hour, _minute, _second);

    //------------------------------------------------
    // Seconds and LDR Routine
    //------------------------------------------------
    if (lastSecond != _second) {

#if GENERAL_VERBOSE
        char currentTime[80];
        strftime(currentTime, sizeof(currentTime), "%F %T (%z)\n", &tm);
        Serial.printf(currentTime);
#endif

        //--------------------------------------------
        // Weather Loop
        //--------------------------------------------
        if (usedUhrType->hasWeatherLayout()) {
            weather.loop();
        }

        //--------------------------------------------
        // Auto Brightness Logic
        //--------------------------------------------
        if (G.autoBrightEnabled == 1) {
            loopAutoBrightLogic();
        }

        if (G.prog == COMMAND_IDLE && G.conf == COMMAND_IDLE) {
            led.clear();
            G.prog = COMMAND_MODE_WORD_CLOCK;
        }

        if (G.prog == COMMAND_MODE_DIGITAL_CLOCK) {
            led.clear();
            led.showDigitalClock(_minute % 10, _minute / 10, _hour % 10,
                                 _hour / 10, parametersChanged);
        }

        lastSecond = _second;
    }

    //------------------------------------------------
    // Minute
    //------------------------------------------------
    if (lastMinute != _minute) {
        lastMinute = _minute;
        if (colorChangedByWebsite) {
            eeprom::write();
            colorChangedByWebsite = false;
            Serial.println("Saved Color");
        }

        //------------------------------------------------
        // MQTT
        //------------------------------------------------

        sendMQTTUpdate();
    }

    switch (G.conf) {

    case COMMAND_RESET: {
        delay(500);
#ifdef ESP8266
        ESP.reset();
        ESP.restart();
#elif defined(ESP32)
        ESP.restart();
        esp_restart();
#endif
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

        // don't expose password:
        char passMasked[32];
        strncpy(passMasked, G.mqtt.password, PAYLOAD_LENGTH);
        strncpy(passMasked, "******************************",
                (strlen(passMasked) - 3));
        config["MQTT_Pass"] = passMasked;

        config["MQTT_ClientId"] = G.mqtt.clientId;
        config["MQTT_Topic"] = G.mqtt.topic;
        serializeJson(config, str);
        Serial.print("Sending Payload:");
        Serial.println(str);
        webSocket.sendTXT(G.client_nr, str, strlen(str));
        break;
    }

    case COMMAND_REQUEST_CONFIG_VALUES: {
        DynamicJsonDocument config(1024);
        config["command"] = "config";
        config["ssid"] = network.getSSID();
        config["timeserver"] = G.timeserver;
        config["hostname"] = G.hostname;
        config["scrollingText"] = G.scrollingText;
        config["h6"] = G.h6;
        config["h8"] = G.h8;
        config["h12"] = G.h12;
        config["h16"] = G.h16;
        config["h18"] = G.h18;
        config["h20"] = G.h20;
        config["h22"] = G.h22;
        config["h24"] = G.h24;
        for (uint8_t i = 0;
             i < sizeof(G.languageVariant) / sizeof(G.languageVariant[0]);
             i++) {
            char string2Send[11];
            sprintf(string2Send, "langVar%d", i);
            config[string2Send] = static_cast<uint8_t>(G.languageVariant[i]);
        }
        for (uint8_t i = 0;
             i < sizeof(G.layoutVariant) / sizeof(G.layoutVariant[0]); i++) {
            char string2Send[11];
            sprintf(string2Send, "layVar%d", i);
            config[string2Send] = static_cast<uint8_t>(G.layoutVariant[i]);
        }
        config["effectBri"] = G.effectBri;
        config["secondVariant"] = static_cast<uint8_t>(G.secondVariant);
        config["minuteVariant"] = static_cast<uint8_t>(G.minuteVariant);
        config["cityid"] = G.openWeatherMap.cityid;
        config["apikey"] = G.openWeatherMap.apikey;
        config["colortype"] = G.Colortype;
        config["buildtype"] = static_cast<uint8_t>(G.buildTypeDef);
        config["wType"] = static_cast<uint8_t>(G.wType);
        config["UhrtypeDef"] = G.UhrtypeDef;
        config["bootLedBlink"] = G.bootLedBlink;
        config["bootLedSweep"] = G.bootLedSweep;
        config["bootShowWifi"] = G.bootShowWifi;
        config["bootShowIP"] = G.bootShowIP;
        config["autoBrightEnabled"] = G.autoBrightEnabled;
        config["isRomanLanguage"] = isRomanLanguage();
        config["hasDreiviertel"] = usedUhrType->hasDreiviertel();
        config["hasTwenty"] = usedUhrType->hasTwenty();
        config["hasWeatherLayout"] = usedUhrType->hasWeatherLayout();
        config["hasSecondsFrame"] = usedUhrType->hasSecondsFrame();
        config["hasMinuteInWords"] = usedUhrType->hasMinuteInWords();
        config["hasHappyBirthday"] = usedUhrType->hasSpecialWordHappyBirthday();
        config["numOfRows"] = usedUhrType->rowsWordMatrix();
        serializeJson(config, str);
        Serial.print("Sending Payload:");
        Serial.println(str);
        webSocket.sendTXT(G.client_nr, str, strlen(str));
        break;
    }

    case COMMAND_REQUEST_BIRTHDAYS: {
        DynamicJsonDocument config(1024);
        config["command"] = "birthdays";
        config["hasHappyBirthday"] = usedUhrType->hasSpecialWordHappyBirthday();
        char dateString[14];
        char string2Send[14];
        for (uint8_t i = 0; i < MAX_BIRTHDAY_COUNT; i++) {
            sprintf(string2Send, "birthdayDate%d", i);
            sprintf(dateString, "%04u-%02u-%02u", G.birthday[i].year,
                    G.birthday[i].month, G.birthday[i].day);
            config[string2Send] = dateString;
        }
        serializeJson(config, str);
        Serial.print("Sending Payload:");
        Serial.println(str);
        webSocket.sendTXT(G.client_nr, str, strlen(str));
        break;
    }

    case COMMAND_REQUEST_COLOR_VALUES: {
        DynamicJsonDocument config(1024);
        config["command"] = "set";
        for (uint8_t i = 0; i < 2; i++) {
            char string2Send[7];
            sprintf(string2Send, "hsb%d%d", i, 0);
            config[string2Send] = static_cast<uint16_t>(G.color[i].H * 360);
            sprintf(string2Send, "hsb%d%d", i, 1);
            config[string2Send] = static_cast<uint8_t>(G.color[i].S * 100);
            sprintf(string2Send, "hsb%d%d", i, 2);
            config[string2Send] = static_cast<uint8_t>(G.color[i].B * 100);
        }
        config["effectBri"] = G.effectBri;
        config["effectSpeed"] = G.effectSpeed;
        config["colortype"] = G.Colortype;
        config["hasHappyBirthday"] = usedUhrType->hasSpecialWordHappyBirthday();
        config["hasSecondsFrame"] = usedUhrType->hasSecondsFrame();
        config["prog"] = G.prog;
        serializeJson(config, str);
        webSocket.sendTXT(G.client_nr, str, strlen(str));
        break;
    }

    case COMMAND_REQUEST_AUTO_BRIGHT: {
        DynamicJsonDocument config(1024);
        config["command"] = "autoBright";
        if (G.param1 == 0) {
            config["autoBrightEnabled"] = G.autoBrightEnabled;
            config["autoBrightMin"] = G.autoBrightMin;
            config["autoBrightMax"] = G.autoBrightMax;
            config["autoBrightPeak"] = G.autoBrightPeak;
        }
        config["autoBrightSensor"] = (uint32_t)lux;
        config["autoBrightGain"] = (uint8_t)ledGain;
        serializeJson(config, str);
        webSocket.sendTXT(G.client_nr, str, strlen(str));
        break;
    }

    case COMMAND_REQUEST_TRANSITION: {
        DynamicJsonDocument config(1024);
        config["command"] = "transition";
        config["transitionType"] = G.transitionType;
        config["transitionDuration"] = G.transitionDuration;
        config["transitionSpeed"] = G.transitionSpeed;
        config["transitionDemo"] = G.transitionDemo;
        config["transitionColorize"] = G.transitionColorize;
        serializeJson(config, str);
        webSocket.sendTXT(G.client_nr, str, strlen(str));
        break;
    }

    case COMMAND_SET_BIRTHDAYS:
    case COMMAND_SET_TIME:
    case COMMAND_SET_INITIAL_VALUES:
    case COMMAND_SET_WEATHER_DATA:
    case COMMAND_SET_SCROLLINGTEXT:
    case COMMAND_SET_BOOT: {
        eeprom::write();
        delay(100);
        break;
    }

    case COMMAND_SET_BUILDTYPE: {
        led.clear();

        // G.param1 sets new buildtype
        Serial.printf("Clock Buildtype: %u\n", G.param1);

        G.buildTypeDef = static_cast<BuildTypeDef>(G.param1);
        eeprom::write();
        led.clear();
        parametersChanged = true;
        break;
    }

    case COMMAND_SET_MINUTE:
    case COMMAND_SET_BRIGHTNESS:
    case COMMAND_SET_AUTO_BRIGHT:
    case COMMAND_SET_WHITETYPE:
    case COMMAND_SET_TIME_MANUAL:
    case COMMAND_SET_LAYOUT_VARIANT: {
        eeprom::write();
        led.clear();
        memset(frameArray, false, sizeof(frameArray));
        parametersChanged = true;
        break;
    }

    case COMMAND_SET_LANGUAGE_VARIANT:
    case COMMAND_SET_SETTING_SECOND: {
        eeprom::write();
        led.clear();
        G.progInit = true;
        parametersChanged = true;
        break;
    }

    case COMMAND_SET_MQTT: {
        if (G.mqtt.state && G.progInit) {
            mqtt.init();
            G.progInit = false;
        }
        delay(100);

        eeprom::write();
        break;
    }

    case COMMAND_SET_MQTT_HA_DISCOVERY: {
        mqtt.sendDiscovery();
        break;
    }

    case COMMAND_SET_COLORTYPE: {
        // G.param1 sets new Colortype
        Serial.printf("LED Colortype: %u\n", G.param1);

        // the G.Colortype must be called at the same time as initLedStrip,
        // otherwise it is referenced via a null-pointer.
        G.Colortype = G.param1;
        eeprom::write();
        initLedStrip(G.Colortype);

        clearClockByProgInit();
        parametersChanged = true;
        break;
    }

    case COMMAND_SET_UHRTYPE: {
        eeprom::write();
        led.clear();
        led.show();
        delay(10);
        Serial.printf("Uhrtype: %u\n", G.UhrtypeDef);
        usedUhrType = getPointer(G.UhrtypeDef);
        resetMinVariantIfNotAvailable();
        if (usedUhrType->numPixelsFrameMatrix() != 0) {
            delete secondsFrame;
            secondsFrame =
                new SecondsFrame(usedUhrType->numPixelsFrameMatrix());
            G.progInit = true;
        }
        parametersChanged = true;
        break;
    }

    case COMMAND_SET_HOSTNAME: {
        Serial.print("Hostname: ");
        Serial.println(G.hostname);
        eeprom::write();
        network.reboot();
        break;
    }

    case COMMAND_SET_WIFI_DISABLED: {
        eeprom::write();
        delay(100);
        Serial.println("Conf: WLAN off");
        network.disable();
        break;
    }

    case COMMAND_SET_WIFI_AND_RESTART: {
        Serial.println("Conf: new Wifi Config");
        network.resetSettings();
        break;
    }

    default:
        break;
    }

    G.conf = COMMAND_IDLE;

    switch (G.prog) {

    case COMMAND_MODE_SECONDS: {
        clearClockByProgInit();

        char d1[5];
        char d2[5];
        sprintf(d1, "%d", static_cast<uint8_t>(_second / 10));
        sprintf(d2, "%d", static_cast<uint8_t>(_second % 10));
        led.showNumbers(d1[0], d2[0]);
        break;
    }

    case COMMAND_MODE_DIGITAL_CLOCK: {
        if (G.progInit) {
            clearClockByProgInit();
        }
        if (parametersChanged) {
            led.showDigitalClock(_minute % 10, _minute / 10, _hour % 10,
                                 _hour / 10, parametersChanged);
            parametersChanged = false;
        }
        break;
    }

    case COMMAND_MODE_SCROLLINGTEXT:
    case COMMAND_MODE_RAINBOWCYCLE:
    case COMMAND_MODE_RAINBOW:
    case COMMAND_MODE_SYMBOL: {
        if (G.progInit) {
            countMillisSpeed = (11u - G.effectSpeed) * 30u;
            clearClockByProgInit();
        }

        if (countMillisSpeed >= (11u - G.effectSpeed) * 30u) {
            switch (G.prog) {
            case COMMAND_MODE_SCROLLINGTEXT: {
                scrollingText(G.scrollingText);
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
            case COMMAND_MODE_SYMBOL: {
                displaySymbols(HEART);
                break;
            }
            default:
                break;
            }
            countMillisSpeed = 0;
        }
        break;
    }

    case COMMAND_MODE_COLOR: {
        if (G.progInit) {
            clearClockByProgInit();
            for (uint8_t row = 0; row < usedUhrType->rowsWordMatrix(); row++) {
                frontMatrix[row] = num32BitWithOnesAccordingToColumns();
            }
        }

        if (parametersChanged) {
            led.setbyFrontMatrix(Foreground, false);
            led.show();
            parametersChanged = false;
        }
        break;
    }

    case COMMAND_MODE_TRANSITION: {
        if (G.progInit) {
            G.progInit = false;
            eeprom::write();
            delay(100);
        }
        // There is no break here on purpose, directly after the call
        // COMMAND_MODE_TRANSITION, COMMAND_MODE_WORD_CLOCK must be called
    }

    case COMMAND_MODE_WORD_CLOCK: {
        clearClockByProgInit();
        calcClockface();

        switch (changesInClockface()) {
        case WordclockChanges::Minute:
            lastMinuteArray = minuteArray;
            memcpy(&lastFrontMatrix, &frontMatrix, sizeof lastFrontMatrix);
            led.set(WordclockChanges::Minute);
            break;
        case WordclockChanges::Words:
            lastMinuteArray = minuteArray;
            memcpy(&lastFrontMatrix, &frontMatrix, sizeof lastFrontMatrix);
            led.set(WordclockChanges::Words);
            break;
        case WordclockChanges::Layout:
            led.set(WordclockChanges::Layout);
            break;
        case WordclockChanges::Parameters:
            led.set(WordclockChanges::Parameters);
            break;
        default:
            break;
        }
        G.prog = COMMAND_IDLE;
    }
    default:
        break;
    }

    if (countMillisSpeed > 10000) {
        countMillisSpeed = 0;
    }
}

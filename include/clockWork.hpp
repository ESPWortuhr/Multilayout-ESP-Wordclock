#include "Animation.h"
#include "NeoMultiFeature.hpp"
#include "Uhr.h"
#include "Uhrtypes/Uhrtype.hpp"
#include "clockWork.h"
#include "openwmap.h"
#include <Arduino.h>

OpenWMap weather;

//------------------------------------------------------------------------------

void ClockWork::copyClockface(const bool source[], bool destination[]) {
    for (uint16_t i = 0; i < usedUhrType->NUM_PIXELS(); i++) {
        destination[i] = source[i];
    }
}

//------------------------------------------------------------------------------

bool ClockWork::changesInClockface() {
    for (uint16_t i = 0; i < usedUhrType->NUM_PIXELS(); i++) {
        if (frontMatrix[i] != lastFrontMatrix[i]) {
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
        led.setPixelHsb(i, hue, 100, G.effectBri);
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
        led.setPixelHsb(usedUhrType->getSMatrix(i), displayedHue, 100,
                        G.effectBri);
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
                    G.rgbw[Effect][0], G.rgbw[Effect][1], G.rgbw[Effect][2],
                    G.rgbw[Effect][3],
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

void ClockWork::setItIs(const uint8_t min) {
    if (!G.languageVariant[NotShowItIs]) {
        usedUhrType->show(FrontWord::es_ist);
    } else if (!G.languageVariant[NotShowPuristMode] && min % 30 == 0) {
        usedUhrType->show(FrontWord::es_ist);
    }
}

//------------------------------------------------------------------------------

void ClockWork::setHour(const uint8_t hour, const bool fullHour) {
    switch (hour % 12) {
    case 0:
        usedUhrType->show(FrontWord::h_zwoelf);
        break;
    case 1:
        if (fullHour || usedUhrType->usedLang() != LanguageAbbreviation::DE) {
            usedUhrType->show(FrontWord::h_ein);
        } else {
            usedUhrType->show(FrontWord::eins);
        }
        break;
    case 2:
        usedUhrType->show(FrontWord::h_zwei);
        break;
    case 3:
        usedUhrType->show(FrontWord::h_drei);
        break;
    case 4:
        usedUhrType->show(FrontWord::h_vier);
        break;
    case 5:
        usedUhrType->show(FrontWord::h_fuenf);
        break;
    case 6:
        usedUhrType->show(FrontWord::h_sechs);
        break;
    case 7:
        usedUhrType->show(FrontWord::h_sieben);
        break;
    case 8:
        usedUhrType->show(FrontWord::h_acht);
        break;
    case 9:
        usedUhrType->show(FrontWord::h_neun);
        break;
    case 10:
        usedUhrType->show(FrontWord::h_zehn);
        break;
    case 11:
        usedUhrType->show(FrontWord::h_elf);
        break;
    case 12:
        usedUhrType->show(FrontWord::h_zwoelf);
        break;
    default:
        break;
    }
}

//------------------------------------------------------------------------------

uint8_t ClockWork::determineWhichMinuteVariant() {
    switch (G.minuteVariant) {
    case MinuteVariant::Row:
        return 0;
        break;
    case MinuteVariant::Corners:
        return 1;
        break;
    default:
        Serial.println("[ERROR] G.minuteVariant undefined");
        return 0;
        break;
    }
}

//------------------------------------------------------------------------------

void ClockWork::showMinuteInWords(uint8_t min) {
    switch (min) {
    case 0:
        usedUhrType->show(FrontWord::nur);
        break;

    case 1:
    case 2:
    case 3:
    case 4:
        usedUhrType->show(FrontWord::gewesen);
        break;

    default:
        break;
    }
}

//------------------------------------------------------------------------------

FrontWord ClockWork::getFrontWordForNum(uint8_t min) {
    switch (min) {

    case 1:
        return FrontWord::m_eine;
        break;

    case 2:
        return FrontWord::m_zwei;
        break;

    case 3:
        return FrontWord::m_drei;
        break;

    case 4:
        return FrontWord::m_vier;
        break;

    case 5:
        return FrontWord::fuenf;
        break;

    case 6:
        return FrontWord::m_sechs;
        break;

    case 7:
        return FrontWord::m_sieben;
        break;

    case 8:
        return FrontWord::m_acht;
        break;

    case 9:
        return FrontWord::m_neun;
        break;

    case 10:
        return FrontWord::zehn;
        break;

    case 11:
        return FrontWord::m_elf;
        break;

    case 12:
        return FrontWord::m_zwoelf;
        break;

    case 13:
        return FrontWord::m_dreizehn;
        break;

    case 14:
        return FrontWord::m_vierzehn;
        break;

    case 16:
        return FrontWord::m_sechzehn;
        break;

    case 17:
        return FrontWord::m_siebzehn;
        break;

    case 18:
        return FrontWord::m_achtzehn;
        break;

    case 19:
        return FrontWord::m_neunzehn;
        break;

    case 20:
        return FrontWord::zwanzig;

    default:
        break;
        Serial.println("[Error] getFrontWordForNum() out of Bounds");
    }
    return FrontWord::error;
}

//------------------------------------------------------------------------------

void ClockWork::showMinute(uint8_t min) {
    min %= 5;
    if (G.UhrtypeDef == Nl10x11 && G.minuteVariant == MinuteVariant::InWords) {
        showMinuteInWords(min);
    } else if (G.minuteVariant != MinuteVariant::Off) {
        uint16_t minArray[4];
        usedUhrType->getMinArr(minArray, determineWhichMinuteVariant());
        if (G.layoutVariant[ReverseMinDirection]) {
            std::reverse(std::begin(minArray), std::end(minArray));
        }
        for (uint8_t i = 0; i < min; i++) {
            frontMatrix[minArray[i]] = true;
        }
    }
}

//------------------------------------------------------------------------------

void ClockWork::setMinute(uint8_t min, uint8_t &offsetHour, bool &fullHour) {
    if (!usedUhrType->has24HourLayout()) {
        showMinute(min);
        min /= 5;
        min *= 5;
    }
    switch (min) {
    case 0: // full hour
        usedUhrType->show(FrontWord::uhr);
        fullHour = true;
        break;
    case 1:
        usedUhrType->show(FrontWord::m_eine);
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
    case 11:
    case 12:
    case 13:
    case 14:
        usedUhrType->show(getFrontWordForNum(min));
        usedUhrType->show(FrontWord::minuten);
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
    case 20: // 20 past
        if (!usedUhrType->hasZwanzig() || G.languageVariant[ItIs20]) {
            usedUhrType->show(FrontWord::zehn);
            usedUhrType->show(FrontWord::vor);
            usedUhrType->show(FrontWord::halb);
            offsetHour = 1;
        } else {
            usedUhrType->show(FrontWord::zwanzig);
            usedUhrType->show(FrontWord::nach);
        }
        break;
    case 21:
    case 22:
    case 23:
    case 24:
    case 25:
        if (usedUhrType->hasTwentyfive()) {
            usedUhrType->show(FrontWord::twentyfive);
            usedUhrType->show(FrontWord::nach);
        } else {
            usedUhrType->show(FrontWord::fuenf);
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
        usedUhrType->show(FrontWord::m_eine);
        usedUhrType->show(FrontWord::minute);
        usedUhrType->show(FrontWord::vor);
        usedUhrType->show(FrontWord::halb);
        offsetHour = 1;
        break;
    case 30: // half
        if (G.UhrtypeDef == Eng10x11) {
            usedUhrType->show(FrontWord::halb);
            usedUhrType->show(FrontWord::nach);
        } else {
            usedUhrType->show(FrontWord::halb);
            offsetHour = 1;
        }
        break;
    case 31:
        usedUhrType->show(FrontWord::m_eine);
        usedUhrType->show(FrontWord::minute);
        usedUhrType->show(FrontWord::nach);
        usedUhrType->show(FrontWord::halb);
        offsetHour = 1;
        break;
    case 32:
    case 33:
    case 34:
    case 35:
        if (usedUhrType->hasTwentyfive()) {
            usedUhrType->show(FrontWord::twentyfive);
            usedUhrType->show(FrontWord::vor);
        } else {
            usedUhrType->show(FrontWord::fuenf);
            usedUhrType->show(FrontWord::nach);
            usedUhrType->show(FrontWord::halb);
        }
        offsetHour = 1;
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
    case 40: // 20 to
        if (!usedUhrType->hasZwanzig() || G.languageVariant[ItIs40]) {
            usedUhrType->show(FrontWord::zehn);
            usedUhrType->show(FrontWord::nach);
            usedUhrType->show(FrontWord::halb);
        } else {
            usedUhrType->show(FrontWord::zwanzig);
            usedUhrType->show(FrontWord::vor);
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
        if (usedUhrType->hasDreiviertel() && G.languageVariant[ItIs45]) {
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
    case 51:
    case 52:
    case 53:
    case 54:
    case 55:
        usedUhrType->show(getFrontWordForNum(60 - min));
        usedUhrType->show(FrontWord::vor);
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
        usedUhrType->show(FrontWord::m_eine);
        usedUhrType->show(FrontWord::minute);
        usedUhrType->show(FrontWord::vor);
        offsetHour = 1;
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
        usedUhrType->show(FrontWord::zehn);
        break;
    case 51:
        usedUhrType->show(FrontWord::h_neun);
        break;
    case 52:
        usedUhrType->show(FrontWord::h_acht);
        break;
    case 53:
        usedUhrType->show(FrontWord::h_sieben);
        break;
    case 54:
        usedUhrType->show(FrontWord::h_sechs);
        break;
    case 55:
        usedUhrType->show(FrontWord::fuenf);
        break;
    case 56:
        usedUhrType->show(FrontWord::h_vier);
        break;
    case 57:
        usedUhrType->show(FrontWord::h_drei);
        break;
    case 58:
        usedUhrType->show(FrontWord::h_zwei);
        break;
    case 59:
        usedUhrType->show(FrontWord::eins);
        break;
    default:
        break;
    }
}

//------------------------------------------------------------------------------

void ClockWork::setClock() {
    uint8_t offsetHour = 0;
    bool fullHour = 0;

    setItIs(_minute);
    setMinute(_minute, offsetHour, fullHour);
    setHour(_hour + offsetHour, fullHour);
}

//------------------------------------------------------------------------------

void ClockWork::calcClockface() {
    uint8_t rr, gg, bb, ww;

    if (_hour == 23 && _minute == 59 && _second >= 50) {
        countdownToMidnight();
    } else {
        setClock();
    }

    // Set Brighness hour dependent
    if (_hour < 6) {
        G.hh = G.h24;
    } else if (_hour < 8) {
        G.hh = G.h6;
    } else if (_hour < 12) {
        G.hh = G.h8;
    } else if (_hour < 16) {
        G.hh = G.h12;
    } else if (_hour < 18) {
        G.hh = G.h16;
    } else if (_hour < 20) {
        G.hh = G.h18;
    } else if (_hour < 22) {
        G.hh = G.h20;
    } else if (_hour < 24) {
        G.hh = G.h22;
    }

    led.setBrightnessLdr(rr, gg, bb, ww, Background);

    // set Background color
    for (uint16_t i = 0; i < usedUhrType->NUM_PIXELS(); i++) {
        led.setPixel(rr, gg, bb, ww, i);
    }

    if (usedUhrType->hasWeatherLayout()) {
        weather.calcWeatherClockface();
    }
}

//------------------------------------------------------------------------------

iUhrType *ClockWork::getPointer(uint8_t type) {
    switch (type) {
    case Ger10x11:
        return &_de10x11;
    case Ger10x11Alternative:
        return &_de10x11Alternative;
    case Ger10x11Clock:
        return &_de10x11Clock;
    case Nl10x11:
        return &_nl10x11;
    case Ger11x11:
        return &_de11x11;
    case Ger11x11V2:
        return &_de11x11V2;
    case Ger10x11Frame:
        return &_de10x11frame;
    case Ger21x11Weather:
        return &_de21x11Weather;
    case Ger17x17:
        return &_de17x17;
    case Eng10x11:
        return &_en10x11;
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
            strip_RGBW = new NeoPixelBus<NeoGrbwFeature, Neo800KbpsMethod>(
                usedUhrType->NUM_PIXELS());
            strip_RGBW->Begin();
        }
    } else {
        if (strip_RGBW != NULL) {
            delete strip_RGBW; // delete the previous dynamically created
                               // strip
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

void resetMinVariantIfNotAvailable() {
    if (G.UhrtypeDef != Nl10x11 && G.minuteVariant == MinuteVariant::InWords) {
        G.minuteVariant = MinuteVariant::Off;
    } else if (G.UhrtypeDef != Ger10x11Frame &&
               G.minuteVariant == MinuteVariant::Row) {
        G.minuteVariant = MinuteVariant::Off;
    }
}

//------------------------------------------------------------------------------

void ClockWork::loop(struct tm &tm) {
    unsigned long currentMillis = millis();
    countMillisSpeed += currentMillis - previousMillis;
    if (usedUhrType->hasSecondsFrame()) {
        countMillis48 += currentMillis - previousMillis;
    }
    previousMillis = currentMillis;

    // Faster runtime for demo
    animation->demoMode(_minute, _second);

    //------------------------------------------------
    // SecondsFrame
    //------------------------------------------------
    if (usedUhrType->hasSecondsFrame()) {
        loopSecondsFrame();
    }

    //------------------------------------------------
    // Secounds and LDR Routine
    //------------------------------------------------
    if (lastSecond != _second) {

        if (DEBUG == true) {
            char currentTime[80];
            strftime(currentTime, sizeof(currentTime), "%F %T (%z)\n", &tm);
            Serial.printf(currentTime);
        }

        //--------------------------------------------
        // Weather Loop
        //--------------------------------------------
        if (usedUhrType->hasWeatherLayout()) {
            weather.loop();
        }

        //--------------------------------------------
        // LDR Routine
        //--------------------------------------------
        if ((G.ldr == 1) || G.autoLdrEnabled) {
            loopLdrLogic();
        }

        if (G.prog == 0 && G.conf == 0) {
            led.clear();
            G.prog = COMMAND_MODE_WORD_CLOCK;
        }

        lastSecond = _second;
    }

    //------------------------------------------------
    // Minute
    //------------------------------------------------
    if (lastMinute != _minute) {
        _second48 = 0;
        lastMinute = _minute;
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
            char stringToSend[11];
            sprintf(stringToSend, "langVar%d", i);
            config[stringToSend] = static_cast<uint8_t>(G.languageVariant[i]);
        }
        for (uint8_t i = 0;
             i < sizeof(G.layoutVariant) / sizeof(G.layoutVariant[0]); i++) {
            char stringToSend[11];
            sprintf(stringToSend, "layVar%d", i);
            config[stringToSend] = static_cast<uint8_t>(G.layoutVariant[i]);
        }
        config["effectBri"] = G.effectBri;
        config["secondVariant"] = G.secondVariant;
        config["minuteVariant"] = static_cast<uint8_t>(G.minuteVariant);
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
                char stringToSend[7];
                sprintf(stringToSend, "rgbw%d%d", i, ii);
                config[stringToSend] = G.rgbw[i][ii];
            }
        }
        config["effectBri"] = G.effectBri;
        config["effectSpeed"] = G.effectSpeed;
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
        eeprom::write();
        delay(100);
        G.conf = COMMAND_IDLE;
        break;
    }

    case COMMAND_SET_MINUTE:
    case COMMAND_SET_BRIGHTNESS:
    case COMMAND_SET_LDR:
    case COMMAND_SET_AUTO_LDR:
    case COMMAND_SET_LANGUAGE_VARIANT:
    case COMMAND_SET_LAYOUT_VARIANT:
    case COMMAND_SET_SETTING_SECOND:
    case COMMAND_SET_TIME_MANUAL: {
        eeprom::write();
        led.clear();
        parametersChanged = true;
        G.conf = COMMAND_IDLE;
        break;
    }

    case COMMAND_SET_MQTT: {
        if (G.mqtt.state && !mqtt.getConnected()) {
            mqtt.reInit();
        }

        eeprom::write();
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
        eeprom::write();
        initLedStrip(G.Colortype);
        led.setIcon(RGB_I, 100, true);
        break;
    }

    case COMMAND_SET_UHRTYPE: {
        eeprom::write();
        Serial.printf("Uhrtype: %u\n", G.UhrtypeDef);
        usedUhrType = getPointer(G.UhrtypeDef);
        resetMinVariantIfNotAvailable();
        G.conf = COMMAND_IDLE;
        break;
    }

    case COMMAND_SET_HOSTNAME: {
        Serial.print("Hostname: ");
        Serial.println(G.hostname);
        eeprom::write();
        network.reboot();
        G.conf = COMMAND_IDLE;
        break;
    }

    case COMMAND_SET_WIFI_DISABLED: {
        eeprom::write();
        delay(100);
        Serial.println("Conf: WLAN off");
        network.disable();
        G.conf = COMMAND_IDLE;
        break;
    }

    case COMMAND_SET_WIFI_AND_RESTART: {
        Serial.println("Conf: new Wifi Config");
        network.resetSettings();
        G.conf = COMMAND_IDLE;
        break;
    }

    default:
        break;
    }

    switch (G.prog) {

    case COMMAND_MODE_SECONDS: {
        if (G.progInit == 1) {
            led.clear();
            G.progInit = 0;
        }
        char d1[5];
        char d2[5];
        sprintf(d1, "%d", (int)(_second / 10));
        sprintf(d2, "%d", (int)(_second % 10));
        led.showNumbers(d1[0], d2[0]);
        break;
    }

    case COMMAND_MODE_SCROLLINGTEXT:
    case COMMAND_MODE_RAINBOWCYCLE:
    case COMMAND_MODE_RAINBOW: {
        if (G.progInit == 1) {
            G.progInit = 0;
            led.clear();
            countMillisSpeed = (11u - G.effectSpeed) * 30u;
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
            default:
                break;
            }
            countMillisSpeed = 0;
        }
        break;
    }

    case COMMAND_MODE_COLOR: {
        if (G.progInit == 1) {
            G.progInit = 0;
            led.setColor();
            led.show();
        }
        break;
    }

    case COMMAND_MODE_ANIMATION: {
        if (G.progInit == 1) {
            G.progInit = 0;
            eeprom::write();
            delay(100);
        }
        // There is no break here on purpose, directly after the call
        // COMMAND_MODE_ANIMATION, COMMAND_MODE_WORD_CLOCK must be called
    }

    case COMMAND_MODE_WORD_CLOCK: {
        calcClockface();

        if (changesInClockface()) {
            copyClockface(frontMatrix, lastFrontMatrix);
            led.set(true);
        } else if (parametersChanged) {
            led.set();
        }
        parametersChanged = false;

        if (usedUhrType->hasSecondsFrame() && G.secondVariant < 1 &&
            G.minuteVariant != MinuteVariant::Corners) {
            led.setFrameColor();
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

//------------------------------------------------------------------------------

void ClockWork::loopSecondsFrame() {
    if (countMillis48 >= interval48) {
        countMillis48 = 0;
        _second48++;
        if (_second48 > 47) {
            _second48 = 0;
        }
    }
    if (lastSecond48 != _second48) {
        if (G.prog == 0 && G.conf == 0) {
            if (G.secondVariant == 1 ||
                G.minuteVariant == MinuteVariant::Corners) {
                led.clearFrame();
            }
            if (G.secondVariant > 0) {
                led.showSeconds();
            }
            led.show();
        }
        lastSecond48 = _second48;
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

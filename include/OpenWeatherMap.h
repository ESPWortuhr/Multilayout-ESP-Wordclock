#pragma once

#include "SensitiveData.h"
#include "WordClockState.h"
#include <Arduino.h>
#include <WiFiClient.h>
#include <cmath>

/*--------------------------------------------------
List of Conditions http://openweathermap.org/weather-conditions
Weather condition code (wetterid):

2xx Thunderstorm
3xx Drizzle
5xx Rain
6xx Snow
7xx Clouds (Warning)
800 Clear
80x Clouds
--------------------------------------------------*/

class OpenWMap {
private:
    static constexpr const char *server = "api.openweathermap.org";
    WiFiClient weatherClient;
    enum class RequestState : uint8_t { Idle, SkippingHeader };
    RequestState requestState = RequestState::Idle;
    uint32_t requestStartMillis = 0;
    uint32_t lastRequestMillis = 0;
    bool requestedOnce = false;
    uint8_t headerEndMatched = 0;
    static constexpr int32_t connectTimeoutMs = 2000;
    static constexpr uint32_t responseTimeoutMs = 10000;
    static constexpr uint32_t bodyTimeoutMs = 1000;
    static constexpr uint32_t requestIntervalMs = 10UL * 60UL * 1000UL;
    static constexpr uint8_t forecastSlots = 4;
    int8_t wTemp[forecastSlots] = {};
    uint16_t wWeather[forecastSlots] = {};
    uint8_t daytime = 0;
    uint8_t shownSlot = 0;
    bool forecastValid = false;

private:
    void determineDaytime(uint8_t hour) { daytime = ((hour + 21) % 24) / 6; }

    //------------------------------------------------------------------------------

    void determineWTemp(const double srctemp, int8_t &destTemp) {
        if (srctemp >= 30) {
            destTemp = 30;
        } else if (srctemp >= 0 && srctemp <= 5) {
            destTemp = 1;
        } else if (srctemp < 0 && srctemp >= -5) {
            destTemp = -1;
        } else if (srctemp <= -25) {
            destTemp = -25;
        } else {
            destTemp = static_cast<int8_t>(srctemp - fmod(srctemp, 5));
        }
    }

    //------------------------------------------------------------------------------

    void determineWid(uint16_t srcId, uint16_t &destId) {
        if (srcId == 800) {
            destId = 800;
        } else if (srcId > 800 && srcId < 900) {
            destId = 801;
        } else if (srcId >= 200 && srcId < 800) {
            destId = srcId / 100 * 100;
        } else {
            Serial.printf("[ERROR] determineWid() unknown weather id %u\n",
                          srcId);
            destId = 0;
        }
    }

    //------------------------------------------------------------------------------

    void printDeterminedData() {
        for (uint8_t slot = 0; slot < forecastSlots; slot++) {
            const int hours = 6 * (slot + 1);
            Serial.printf("Temp_%d - %d\n", hours, wTemp[slot]);
            Serial.printf("wWeather%d - %u\n", hours, wWeather[slot]);
            Serial.println("--------- ");
        }
        Serial.print("daytime - ");
        Serial.println(daytime);
        Serial.println("--------- ");
    }

    //------------------------------------------------------------------------------

    template <size_t N>
    void buildResource(char (&dest)[N], const char *apikey) {
        snprintf(dest, N,
                 "/data/2.5/forecast?id=%s&units=metric&APPID=%s&cnt=%u",
                 G.openWeatherMap.cityid, apikey,
                 static_cast<unsigned>(2 * forecastSlots));
    }

    //------------------------------------------------------------------------------

    void startWeatherRequest() {
        Serial.println("");
        Serial.println("--------------------------------------");
        Serial.println("Connecting to Openweathermap.org");
        Serial.println("--------------------------------------");

        char resource[100];
        buildResource(resource, G.openWeatherMap.apikey);

        char apiKeyMasked[sizeof(G.openWeatherMap.apikey) + 1] = {0};
        sensitive::maskPreservingSuffix(apiKeyMasked, G.openWeatherMap.apikey);
        char maskedResource[sizeof(resource)];
        buildResource(maskedResource, apiKeyMasked);

        Serial.print("Calling URL: ");
        Serial.println(maskedResource);

#ifdef ESP8266
        weatherClient.setTimeout(connectTimeoutMs);
        const bool ok = weatherClient.connect(server, 80);
#else
        const bool ok = weatherClient.connect(server, 80, connectTimeoutMs);
#endif
        if (!ok) {
            Serial.println("Connection to Openweathermap.org failed");
            return;
        }

        weatherClient.print("GET ");
        weatherClient.print(resource);
        weatherClient.println(" HTTP/1.0");
        weatherClient.print("Host: ");
        weatherClient.println(server);
        weatherClient.println("Connection: close");
        weatherClient.println();

        headerEndMatched = 0;
        requestStartMillis = millis();
        requestState = RequestState::SkippingHeader;
    }

    //------------------------------------------------------------------------------

    void finishWeatherRequest() {
        weatherClient.stop();
        requestState = RequestState::Idle;
    }

    //------------------------------------------------------------------------------

    void pollWeatherResponse() {
        while (weatherClient.available() > 0) {
            const int c = weatherClient.read();
            if (c < 0) {
                break;
            }
            if (c == "\r\n\r\n"[headerEndMatched]) {
                headerEndMatched++;
            } else {
                headerEndMatched = (c == '\r') ? 1 : 0;
            }
            if (headerEndMatched == 4) {
                processWeatherResponse();
                finishWeatherRequest();
                return;
            }
        }

        if (!weatherClient.connected()) {
            Serial.println("Connection closed before response header ended");
            finishWeatherRequest();
        } else if (millis() - requestStartMillis > responseTimeoutMs) {
            Serial.println("Timeout while waiting for weather response");
            finishWeatherRequest();
        }
    }

    //------------------------------------------------------------------------------

    void processWeatherResponse() {
        StaticJsonDocument<512> filter;
        filter["cod"] = true;
        filter["message"] = true;
        filter["list"][0]["main"]["temp"] = true;
        filter["list"][0]["weather"][0]["id"] = true;
#if WEATHER_VERBOSE
        filter["city"]["name"] = true;
        filter["list"][0]["weather"][0]["description"] = true;
        const uint32_t parseStartMillis = millis();
#endif

        DynamicJsonDocument doc(3072);
        weatherClient.setTimeout(bodyTimeoutMs);
        auto error = deserializeJson(doc, weatherClient,
                                     DeserializationOption::Filter(filter));
        if (error) {
            Serial.print(F("deserializeJson() failed with code "));
            Serial.println(error.c_str());
            return;
        }

#if WEATHER_VERBOSE
        Serial.printf("Weather body parsed in %lu ms: ",
                      static_cast<unsigned long>(millis() - parseStartMillis));
        serializeJson(doc, Serial);
        Serial.println();
#endif

        if (doc["cod"].as<int>() != 200 ||
            doc["list"].size() < 2 * forecastSlots) {
            Serial.print("OpenWeatherMap error ");
            serializeJson(doc["cod"], Serial);
            Serial.print(": ");
            serializeJson(doc["message"], Serial);
            Serial.println();
            return;
        }

#if WEATHER_VERBOSE
        Serial.print("*** ");
        Serial.print(doc["city"]["name"].as<const char *>());
        Serial.println(" ***");
#endif

        for (uint8_t slot = 0; slot < forecastSlots; slot++) {
            JsonVariant forecast = doc["list"][2 * slot + 1];
            const int weatherId = forecast["weather"][0]["id"];
            const double temp = forecast["main"]["temp"];

#if WEATHER_VERBOSE
            Serial.println("----------");
            Serial.printf("+%dh\n", 6 * (slot + 1));
            Serial.print("Type: ");
            Serial.println(
                forecast["weather"][0]["description"].as<const char *>());
            Serial.print("Wetter ID: ");
            Serial.println(weatherId);
            Serial.print("Temp: ");
            Serial.print(temp);
            Serial.println("°C");
#endif

            determineWTemp(temp, wTemp[slot]);
            determineWid(weatherId, wWeather[slot]);
        }

        determineDaytime(_hour);
        forecastValid = true;

#if WEATHER_VERBOSE
        Serial.println("Hour");
        Serial.println(_hour);
        Serial.println("----------");
        printDeterminedData();
#endif
    }

    //------------------------------------------------------------------------------

    bool requestDue() const {
        return !requestedOnce ||
               millis() - lastRequestMillis >= requestIntervalMs;
    }

    //------------------------------------------------------------------------------

    template <size_t N> void showWords(const FrontWord (&words)[N]) {
        for (FrontWord word : words) {
            if (word != FrontWord{}) {
                usedClockType->show(word);
            }
        }
    }

    //------------------------------------------------------------------------------

    void showTemperature(int8_t temp) {
        struct Degrees {
            uint8_t value;
            FrontWord words[3];
        };
        static const Degrees degrees[] = {
            {1, {FrontWord::w_null}},
            {5, {FrontWord::w_fuenf}},
            {10, {FrontWord::w_zehn}},
            {15, {FrontWord::w_fuenf, FrontWord::w_zehn}},
            {20, {FrontWord::w_zwanzig}},
            {25, {FrontWord::w_fuenf, FrontWord::w_und, FrontWord::w_zwanzig}},
            {30, {FrontWord::w_dreissig}},
        };

        for (const Degrees &entry : degrees) {
            if (entry.value != abs(temp)) {
                continue;
            }
            if (temp > 0) {
                usedClockType->show(FrontWord::w_ueber);
            } else {
                usedClockType->show(FrontWord::w_unter);
                usedClockType->show(FrontWord::w_minus);
            }
            showWords(entry.words);
            usedClockType->show(FrontWord::w_grad);
            return;
        }
    }

    //------------------------------------------------------------------------------

    void showCondition(uint16_t condition) {
        switch (condition) {
        case 200:
            usedClockType->show(FrontWord::w_gewitter);
            break;
        case 300:
        case 500:
            usedClockType->show(FrontWord::w_regen);
            break;
        case 600:
            usedClockType->show(FrontWord::w_schnee);
            break;
        case 700:
            usedClockType->show(FrontWord::w_warnung);
            break;
        case 800:
            usedClockType->show(FrontWord::w_klar);
            break;
        case 801:
            usedClockType->show(FrontWord::w_wolken);
            break;
        }
    }

public:
    void calcWeatherClockface() {
        if (!forecastValid) {
            return;
        }

        static const FrontWord daytimeWords[7][2] = {
            {FrontWord::w_mittag},
            {FrontWord::w_abend},
            {FrontWord::w_nacht},
            {FrontWord::w_morgen, FrontWord::w_frueh},
            {FrontWord::w_morgen, FrontWord::w_mittag},
            {FrontWord::w_morgen, FrontWord::w_abend},
            {FrontWord::w_morgen, FrontWord::w_nacht},
        };
        showWords(daytimeWords[shownSlot + daytime]);
        showTemperature(wTemp[shownSlot]);
        showCondition(wWeather[shownSlot]);
    }

    //------------------------------------------------------------------------------

    void loop() {
        if (_second % 10 == 0) {
            shownSlot = (shownSlot + 1) % forecastSlots;
            led.clear();
        }

        if (requestState != RequestState::Idle) {
            pollWeatherResponse();
        } else if (WiFi.status() == WL_CONNECTED && requestDue()) {
            requestedOnce = true;
            lastRequestMillis = millis();
            startWeatherRequest();
        }
    }
};
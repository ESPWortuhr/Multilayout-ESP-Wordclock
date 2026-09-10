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
    const char *server =
        "api.openweathermap.org"; // Openweather server's address
    const char *resource1 =
        "/data/2.5/forecast?id="; // Openweather API URL part 1
    const char *resource2 =
        "&units=metric&APPID=";       // Openweather API URL part 2
    const char *resource3 = "&cnt=8"; // Openweather API forecast time
    char resource[100];
    char response[5120];     // fixed size buffer
    uint16_t weatherCounter; // counter fuer Wetterdaten abrufen
    WiFiClient weatherClient;
    enum class RequestState : uint8_t { Idle, SkippingHeader, ReadingBody };
    RequestState requestState = RequestState::Idle;
    uint32_t requestStartMillis = 0;
    uint8_t headerEndMatched = 0;
    size_t responseLength = 0;
    bool responseOverflow = false;
    static constexpr int32_t connectTimeoutMs = 2000;
    static constexpr uint32_t responseTimeoutMs = 10000;
    static constexpr uint8_t forecastSlots = 4;
    int8_t wTemp[forecastSlots];
    uint16_t wWeather[forecastSlots];
    uint16_t wHour;
    uint16_t wWeatherSwitch;

private:
    void determineDaytime(uint8_t hour) {
        uint8_t countId = 0;
        hour += 24; // Offset by 24 hours
        hour -= 3;  // Offset to get corrospondance
        hour %= 24; // Offset for nighttime

        for (uint8_t i = 0; i < 4; i++) {
            if (hour < 6 /* Timeframe for each Daytime*/) {
                wHour = countId + 1;
                break;
            }
            hour -= 6;
            countId++;
        }
    }

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
        uint8_t countId = 0;
        if (srcId == 800) {
            destId = 800;
        } else {
            for (uint8_t i = 0; i < 9; i++) {
                if (srcId < 100) {
                    if (countId < 2) {
                        Serial.println("[ERROR] determineWid() Out of bounds");
                    }
                    destId = countId * 100;
                }
                srcId -= 100;
                countId++;
            }
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
        Serial.print("wHour - ");
        Serial.println(wHour);
        Serial.println("--------- ");
    }

    //------------------------------------------------------------------------------

    void buildResource(char (&dest)[sizeof(resource)], const char *apikey) {
        memset(dest, 0, sizeof(dest));
        strncat(dest, resource1, 22);
        strncat(dest, G.openWeatherMap.cityid, 8);
        strncat(dest, resource2, 20);
        strncat(dest, apikey, 35);
        strncat(dest, resource3, 6);
    }

    //------------------------------------------------------------------------------

    void startWeatherRequest() {
        Serial.println("");
        Serial.println("--------------------------------------");
        Serial.println("Connecting to Openweathermap.org");
        Serial.println("--------------------------------------");

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
        weatherClient.println(" HTTP/1.1");
        weatherClient.print("Host: ");
        weatherClient.println(server);
        weatherClient.println("Connection: close");
        weatherClient.println();

        memset(response, 0, sizeof(response));
        responseLength = 0;
        responseOverflow = false;
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
            if (requestState == RequestState::SkippingHeader) {
                if (c == "\r\n\r\n"[headerEndMatched]) {
                    headerEndMatched++;
                } else {
                    headerEndMatched = (c == '\r') ? 1 : 0;
                }
                if (headerEndMatched == 4) {
                    requestState = RequestState::ReadingBody;
                }
            } else if (responseLength < sizeof(response) - 1) {
                response[responseLength++] = static_cast<char>(c);
            } else {
                responseOverflow = true;
            }
        }

        if (requestState == RequestState::ReadingBody &&
            !weatherClient.connected()) {
            finishWeatherRequest();
            if (responseOverflow) {
                Serial.printf(
                    "Weather response exceeds %u bytes, ignoring it\n",
                    static_cast<unsigned>(sizeof(response) - 1));
            } else {
                processWeatherResponse();
            }
        } else if (!weatherClient.connected()) {
            Serial.println("Connection closed before response header ended");
            finishWeatherRequest();
        } else if (millis() - requestStartMillis > responseTimeoutMs) {
            Serial.println("Timeout while waiting for weather response");
            finishWeatherRequest();
        }
    }

    //------------------------------------------------------------------------------

    void processWeatherResponse() {
        bool beginFound = false;

        Serial.println("Antwort: ");
        Serial.println(response);

        int eol = sizeof(response);
        Serial.print("Length = ");
        Serial.println(eol);

        // process JSON
        DynamicJsonDocument doc(3072);

        // But.....make sure the stream header is valid
        // Sometime OWM includes invalid data after the header
        // Parsing fails if this data is not removed

        if (int(response[0]) != 123) {
            Serial.println("Wrong start char detected");
            uint32_t i = 0;
            while (!beginFound && i < sizeof(response)) {
                if (int(response[i]) == 123) { // check for the "{"
                    beginFound = true;
                    Serial.println("{ found at ");
                    Serial.println(i);
                }
                i++;
            }

            if (!beginFound) {
                Serial.println("No JSON object found in response");
                return;
            }

            int eol = sizeof(response);
            Serial.println("Length = ");
            Serial.println(eol);

            // restructure by shifting the correct data
            Serial.println("restructure");
            for (uint32_t c = 0; c < (eol - i); c++) {
                response[c] = response[((c + i) - 1)];
                Serial.println(response[c]);
            }

            Serial.println("Done...!");
        }

        StaticJsonDocument<512> filter;
        filter["city"]["name"] = true;
        filter["list"][0]["main"]["temp"] = true;
        filter["list"][0]["weather"][0]["id"] = true;
        filter["list"][0]["weather"][0]["description"] = true;

        auto error = deserializeJson(doc, response,
                                     DeserializationOption::Filter(filter));
        if (error) {
            Serial.print(F("deserializeJson() failed with code "));
            Serial.println(error.c_str());
            return;
        } else {
            Serial.println("JSON parsing worked!");
        }

        // Fill Variable with json information
        const char *location = doc["city"]["name"];
        Serial.print("*** ");
        Serial.print(location);
        Serial.println(" ***");

        for (uint8_t slot = 0; slot < forecastSlots; slot++) {
            JsonVariant forecast = doc["list"][2 * slot + 1];
            const char *description = forecast["weather"][0]["description"];
            const int weatherId = forecast["weather"][0]["id"];
            const double temp = forecast["main"]["temp"];

            Serial.println("----------");
            Serial.printf("+%dh\n", 6 * (slot + 1));
            Serial.print("Type: ");
            Serial.println(description);
            Serial.print("Wetter ID: ");
            Serial.println(weatherId);
            Serial.print("Temp: ");
            Serial.print(temp);
            Serial.println("°C");

            determineWTemp(temp, wTemp[slot]);
            determineWid(weatherId, wWeather[slot]);
        }

        Serial.println("Hour");
        Serial.println(_hour);
        Serial.println("----------");

        determineDaytime(_hour);

        printDeterminedData();
    }

    //------------------------------------------------------------------------------

    bool checkWeatherCounter() {
        if (weatherCounter == 0) {
            weatherCounter = 600;
            return true;
        }
        return false;
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
    OpenWMap(/* args */) = default;
    ~OpenWMap() = default;

    //------------------------------------------------------------------------------

    void calcWeatherClockface() {
        if (wWeatherSwitch < 1 || wWeatherSwitch > forecastSlots) {
            return;
        }
        const uint8_t slot = wWeatherSwitch - 1;

        static const FrontWord daytime[7][2] = {
            {FrontWord::w_mittag},
            {FrontWord::w_abend},
            {FrontWord::w_nacht},
            {FrontWord::w_morgen, FrontWord::w_frueh},
            {FrontWord::w_morgen, FrontWord::w_mittag},
            {FrontWord::w_morgen, FrontWord::w_abend},
            {FrontWord::w_morgen, FrontWord::w_nacht},
        };
        if (wHour >= 1 && wHour <= 4) {
            showWords(daytime[slot + wHour - 1]);
        }

        showTemperature(wTemp[slot]);
        showCondition(wWeather[slot]);
    }

    //------------------------------------------------------------------------------

    void loop() {
        if (_second % 10 == 0) {
            wWeatherSwitch++;
            led.clear();
            if (wWeatherSwitch > 4) {
                wWeatherSwitch = 1;
            }
            Serial.print("wWeatherSwitch: ");
            Serial.println(wWeatherSwitch);
            Serial.print("wHour: ");
            Serial.println(wHour);
        }

        if (requestState != RequestState::Idle) {
            pollWeatherResponse();
        } else if (WiFi.status() == WL_CONNECTED && checkWeatherCounter()) {
            startWeatherRequest();
        }
        if (weatherCounter > 0) {
            weatherCounter--;
        }
    }
};
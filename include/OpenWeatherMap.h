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
    char response[3500];     // fixed size buffer
    uint16_t weatherCounter; // counter fuer Wetterdaten abrufen
    WiFiClient weatherClient;
    enum class RequestState : uint8_t { Idle, SkippingHeader, ReadingBody };
    RequestState requestState = RequestState::Idle;
    uint32_t requestStartMillis = 0;
    uint8_t headerEndMatched = 0;
    size_t responseLength = 0;
    static constexpr int32_t connectTimeoutMs = 2000;
    static constexpr uint32_t responseTimeoutMs = 10000;
    int8_t wTemp6;
    int8_t wTemp12;
    int8_t wTemp18;
    int8_t wTemp24;
    uint16_t wWeather6;
    uint16_t wWeather12;
    uint16_t wWeather18;
    uint16_t wWeather24;
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
        Serial.print("Temp_6 - ");
        Serial.println(wTemp6);
        Serial.print("wWeather6 - ");
        Serial.println(wWeather6);
        Serial.println("--------- ");
        Serial.print("Temp_12 - ");
        Serial.println(wTemp12);
        Serial.print("wWeather12 - ");
        Serial.println(wWeather12);
        Serial.println("--------- ");
        Serial.print("Temp_18 - ");
        Serial.println(wTemp18);
        Serial.print("wWeather18 - ");
        Serial.println(wWeather18);
        Serial.println("--------- ");
        Serial.print("Temp_24 - ");
        Serial.println(wTemp24);
        Serial.print("wWeather24 - ");
        Serial.println(wWeather24);
        Serial.println("--------- ");
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
            }
        }

        if (requestState == RequestState::ReadingBody &&
            (!weatherClient.connected() ||
             responseLength >= sizeof(response) - 1)) {
            finishWeatherRequest();
            processWeatherResponse();
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
        DynamicJsonDocument doc(6144);

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

        auto error = deserializeJson(doc, response);
        if (error) {
            Serial.print(F("deserializeJson() failed with code "));
            Serial.println(error.c_str());
            return;
        } else {
            Serial.println("JSON parsing worked!");
        }

        // Fill Variable with json information
        const char *location = doc["city"]["name"];
        const char *wetter_6 = doc["list"][1]["weather"][0]["description"];
        const int wetterid_6 = doc["list"][1]["weather"][0]["id"];
        double temp_6 = doc["list"][1]["main"]["temp"];
        const char *wetter_12 = doc["list"][3]["weather"][0]["description"];
        const int wetterid_12 = doc["list"][3]["weather"][0]["id"];
        double temp_12 = doc["list"][3]["main"]["temp"];
        const char *wetter_18 = doc["list"][5]["weather"][0]["description"];
        const int wetterid_18 = doc["list"][5]["weather"][0]["id"];
        double temp_18 = doc["list"][5]["main"]["temp"];
        const char *wetter_24 = doc["list"][7]["weather"][0]["description"];
        const int wetterid_24 = doc["list"][7]["weather"][0]["id"];
        double temp_24 = doc["list"][7]["main"]["temp"];

        Serial.print("*** ");
        Serial.print(location);
        Serial.println(" ***");
        Serial.println("----------");
        Serial.println("+6h");
        Serial.print("Type: ");
        Serial.println(wetter_6);
        Serial.print("Wetter ID: ");
        Serial.println(wetterid_6);
        Serial.print("Temp: ");
        Serial.print(temp_6);
        Serial.println("°C");
        Serial.println("----------");
        Serial.println("+12h");
        Serial.print("Type: ");
        Serial.println(wetter_12);
        Serial.print("Wetter ID: ");
        Serial.println(wetterid_12);
        Serial.print("Temp: ");
        Serial.print(temp_12);
        Serial.println("°C");
        Serial.println("----------");
        Serial.println("+18h");
        Serial.print("Type: ");
        Serial.println(wetter_18);
        Serial.print("Wetter ID: ");
        Serial.println(wetterid_18);
        Serial.print("Temp: ");
        Serial.print(temp_18);
        Serial.println("°C");
        Serial.println("----------");
        Serial.println("+24h");
        Serial.print("Type: ");
        Serial.println(wetter_24);
        Serial.print("Wetter ID: ");
        Serial.println(wetterid_24);
        Serial.print("Temp: ");
        Serial.print(temp_24);
        Serial.println("°C");
        Serial.println("Hour");
        Serial.println(_hour);
        Serial.println("----------");

        determineWTemp(temp_6, wTemp6);
        determineWid(wetterid_6, wWeather6);

        determineWTemp(temp_12, wTemp12);
        determineWid(wetterid_12, wWeather12);

        determineWTemp(temp_18, wTemp18);
        determineWid(wetterid_18, wWeather18);

        determineWTemp(temp_24, wTemp24);
        determineWid(wetterid_24, wWeather24);

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

public:
    OpenWMap(/* args */) = default;
    ~OpenWMap() = default;

    //------------------------------------------------------------------------------

    void calcWeatherClockface() {

        switch (wWeatherSwitch) {
            // +6h
        case 1: {
            switch (wHour) {
            case 1:
                usedClockType->show(FrontWord::w_mittag);
                break;
            case 2:
                usedClockType->show(FrontWord::w_abend);
                break;
            case 3:
                usedClockType->show(FrontWord::w_nacht);
                break;
            case 4: {
                usedClockType->show(FrontWord::w_morgen);
                usedClockType->show(FrontWord::w_frueh);
            } break;
            }
            switch (wTemp6) {
            case 30: {
                usedClockType->show(FrontWord::w_ueber);
                usedClockType->show(FrontWord::w_dreissig);
                usedClockType->show(FrontWord::w_grad);
            } break;
            case 25: {
                usedClockType->show(FrontWord::w_ueber);
                usedClockType->show(FrontWord::w_fuenf);
                usedClockType->show(FrontWord::w_und);
                usedClockType->show(FrontWord::w_zwanzig);
                usedClockType->show(FrontWord::w_grad);
            } break;
            case 20: {
                usedClockType->show(FrontWord::w_ueber);
                usedClockType->show(FrontWord::w_zwanzig);
                usedClockType->show(FrontWord::w_grad);
            } break;
            case 15: {
                usedClockType->show(FrontWord::w_ueber);
                usedClockType->show(FrontWord::w_fuenf);
                usedClockType->show(FrontWord::w_zehn);
                usedClockType->show(FrontWord::w_grad);
            } break;
            case 10: {
                usedClockType->show(FrontWord::w_ueber);
                usedClockType->show(FrontWord::w_zehn);
                usedClockType->show(FrontWord::w_grad);
            } break;
            case 5: {
                usedClockType->show(FrontWord::w_ueber);
                usedClockType->show(FrontWord::w_fuenf);
                usedClockType->show(FrontWord::w_grad);
            } break;
            case 1: {
                usedClockType->show(FrontWord::w_ueber);
                usedClockType->show(FrontWord::w_null);
                usedClockType->show(FrontWord::w_grad);
            } break;
            case -1: {
                usedClockType->show(FrontWord::w_unter);
                usedClockType->show(FrontWord::w_minus);
                usedClockType->show(FrontWord::w_null);
                usedClockType->show(FrontWord::w_grad);
            } break;
            case -5: {
                usedClockType->show(FrontWord::w_unter);
                usedClockType->show(FrontWord::w_minus);
                usedClockType->show(FrontWord::w_fuenf);
                usedClockType->show(FrontWord::w_grad);
            } break;
            case -10: {
                usedClockType->show(FrontWord::w_unter);
                usedClockType->show(FrontWord::w_minus);
                usedClockType->show(FrontWord::w_zehn);
                usedClockType->show(FrontWord::w_grad);
            } break;
            case -15: {
                usedClockType->show(FrontWord::w_unter);
                usedClockType->show(FrontWord::w_minus);
                usedClockType->show(FrontWord::w_fuenf);
                usedClockType->show(FrontWord::w_zehn);
                usedClockType->show(FrontWord::w_grad);
            } break;
            case -20: {
                usedClockType->show(FrontWord::w_unter);
                usedClockType->show(FrontWord::w_minus);
                usedClockType->show(FrontWord::w_zwanzig);
                usedClockType->show(FrontWord::w_grad);
            } break;
            case -25: {
                usedClockType->show(FrontWord::w_unter);
                usedClockType->show(FrontWord::w_minus);
                usedClockType->show(FrontWord::w_fuenf);
                usedClockType->show(FrontWord::w_und);
                usedClockType->show(FrontWord::w_zwanzig);
                usedClockType->show(FrontWord::w_grad);
            } break;
            }
            switch (wWeather6) {
            case 200:
                usedClockType->show(FrontWord::w_gewitter);
                break;
            case 300:
                usedClockType->show(FrontWord::w_regen);
                break;
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
        } break;
            // +12h
        case 2: {
            switch (wHour) {
            case 1:
                usedClockType->show(FrontWord::w_abend);
                break;
            case 2:
                usedClockType->show(FrontWord::w_nacht);
                break;
            case 3: {
                usedClockType->show(FrontWord::w_morgen);
                usedClockType->show(FrontWord::w_frueh);
            } break;
            case 4: {
                usedClockType->show(FrontWord::w_morgen);
                usedClockType->show(FrontWord::w_mittag);
            } break;
            }
            switch (wTemp12) {
            case 30: {
                usedClockType->show(FrontWord::w_ueber);
                usedClockType->show(FrontWord::w_dreissig);
                usedClockType->show(FrontWord::w_grad);
            } break;
            case 25: {
                usedClockType->show(FrontWord::w_ueber);
                usedClockType->show(FrontWord::w_fuenf);
                usedClockType->show(FrontWord::w_und);
                usedClockType->show(FrontWord::w_zwanzig);
                usedClockType->show(FrontWord::w_grad);
            } break;
            case 20: {
                usedClockType->show(FrontWord::w_ueber);
                usedClockType->show(FrontWord::w_zwanzig);
                usedClockType->show(FrontWord::w_grad);
            } break;
            case 15: {
                usedClockType->show(FrontWord::w_ueber);
                usedClockType->show(FrontWord::w_fuenf);
                usedClockType->show(FrontWord::w_zehn);
                usedClockType->show(FrontWord::w_grad);
            } break;
            case 10: {
                usedClockType->show(FrontWord::w_ueber);
                usedClockType->show(FrontWord::w_zehn);
                usedClockType->show(FrontWord::w_grad);
            } break;
            case 5: {
                usedClockType->show(FrontWord::w_ueber);
                usedClockType->show(FrontWord::w_fuenf);
                usedClockType->show(FrontWord::w_grad);
            } break;
            case 1: {
                usedClockType->show(FrontWord::w_ueber);
                usedClockType->show(FrontWord::w_null);
                usedClockType->show(FrontWord::w_grad);
            } break;
            case -1: {
                usedClockType->show(FrontWord::w_unter);
                usedClockType->show(FrontWord::w_minus);
                usedClockType->show(FrontWord::w_null);
                usedClockType->show(FrontWord::w_grad);
            } break;
            case -5: {
                usedClockType->show(FrontWord::w_unter);
                usedClockType->show(FrontWord::w_minus);
                usedClockType->show(FrontWord::w_fuenf);
                usedClockType->show(FrontWord::w_grad);
            } break;
            case -10: {
                usedClockType->show(FrontWord::w_unter);
                usedClockType->show(FrontWord::w_minus);
                usedClockType->show(FrontWord::w_zehn);
                usedClockType->show(FrontWord::w_grad);
            } break;
            case -15: {
                usedClockType->show(FrontWord::w_unter);
                usedClockType->show(FrontWord::w_minus);
                usedClockType->show(FrontWord::w_fuenf);
                usedClockType->show(FrontWord::w_zehn);
                usedClockType->show(FrontWord::w_grad);
            } break;
            case -20: {
                usedClockType->show(FrontWord::w_unter);
                usedClockType->show(FrontWord::w_minus);
                usedClockType->show(FrontWord::w_zwanzig);
                usedClockType->show(FrontWord::w_grad);
            } break;
            case -25: {
                usedClockType->show(FrontWord::w_unter);
                usedClockType->show(FrontWord::w_minus);
                usedClockType->show(FrontWord::w_fuenf);
                usedClockType->show(FrontWord::w_und);
                usedClockType->show(FrontWord::w_zwanzig);
                usedClockType->show(FrontWord::w_grad);
            } break;
            }
            switch (wWeather12) {
            case 200:
                usedClockType->show(FrontWord::w_gewitter);
                break;
            case 300:
                usedClockType->show(FrontWord::w_regen);
                break;
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

        } break;
            // +18h
        case 3: {
            switch (wHour) {
            case 1:
                usedClockType->show(FrontWord::w_nacht);
                break;
            case 2: {
                usedClockType->show(FrontWord::w_morgen);
                usedClockType->show(FrontWord::w_frueh);
            } break;
            case 3: {
                usedClockType->show(FrontWord::w_morgen);
                usedClockType->show(FrontWord::w_mittag);
            } break;
            case 4: {
                usedClockType->show(FrontWord::w_morgen);
                usedClockType->show(FrontWord::w_abend);
            } break;
            }
            switch (wTemp18) {
            case 30: {
                usedClockType->show(FrontWord::w_ueber);
                usedClockType->show(FrontWord::w_dreissig);
                usedClockType->show(FrontWord::w_grad);
            } break;
            case 25: {
                usedClockType->show(FrontWord::w_ueber);
                usedClockType->show(FrontWord::w_fuenf);
                usedClockType->show(FrontWord::w_und);
                usedClockType->show(FrontWord::w_zwanzig);
                usedClockType->show(FrontWord::w_grad);
            } break;
            case 20: {
                usedClockType->show(FrontWord::w_ueber);
                usedClockType->show(FrontWord::w_zwanzig);
                usedClockType->show(FrontWord::w_grad);
            } break;
            case 15: {
                usedClockType->show(FrontWord::w_ueber);
                usedClockType->show(FrontWord::w_fuenf);
                usedClockType->show(FrontWord::w_zehn);
                usedClockType->show(FrontWord::w_grad);
            } break;
            case 10: {
                usedClockType->show(FrontWord::w_ueber);
                usedClockType->show(FrontWord::w_zehn);
                usedClockType->show(FrontWord::w_grad);
            } break;
            case 5: {
                usedClockType->show(FrontWord::w_ueber);
                usedClockType->show(FrontWord::w_fuenf);
                usedClockType->show(FrontWord::w_grad);
            } break;
            case 1: {
                usedClockType->show(FrontWord::w_ueber);
                usedClockType->show(FrontWord::w_null);
                usedClockType->show(FrontWord::w_grad);
            } break;
            case -1: {
                usedClockType->show(FrontWord::w_unter);
                usedClockType->show(FrontWord::w_minus);
                usedClockType->show(FrontWord::w_null);
                usedClockType->show(FrontWord::w_grad);
            } break;
            case -5: {
                usedClockType->show(FrontWord::w_unter);
                usedClockType->show(FrontWord::w_minus);
                usedClockType->show(FrontWord::w_fuenf);
                usedClockType->show(FrontWord::w_grad);
            } break;
            case -10: {
                usedClockType->show(FrontWord::w_unter);
                usedClockType->show(FrontWord::w_minus);
                usedClockType->show(FrontWord::w_zehn);
                usedClockType->show(FrontWord::w_grad);
            } break;
            case -15: {
                usedClockType->show(FrontWord::w_unter);
                usedClockType->show(FrontWord::w_minus);
                usedClockType->show(FrontWord::w_fuenf);
                usedClockType->show(FrontWord::w_zehn);
                usedClockType->show(FrontWord::w_grad);
            } break;
            case -20: {
                usedClockType->show(FrontWord::w_unter);
                usedClockType->show(FrontWord::w_minus);
                usedClockType->show(FrontWord::w_zwanzig);
                usedClockType->show(FrontWord::w_grad);
            } break;
            case -25: {
                usedClockType->show(FrontWord::w_unter);
                usedClockType->show(FrontWord::w_minus);
                usedClockType->show(FrontWord::w_fuenf);
                usedClockType->show(FrontWord::w_und);
                usedClockType->show(FrontWord::w_zwanzig);
                usedClockType->show(FrontWord::w_grad);
            } break;
            }
            switch (wWeather18) {
            case 200:
                usedClockType->show(FrontWord::w_gewitter);
                break;
            case 300:
                usedClockType->show(FrontWord::w_regen);
                break;
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

        } break;
            // +24h
        case 4: {
            switch (wHour) {
            case 1: {
                usedClockType->show(FrontWord::w_morgen);
                usedClockType->show(FrontWord::w_frueh);
            } break;
            case 2: {
                usedClockType->show(FrontWord::w_morgen);
                usedClockType->show(FrontWord::w_mittag);
            } break;
            case 3: {
                usedClockType->show(FrontWord::w_morgen);
                usedClockType->show(FrontWord::w_abend);
            } break;
            case 4: {
                usedClockType->show(FrontWord::w_morgen);
                usedClockType->show(FrontWord::w_nacht);
            } break;
            }
            switch (wTemp24) {
            case 30: {
                usedClockType->show(FrontWord::w_ueber);
                usedClockType->show(FrontWord::w_dreissig);
                usedClockType->show(FrontWord::w_grad);
            } break;
            case 25: {
                usedClockType->show(FrontWord::w_ueber);
                usedClockType->show(FrontWord::w_fuenf);
                usedClockType->show(FrontWord::w_und);
                usedClockType->show(FrontWord::w_zwanzig);
                usedClockType->show(FrontWord::w_grad);
            } break;
            case 20: {
                usedClockType->show(FrontWord::w_ueber);
                usedClockType->show(FrontWord::w_zwanzig);
                usedClockType->show(FrontWord::w_grad);
            } break;
            case 15: {
                usedClockType->show(FrontWord::w_ueber);
                usedClockType->show(FrontWord::w_fuenf);
                usedClockType->show(FrontWord::w_zehn);
                usedClockType->show(FrontWord::w_grad);
            } break;
            case 10: {
                usedClockType->show(FrontWord::w_ueber);
                usedClockType->show(FrontWord::w_zehn);
                usedClockType->show(FrontWord::w_grad);
            } break;
            case 5: {
                usedClockType->show(FrontWord::w_ueber);
                usedClockType->show(FrontWord::w_fuenf);
                usedClockType->show(FrontWord::w_grad);
            } break;
            case 1: {
                usedClockType->show(FrontWord::w_ueber);
                usedClockType->show(FrontWord::w_null);
                usedClockType->show(FrontWord::w_grad);
            } break;
            case -1: {
                usedClockType->show(FrontWord::w_unter);
                usedClockType->show(FrontWord::w_minus);
                usedClockType->show(FrontWord::w_null);
                usedClockType->show(FrontWord::w_grad);
            } break;
            case -5: {
                usedClockType->show(FrontWord::w_unter);
                usedClockType->show(FrontWord::w_minus);
                usedClockType->show(FrontWord::w_fuenf);
                usedClockType->show(FrontWord::w_grad);
            } break;
            case -10: {
                usedClockType->show(FrontWord::w_unter);
                usedClockType->show(FrontWord::w_minus);
                usedClockType->show(FrontWord::w_zehn);
                usedClockType->show(FrontWord::w_grad);
            } break;
            case -15: {
                usedClockType->show(FrontWord::w_unter);
                usedClockType->show(FrontWord::w_minus);
                usedClockType->show(FrontWord::w_fuenf);
                usedClockType->show(FrontWord::w_zehn);
                usedClockType->show(FrontWord::w_grad);
            } break;
            case -20: {
                usedClockType->show(FrontWord::w_unter);
                usedClockType->show(FrontWord::w_minus);
                usedClockType->show(FrontWord::w_zwanzig);
                usedClockType->show(FrontWord::w_grad);
            } break;
            case -25: {
                usedClockType->show(FrontWord::w_unter);
                usedClockType->show(FrontWord::w_minus);
                usedClockType->show(FrontWord::w_fuenf);
                usedClockType->show(FrontWord::w_und);
                usedClockType->show(FrontWord::w_zwanzig);
                usedClockType->show(FrontWord::w_grad);
            } break;
            }
            switch (wWeather24) {
            case 200:
                usedClockType->show(FrontWord::w_gewitter);
                break;
            case 300:
                usedClockType->show(FrontWord::w_regen);
                break;
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

        } break;
        }
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
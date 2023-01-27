#pragma once

#include "Uhr.h"
#include <Arduino.h>
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
            if (hour <= 6 /* Timeframe for each Daytime*/) {
                wHour = countId + 1;
                break;
            }
            hour -= 6;
            countId++;
        }
    }

    //------------------------------------------------------------------------------

    void determineWTemp(const double srctemp, int8_t &destTemp) {
        if (srctemp >= 35) {
            destTemp = 35;
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

    void pullWeatherData() {

        // connect to server
        bool ok = client.connect(server, 80);
        bool beginFound = false;

        Serial.println("");
        Serial.println("--------------------------------------");
        Serial.println("Connecting to Openweathermap.org");
        Serial.println("--------------------------------------");

        // create calling URL
        memset(resource, 0, sizeof(resource));
        strncat(resource, resource1, 22);
        strncat(resource, G.openWeatherMap.cityid, 7);
        strncat(resource, resource2, 20);
        strncat(resource, G.openWeatherMap.apikey, 35);
        strncat(resource, resource3, 6);

        Serial.print("Calling URL: ");
        Serial.println(resource);

        if (ok == 1) {

            // Send request to resource
            client.print("GET ");
            client.print(resource);
            client.println(" HTTP/1.1");
            client.print("Host: ");
            client.println(server);
            client.println("Connection: close");
            client.println();

            delay(100);

            // Reading stream and remove headers
            client.setTimeout(10000);

            bool ok_header = client.find("\r\n\r\n");

            while (!ok_header) {
                // wait
            }

            memset(response, 0, sizeof(response));
            client.readBytes(response, 3500);

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
                while (!beginFound) {
                    if (int(response[i]) == 123) { // check for the "{"
                        beginFound = true;
                        Serial.println("{ found at ");
                        Serial.println(i);
                    }
                    i++;
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
        client.stop(); // disconnect from server
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
                usedUhrType->show(FrontWord::w_mittag);
                break;
            case 2:
                usedUhrType->show(FrontWord::w_abend);
                break;
            case 3:
                usedUhrType->show(FrontWord::w_nacht);
                break;
            case 4: {
                usedUhrType->show(FrontWord::w_morgen);
                usedUhrType->show(FrontWord::w_frueh);
            } break;
            }
            switch (wTemp6) {
            case 30: {
                usedUhrType->show(FrontWord::w_ueber);
                usedUhrType->show(FrontWord::w_dreissig);
                usedUhrType->show(FrontWord::w_grad);
            } break;
            case 25: {
                usedUhrType->show(FrontWord::w_ueber);
                usedUhrType->show(FrontWord::w_fuenf);
                usedUhrType->show(FrontWord::w_und);
                usedUhrType->show(FrontWord::w_zwanzig);
                usedUhrType->show(FrontWord::w_grad);
            } break;
            case 20: {
                usedUhrType->show(FrontWord::w_ueber);
                usedUhrType->show(FrontWord::w_zwanzig);
                usedUhrType->show(FrontWord::w_grad);
            } break;
            case 15: {
                usedUhrType->show(FrontWord::w_ueber);
                usedUhrType->show(FrontWord::w_fuenf);
                usedUhrType->show(FrontWord::w_zehn);
                usedUhrType->show(FrontWord::w_grad);
            } break;
            case 10: {
                usedUhrType->show(FrontWord::w_ueber);
                usedUhrType->show(FrontWord::w_zehn);
                usedUhrType->show(FrontWord::w_grad);
            } break;
            case 5: {
                usedUhrType->show(FrontWord::w_ueber);
                usedUhrType->show(FrontWord::w_fuenf);
                usedUhrType->show(FrontWord::w_grad);
            } break;
            case 1: {
                usedUhrType->show(FrontWord::w_ueber);
                usedUhrType->show(FrontWord::w_null);
                usedUhrType->show(FrontWord::w_grad);
            } break;
            case -1: {
                usedUhrType->show(FrontWord::w_unter);
                usedUhrType->show(FrontWord::w_minus);
                usedUhrType->show(FrontWord::w_null);
                usedUhrType->show(FrontWord::w_grad);
            } break;
            case -5: {
                usedUhrType->show(FrontWord::w_unter);
                usedUhrType->show(FrontWord::w_minus);
                usedUhrType->show(FrontWord::w_fuenf);
                usedUhrType->show(FrontWord::w_grad);
            } break;
            case -10: {
                usedUhrType->show(FrontWord::w_unter);
                usedUhrType->show(FrontWord::w_minus);
                usedUhrType->show(FrontWord::w_zehn);
                usedUhrType->show(FrontWord::w_grad);
            } break;
            case -15: {
                usedUhrType->show(FrontWord::w_unter);
                usedUhrType->show(FrontWord::w_minus);
                usedUhrType->show(FrontWord::w_fuenf);
                usedUhrType->show(FrontWord::w_zehn);
                usedUhrType->show(FrontWord::w_grad);
            } break;
            case -20: {
                usedUhrType->show(FrontWord::w_unter);
                usedUhrType->show(FrontWord::w_minus);
                usedUhrType->show(FrontWord::w_zwanzig);
                usedUhrType->show(FrontWord::w_grad);
            } break;
            case -25: {
                usedUhrType->show(FrontWord::w_unter);
                usedUhrType->show(FrontWord::w_minus);
                usedUhrType->show(FrontWord::w_fuenf);
                usedUhrType->show(FrontWord::w_und);
                usedUhrType->show(FrontWord::w_zwanzig);
                usedUhrType->show(FrontWord::w_grad);
            } break;
            }
            switch (wWeather6) {
            case 200:
                usedUhrType->show(FrontWord::w_gewitter);
                break;
            case 300:
                usedUhrType->show(FrontWord::w_regen);
                break;
            case 500:
                usedUhrType->show(FrontWord::w_regen);
                break;
            case 600:
                usedUhrType->show(FrontWord::w_schnee);
                break;
            case 700:
                usedUhrType->show(FrontWord::w_warnung);
                break;
            case 800:
                usedUhrType->show(FrontWord::w_klar);
                break;
            case 801:
                usedUhrType->show(FrontWord::w_wolken);
                break;
            }
        } break;
            // +12h
        case 2: {
            switch (wHour) {
            case 1:
                usedUhrType->show(FrontWord::w_abend);
                break;
            case 2:
                usedUhrType->show(FrontWord::w_nacht);
                break;
            case 3: {
                usedUhrType->show(FrontWord::w_morgen);
                usedUhrType->show(FrontWord::w_frueh);
            } break;
            case 4: {
                usedUhrType->show(FrontWord::w_morgen);
                usedUhrType->show(FrontWord::w_mittag);
            } break;
            }
            switch (wTemp12) {
            case 30: {
                usedUhrType->show(FrontWord::w_ueber);
                usedUhrType->show(FrontWord::w_dreissig);
                usedUhrType->show(FrontWord::w_grad);
            } break;
            case 25: {
                usedUhrType->show(FrontWord::w_ueber);
                usedUhrType->show(FrontWord::w_fuenf);
                usedUhrType->show(FrontWord::w_und);
                usedUhrType->show(FrontWord::w_zwanzig);
                usedUhrType->show(FrontWord::w_grad);
            } break;
            case 20: {
                usedUhrType->show(FrontWord::w_ueber);
                usedUhrType->show(FrontWord::w_zwanzig);
                usedUhrType->show(FrontWord::w_grad);
            } break;
            case 15: {
                usedUhrType->show(FrontWord::w_ueber);
                usedUhrType->show(FrontWord::w_fuenf);
                usedUhrType->show(FrontWord::w_zehn);
                usedUhrType->show(FrontWord::w_grad);
            } break;
            case 10: {
                usedUhrType->show(FrontWord::w_ueber);
                usedUhrType->show(FrontWord::w_zehn);
                usedUhrType->show(FrontWord::w_grad);
            } break;
            case 5: {
                usedUhrType->show(FrontWord::w_ueber);
                usedUhrType->show(FrontWord::w_fuenf);
                usedUhrType->show(FrontWord::w_grad);
            } break;
            case 1: {
                usedUhrType->show(FrontWord::w_ueber);
                usedUhrType->show(FrontWord::w_null);
                usedUhrType->show(FrontWord::w_grad);
            } break;
            case -1: {
                usedUhrType->show(FrontWord::w_unter);
                usedUhrType->show(FrontWord::w_minus);
                usedUhrType->show(FrontWord::w_null);
                usedUhrType->show(FrontWord::w_grad);
            } break;
            case -5: {
                usedUhrType->show(FrontWord::w_unter);
                usedUhrType->show(FrontWord::w_minus);
                usedUhrType->show(FrontWord::w_fuenf);
                usedUhrType->show(FrontWord::w_grad);
            } break;
            case -10: {
                usedUhrType->show(FrontWord::w_unter);
                usedUhrType->show(FrontWord::w_minus);
                usedUhrType->show(FrontWord::w_zehn);
                usedUhrType->show(FrontWord::w_grad);
            } break;
            case -15: {
                usedUhrType->show(FrontWord::w_unter);
                usedUhrType->show(FrontWord::w_minus);
                usedUhrType->show(FrontWord::w_fuenf);
                usedUhrType->show(FrontWord::w_zehn);
                usedUhrType->show(FrontWord::w_grad);
            } break;
            case -20: {
                usedUhrType->show(FrontWord::w_unter);
                usedUhrType->show(FrontWord::w_minus);
                usedUhrType->show(FrontWord::w_zwanzig);
                usedUhrType->show(FrontWord::w_grad);
            } break;
            case -25: {
                usedUhrType->show(FrontWord::w_unter);
                usedUhrType->show(FrontWord::w_minus);
                usedUhrType->show(FrontWord::w_fuenf);
                usedUhrType->show(FrontWord::w_und);
                usedUhrType->show(FrontWord::w_zwanzig);
                usedUhrType->show(FrontWord::w_grad);
            } break;
            }
            switch (wWeather12) {
            case 200:
                usedUhrType->show(FrontWord::w_gewitter);
                break;
            case 300:
                usedUhrType->show(FrontWord::w_regen);
                break;
            case 500:
                usedUhrType->show(FrontWord::w_regen);
                break;
            case 600:
                usedUhrType->show(FrontWord::w_schnee);
                break;
            case 700:
                usedUhrType->show(FrontWord::w_warnung);
                break;
            case 800:
                usedUhrType->show(FrontWord::w_klar);
                break;
            case 801:
                usedUhrType->show(FrontWord::w_wolken);
                break;
            }

        } break;
            // +18h
        case 3: {
            switch (wHour) {
            case 1:
                usedUhrType->show(FrontWord::w_nacht);
                break;
            case 2: {
                usedUhrType->show(FrontWord::w_morgen);
                usedUhrType->show(FrontWord::w_frueh);
            } break;
            case 3: {
                usedUhrType->show(FrontWord::w_morgen);
                usedUhrType->show(FrontWord::w_mittag);
            } break;
            case 4: {
                usedUhrType->show(FrontWord::w_morgen);
                usedUhrType->show(FrontWord::w_abend);
            } break;
            }
            switch (wTemp18) {
            case 30: {
                usedUhrType->show(FrontWord::w_ueber);
                usedUhrType->show(FrontWord::w_dreissig);
                usedUhrType->show(FrontWord::w_grad);
            } break;
            case 25: {
                usedUhrType->show(FrontWord::w_ueber);
                usedUhrType->show(FrontWord::w_fuenf);
                usedUhrType->show(FrontWord::w_und);
                usedUhrType->show(FrontWord::w_zwanzig);
                usedUhrType->show(FrontWord::w_grad);
            } break;
            case 20: {
                usedUhrType->show(FrontWord::w_ueber);
                usedUhrType->show(FrontWord::w_zwanzig);
                usedUhrType->show(FrontWord::w_grad);
            } break;
            case 15: {
                usedUhrType->show(FrontWord::w_ueber);
                usedUhrType->show(FrontWord::w_fuenf);
                usedUhrType->show(FrontWord::w_zehn);
                usedUhrType->show(FrontWord::w_grad);
            } break;
            case 10: {
                usedUhrType->show(FrontWord::w_ueber);
                usedUhrType->show(FrontWord::w_zehn);
                usedUhrType->show(FrontWord::w_grad);
            } break;
            case 5: {
                usedUhrType->show(FrontWord::w_ueber);
                usedUhrType->show(FrontWord::w_fuenf);
                usedUhrType->show(FrontWord::w_grad);
            } break;
            case 1: {
                usedUhrType->show(FrontWord::w_ueber);
                usedUhrType->show(FrontWord::w_null);
                usedUhrType->show(FrontWord::w_grad);
            } break;
            case -1: {
                usedUhrType->show(FrontWord::w_unter);
                usedUhrType->show(FrontWord::w_minus);
                usedUhrType->show(FrontWord::w_null);
                usedUhrType->show(FrontWord::w_grad);
            } break;
            case -5: {
                usedUhrType->show(FrontWord::w_unter);
                usedUhrType->show(FrontWord::w_minus);
                usedUhrType->show(FrontWord::w_fuenf);
                usedUhrType->show(FrontWord::w_grad);
            } break;
            case -10: {
                usedUhrType->show(FrontWord::w_unter);
                usedUhrType->show(FrontWord::w_minus);
                usedUhrType->show(FrontWord::w_zehn);
                usedUhrType->show(FrontWord::w_grad);
            } break;
            case -15: {
                usedUhrType->show(FrontWord::w_unter);
                usedUhrType->show(FrontWord::w_minus);
                usedUhrType->show(FrontWord::w_fuenf);
                usedUhrType->show(FrontWord::w_zehn);
                usedUhrType->show(FrontWord::w_grad);
            } break;
            case -20: {
                usedUhrType->show(FrontWord::w_unter);
                usedUhrType->show(FrontWord::w_minus);
                usedUhrType->show(FrontWord::w_zwanzig);
                usedUhrType->show(FrontWord::w_grad);
            } break;
            case -25: {
                usedUhrType->show(FrontWord::w_unter);
                usedUhrType->show(FrontWord::w_minus);
                usedUhrType->show(FrontWord::w_fuenf);
                usedUhrType->show(FrontWord::w_und);
                usedUhrType->show(FrontWord::w_zwanzig);
                usedUhrType->show(FrontWord::w_grad);
            } break;
            }
            switch (wWeather18) {
            case 200:
                usedUhrType->show(FrontWord::w_gewitter);
                break;
            case 300:
                usedUhrType->show(FrontWord::w_regen);
                break;
            case 500:
                usedUhrType->show(FrontWord::w_regen);
                break;
            case 600:
                usedUhrType->show(FrontWord::w_schnee);
                break;
            case 700:
                usedUhrType->show(FrontWord::w_warnung);
                break;
            case 800:
                usedUhrType->show(FrontWord::w_klar);
                break;
            case 801:
                usedUhrType->show(FrontWord::w_wolken);
                break;
            }

        } break;
            // +24h
        case 4: {
            switch (wHour) {
            case 1: {
                usedUhrType->show(FrontWord::w_morgen);
                usedUhrType->show(FrontWord::w_frueh);
            } break;
            case 2: {
                usedUhrType->show(FrontWord::w_morgen);
                usedUhrType->show(FrontWord::w_mittag);
            } break;
            case 3: {
                usedUhrType->show(FrontWord::w_morgen);
                usedUhrType->show(FrontWord::w_abend);
            } break;
            case 4: {
                usedUhrType->show(FrontWord::w_morgen);
                usedUhrType->show(FrontWord::w_nacht);
            } break;
            }
            switch (wTemp24) {
            case 30: {
                usedUhrType->show(FrontWord::w_ueber);
                usedUhrType->show(FrontWord::w_dreissig);
                usedUhrType->show(FrontWord::w_grad);
            } break;
            case 25: {
                usedUhrType->show(FrontWord::w_ueber);
                usedUhrType->show(FrontWord::w_fuenf);
                usedUhrType->show(FrontWord::w_und);
                usedUhrType->show(FrontWord::w_zwanzig);
                usedUhrType->show(FrontWord::w_grad);
            } break;
            case 20: {
                usedUhrType->show(FrontWord::w_ueber);
                usedUhrType->show(FrontWord::w_zwanzig);
                usedUhrType->show(FrontWord::w_grad);
            } break;
            case 15: {
                usedUhrType->show(FrontWord::w_ueber);
                usedUhrType->show(FrontWord::w_fuenf);
                usedUhrType->show(FrontWord::w_zehn);
                usedUhrType->show(FrontWord::w_grad);
            } break;
            case 10: {
                usedUhrType->show(FrontWord::w_ueber);
                usedUhrType->show(FrontWord::w_zehn);
                usedUhrType->show(FrontWord::w_grad);
            } break;
            case 5: {
                usedUhrType->show(FrontWord::w_ueber);
                usedUhrType->show(FrontWord::w_fuenf);
                usedUhrType->show(FrontWord::w_grad);
            } break;
            case 1: {
                usedUhrType->show(FrontWord::w_ueber);
                usedUhrType->show(FrontWord::w_null);
                usedUhrType->show(FrontWord::w_grad);
            } break;
            case -1: {
                usedUhrType->show(FrontWord::w_unter);
                usedUhrType->show(FrontWord::w_minus);
                usedUhrType->show(FrontWord::w_null);
                usedUhrType->show(FrontWord::w_grad);
            } break;
            case -5: {
                usedUhrType->show(FrontWord::w_unter);
                usedUhrType->show(FrontWord::w_minus);
                usedUhrType->show(FrontWord::w_fuenf);
                usedUhrType->show(FrontWord::w_grad);
            } break;
            case -10: {
                usedUhrType->show(FrontWord::w_unter);
                usedUhrType->show(FrontWord::w_minus);
                usedUhrType->show(FrontWord::w_zehn);
                usedUhrType->show(FrontWord::w_grad);
            } break;
            case -15: {
                usedUhrType->show(FrontWord::w_unter);
                usedUhrType->show(FrontWord::w_minus);
                usedUhrType->show(FrontWord::w_fuenf);
                usedUhrType->show(FrontWord::w_zehn);
                usedUhrType->show(FrontWord::w_grad);
            } break;
            case -20: {
                usedUhrType->show(FrontWord::w_unter);
                usedUhrType->show(FrontWord::w_minus);
                usedUhrType->show(FrontWord::w_zwanzig);
                usedUhrType->show(FrontWord::w_grad);
            } break;
            case -25: {
                usedUhrType->show(FrontWord::w_unter);
                usedUhrType->show(FrontWord::w_minus);
                usedUhrType->show(FrontWord::w_fuenf);
                usedUhrType->show(FrontWord::w_und);
                usedUhrType->show(FrontWord::w_zwanzig);
                usedUhrType->show(FrontWord::w_grad);
            } break;
            }
            switch (wWeather24) {
            case 200:
                usedUhrType->show(FrontWord::w_gewitter);
                break;
            case 300:
                usedUhrType->show(FrontWord::w_regen);
                break;
            case 500:
                usedUhrType->show(FrontWord::w_regen);
                break;
            case 600:
                usedUhrType->show(FrontWord::w_schnee);
                break;
            case 700:
                usedUhrType->show(FrontWord::w_warnung);
                break;
            case 800:
                usedUhrType->show(FrontWord::w_klar);
                break;
            case 801:
                usedUhrType->show(FrontWord::w_wolken);
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

        if (WiFi.status() == WL_CONNECTED && checkWeatherCounter()) {
            pullWeatherData();
        }
        weatherCounter--;
    }
};
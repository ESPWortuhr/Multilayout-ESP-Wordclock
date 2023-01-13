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
    void determineDaytime(uint8_t hour) {
        uint8_t count_id = 0;
        hour += 24; // Offset by 24 hours
        hour -= 3;  // Offset to get corrospondance
        hour %= 24; // Offset for nighttime

        for (uint8_t i = 0; i < 4; i++) {
            if (hour <= 6 /* Timeframe for each Daytime*/) {
                wstunde = count_id + 1;
                break;
            }
            hour -= 6;
            count_id++;
        }
    }

    void determineWTemp(const double srctemp, int8_t &destTemp) {
        if (srctemp >= 35) {
            destTemp = 35;
        } else if (srctemp >= 0 && srctemp <= 5) {
            destTemp = 1;
        } else if (srctemp < 0 && srctemp >= -5) {
            destTemp = -1;
        } else if (srctemp <= -25) {
            destTemp = -25;
        }
        destTemp = static_cast<int8_t>(srctemp - fmod(srctemp, 5));
    }

    void determineWid(uint16_t srcId, uint16_t &destId) {
        uint8_t count_id = 0;
        if (srcId == 800) {
            destId = 800;
        } else {
            for (uint8_t i = 0; i < 9; i++) {
                if (srcId < 100) {
                    if (count_id < 2) {
                        Serial.println("[ERROR] determineWid() Out of bounds");
                    }
                    destId = count_id * 100;
                }
                srcId -= 100;
                count_id++;
            }
        }
    }

    void printDeterminedData() {
        Serial.print("Temp_6 - ");
        Serial.println(wtemp_6);
        Serial.print("wwetter_6 - ");
        Serial.println(wwetter_6);
        Serial.println("--------- ");
        Serial.print("Temp_12 - ");
        Serial.println(wtemp_12);
        Serial.print("wwetter_12 - ");
        Serial.println(wwetter_12);
        Serial.println("--------- ");
        Serial.print("Temp_18 - ");
        Serial.println(wtemp_18);
        Serial.print("wwetter_18 - ");
        Serial.println(wwetter_18);
        Serial.println("--------- ");
        Serial.print("Temp_24 - ");
        Serial.println(wtemp_24);
        Serial.print("wwetter_24 - ");
        Serial.println(wwetter_24);
        Serial.println("--------- ");
        Serial.print("wstunde - ");
        Serial.println(wstunde);
        Serial.println("--------- ");
    }

public:
    OpenWMap(/* args */) = default;
    ~OpenWMap() = default;

    void loop() {

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
            Serial.println("Stunde");
            Serial.println(_stunde);
            Serial.println("----------");

            determineWTemp(temp_6, wtemp_6);
            determineWid(wetterid_6, wwetter_6);

            determineWTemp(temp_12, wtemp_12);
            determineWid(wetterid_12, wwetter_12);

            determineWTemp(temp_18, wtemp_18);
            determineWid(wetterid_18, wwetter_18);

            determineWTemp(temp_24, wtemp_24);
            determineWid(wetterid_24, wwetter_24);

            determineDaytime(_stunde);

            printDeterminedData();
        }
        client.stop(); // disconnect from server
    }
};

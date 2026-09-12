#include "OpenWeatherMap.h"

#include "Config.h"
#include "Led.h"
#include "SensitiveData.h"
#include "WordClockState.h"
#include "WordClockTypes/ClockType.hpp"
#include <Arduino.h>
#include <ArduinoJson.h>
#ifdef ESP8266
#include <ESP8266WiFi.h>
#elif defined(ESP32)
#include <WiFi.h>
#endif

extern ClockType *usedClockType;
extern Led led;

OpenWMap weather;

namespace {

constexpr const char *server = "api.openweathermap.org";
constexpr int32_t connectTimeoutMs = 2000;
constexpr uint32_t responseTimeoutMs = 10000;
constexpr uint32_t bodyTimeoutMs = 1000;
constexpr uint32_t requestIntervalMs = 10UL * 60UL * 1000UL;
constexpr uint32_t forecastMaxAgeMs =
    3 * requestIntervalMs + requestIntervalMs / 2;

template <size_t N> void buildResource(char (&dest)[N], const char *apikey) {
    snprintf(dest, N, "/data/2.5/forecast?id=%s&units=metric&APPID=%s&cnt=%u",
             G.openWeatherMap.cityid, apikey,
             static_cast<unsigned>(2 * forecast::slots));
}

} // namespace

//------------------------------------------------------------------------------

void OpenWMap::calcWeatherClockface() {
    if (!forecastValid) {
        return;
    }

    for (FrontWord word : forecast::wordsFor(
             shownSlot, daytime, wTemp[shownSlot], wWeather[shownSlot])) {
        usedClockType->show(word);
    }
}

//------------------------------------------------------------------------------

void OpenWMap::loop() {
    if (forecastValid && millis() - forecastMillis > forecastMaxAgeMs) {
        forecastValid = false;
        Serial.println("Weather forecast is outdated, hiding it");
    }

    if (_second % 10 == 0) {
        shownSlot = (shownSlot + 1) % forecast::slots;
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

//------------------------------------------------------------------------------

bool OpenWMap::requestDue() const {
    return !requestedOnce || millis() - lastRequestMillis >= requestIntervalMs;
}

//------------------------------------------------------------------------------

bool OpenWMap::connectToServer() {
    if (static_cast<uint32_t>(serverIp) == 0) {
#ifdef ESP8266
        const int resolved =
            WiFi.hostByName(server, serverIp, connectTimeoutMs);
#else
        const int resolved = WiFi.hostByName(server, serverIp);
#endif
        if (resolved != 1) {
            serverIp = IPAddress();
            Serial.println("DNS lookup for Openweathermap.org failed");
            return false;
        }
    }

#ifdef ESP8266
    weatherClient.setTimeout(connectTimeoutMs);
    const bool ok = weatherClient.connect(serverIp, 80);
#else
    const bool ok = weatherClient.connect(serverIp, 80, connectTimeoutMs);
#endif
    if (!ok) {
        serverIp = IPAddress();
        Serial.println("Connection to Openweathermap.org failed");
    }
    return ok;
}

//------------------------------------------------------------------------------

void OpenWMap::startWeatherRequest() {
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

    if (!connectToServer()) {
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

void OpenWMap::finishWeatherRequest() {
    weatherClient.stop();
    requestState = RequestState::Idle;
}

//------------------------------------------------------------------------------

void OpenWMap::pollWeatherResponse() {
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

void OpenWMap::processWeatherResponse() {
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
        doc["list"].size() < 2 * forecast::slots) {
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

    for (uint8_t slot = 0; slot < forecast::slots; slot++) {
        JsonVariant entry = doc["list"][2 * slot + 1];
        const int weatherId = entry["weather"][0]["id"];
        const double temp = entry["main"]["temp"];

#if WEATHER_VERBOSE
        Serial.println("----------");
        Serial.printf("+%dh\n", 6 * (slot + 1));
        Serial.print("Type: ");
        Serial.println(entry["weather"][0]["description"].as<const char *>());
        Serial.print("Wetter ID: ");
        Serial.println(weatherId);
        Serial.print("Temp: ");
        Serial.print(temp);
        Serial.println("°C");
#endif

        wTemp[slot] = forecast::temperatureStep(temp);
        wWeather[slot] =
            forecast::conditionGroup(static_cast<uint16_t>(weatherId));
        if (wWeather[slot] == 0) {
            Serial.printf("[ERROR] Unknown weather id %d\n", weatherId);
        }
    }

    daytime = forecast::daytimeFor(_hour);
    forecastValid = true;
    forecastMillis = millis();

#if WEATHER_VERBOSE
    Serial.println("Hour");
    Serial.println(_hour);
    Serial.println("----------");
    printDeterminedData();
#endif
}

//------------------------------------------------------------------------------

void OpenWMap::printDeterminedData() {
    for (uint8_t slot = 0; slot < forecast::slots; slot++) {
        const int hours = 6 * (slot + 1);
        Serial.printf("Temp_%d - %d\n", hours, wTemp[slot]);
        Serial.printf("wWeather%d - %u\n", hours, wWeather[slot]);
        Serial.println("--------- ");
    }
    Serial.print("daytime - ");
    Serial.println(daytime);
    Serial.println("--------- ");
}

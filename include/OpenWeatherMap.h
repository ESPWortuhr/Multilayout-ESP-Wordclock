#pragma once

#include "WeatherMapping.h"
#include <WiFiClient.h>
#include <stdint.h>

class OpenWMap {
public:
    void calcWeatherClockface();
    void loop();

private:
    enum class RequestState : uint8_t { Idle, SkippingHeader };

    bool requestDue() const;
    void startWeatherRequest();
    void finishWeatherRequest();
    void pollWeatherResponse();
    void processWeatherResponse();
    void printDeterminedData();

    WiFiClient weatherClient;
    RequestState requestState = RequestState::Idle;
    uint32_t requestStartMillis = 0;
    uint32_t lastRequestMillis = 0;
    bool requestedOnce = false;
    uint8_t headerEndMatched = 0;
    int8_t wTemp[forecast::slots] = {};
    uint16_t wWeather[forecast::slots] = {};
    uint8_t daytime = 0;
    uint8_t shownSlot = 0;
    bool forecastValid = false;
};

extern OpenWMap weather;

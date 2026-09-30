#include "WifiHelper.h"

// ClockWork.h is not self-contained yet; include the types it relies on first
// (matching the unity-build order) so it can be pulled into this separate TU.
#include "WordClockState.h"
#include "WordClockTypes/ClockType.hpp"

#include "ClockWork.h"
#include <Arduino.h>

extern ClockWork clockWork;

namespace {

bool wifiConnected = false;

void logGotIp(const IPAddress &ip) {
    wifiConnected = true;
    Serial.print("[WiFi] connected, IP: ");
    Serial.println(ip);
}

void logDisconnected(int reason) {
    if (!wifiConnected) {
        return;
    }
    wifiConnected = false;
    Serial.printf("[WiFi] connection lost (reason %d)\n", reason);
}

#ifdef ESP8266
WiFiEventHandler gotIpHandler;
WiFiEventHandler disconnectedHandler;
#endif

void registerWifiEventLogging() {
    wifiConnected = WiFi.isConnected();
#ifdef ESP8266
    gotIpHandler = WiFi.onStationModeGotIP(
        [](const WiFiEventStationModeGotIP &event) { logGotIp(event.ip); });
    disconnectedHandler = WiFi.onStationModeDisconnected(
        [](const WiFiEventStationModeDisconnected &event) {
            logDisconnected(event.reason);
        });
#elif defined(ESP32)
    WiFi.onEvent(
        [](arduino_event_id_t, arduino_event_info_t info) {
            logGotIp(IPAddress(info.got_ip.ip_info.ip.addr));
        },
        ARDUINO_EVENT_WIFI_STA_GOT_IP);
    WiFi.onEvent(
        [](arduino_event_id_t, arduino_event_info_t info) {
            logDisconnected(info.wifi_sta_disconnected.reason);
        },
        ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
#endif
}

} // namespace

//------------------------------------------------------------------------------

void wifiStart() {
    Serial.printf("\n-- Begin Wlan -- \n");

    char ip_adress[20];
    sprintf(ip_adress, "IP:%d.%d.%d.%d", WiFi.localIP()[0], WiFi.localIP()[1],
            WiFi.localIP()[2], WiFi.localIP()[3]);
    Serial.println(ip_adress);
    Serial.println("");
    // Show IP as scrollingtext
    if (G.bootShowIP) {
        clockWork.initBootShowIp(ip_adress);
    }

    registerWifiEventLogging();

    Serial.printf("-- End  Wlan -- \n\n");
}

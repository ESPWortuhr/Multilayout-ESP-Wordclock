#include "Network.h"

#include "Config.h" // WIFI_VERBOSE, MANUAL_WIFI_SETTINGS, CP_* ...
#include <WiFiManager.h>

namespace {
// The WiFiManager instance is an implementation detail of Network and is not
// used outside this translation unit.
WiFiManager wifiManager(Serial);
} // namespace

void Network::info() {
#if WIFI_VERBOSE
    WiFi.printDiag(Serial);
#endif
}

int Network::getQuality() {
    int rssi = WiFi.RSSI();
    return wifiManager.getRSSIasQuality(rssi);
}

void Network::rtcMode() { wifiManager.setConfigPortalTimeout(120); }

void Network::disable() { wifiManager.disconnect(); }

void Network::reboot() { wifiManager.reboot(); }

void Network::resetSettings() {
    wifiManager.resetSettings();
    wifiManager.reboot();
}

String Network::getSSID() { return wifiManager.getWiFiSSID(); }

void Network::setup(const char *hostname) {
    wifiManager.setHostname(hostname);
#if MANUAL_WIFI_SETTINGS
    wifiManager.preloadWiFi(WIFI_SSID, WIFI_PASSWORD);
#endif
    wifiManager.setConnectTimeout(20);
#if CP_PROTECTED
    wifiManager.autoConnect(CP_SSID, CP_PASSWORD);
#else
    wifiManager.autoConnect(CP_SSID);
#endif
    // explicitly disable AP, esp defaults to STA+AP
    WiFi.enableAP(false);

#if defined(ESP8266)
    WiFi.setSleepMode(WIFI_NONE_SLEEP);
#elif defined(ESP32)
    WiFi.setSleep(false);
#endif
    WiFi.setAutoReconnect(true);

    Network::info();
}

void Network::startConfigPortal() {
#if CP_PROTECTED
    wifiManager.startConfigPortal(CP_SSID, CP_PASSWORD);
#else
    wifiManager.startConfigPortal(CP_SSID);
#endif
}

void Network::loop() { wifiManager.process(); }

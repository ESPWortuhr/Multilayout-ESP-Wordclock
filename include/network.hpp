#include "network.h"

#include <WiFiManager.h>

WiFiManager wifiManager(Serial);

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
    Network::info();
}

void Network::loop() { wifiManager.process(); }

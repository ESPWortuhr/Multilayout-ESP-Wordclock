#include "network.h"

#include <WiFiManager.h>

WiFiManager wifiManager(Serial);

void Network::info() { WiFi.printDiag(Serial); }

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
    if (G.transitionDemo) {
        wifiManager.setTimeout(30);
    }
    wifiManager.autoConnect(connectionSSID);
    if (WiFi.status() == WL_CONNECTED) {
        G.transitionDemo = false;
    }
    // explicitly disable AP, esp defaults to STA+AP
    WiFi.enableAP(false);
    Network::info();
}

void Network::loop() { wifiManager.process(); }

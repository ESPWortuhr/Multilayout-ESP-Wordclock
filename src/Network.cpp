#include "Network.h"

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiManager.h>

WiFiManager wifiManager(Serial);

const char *connectionSSID = "WortUhr_verbinden";

void Network_info() { WiFi.printDiag(Serial); }

int Network_getQuality() {
    int rssi = WiFi.RSSI();
    return wifiManager.getRSSIasQuality(rssi);
}

void Network_rtcMode() {
    wifiManager.setConfigPortalTimeout(120);
}

void Network_disable() { wifiManager.disconnect(); }

void Network_reboot() { wifiManager.reboot(); }

void Network_resetSettings() {
    wifiManager.resetSettings();
    wifiManager.reboot();
}

String Network_getSSID() { return wifiManager.getWiFiSSID(); }

void Network_setup(const char *hostname) {
    wifiManager.setHostname(hostname);
    wifiManager.autoConnect(connectionSSID);
    // explicitly disable AP, esp defaults to STA+AP
    WiFi.enableAP(false);
    Network_info();
}

void Network_loop() { wifiManager.process(); }

#include "Network.h"

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>
#include <WiFiClient.h>
#include <WifiManager.h>

WiFiManager wifiManager;

const char * connectionSSID = "WortUhr_verbinden";

void Network_info(){
	WiFi.printDiag(Serial);
}

void Network_disable() {
	wifiManager.disconnect();
}

void Network_reboot() {
	wifiManager.reboot();
}

void Network_resetSettings() {
	wifiManager.resetSettings();
	wifiManager.reboot();
}

String Network_getSSID() {
	return wifiManager.getWiFiSSID();
}

void Network_setup(const char * hostname) {
	wifiManager.setHostname(hostname);
	wifiManager.autoConnect(connectionSSID);
	Network_info();
}

void Network_loop() {
	wifiManager.process();
}

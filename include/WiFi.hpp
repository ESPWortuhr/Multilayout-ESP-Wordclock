#pragma once

#ifdef ESP8266
#include <ESP8266WiFi.h>
#elif defined(ESP32)
#include <WiFi.h>
#endif

//---------------------------------------------------------
// WLAN-Status
//---------------------------------------------------------
char wstatus[7][25] = {"WL_IDLE_STATUS",    "WL_NO_SSID_AVAIL",
                       "WL_SCAN_COMPLETED", "WL_CONNECTED",
                       "WL_CONNECT_FAILED", "WL_CONNECTION_LOST",
                       "WL_DISCONNECTED"};
// WL_NO_SHIELD        = 255,   // for compatibility with WiFi Shield library
// WL_IDLE_STATUS      = 0,
// WL_NO_SSID_AVAIL    = 1,
// WL_SCAN_COMPLETED   = 2,
// WL_CONNECTED        = 3,
// WL_CONNECT_FAILED   = 4,
// WL_CONNECTION_LOST  = 5,
// WL_DISCONNECTED     = 6
//---------------------------------------------------------
// WIFI_EVENT_STAMODE_CONNECTED = 0,
// WIFI_EVENT_STAMODE_DISCONNECTED
// WIFI_EVENT_STAMODE_AUTHMODE_CHANGE
// WIFI_EVENT_STAMODE_GOT_IP
// WIFI_EVENT_STAMODE_DHCP_TIMEOUT
// WIFI_EVENT_SOFTAPMODE_STACONNECTED
//---------------------------------------------------------

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

    Serial.printf("-- End  Wlan -- \n\n");
}

//------------------------------------------------------------------------------

void handleWiFiEvent(const char *eventType, const IPAddress &ip) {
    Serial.printf("[WiFi-event] event: %s\n", eventType);
    if (strcmp(eventType, "GOT_IP") == 0) {
        Serial.println("WiFi connected");
        Serial.println("IP address: ");
        Serial.println(ip);
    } else if (strcmp(eventType, "DISCONNECTED") == 0) {
        Serial.println("WiFi lost connection");
    }
}

//------------------------------------------------------------------------------

#ifdef ESP8266
void WiFiEvent(WiFiEvent_t event) {
    switch (event) {
    case WIFI_EVENT_STAMODE_GOT_IP:
        handleWiFiEvent("GOT_IP", WiFi.localIP());
        break;
    case WIFI_EVENT_STAMODE_DISCONNECTED:
        handleWiFiEvent("DISCONNECTED", IPAddress());
        break;
    default:
        break;
    }
}
#elif defined(ESP32)
void WiFiEvent(WiFiEvent_t event, WiFiEventInfo_t info) {
    switch (event) {
    case SYSTEM_EVENT_STA_GOT_IP:
        handleWiFiEvent("GOT_IP", WiFi.localIP());
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        handleWiFiEvent("DISCONNECTED", IPAddress());
        break;
    default:
        break;
    }
}
#endif
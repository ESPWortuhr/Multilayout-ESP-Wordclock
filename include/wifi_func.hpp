#pragma once

//---------------------------------------------------------
// WLAN-Status
//---------------------------------------------------------
char wstatus[7][25] = {

    "WL_IDLE_STATUS", "WL_NO_SSID_AVAIL",  "WL_SCAN_COMPLETED",
    "WL_CONNECTED",   "WL_CONNECT_FAILED", "WL_CONNECTION_LOST",
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

void WlanStart() {
    Serial.printf("\n-- Begin WlanStart -- \n");

    char ip_adress[20];
    sprintf(ip_adress, "IP:%d.%d.%d.%d", WiFi.localIP()[0], WiFi.localIP()[1],
            WiFi.localIP()[2], WiFi.localIP()[3]);
    Serial.println(ip_adress);
    Serial.println("");
    // IP-Adresse als Laufschrift anzeigen
    if (G.bootShowIP) {
        zeigeip(ip_adress);
    }

    Serial.printf("-- Ende  WlanStart -- \n\n");
}

//------------------------------------------------------------------------------

void WiFiEvent(WiFiEvent_t event) {
    Serial.printf("[WiFi-event] event: %d\n", event);

    switch (event) {
    case WIFI_EVENT_STAMODE_GOT_IP:
        Serial.println("WiFi connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
        break;
    case WIFI_EVENT_STAMODE_DISCONNECTED:
        Serial.println("WiFi lost connection");
        break;
    default:
        break;
    }
}

#include "Network.h"

#include "Config.h" // WIFI_VERBOSE, MANUAL_WIFI_SETTINGS, CP_* ...
#include <WiFiManager.h>

#if defined(ESP8266)
#include <ESP8266mDNS.h>
#elif defined(ESP32)
#include <ESPmDNS.h>
#include <esp_netif.h>
#endif

namespace {
// The WiFiManager instance is an implementation detail of Network and is not
// used outside this translation unit.
WiFiManager wifiManager(Serial);

Network::TimeHandler portalSetTime;
Network::SettingsRenderer portalRenderSettings;
Network::SettingsApplier portalApplySettings;

std::vector<const char *> portalMenu = {"wifi", "custom", "info",
                                        "exit", "sep",    "update"};

const char PORTAL_TIME_SCRIPT[] =
    "<script>(function(){var x=new XMLHttpRequest();"
    "x.open('GET','/clocktime?t='+Math.floor(Date.now()/1000));"
    "x.send();})();</script>";

const char PORTAL_MENU_HTML[] =
    "<form action='/clock' method='get'><button>Clock</button></form><br/>";

constexpr time_t MIN_VALID_TIME = 1700000000;

void applyPortalTime(time_t utc) {
    if (utc < MIN_VALID_TIME || !portalSetTime) {
        return;
    }
    Serial.println("Time set from captive portal");
    portalSetTime(utc);
}

bool parseLocalTime(const String &value, time_t &utc) {
    int hour = 0;
    int minute = 0;
    if (sscanf(value.c_str(), "%d:%d", &hour, &minute) != 2 || hour < 0 ||
        hour > 23 || minute < 0 || minute > 59) {
        return false;
    }
    time_t now = time(nullptr);
    struct tm tm;
    localtime_r(&now, &tm);
    if (now < MIN_VALID_TIME) {
        tm.tm_year = 2025 - 1900;
        tm.tm_mon = 0;
        tm.tm_mday = 1;
    }
    tm.tm_hour = hour;
    tm.tm_min = minute;
    tm.tm_sec = 0;
    tm.tm_isdst = -1;
    utc = mktime(&tm);
    return true;
}

void handlePortalTime() {
    auto &server = *wifiManager.server;
    applyPortalTime(strtoul(server.arg("t").c_str(), nullptr, 10));
    server.send(204);
}

void handlePortalClock() {
    auto &server = *wifiManager.server;
    String status;

    if (server.hasArg("settings") && portalApplySettings) {
        portalApplySettings([&server](const char *name, String &value) {
            if (!server.hasArg(name)) {
                return false;
            }
            value = server.arg(name);
            return true;
        });
        status = "Settings saved";
    }

    time_t manualTime = 0;
    if (server.hasArg("time") &&
        parseLocalTime(server.arg("time"), manualTime)) {
        applyPortalTime(manualTime);
        status = "Time saved";
    }

    time_t now = time(nullptr);
    struct tm tm;
    localtime_r(&now, &tm);
    char timeValue[6];
    snprintf(timeValue, sizeof(timeValue), "%02d:%02d", tm.tm_hour, tm.tm_min);

    String page = FPSTR(HTTP_HEAD_START);
    page.replace(FPSTR(T_v), "Clock");
    page += FPSTR(HTTP_STYLE);
    page += PORTAL_TIME_SCRIPT;
    String headEnd = FPSTR(HTTP_HEAD_END);
    headEnd.replace(FPSTR(T_c), "");
    page += headEnd;
    page += "<h3>Clock</h3>";
    if (status.length() > 0) {
        page += "<div class='msg S'>" + status + "</div>";
    }
    if (now >= MIN_VALID_TIME) {
        page += "<p>Current time: " + String(timeValue) + "</p>";
    }
    if (portalRenderSettings) {
        page += "<form action='/clock' method='post'>"
                "<input type='hidden' name='settings' value='1'>";
        page += portalRenderSettings();
        page += "<button type='submit'>Save settings</button></form><br/>";
    }
    page += "<form action='/clock' method='get'>"
            "<label for='time'>Time</label>"
            "<input type='time' id='time' name='time' required value='";
    page += timeValue;
    page += "'><button type='submit'>Save time</button></form><br/>"
            "<form action='/exit' method='get'>"
            "<button>Continue without WiFi</button></form>";
    page += FPSTR(HTTP_BACKBTN);
    page += FPSTR(HTTP_END);

    server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    server.send(200, "text/html", page);
}

void registerPortalRoutes() {
    wifiManager.server->on("/clocktime", handlePortalTime);
    wifiManager.server->on("/clock", handlePortalClock);
}

void configurePortal() {
    wifiManager.setMenu(portalMenu);
    wifiManager.setCustomMenuHTML(PORTAL_MENU_HTML);
    wifiManager.setCustomHeadElement(PORTAL_TIME_SCRIPT);
    wifiManager.setWebServerCallback(registerPortalRoutes);
}
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

void Network::resetSettings() {
    wifiManager.resetSettings();
    wifiManager.reboot();
}

String Network::getSSID() { return wifiManager.getWiFiSSID(); }

void Network::changeHostname(const char *hostname) {
    wifiManager.setHostname(hostname);
#if defined(ESP8266)
    WiFi.hostname(hostname);
    MDNS.setHostname(hostname);
#elif defined(ESP32)
    WiFi.setHostname(hostname);
    esp_netif_set_hostname(esp_netif_get_handle_from_ifkey("WIFI_STA_DEF"),
                           hostname);
    mdns_hostname_set(hostname);
    WiFi.reconnect();
#endif
}

void Network::setPortalHandlers(TimeHandler setTime,
                                SettingsRenderer renderSettings,
                                SettingsApplier applySettings) {
    portalSetTime = setTime;
    portalRenderSettings = renderSettings;
    portalApplySettings = applySettings;
}

void Network::setup(const char *hostname) {
    wifiManager.setHostname(hostname);
    configurePortal();
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
    configurePortal();
#if CP_PROTECTED
    wifiManager.startConfigPortal(CP_SSID, CP_PASSWORD);
#else
    wifiManager.startConfigPortal(CP_SSID);
#endif
}

void Network::loop() { wifiManager.process(); }

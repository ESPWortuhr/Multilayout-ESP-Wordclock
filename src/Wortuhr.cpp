/*
 * Standard Wortuhr Konfiguration. Kann später in den Einstellungen auf der
 * Webseite geändert werden.
 */

// Layout der Frontplatte:
// - Uhr_114
//   10 Reihen, jeweils 11 LED's pro Reihe + 4 LED's für Minuten
// - Uhr_114_Alternative
//   10 Reihen, jeweils 11 LED's pro Reihe + 4 LED's für Minuten, mit geändertem
//   Layout für extra Wörter in der Matrix
// - Uhr_114_2Clock
//   10 Reihen, jeweils 11 LED's pro Reihe + 4 LED's für Minuten, mit dem Layout
//   vom orginal Hersteller
// - Uhr_114_Dutch
//   10 Reihen, jeweils 11 LED's pro Reihe + 4 LED's für Minuten, mit geändertem
//   Layout für die niederländische Sprache
// - Uhr_125
//   11 Reihen, jeweils 11 LED's pro Reihe + 4 LED's für Minuten
// - Uhr_169
//   mit zusätzlichen LED's um den Rahmen seitlich zu beleuchten
// - Uhr_242
//   Uhr mit Wettervorhersage 242 LED's
// - Uhr_291
//   Uhr mit 24 Stunden Anzeige 18x16
#define DEFAULT_LAYOUT Uhr_114

// Typ der LEDs:
// - Brg, Grb, Rgb, Rbg (WS2812b)
// - Grbw (SK6812)
#define DEFAULT_LEDTYPE Brg

// External Realtime Clock: RTC_DS1307, RTC_PCF8523 oder RTC_DS3231
#define RTC_Type RTC_DS3231

// um das eeprom zu löschen, bzw. zu initialisieren, hier eine andere
// Seriennummer eintragen!
#define SERNR 62

bool DEBUG = true; // DEBUG ON|OFF wenn auskommentiert
//#define VERBOSE          // DEBUG VERBOSE Openweathermap

#define MANUAL_WIFI_SETTINGS false
#define WIFI_SSID "Wifi-SSID"
#define WIFI_PASSWORD "Wifi-Pwd"

/*--------------------------------------------------------------------------
 * ENDE Hardware Konfiguration. Ab hier nichts mehr aendern!!!
 *--------------------------------------------------------------------------
 */

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <Hash.h>
#include <NeoPixelBus.h>
#include <RTClib.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>
#include <Wire.h>
#include <coredecls.h>
#include <sntp.h>

#include "Uhr.h"

#include "EEPROMAnything.h"
#include "webPageAdapter.h"

#include "Uhrtypes/uhr_func_114.hpp"
#include "Uhrtypes/uhr_func_114_2Clock.hpp"
#include "Uhrtypes/uhr_func_114_Alternative.hpp"
#include "Uhrtypes/uhr_func_114_dutch.hpp"
#include "Uhrtypes/uhr_func_125.hpp"
#include "Uhrtypes/uhr_func_125_Type2.hpp"
#include "Uhrtypes/uhr_func_169.hpp"
#include "Uhrtypes/uhr_func_242.hpp"
#include "Uhrtypes/uhr_func_291.hpp"

UHR_114_t Uhr_114_type;
UHR_114_Alternative_t Uhr_114_Alternative_type;
UHR_114_2Clock_t Uhr_114_2Clock_type;
UHR_114_dutch_t Uhr_114_dutch_type;
UHR_125_t Uhr_125_type;
UHR_125_Type2_t Uhr_125_type2_type;
UHR_169_t Uhr_169_type;
UHR_242_t Uhr_242_type;
UHR_291_t Uhr_291_type;

iUhrType *usedUhrType = nullptr;

#include "NeoMultiFeature.hpp"
NeoPixelBus<NeoMultiFeature, Neo800KbpsMethod> *strip_RGB = NULL;
NeoPixelBus<NeoGrbwFeature, Neo800KbpsMethod> *strip_RGBW = NULL;

WiFiClient client;

//--OTA--
ESP8266WebServer httpServer(81);
ESP8266HTTPUpdateServer httpUpdater;
//--OTA--

// Timezone from
// https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv
const char TZ_Europe_Berlin[] = "CET-1CEST,M3.5.0,M10.5.0/3";

RTC_Type RTC;

#include "Animation.h"
#include "clockWork.h"
#include "led.h"
#include "mqtt.h"
#include "network.h"

Animation *animation;
Led led;
ClockWork clockWork;
Mqtt mqtt;
Network network;

#include "Animation.hpp"
#include "Wifi.hpp"
#include "clockWork.hpp"
#include "icons.h"
#include "led.hpp"
#include "mqtt.hpp"
#include "network.hpp"

#define EEPROM_SIZE 512
_Static_assert(sizeof(G) <= EEPROM_SIZE,
               "Datenstruktur G zu gross für reservierten EEPROM Bereich");

//------------------------------------------------------------------------------

uint32_t sntp_startup_delay_MS_rfc_not_less_than_60000() {
    if (externalRTC) {
        Serial.println("SNTP startup delay 1min");
        return 60000;
    } else {
        // yes this is against the RFC, but we don't have an RTC and want the
        // time now.
        Serial.println("no RTC clock - disable SNTP startup delay");
        return 500;
    }
}

//------------------------------------------------------------------------------

void time_is_set() {
    time_t utc = time(nullptr);
    if (externalRTC) {
        RTC.adjust(DateTime(utc));
    }

    struct tm tm;
    localtime_r(&utc, &tm);
    _second = tm.tm_sec;
    _minute = tm.tm_min;
    _hour = tm.tm_hour;
    if (usedUhrType->hasSecondsFrame()) {
        _second48 = _second * 48 / 60;
    }

    String origin;
    if (sntp_getreachability(0)) {
        origin = sntp_getservername(0);
        if (origin.length() == 0) {
            origin = IPAddress(sntp_getserver(0)).toString();
        }
    } else {
        origin = "SNTP not reachable";
    }
    Serial.printf("Set new time: %02d:%02d:%02d (%s)\n", _hour, _minute,
                  _second, origin.c_str());
}

//------------------------------------------------------------------------------
// Start setup()
//------------------------------------------------------------------------------

void setup() {
    //-------------------------------------
    // Start Serielle Schnittstelle bei Bedarf
    //-------------------------------------
    if (DEBUG == true) {
        Serial.begin(115200);
        Serial.println("");
        Serial.println("--------------------------------------");
        Serial.println("Begin Setup");
        Serial.println("--------------------------------------");
    }
    //-------------------------------------
    // EEPROM lesen / initialisieren
    //-------------------------------------
    EEPROM.begin(EEPROM_SIZE);

    eeprom::read();

    if (G.sernr != SERNR) {
        for (uint16_t i = 0; i < EEPROM_SIZE; i++) {
            EEPROM.write(i, i);
        }
        EEPROM.commit();

        G.sernr = SERNR;
        G.prog = 1;
        G.param1 = 0;
        G.prog_init = 1;
        G.conf = COMMAND_IDLE;
        for (uint8_t i = 0; i < 4; i++) {
            for (uint8_t ii = 0; ii < 4; ii++) {
                G.rgbw[i][ii] = 0;
            }
        }
        G.rgbw[Foreground][2] = 100;
        G.rgbw[Effect][1] = 100;
        G.rr = 0;
        G.gg = 0;
        G.bb = 0;
        G.ww = 0;
        G.hell = 2;
        G.geschw = 10;
        G.client_nr = 0;
        G.zeige_sek = 0;
        G.zeige_min = 1;
        G.ldr = 0;
        G.ldrCal = 0;
        strcpy(G.openWeatherMap.cityid, "");
        strcpy(G.openWeatherMap.apikey, "");
        strcpy(G.zeitserver, "europe.pool.ntp.org");
        strcpy(G.hostname, "uhr");
        strcpy(G.ltext, "HELLO WORLD ");

        G.hh = 100;
        G.h6 = 100;
        G.h8 = 100;
        G.h12 = 100;
        G.h16 = 100;
        G.h18 = 100;
        G.h20 = 100;
        G.h22 = 100;
        G.h24 = 100;
        for (uint8_t i = 0; i < 5; i++) {
            G.Sprachvariation[i] = false;
        }

        G.mqtt.state = 0;
        G.mqtt.port = 1883;
        strcpy(G.mqtt.serverAdress, "192.168.4.1");
        strcpy(G.mqtt.user, "User");
        strcpy(G.mqtt.password, "Passwort");
        strcpy(G.mqtt.clientId, "ClientId");
        strcpy(G.mqtt.topic, "Wortuhr");

        G.UhrtypeDef = DEFAULT_LAYOUT;
        G.Colortype = DEFAULT_LEDTYPE;
        G.bootLedBlink = false;
        G.bootLedSweep = false;
        G.bootShowWifi = true;
        G.bootShowIP = false;

        G.autoLdrEnabled = 0;
        G.autoLdrBright = 100;
        G.autoLdrDark = 10;
        G.animType = 0; // Animation::KEINE;
        G.animDuration = 2;
        G.animSpeed = 30;
        G.animColorize = 1;
        G.animDemo = false;

        eeprom::write();
        Serial.println("eeprom schreiben");
    }
    // Initialisierung der COMMAND_MODE_xxx (Farbe)
    G.prog_init = 1;

    //-------------------------------------
    // Get Pointer for Uhrtype
    //-------------------------------------

    usedUhrType = clockWork.getPointer(G.UhrtypeDef);

    // Bereich der animiert wird:
    //     LED-Rahmen horizontal
    //     LED-Rahmen vertical
    //     Anzahl Reihen (einschliesslich Rahmen)
    //     Anzahl Spalten (einschliesslich Rahmen)
    // TODO Rahmenbreite aus usedUhrTyp holen
    animation = new Animation(0, 0, usedUhrType->ROWS_MATRIX() - 1,
                              usedUhrType->COLS_MATRIX());

    //-------------------------------------
    // LEDs initialisieren
    //-------------------------------------
    Serial.println("LED Init");
    clockWork.initLedStrip(G.Colortype);
    if (G.bootLedBlink) {
        clockWork.initBootLedBlink();
        delay(20);
    }
    if (G.bootLedSweep) {
        clockWork.initBootLedSweep(20);
    }
    clockWork.initBootLed();

    G.conf = COMMAND_IDLE;

    //-------------------------------------
    // Start External RealtimeClock
    //-------------------------------------

    if (RTC.begin() == true) {
        Serial.println("External RealtimeClock found");
        struct timeval tv;
        tv.tv_sec = RTC.now().unixtime();
        settimeofday(&tv, nullptr);
        network.rtcMode();
        externalRTC = true;
    } else {
        Serial.println("No external RealtimeClock found");
        externalRTC = false;
    }
    settimeofday_cb(time_is_set);

    //-------------------------------------
    // Init Array
    //-------------------------------------

    for (uint16_t i = 0; i < usedUhrType->NUM_PIXELS(); i++) {
        lastFrontMatrix[i] = 500;
    }

    //-------------------------------------
    // Start WiFi
    //-------------------------------------
    if (G.bootShowWifi) {
        clockWork.initBootWifiSignalStrength(0);
    }
    network.setup(G.hostname);
    int strength = network.getQuality();
    Serial.printf("Signal strength: %i\n", strength);
    if (G.bootShowWifi) {
        clockWork.initBootWifiSignalStrength(strength);
    }
    wifiStart();
    configTime(0, 0, G.zeitserver);
    setenv("TZ", TZ_Europe_Berlin, true);
    tzset();

    delay(50);
    //-------------------------------------
    // OTA--
    //-------------------------------------
    httpUpdater.setup(&httpServer);
    httpServer.onNotFound([]() {
        // redirect port 81 not found pages to port 80
        httpServer.sendHeader(
            "Location", "http://" + WiFi.localIP().toString() + "/", true);
        httpServer.send(308, "text/plain", "");
    });
    httpServer.begin();

    //-------------------------------------
    // MQTT
    //-------------------------------------

    if (G.mqtt.state == 1) {
        mqtt.init();
    }

    //-------------------------------------
    // Start Websocket
    //-------------------------------------
    webSocket.begin();
    webSocket.onEvent(webSocketEvent);

    Serial.println("Websocket started");

    //-------------------------------------
    // mDNS
    //-------------------------------------
    MDNS.begin(G.hostname);
    MDNS.addService("http", "tcp", 80);
    MDNS.addService("http", "tcp", 81);

    /*
    // setup frame
    if (usedUhrType->hasSecondsFrame() && G.zeige_sek < 1 && G.zeige_min < 2) {
        led.setFrameColor();
    }
    */

    //-------------------------------------
    Serial.println("--------------------------------------");
    Serial.println("ESP Uhr");
    Serial.printf("Version         : %s\n", VERSION);
    Serial.printf("Chip ID         : %08X\n", ESP.getChipId());
    Serial.printf("Flash ID        : %08X\n\n", ESP.getFlashChipId());
    Serial.printf("CPU Speed       : %u MHz \n\n", ESP.getCpuFreqMHz());

    Serial.printf("Flash real Size : %u KByte\n",
                  ESP.getFlashChipRealSize() / 1024);
    Serial.printf("Flash ide  Size : %u KByte\n",
                  ESP.getFlashChipSize() / 1024);
    Serial.printf("Flash ide Speed : %u\n\n", ESP.getFlashChipSpeed());

    Serial.printf("Free Heap Size  : %u Byte\n", ESP.getFreeHeap());
    Serial.printf("Sketch Size     : %u Byte \n", ESP.getSketchSize());
    Serial.printf("Free Sketch Size: %u Byte \n\n", ESP.getFreeSketchSpace());

    Serial.printf("SDK Version     : %s\n", ESP.getSdkVersion());
    Serial.print("RESET Info      : ");
    Serial.println(ESP.getResetInfo());
    Serial.print("COMPILED        : ");
    Serial.print(__DATE__);
    Serial.print(" ");
    Serial.println(__TIME__);

    Serial.println("--------------------------------------");
    Serial.println("Ende Setup");
    Serial.println("--------------------------------------");
    Serial.println("");
}

//------------------------------------------------------------------------------
// Start loop()
//------------------------------------------------------------------------------

void loop() {
    time_t utc = time(nullptr);
    struct tm tm;
    localtime_r(&utc, &tm);
    if (utc > 100000000) {
        _second = tm.tm_sec;
        _minute = tm.tm_min;
        _hour = tm.tm_hour;
    }

    network.loop();

    MDNS.update();

    httpServer.handleClient();

    webSocket.loop();

    //------------------------------------------------
    // MQTT
    //------------------------------------------------
    if (G.mqtt.state == 1 && WiFi.status() == WL_CONNECTED) {
        mqtt.loop();
    }

    animation->loop(tm); // muss periodisch aufgerufen werden

    // lass die Zeit im Demo Mode der Animation schneller ablaufen
    animation->demoMode(_minute, _second);

    clockWork.loop(tm);
}

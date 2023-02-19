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
#include "config.h"
#include "uhrtype.gen.h"
#include "webPageAdapter.h"

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
#include "frame.h"
#include "led.h"
#include "mqtt.h"
#include "network.h"

Animation *animation;
SecondsFrame *secondsFrame;
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
    if (usedUhrType->numPixelsFrameMatrix() != 0) {
        _secondFrame = _second / (usedUhrType->numPixelsFrameMatrix() / 60.f);
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
    // Start serial interface if required
    //-------------------------------------
#if GENERAL_VERBOSE
    Serial.begin(115200);
    Serial.println("");
    Serial.println("--------------------------------------");
    Serial.println("Begin Setup");
    Serial.println("--------------------------------------");
#endif
    //-------------------------------------
    // Read / initialize EEPROM
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
        G.progInit = 1;
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
        G.effectBri = 2;
        G.effectSpeed = 10;
        G.client_nr = 0;
        G.secondVariant = SecondVariant::Off;
        G.minuteVariant = MinuteVariant::Corners;
        G.ldr = 0;
        G.ldrCal = 0;
        strcpy(G.openWeatherMap.cityid, "");
        strcpy(G.openWeatherMap.apikey, "");
        strcpy(G.timeserver, "europe.pool.ntp.org");
        strcpy(G.hostname, "ESPWordclock");
        strcpy(G.scrollingText, "HELLO WORLD ");

        G.hh = 100;
        G.h6 = 100;
        G.h8 = 100;
        G.h12 = 100;
        G.h16 = 100;
        G.h18 = 100;
        G.h20 = 100;
        G.h22 = 100;
        G.h24 = 100;
        for (uint8_t i = 0;
             i < sizeof(G.layoutVariant) / sizeof(G.layoutVariant[0]); i++) {
            G.layoutVariant[i] = false;
        }
        for (uint8_t i = 0;
             i < sizeof(G.languageVariant) / sizeof(G.languageVariant[0]);
             i++) {
            G.languageVariant[i] = false;
        }
        G.mqtt.state = 0;
        G.mqtt.port = 1883;
        strcpy(G.mqtt.serverAdress, "192.168.4.1");
        strcpy(G.mqtt.user, "User");
        strcpy(G.mqtt.password, "Password");
        strcpy(G.mqtt.clientId, "ClientId");
        strcpy(G.mqtt.topic, "ESPWordclock");

        G.UhrtypeDef = DEFAULT_LAYOUT;
        G.Colortype = DEFAULT_LEDTYPE;
        G.bootLedBlink = false;
        G.bootLedSweep = BOOT_LEDSWEEP;
        G.bootShowWifi = true;
        G.bootShowIP = BOOT_SHOWIP;

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

    // Initialization of COMMAND_MODE_xxx (color)
    G.progInit = 1;

    //-------------------------------------
    // Get Pointer for Uhrtype
    //-------------------------------------

    usedUhrType = clockWork.getPointer(G.UhrtypeDef);

    // Area that will be animated:
    //         LED frame horizontal
    //         LED frame vertical
    //         Number of rows (including frames)
    //         Number of columns (including frames)
    // Get TODO frame width from usedUhrTyp
    animation = new Animation(0, 0, usedUhrType->ROWS_MATRIX() - 1,
                              usedUhrType->COLS_MATRIX());

    if (usedUhrType->numPixelsFrameMatrix() != 0) {
        secondsFrame = new SecondsFrame(usedUhrType->numPixelsFrameMatrix());
    } else {
        secondsFrame = nullptr;
    }

    //-------------------------------------
    // Initialize LEDs
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
    // Start external real-time clock
    //-------------------------------------

    if (RTC.begin() == true) {
        Serial.println("External real-time clock found");
        struct timeval tv;
        tv.tv_sec = RTC.now().unixtime();
        settimeofday(&tv, nullptr);
        network.rtcMode();
        externalRTC = true;
    } else {
        Serial.println("No external real-time clock found");
        externalRTC = false;
    }
    settimeofday_cb(time_is_set);

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
    configTime(0, 0, G.timeserver);
    setenv("TZ", TZ_Europe_Berlin, true);
    tzset();

    delay(50);

    //-------------------------------------
    // OTA
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

    animation->loop(tm); // must be called periodically

    // make the time run faster in the demo mode of the animation
    animation->demoMode(_minute, _second);

    if (usedUhrType->numPixelsFrameMatrix() != 0) {
        secondsFrame->loop();
    }

    clockWork.loop(tm);
}

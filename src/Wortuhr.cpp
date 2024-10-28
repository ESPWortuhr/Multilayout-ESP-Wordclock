#include <Arduino.h>
#include <ArduinoJson.h>
#ifdef ESP8266
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <coredecls.h>
#include <sntp.h>
#elif defined(ESP32)
#include <ESPmDNS.h>
#include <HTTPUpdateServer.h>
#include <Update.h>
#include <WebServer.h>
#include <WiFi.h>
#include <esp_sntp.h>
#endif
#include <NeoPixelBus.h>
#include <RTClib.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>
#include <Wire.h>

#include "Uhr.h"

#include "EEPROMAnything.h"
#include "config.h"
#include "uhrtype.gen.h"
#include "webPageAdapter.h"

iUhrType *usedUhrType = nullptr;

#include "NeoMultiFeature.hpp"

#ifdef ESP8266
NeoPixelBus<NeoMultiFeature, Neo800KbpsMethod> *strip_RGB = NULL;
NeoPixelBus<NeoGrbwFeature, Neo800KbpsMethod> *strip_RGBW = NULL;
#elif defined(ESP32)
NeoPixelBus<NeoGrbwFeature, NeoSk6812Method> *strip_RGBW = NULL;
NeoPixelBus<NeoMultiFeature, NeoWs2812xMethod> *strip_RGB = NULL;
#endif

WiFiClient client;

//--OTA--
#ifdef ESP8266
ESP8266WebServer httpServer(81);
ESP8266HTTPUpdateServer httpUpdater;
#elif defined(ESP32)
WebServer httpServer(81);
HTTPUpdateServer httpUpdater;
#endif
//--OTA--

// Timezone from
// https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv
const char TZ_Europe_Berlin[] = "CET-1CEST,M3.5.0,M10.5.0/3";

RTC_Type RTC;

#include "Transitiontypes/Transition.h"
#include "clockWork.h"
#include "frame.h"
#include "led.h"
#include "mqtt.h"
#include "network.h"

Transition *transition;
SecondsFrame *secondsFrame;
Led led;
ClockWork clockWork;
Mqtt mqtt;
Network network;

#include "Transitiontypes/Transition.hpp"
#include "Wifi.hpp"
#include "clockWork.hpp"
#include "icons.h"
#include "led.hpp"
#include "mqtt.hpp"
#include "network.hpp"

#define EEPROM_SIZE 512
_Static_assert(sizeof(G) <= EEPROM_SIZE,
               "Datenstruktur G zu gross für reservierten EEPROM Bereich");

uint16_t powerCycleCountAddr =
    EEPROM_SIZE - 1;          // Address in EEPROM to store power cycle count
uint16_t powerCycleCount = 0; // Variable to store power cycle count

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
            const ip_addr_t *ip_addr = sntp_getserver(0);
#ifdef ESP8266
            origin = IPAddress(ip_addr->addr).toString();
#elif defined(ESP32)
            origin = IPAddress(ip_addr->u_addr.ip4.addr).toString();
#endif
        }
    } else {
        origin = "SNTP not reachable";
    }
    Serial.printf("Set new time: %02d:%02d:%02d (%s)\n", _hour, _minute,
                  _second, origin.c_str());

    G.progInit = true;
    parametersChanged = true;
}

//------------------------------------------------------------------------------

void incrementPowerCycleCount() {
    if (powerCycleCount > 5) {
        powerCycleCount = 0;
    }
    powerCycleCount++;
    EEPROM.write(powerCycleCountAddr, powerCycleCount);
    EEPROM.commit();
}

//------------------------------------------------------------------------------
// Start setup()
//------------------------------------------------------------------------------

void setup() {
    //-------------------------------------
    // Start serial interface if required
    //-------------------------------------
#if GENERAL_VERBOSE
    Serial.begin(460800);
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

    //-------------------------------------

    // Read the power cycle count from EEPROM
    powerCycleCount = EEPROM.read(powerCycleCountAddr);
    incrementPowerCycleCount();
    Serial.print("Power cycle count: ");
    Serial.println(powerCycleCount);
    if (powerCycleCount == 5) {
        G.sernr++;
        Serial.println("Reset to initial values");
    }

    //-------------------------------------

    if (G.sernr != SERNR) {
        for (uint16_t i = 0; i < EEPROM_SIZE; i++) {
            EEPROM.write(i, i);
        }
        EEPROM.commit();

        G.sernr = SERNR;
        G.prog = 1;
        G.param1 = 0;
        G.progInit = true;
        G.conf = COMMAND_IDLE;
        for (uint8_t i = 0; i < 3; i++) {
            G.color[i] = {0, 0, 0};
        }
        G.color[Foreground] = HsbColor(120 / 360.f, 1.f, 0.5f);
        G.effectBri = 2;
        G.effectSpeed = 10;
        G.client_nr = 0;
        G.secondVariant = SecondVariant::Off;
// C++23 #elifdef doesn't work yet
#ifdef MINUTE_Off
        G.minuteVariant = MinuteVariant::Off;
#endif
#ifdef MINUTE_LED4x
        G.minuteVariant = MinuteVariant::LED4x;
#endif
#ifdef MINUTE_LED7x
        G.minuteVariant = MinuteVariant::LED7x;
#endif
        strcpy(G.openWeatherMap.cityid, "");
        strcpy(G.openWeatherMap.apikey, "");
        strcpy(G.timeserver, "europe.pool.ntp.org");
        strcpy(G.hostname, "ESPWordclock");
        strcpy(G.scrollingText, "HELLO WORLD ");

        G.h6 = 100;
        G.h8 = 100;
        G.h12 = 100;
        G.h16 = 100;
        G.h18 = 100;
        G.h20 = 100;
        G.h22 = 100;
        G.h24 = 100;
        G.layoutVariant[ReverseMinDirection] = REVERSE_MINUTE_DIR;
        G.layoutVariant[MirrorVertical] = MIRROR_FRONT_VERTICAL;
        G.layoutVariant[MirrorHorizontal] = MIRROR_FRONT_HORIZONTAL;
        for (uint8_t i = 0;
             i < sizeof(G.languageVariant) / sizeof(G.languageVariant[0]);
             i++) {
            G.languageVariant[i] = false;
        }
        G.mqtt.state = false;
        G.mqtt.port = 1883;
        strcpy(G.mqtt.serverAdress, "192.168.4.1");
        strcpy(G.mqtt.user, "User");
        strcpy(G.mqtt.password, "Password");
        strcpy(G.mqtt.clientId, "ClientId");
        strcpy(G.mqtt.topic, "ESPWordclock");

        G.UhrtypeDef = DEFAULT_LAYOUT;
        G.buildTypeDef = DEFAULT_BUILDTYPE;
        G.Colortype = DEFAULT_LEDTYPE;
        G.wType = WHITE_LEDTYPE;

        G.bootLedBlink = false;
        G.bootLedSweep = BOOT_LEDSWEEP;
        G.bootShowWifi = true;
        G.bootShowIP = BOOT_SHOWIP;

        G.autoBrightEnabled = 0;
        G.autoBrightOffset = 100;
        G.autoBrightSlope = 10;
        G.transitionType = 0; // Transition::NO_TRANSITION;
        G.transitionDuration = 2;
        G.transitionSpeed = 30;
        G.transitionColorize = 0;
        G.transitionDemo = false;

        for (uint8_t i = 0; i < MAX_BIRTHDAY_COUNT; i++) {
            G.birthday[i].day = 1;
            G.birthday[i].month = 1;
            G.birthday[i].year = 1900;
        }

        eeprom::write();
        Serial.println("eeprom schreiben");
    }

    // Initialization of COMMAND_MODE_xxx (color)
    G.progInit = true;

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
    transition = new Transition(usedUhrType->rowsWordMatrix(),
                                usedUhrType->colsWordMatrix());

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
#ifdef ESP8266
    settimeofday_cb(time_is_set);
#endif

    //-------------------------------------
    // Start WiFi
    //-------------------------------------

    if (G.bootShowWifi) {
        led.setIcon(WLAN100);
    }
    network.setup(G.hostname);
    int strength = network.getQuality();
    Serial.printf("Signal strength: %i\n", strength);
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

    if (G.mqtt.state) {
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

#ifdef ESP8266
    Serial.printf("Chip ID         : %08X\n", ESP.getChipId());
    Serial.printf("CPU Speed       : %u MHz \n\n", ESP.getCpuFreqMHz());
#elif defined(ESP32)
    Serial.printf("Chip ID         : %08X\n", (uint32_t)ESP.getEfuseMac());
    Serial.printf("CPU Speed       : %u MHz \n\n", getCpuFrequencyMhz());
#endif

    Serial.printf("Flash Size : %u KByte\n", ESP.getFlashChipSize() / 1024);
    Serial.printf("Flash ide Speed : %u\n\n", ESP.getFlashChipSpeed());

    Serial.printf("Free Heap Size  : %u Byte\n", ESP.getFreeHeap());
    Serial.printf("Sketch Size     : %u Byte \n", ESP.getSketchSize());
    Serial.printf("Free Sketch Size: %u Byte \n\n", ESP.getFreeSketchSpace());

    Serial.printf("SDK Version     : %s\n", ESP.getSdkVersion());
    Serial.print("COMPILED        : ");
    Serial.print(__DATE__);
    Serial.print(" ");
    Serial.println(__TIME__);

    Serial.println("--------------------------------------");
    Serial.println("Ende Setup");
    Serial.println("--------------------------------------");
    Serial.println("");

    //-------------------------------------
    // Reset Powercycle Counter
    //-------------------------------------
    delay(500);
    powerCycleCount = 0;
    EEPROM.write(powerCycleCountAddr, powerCycleCount);
    EEPROM.commit();

    //-------------------------------------
    // Transition Init
    //-------------------------------------
    transition->init();

    //-------------------------------------
    // Setup Done
    //-------------------------------------
    parametersChanged = true;
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

#ifdef ESP8266
    MDNS.update();
#endif

    httpServer.handleClient();

    webSocket.loop();

    //------------------------------------------------
    // MQTT
    //------------------------------------------------
    if (G.mqtt.state && WiFi.status() == WL_CONNECTED) {
        mqtt.loop();
    }

    transition->loop(tm); // must be called periodically

    // make the time run faster in the demo mode of the transition
    transition->demoMode(_minute, _second);

    if (usedUhrType->numPixelsFrameMatrix() != 0) {
        secondsFrame->loop();
    }

    clockWork.loop(tm);
}

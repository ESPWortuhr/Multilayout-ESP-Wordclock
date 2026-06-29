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

#include "Config.h"

#include "WordClock.h"
#include <NeoPixelBus.h>
#include <RTClib.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>

#include "WordClockState.h"

#include "ClockType.gen.h"
#include "WebPageAdapter.h"

#include "EEPROMAnything.h"

ClockType *usedClockType = nullptr;

#include "LedStripInterface.h"
#include "NeoPixelBusLedStripAdapter.h"

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

#include "ClockWork.h"
#include "Frame.h"
#include "I2CBus.h"
#include "Led.h"
#include "Mqtt.h"
#include "Network.h"
#include "TransitionTypes/Transition.h"

Transition *transition;
SecondsFrame *secondsFrame;
Led led;
ClockWork clockWork;
Mqtt mqtt(clockWork);
Network network;

void setDefaultHardwarePins();
bool hardwarePinsAreValid();
void ensureI2CPins();
void ensureTimezone();

LedStripInterface *activeLedStrip = nullptr;

void deleteActiveLedStrip() {
    delete activeLedStrip;
    activeLedStrip = nullptr;
}

#include "ClockWork.hpp"
#include "I2CBus.hpp"
#include "Led.hpp"
#include "Mqtt.hpp"
#include "Network.hpp"
#include "Symbols.h"
#include "TransitionTypes/Transition.hpp"
#include "WiFi.hpp"

namespace {
constexpr uint16_t EEPROM_SIZE = 512;
constexpr uint16_t POWER_CYCLE_COUNT_ADDRESS = EEPROM_SIZE - 1;
constexpr uint8_t POWER_CYCLE_RESET_LIMIT = 5;
constexpr uint8_t CAPTIVE_PORTAL_POWER_CYCLE_COUNT = 3;
constexpr uint8_t FACTORY_RESET_POWER_CYCLE_COUNT = 6;

#ifdef ESP8266
constexpr uint8_t MAX_HARDWARE_PIN = 16;
#else
constexpr uint8_t MAX_HARDWARE_PIN = 39;
#endif
} // namespace

static_assert(sizeof(G) < EEPROM_SIZE,
              "Configuration data is too large for reserved EEPROM storage");

uint16_t powerCycleCount = 0; // Variable to store power cycle count

//------------------------------------------------------------------------------

#if defined(ESP32)
void time_is_set(struct timeval *tv) {
    const time_t utc = tv != nullptr ? tv->tv_sec : time(nullptr);
#else
void time_is_set() {
    const time_t utc = time(nullptr);
#endif
    if (externalRTC) {
        RTC.adjust(DateTime(utc));
    }

    struct tm tm;
    localtime_r(&utc, &tm);

    String origin = sntp_getservername(0);
    if (origin.isEmpty()) {
        const ip_addr_t *ip_addr = sntp_getserver(0);

        if (ip_addr != nullptr) {
#ifdef ESP8266
            origin = IPAddress(ip_addr->addr).toString();
#elif defined(ESP32)
            origin = IPAddress(ip_addr->u_addr.ip4.addr).toString();
#endif
        } else {
            origin = "SNTP";
        }
    }
    Serial.printf("Set new time: %02d:%02d:%02d (%s)\n", tm.tm_hour, tm.tm_min,
                  tm.tm_sec, origin.c_str());

    // Calc second frame for seconds variants that use a frame
    uint16_t numPixels = usedClockType->numPixelsFrameMatrix();
    if (numPixels != 0) {
        _secondFrame = (tm.tm_sec * numPixels) / 60;
    }

    G.progInit = true;
    parametersChanged = true;
}

//------------------------------------------------------------------------------

void incrementPowerCycleCount() {
    if (powerCycleCount > POWER_CYCLE_RESET_LIMIT) {
        powerCycleCount = 0;
    }
    powerCycleCount++;
    EEPROM.write(POWER_CYCLE_COUNT_ADDRESS, powerCycleCount);
    EEPROM.commit();
}

//------------------------------------------------------------------------------

void sendMQTTUpdate() {
    // send status update via MQTT
    if ((G.mqtt.state) && (WiFi.status() == WL_CONNECTED)) {
        mqtt.sendState();
    }
}

//------------------------------------------------------------------------------

void setDefaultHardwarePins() {
    G.hardwarePins.led = LED_PIN;
    G.hardwarePins.powerButton = POWER_BUTTON_PIN;
    G.hardwarePins.modeButton = MODE_BUTTON_PIN;
    G.hardwarePins.speedButton = SPEED_BUTTON_PIN;
}

//------------------------------------------------------------------------------

bool isHardwarePinInRange(uint8_t pin) { return pin <= MAX_HARDWARE_PIN; }

//------------------------------------------------------------------------------

bool allHardwarePinsAreInRange() {
    if (!isHardwarePinInRange(G.hardwarePins.led)) {
        return false;
    }

    const uint8_t buttonPins[] = {
        G.hardwarePins.powerButton,
        G.hardwarePins.modeButton,
        G.hardwarePins.speedButton,
    };
    const uint8_t pinCount = sizeof(buttonPins) / sizeof(buttonPins[0]);

    for (uint8_t i = 0; i < pinCount; i++) {
        if (buttonPins[i] != HARDWARE_PIN_DISABLED &&
            !isHardwarePinInRange(buttonPins[i])) {
            return false;
        }
    }
    return i2cBus::pinsAreValid(G.i2cSdaPin, G.i2cSclPin);
}

//------------------------------------------------------------------------------

bool hasDuplicateHardwarePins() {
    const uint8_t pins[] = {
        G.hardwarePins.led,
        G.hardwarePins.powerButton,
        G.hardwarePins.modeButton,
        G.hardwarePins.speedButton,
        G.i2cSdaPin,
        G.i2cSclPin,
    };
    const uint8_t pinCount = sizeof(pins) / sizeof(pins[0]);

    for (uint8_t i = 0; i < pinCount; i++) {
        for (uint8_t j = i + 1; j < pinCount; j++) {
            if (pins[i] != HARDWARE_PIN_DISABLED && pins[i] == pins[j]) {
                return true;
            }
        }
    }
    return false;
}

//------------------------------------------------------------------------------

bool hardwarePinsAreValid() {
    return allHardwarePinsAreInRange() && !hasDuplicateHardwarePins();
}

//------------------------------------------------------------------------------

bool timezoneIsValid() {
    for (uint8_t i = 0; i < sizeof(G.timezone); i++) {
        const char c = G.timezone[i];
        if (c == '\0') {
            return i > 0;
        }
        if (c < 33 || c > 126) {
            return false;
        }
    }
    return false;
}

//------------------------------------------------------------------------------

void ensureTimezone() {
    if (!timezoneIsValid()) {
        strlcpy(G.timezone, TZ_Europe_Berlin, sizeof(G.timezone));
    }
}

//------------------------------------------------------------------------------

void applyTimezone() {
    ensureTimezone();
    setenv("TZ", G.timezone, true);
    tzset();
}

//------------------------------------------------------------------------------

void ensureHardwarePins() {
    if (hardwarePinsAreValid()) {
        return;
    }

    Serial.println("Invalid hardware pins in EEPROM, restoring defaults");
    setDefaultHardwarePins();
    eeprom::write();
}

//------------------------------------------------------------------------------

void ensureI2CPins() {
    if (i2cBus::pinsAreValid(G.i2cSdaPin, G.i2cSclPin)) {
        return;
    }

    Serial.println("Invalid I2C pins in EEPROM, restoring defaults");
    i2cBus::setDefaultPins(G.i2cSdaPin, G.i2cSclPin);
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
    ensureHardwarePins();
    ensureI2CPins();
    ensureTimezone();

    //-------------------------------------

    // Read the power cycle count from EEPROM
    powerCycleCount = EEPROM.read(POWER_CYCLE_COUNT_ADDRESS);
    incrementPowerCycleCount();
    Serial.print("Power cycle count: ");
    Serial.println(powerCycleCount);
    if (powerCycleCount == CAPTIVE_PORTAL_POWER_CYCLE_COUNT) {
        Serial.println("Enable captive portal");
#if CP_PROTECTED
        wifiManager.startConfigPortal(CP_SSID, CP_PASSWORD);
#else
        wifiManager.startConfigPortal(CP_SSID);
#endif
    } else if (powerCycleCount == FACTORY_RESET_POWER_CYCLE_COUNT) {
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
        G.prog = COMMAND_MODE_WORD_CLOCK;
        G.param1 = 0;
        G.progInit = true;
        G.conf = COMMAND_IDLE;
        for (uint8_t i = 0; i < 3; i++) {
            G.color[i] = {0, 0, 0};
        }
        G.color[Foreground] = HsbColor(DEFAULT_HUE / 360.f, 1.f, 0.5f);
        G.effectBri = 2;
        G.effectSpeed = 5;
        G.client_nr = 0;
        G.secondVariant = SecondVariant::Off;
        G.bitmapSymbol = BitmapSymbol::HEART;
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
        G.itIsVariant = ItIsVariant::Permanent;
        strcpy(G.openWeatherMap.cityid, "");
        strcpy(G.openWeatherMap.apikey, "");
        strcpy(G.timeserver, "europe.pool.ntp.org");
        strlcpy(G.timezone, TZ_Europe_Berlin, sizeof(G.timezone));
        strcpy(G.hostname, "ESPWordclock");
        strcpy(G.scrollingText, "HELLO WORLD ");

        G.h6 = DEFAULT_BRIGHTNESS;
        G.h8 = DEFAULT_BRIGHTNESS;
        G.h12 = DEFAULT_BRIGHTNESS;
        G.h16 = DEFAULT_BRIGHTNESS;
        G.h18 = DEFAULT_BRIGHTNESS;
        G.h20 = DEFAULT_BRIGHTNESS;
        G.h22 = DEFAULT_BRIGHTNESS;
        G.h24 = DEFAULT_BRIGHTNESS;
        G.layoutVariant[ReverseMinDirection] = REVERSE_MINUTE_DIR;
        G.layoutVariant[MirrorVertical] = MIRROR_FRONT_VERTICAL;
        G.layoutVariant[MirrorHorizontal] = MIRROR_FRONT_HORIZONTAL;
        G.layoutVariant[FlipHorzVert] = FLIP_HORIZONTAL_VERTICAL;
        G.layoutVariant[ExtraLedPerRow] = EXTRA_LED_PER_ROW;
        G.layoutVariant[MeanderRows] = MEANDER_ROWS;
        G.languageVariant[ItIs15] = false;
        G.languageVariant[ItIs20] = false;
        G.languageVariant[ItIs40] = false;
        G.languageVariant[ItIs45] = false;
        G.languageVariant[EN_ShowAQuarter] = false;

#ifdef IT_IS_Off
        G.itIsVariant = ItIsVariant::Off;

#elif defined(IT_IS_Permanent)
        G.itIsVariant = ItIsVariant::Permanent;

#elif defined(IT_IS_Quarterly)
        G.itIsVariant = ItIsVariant::Quarterly;

#elif defined(IT_IS_HalfHourly)
        G.itIsVariant = ItIsVariant::HalfHourly;

#elif defined(IT_IS_Hourly)
        G.itIsVariant = ItIsVariant::Hourly;
#endif

#ifdef MQTT_SERVER
        strlcpy(G.mqtt.serverAdress, MQTT_SERVER, sizeof(G.mqtt.serverAdress));
        G.mqtt.state = true;
#else
        strcpy(G.mqtt.serverAdress, "192.168.4.1");
        G.mqtt.state = false;
#endif

#ifdef MQTT_PORT
        G.mqtt.port = MQTT_PORT;
#else
        G.mqtt.port = 1883;
#endif

#ifdef MQTT_USER
        strlcpy(G.mqtt.user, MQTT_USER, sizeof(G.mqtt.user));
#else
        strcpy(G.mqtt.user, "User");
#endif

#ifdef MQTT_PASSWORD
        strlcpy(G.mqtt.password, MQTT_PASSWORD, sizeof(G.mqtt.password));
#else
        strcpy(G.mqtt.password, "Password");
#endif

#ifdef MQTT_CLIENT_ID
        strlcpy(G.mqtt.clientId, MQTT_CLIENT_ID, sizeof(G.mqtt.clientId));
#else
        strcpy(G.mqtt.clientId, "ClientId");
#endif

#ifdef MQTT_TOPIC
        strlcpy(G.mqtt.topic, MQTT_TOPIC, sizeof(G.mqtt.topic));
#else
        strcpy(G.mqtt.topic, "ESPWordclock");
#endif

        G.clockTypeDef = DEFAULT_LAYOUT;
        G.buildTypeDef = DEFAULT_BUILDTYPE;
        G.Colortype = DEFAULT_LEDTYPE;
        G.wType = WHITE_LEDTYPE;

        G.bootLedBlink = false;
        G.bootLedSweep = BOOT_LEDSWEEP;
        G.bootShowWifi = true;
        G.bootShowIP = BOOT_SHOWIP;

        G.autoBrightEnabled = 0;
        G.autoBrightMin = 10;
        G.autoBrightMax = 80;
        G.autoBrightPeak = 750;
        G.transitionType = 0; // Transition::NO_TRANSITION;
        G.transitionDuration = 2;
        G.transitionSpeed = 30;
        G.transitionColorize = 0;
        G.transitionDemo = false;

        for (uint8_t i = 0; i < MAX_BIRTHDAY_COUNT; i++) {
            G.birthday[i].day = 0;
            G.birthday[i].month = 0;
        }
        setDefaultHardwarePins();
        i2cBus::setDefaultPins(G.i2cSdaPin, G.i2cSclPin);

        eeprom::write();
        Serial.println("EEPROM written");
    }

    // Initialization of COMMAND_MODE_xxx (color)
    G.progInit = true;

    //-------------------------------------
    // Get Pointer for ClockType
    //-------------------------------------

    if (!isValidClockTypeDef(G.clockTypeDef)) {
        Serial.printf("Invalid ClockType %u in EEPROM, using default %u\n",
                      G.clockTypeDef, DEFAULT_LAYOUT);
        G.clockTypeDef = DEFAULT_LAYOUT;
        eeprom::write();
    }

    usedClockType = clockWork.getPointer(G.clockTypeDef);

    // Area that will be animated:
    //         LED frame horizontal
    //         LED frame vertical
    //         Number of rows (including frames)
    //         Number of columns (including frames)
    // TODO: Get frame width from usedClockType.
    transition = new Transition(usedClockType->rowsWordMatrix(),
                                usedClockType->colsWordMatrix());

    if (usedClockType->numPixelsFrameMatrix() != 0) {
        secondsFrame = new SecondsFrame(usedClockType->numPixelsFrameMatrix());
    } else {
        secondsFrame = nullptr;
    }

    //-------------------------------------
    // Initialize LEDs
    //-------------------------------------

    clockWork.initHardwareButtons();

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
    // Initialize I2C
    //-------------------------------------

    i2cBus::begin(G.i2cSdaPin, G.i2cSclPin);

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
#elif defined(ESP32)
    sntp_set_time_sync_notification_cb(time_is_set);
#endif

    //-------------------------------------
    // Start WiFi
    //-------------------------------------

    if (G.bootShowWifi) {
        led.setBitmapSymbol(WLAN100, HsbColor(0.66f, 1.0f, 0.2f));
    }
    network.setup(G.hostname);
#if WIFI_VERBOSE
    int strength = network.getQuality();
    Serial.printf("Signal strength: %i\n", strength);
#endif
    wifiStart();
    configTime(0, 0, G.timeserver);
    applyTimezone();

    delay(50);

    //-------------------------------------
    // OTA
    //-------------------------------------

#if WEB_PROTECTED
    httpUpdater.setup(&httpServer, WEB_USER, WEB_PASSWORD);
#else
    httpUpdater.setup(&httpServer);
#endif
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

#if WEB_PROTECTED
    webSocket.setAuthorization(WEB_USER, WEB_PASSWORD);
#endif
    webSocket.begin();
    webSocket.onEvent(webSocketEvent);

    Serial.println("Websocket started");

    //-------------------------------------
    // mDNS
    //-------------------------------------
    MDNS.begin(G.hostname);
    MDNS.addService("http", "tcp", 80);
    MDNS.addService("ota", "tcp", 81);

    //-------------------------------------
    Serial.println("--------------------------------------");
    Serial.println("ESP Wordclock");
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

    //-------------------------------------
    // Auto brightness
    //-------------------------------------

    // Find BH1750 and initialize if available else fallback to LDR if available
    byte bh1750Address = i2cBus::findBH1750Address(G.i2cSdaPin, G.i2cSclPin);

    if (bh1750Address != 0) {
        lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE, bh1750Address);
        Serial.println("BH1750 initialized");
        bh1750Initialized = true;
    } else {
        Serial.println("BH1750 initialisation error, using LDR if available");
        bh1750Initialized = false;
    }

    //-------------------------------------

    Serial.println("--------------------------------------");
    Serial.println("End Setup");
    Serial.println("--------------------------------------");
    Serial.println("");

    //-------------------------------------
    // Reset Powercycle Counter
    //-------------------------------------
    delay(500);
    powerCycleCount = 0;
    EEPROM.write(POWER_CYCLE_COUNT_ADDRESS, powerCycleCount);
    EEPROM.commit();

    //-------------------------------------
    // Frame Init
    //-------------------------------------
    if (secondsFrame) {
        secondsFrame->initFrame();
    }

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

    //------------------------------------------------
    // Time
    //------------------------------------------------
    time_t utc = time(nullptr);
    struct tm tm;
    localtime_r(&utc, &tm);
    if (utc > 100000000) {
        _second = tm.tm_sec;
        _minute = tm.tm_min;
        _hour = tm.tm_hour;
    }

    //------------------------------------------------
    // Network
    //------------------------------------------------
    network.loop();
#ifdef ESP8266
    MDNS.update();
#endif
    httpServer.handleClient();

    //------------------------------------------------
    // Websocket
    //------------------------------------------------
    webSocket.loop();

    //------------------------------------------------
    // MQTT
    //------------------------------------------------
    if (G.mqtt.state && WiFi.status() == WL_CONNECTED) {
        mqtt.loop();
    }

    //------------------------------------------------
    // Frame
    //------------------------------------------------
    if (secondsFrame) {
        secondsFrame->loop();
    }

    //------------------------------------------------
    // Transition
    //------------------------------------------------
    transition->loop(tm); // must be called periodically

    // make the time run faster in the demo mode of the transition
    transition->demoMode(_hour, _minute, _second);

    //------------------------------------------------
    // Clockwork
    //------------------------------------------------
    clockWork.loop(tm);
}

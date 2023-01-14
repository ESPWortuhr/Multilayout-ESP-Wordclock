#pragma once
#include "Arduino.h"
#include "version.gen.h"

#define PAYLOAD_LENGTH 30
#define MAX_ARRAY_SIZE 291

enum ClockWords {
    ESIST = 0,
    VOR = 1,
    NACH = 2,
    UHR = 3,

    FUENF = 4,
    ZEHN = 5,
    VIERTEL = 6,
    DREIVIERTEL = 22,
    ZWANZIG = 7,
    HALB = 8,
    EINS = 9,

    H_EIN = 10,
    H_ZWEI = 11,
    H_DREI = 12,
    H_VIER = 13,
    H_FUENF = 14,
    H_SECHS = 15,
    H_SIEBEN = 16,
    H_ACHT = 17,
    H_NEUN = 18,
    H_ZEHN = 19,
    H_ELF = 20,
    H_ZWOELF = 21
};

struct MqttData {
    uint8_t state;
    char serverAdress[PAYLOAD_LENGTH];
    char user[PAYLOAD_LENGTH];
    char password[PAYLOAD_LENGTH];
    char clientId[PAYLOAD_LENGTH];
    char topic[PAYLOAD_LENGTH];
    uint16_t port;
};

struct OpenWeatherMapData {
    char apikey[35];
    char cityid[8];
};

struct GLOBAL {
    uint8_t sernr;
    uint16_t prog;
    uint8_t param1;
    uint8_t prog_init;
    uint16_t conf;
    uint8_t rgb[4][4];
    uint8_t rr;
    uint8_t gg;
    uint8_t bb;
    uint8_t ww;
    uint8_t hell;
    uint16_t ldr;
    uint16_t ldrCal;
    int geschw;
    uint8_t client_nr;
    uint8_t zeige_sek;
    uint8_t zeige_min;
    char zeitserver[PAYLOAD_LENGTH];
    char hostname[PAYLOAD_LENGTH];
    char ltext[PAYLOAD_LENGTH];
    uint8_t hh;
    uint8_t h6;
    uint8_t h8;
    uint8_t h12;
    uint8_t h16;
    uint8_t h18;
    uint8_t h20;
    uint8_t h22;
    uint8_t h24;
    bool Sprachvariation[5];

    uint8_t UhrtypeDef;
    uint8_t Colortype;

    MqttData mqtt;

    OpenWeatherMapData openWeatherMap;

    uint8_t autoLdrEnabled;
    uint8_t autoLdrBright;
    uint8_t autoLdrDark;
    uint8_t animType;
    uint8_t animDuration;
    uint8_t animSpeed;
    uint8_t animColorize;
    uint8_t animDemo;

    bool bootLedBlink;
    bool bootLedSweep;
    bool bootShowWifi;
    bool bootShowIP;
};
GLOBAL G = {};

// LDR
uint8_t ldrVal = 100;

uint8_t _second = 0;
uint8_t _second48 = 0;
uint8_t _minute = 0;
uint8_t _hour = 0;
uint8_t lastSecond = 0;
uint8_t lastMinute = 0;
uint8_t lastHour = 0;

unsigned int count_delay = 0;
unsigned long previousMillis = 0;

uint32_t clockTime;
uint16_t frontMatrix[MAX_ARRAY_SIZE] = {0};
uint16_t lastFrontMatrix[MAX_ARRAY_SIZE] = {0};
bool parametersChanged = false;
uint8_t statusAccessPoint = 0;

char str[1024];

bool externalRTC = false;

enum ColorPosition {
    Foreground = 0,
    Background = 1,
    Frame = 2,
    Effect = 3,
};

enum LedColorVariants {
    Brg = 0,
    Grb = 1,
    Rgb = 2,
    Rbg = 3,
    Grbw = 4,
};

enum LanguageDialects {
    ItIs15 = 0,
    ItIs20 = 1,
    ItIs40 = 2,
    ItIs45 = 3,
    NotShowItIs = 4,
};

enum CommandWords {
    COMMAND_IDLE = 0,

    COMMAND_MODE_WORD_CLOCK = 1,
    COMMAND_MODE_SECONDS = 2,
    COMMAND_MODE_SCROLLINGTEXT = 3,
    COMMAND_MODE_RAINBOWCYCLE = 4,
    COMMAND_MODE_RAINBOW = 5,
    COMMAND_MODE_COLOR = 6,

    COMMAND_MODE_ANIMATION = 10,
    COMMAND_SET_INITIAL_VALUES = 20,
    COMMAND_SET_TIME = 30,
    COMMAND_SET_LANGUAGE_VARIANT = 84,
    COMMAND_SET_MQTT = 85,
    COMMAND_SET_TIME_MANUAL = 86,
    COMMAND_SET_WPS_MODE = 87,
    COMMAND_SET_COLORTYPE = 88,
    COMMAND_SET_UHRTYPE = 89,
    COMMAND_SET_WEATHER_DATA = 90,
    COMMAND_SET_LDR = 91,
    COMMAND_SET_HOSTNAME = 92,
    COMMAND_SET_SETTING_SECOND = 93,
    COMMAND_SET_MINUTE = 94,
    COMMAND_SET_BRIGHTNESS = 95,
    COMMAND_SET_MARQUEE_TEXT = 96,
    COMMAND_SET_TIMESERVER = 97,
    COMMAND_SET_WIFI_DISABLED = 98,
    COMMAND_SET_WIFI_AND_RESTART = 99,
    COMMAND_RESET = 100,
    COMMAND_SET_BOOT = 101,
    COMMAND_SET_AUTO_LDR = 102,

    COMMAND_SPEED = 152,

    COMMAND_REQUEST_CONFIG_VALUES = 200,
    COMMAND_REQUEST_COLOR_VALUES = 201,
    COMMAND_REQUEST_WIFI_LIST = 202,
    COMMAND_REQUEST_AUTO_LDR = 203,
    COMMAND_REQUEST_ANIMATION = 204,
    COMMAND_REQUEST_MQTT_VALUES = 205,

};

enum ClockType {
    Uhr_114 = 1,
    Uhr_114_Alternative = 2,
    Uhr_114_2Clock = 6,
    Uhr_114_Dutch = 9,
    Uhr_125 = 3,
    Uhr_125_Type2 = 8,
    Uhr_169 = 4,
    Uhr_242 = 5,
    Uhr_291 = 7,
};

enum Icons {
    WLANT = 0,
    WLAN100 = 1,
    WLAN60 = 2,
    WLAN30 = 3,
    HERZ = 4,
    SMILY = 5,
    FIRE_1 = 6,
    FIRE_2 = 7,
    FIRE_3 = 8,
    FIRE_4 = 9,
    FIRE_5 = 10,
    FIRE_6 = 11,
    RGB_I = 12,
};
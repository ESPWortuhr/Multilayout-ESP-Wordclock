#pragma once
#include "Arduino.h"

const char *VER = "2.6.0"; // Software Version

#define MAX_ARRAY_SIZE 291
#define ROW_START 0 /* Startindex der Reihe */
#define MAX_ROWS 10 /* Anzahl der Reihen (Y-Richtung) */
#define COL_START 0 /* Startindex der Spalte */
#define MAX_COL 11 /* Anzahl der Spalten (X-Richtung) */

#define MAX_BAELLE MAX_COL
#define MAX_ANIM_PERIOD 5
#define MAX_RANDOM 10
#define STRIPE NULL

#if (defined(UHR_125) || defined(UHR_242))
#undef MAX_ROWS
#define MAX_ROWS 11 /* Anzahl der Reihen (Y-Richtung) */
#endif

enum uhrzeit_t {
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

struct GLOBAL {
    uint8_t sernr;
    uint16_t prog;
    uint8_t param1;
    uint8_t param2;
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
    char apikey[35];
    char cityid[8];
    int geschw;
    uint8_t client_nr;
    uint8_t zeige_sek;
    uint8_t zeige_min;
    char zeitserver[30];
    char hostname[30];
    char ltext[30];
    uint8_t hh;
    uint8_t h6;
    uint8_t h8;
    uint8_t h12;
    uint8_t h16;
    uint8_t h18;
    uint8_t h20;
    uint8_t h22;
    uint8_t h24;
    uint8_t Sprachvariation[4];

    uint8_t UhrtypeDef;
    uint8_t Colortype;
    uint8_t MQTT_State;
    char MQTT_Server[30];
    uint16_t MQTT_Port;

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

const char *server = "api.openweathermap.org"; // Openweather server's address
const char *resource1 = "/data/2.5/forecast?id="; // Openweather API URL part 1
const char *resource2 = "&units=metric&APPID=";   // Openweather API URL part 2
const char *resource3 = "&cnt=8"; // Openweather API forecast time
char resource[100];
char response[3500];        // fixed size buffer
uint16_t weather_tag = 600; // counter fuer Wetterdaten abrufen
int16_t wtemp_6;
int16_t wtemp_12;
int16_t wtemp_18;
int16_t wtemp_24;
uint16_t wwetter_6;
uint16_t wwetter_12;
uint16_t wwetter_18;
uint16_t wwetter_24;
uint16_t wstunde;
uint16_t wetterswitch;

// LDR
uint8_t ldrVal = 100;

// Telnet vars
bool ConnectionEstablished; // Flag for successfully handled connection
#define MAX_TELNET_CLIENTS 2
WiFiServer TelnetServer(23);
WiFiClient TelnetClient[MAX_TELNET_CLIENTS];

unsigned char wlan_client = false;
unsigned char wlan_status = 99;
unsigned char wlan_ssid = false;

uint8_t _sekunde = 0;
uint8_t _minute = 0;
uint8_t _stunde = 0;
uint8_t last_sekunde = 0;
uint8_t last_minute = 0;
uint8_t last_stunde = 0;

unsigned long count_millis48 = 0;
unsigned long previous48 = 0;
unsigned long transitionDelay = 0;
const long interval48 = 1250;
uint8_t _sekunde48 = 0;
uint8_t last_sekunde48 = 100;

unsigned int count_millis = 0;
unsigned int count_delay = 0;

unsigned long previousMillis = 0;
const long interval = 1000; // 1 Sekunde

uint32_t uhrzeit;
uint16_t Word_array[MAX_ARRAY_SIZE] = {0};
uint16_t Word_array_old[MAX_ARRAY_SIZE] = {0};
bool parameters_changed = false;
uint8_t AP_Status = 0;

char str[1024];
char s[6];

bool externalRTC = false;

enum ledPositions { Foreground = 0, Background = 1, Frame = 2, Effect = 3 };

enum ledColortypes {
    Brg = 0,
    Grb = 1,
    Rgb = 2,
    Rbg = 3,
    Grbw = 4,
};

enum Sprachvariationen {
    ItIs15 = 0,
    ItIs20 = 1,
    ItIs40 = 2,
    ItIs45 = 3,
};

enum Command {
    COMMAND_IDLE = 0,

    COMMAND_MODE_WORD_CLOCK = 1,
    COMMAND_MODE_SECONDS = 2,
    COMMAND_MODE_MARQUEE = 3,
    COMMAND_MODE_RAINBOW = 4,
    COMMAND_MODE_CHANGE = 5,
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

    COMMAND_BRIGHTNESS = 151,
    COMMAND_SPEED = 152,
    COMMAND_LEDS = 153,
    COMMAND_POSITION = 154,

    COMMAND_REQUEST_CONFIG_VALUES = 200,
    COMMAND_REQUEST_COLOR_VALUES = 201,
    COMMAND_REQUEST_WIFI_LIST = 202,
    COMMAND_REQUEST_AUTO_LDR = 203,
    COMMAND_REQUEST_ANIMATION = 204,

};

enum ledText {

    /* Numbers from 0 to 60 are reserved */

    es_ist = 101,
    nach = 102,
    vor = 103,
    viertel = 104,
    dreiviertel = 105,
    uhr = 106,
    halb = 107,
    fuenf = 108,
    zehn = 109,
    zwanzig = 110,
    eins = 111,
    frueh = 112,
    minute = 113,
    minuten = 114,
    minuten_extra = 115,
    abends = 116,
    mitternacht = 117,
    mittags = 118,
    warm = 119,
    morgens = 120,
    und = 121,
    minuten_uhr = 122,

    h_ein = 151,
    h_zwei = 152,
    h_drei = 153,
    h_vier = 154,
    h_fuenf = 155,
    h_sechs = 156,
    h_sieben = 157,
    h_acht = 158,
    h_neun = 159,
    h_zehn = 160,
    h_elf = 161,
    h_zwoelf = 162,
    h_dreizehn = 163,

    w_morgen = 200,
    w_frueh = 201,
    w_abend = 202,
    w_mittag = 203,
    w_nacht = 204,
    w_schnee = 205,
    w_klar = 206,
    w_warnung = 207,
    w_regen = 208,
    w_wolken = 209,
    w_gewitter = 210,
    w_unter = 211,
    w_ueber = 212,
    w_minus = 213,
    w_null = 214,
    w_fuenf = 215,
    w_zehn = 216,
    w_und = 217,
    w_zwanzig = 218,
    w_dreissig = 219,
    w_grad = 220,

    h_droelf = 240,
    happy_birthday = 241
};

enum UhrTypeDefinitions {
    Uhr_114 = 1,
    Uhr_114_Alternative = 2,
    Uhr_114_2Clock = 6,
    Uhr_125 = 3,
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

const uint8_t dim[20] = {30,  50,  70,  90,  110, 130, 140, 160, 200, 255,
                         255, 200, 160, 100, 80,  60,  40,  20,  10,  0};
const int8_t diff[20] = {-30, -20, -20, -20, -20, -20, -10, -20, -40, -55,
                         0,   55,  40,  60,  20,  20,  20,  20,  10,  10};

//--OTA--
ESP8266WebServer httpServer(81);
ESP8266HTTPUpdateServer httpUpdater;
//--OTA--

#pragma once
#include "Arduino.h"

const char *VER = "2.6.0"; // Software Version

#define MAX_ARRAY_SIZE 291

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
    char MQTT_User[30];
    char MQTT_Pass[30];
    char MQTT_ClientId[30];
    char MQTT_Topic[30];
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
    COMMAND_REQUEST_MQTT_VALUES = 205,

};

enum UhrTypeDefinitions {
    Uhr_114 = 1,
    Uhr_114_Alternative = 2,
    Uhr_114_2Clock = 6,
    Uhr_125 = 3,
    Uhr_125_Type2 = 8,
    Uhr_169 = 4,
    Uhr_242 = 5,
    Uhr_291 = 7,
    Uhr_114_mc = 9,
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

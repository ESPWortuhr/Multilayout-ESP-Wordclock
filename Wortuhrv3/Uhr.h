#include "Arduino.h"

const char *VER = "2.5.0";  // Software Version

#pragma once

enum uhrzeit_t {
    ESIST = 0,
    VOR = 1,
    NACH = 2,
    UHR = 3,

    FUENF = 4,
    ZEHN = 5,
    VIERTEL = 6,
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
    char ssid[WL_SSID_MAX_LENGTH];
    char passwd[WL_WPA_KEY_MAX_LENGTH];
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
    uint8_t UhrtypeDef;
    uint8_t Colortype;
};
GLOBAL G = {};

const char* server = "api.openweathermap.org";  // Openweather server's address
const char* resource1 = "/data/2.5/forecast?id="; // Openweather API URL part 1
const char* resource2 = "&units=metric&APPID="; // Openweather API URL part 2
const char* resource3 = "&cnt=8"; // Openweather API forecast time
char resource[100];
char response[3500];       //fixed size buffer
WiFiClient client;
uint16_t weather_tag = 600;    //counter fuer Wetterdaten abrufen
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
unsigned long waitUntilLDR = 0;
uint16_t autoBrightnessEnabled = 1;
uint16_t ldrVal = 50;
uint16_t oneseconddelay = 1000;

// Telnet vars
bool ConnectionEstablished; // Flag for successfully handled connection
#define MAX_TELNET_CLIENTS 2
WiFiServer TelnetServer(23);
WiFiClient TelnetClient[MAX_TELNET_CLIENTS];

const char *ssid_ap = "Uhr";
const char *password_ap = "12345678";
//--OTA--
const char *host = "webupdate";
//--OTA--

unsigned char wlan_client = false;
unsigned char wlan_status = 99;
unsigned char wlan_ssid = false;

uint8_t _sekunde = 0;
uint8_t _minute = 0;
uint8_t _stunde = 0;
uint8_t last_sekunde = 100;
uint8_t last_minute = 100;
uint8_t last_stunde = 100;

uint8_t count_millis48 = 0;
unsigned long previous48 = 0;
const long interval48 = 1250;
uint8_t _sekunde48 = 0;
uint8_t last_sekunde48 = 100;

unsigned long unix_time = 0;

unsigned int count_millis = 0;
unsigned int count_tag = 0;
unsigned int count_delay = 0;
unsigned char ntp_flag = false;

unsigned long previousMillis = 0;
const long interval = 1000;   // 1 Sekunde

uint32_t uhrzeit;
uint8_t Word_array[242] = { 255 };
uint16_t Five_Min_Counter = 0;
bool Wifi_off = false;

char str[450];
char s[5];

bool externalRTC = false;

enum ledPositions {
    Foreground = 0,
    Background = 1,
    Frame = 2,
    Effect = 3
};

enum ledColortypes {
    Brg = 0,
    Grb = 1,
    Rgb = 2,
    Rbg = 3,
    Grbw = 4,
};

enum Command {
    COMMAND_IDLE = 0,

    COMMAND_MODE_WORD_CLOCK = 1,
	COMMAND_MODE_SECONDS = 2,
	COMMAND_MODE_MARQUEE = 3,
	COMMAND_MODE_RAINBOW = 4,
	COMMAND_MODE_CHANGE = 5,
	COMMAND_MODE_COLOR = 6,

    COMMAND_SET_INITIAL_VALUES = 20,
    COMMAND_SET_TIME = 30,
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

    COMMAND_BRIGHTNESS = 151,
    COMMAND_SPEED = 152,
    COMMAND_LEDS = 153,
    COMMAND_POSITION = 154,

    COMMAND_REQUEST_CONFIG_VALUES = 200,
    COMMAND_REQUEST_COLOR_VALUES = 201,
    COMMAND_REQUEST_WIFI_LIST = 202,
};

enum ledText {
    es_ist =  1,
    nach =  2,
    vor =  3,
    viertel =  4,
    dreiviertel = 51,
    uhr =  5,
    halb =  6,
    fuenf =  7,
    zehn =  8,
    zwanzig =  9,
    eins =  10,

    h_ein =  11,
    h_zwei =  12,
    h_drei =  13,
    h_vier =  14,
    h_fuenf =  15,
    h_sechs =  16,
    h_sieben =  17,
    h_acht =  18,
    h_neun =  19,
    h_zehn =  20,
    h_elf =  21,
    h_zwoelf =  22,

    w_morgen =  30,
    w_frueh =  31,
    w_abend =  32,
    w_mittag =  33,
    w_nacht =  34,
    w_schnee =  35,
    w_klar =  36,
    w_warnung =  37,
    w_regen =  38,
    w_wolken =  39,
    w_gewitter =  40,
    w_unter =  41,
    w_ueber =  42,
    w_minus =  43,
    w_null =  44,
    w_fuenf =  45,
    w_zehn =  46,
    w_und =  47,
    w_zwanzig =  48,
    w_dreissig =  49,
    w_grad =  50,

    h_droelf = 90,
    happy_birthday = 91
};

enum UhrTypeDefinitions {
	Uhr_114 = 1,
	Uhr_114_Alternative = 2,
	Uhr_125 = 3,
	Uhr_169 = 4,
	Uhr_242 = 5,
};


const uint8_t dim[20] = {30, 50, 70, 90, 110, 130, 140, 160, 200, 255, 255, 200, 160, 100, 80, 60, 40, 20, 10, 0};
const int8_t diff[20] = {-30, -20, -20, -20, -20, -20, -10, -20, -40, -55, 0, 55, 40, 60, 20, 20, 20, 20, 10, 10};


//--OTA--
ESP8266WebServer httpServer(81);
ESP8266HTTPUpdateServer httpUpdater;
//--OTA--

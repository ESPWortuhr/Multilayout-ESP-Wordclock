#pragma once
#include "Arduino.h"
#include "version.gen.h"

#define PAYLOAD_LENGTH 30
#define MAX_ROW_SIZE 22
#define MAX_BIRTHDAY_COUNT 5

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
    H_ZWOELF = 21,
    H_MITTERNACHT = 23
};

struct MqttData {
    bool state;
    char serverAdress[PAYLOAD_LENGTH];
    char user[PAYLOAD_LENGTH];
    char password[PAYLOAD_LENGTH];
    char clientId[PAYLOAD_LENGTH];
    char topic[PAYLOAD_LENGTH];
    uint16_t port;
};

enum class WhiteType {
    WarmWhite = 0,
    NeutralWhite = 1,
    ColdWhite = 2,
};

struct Birthday {
    uint8_t day;
    uint8_t month;
    uint16_t year;
};

struct OpenWeatherMapData {
    char apikey[35];
    char cityid[8];
};

enum class BuildTypeDef {
    Normal = 0,
    DoubleResM1 = 1,
    DoubleRes = 2,
};

enum class MinuteVariant {
    Off = 0,
    LED4x = 1,
    LED7x = 2,
    Corners = 3,
    InWords = 4,
};

enum class SecondVariant {
    Off = 0,
    FrameDot = 1,
    FrameSector = 2,
    FrameSectorToggle = 3,
};

enum class WordclockChanges {
    Null,
    Parameters,
    Minute,
    Layout,
    Words,
};

struct GLOBAL {
    uint8_t sernr;
    uint16_t prog;
    uint8_t param1;
    bool progInit;
    uint16_t conf;
    HsbColor color[3];
    uint8_t effectBri;
    uint8_t effectSpeed;
    uint8_t client_nr;
    SecondVariant secondVariant;
    MinuteVariant minuteVariant;
    bool languageVariant[6];
    bool layoutVariant[3];
    char timeserver[PAYLOAD_LENGTH];
    char hostname[PAYLOAD_LENGTH];
    char scrollingText[PAYLOAD_LENGTH];

    uint8_t h6;
    uint8_t h8;
    uint8_t h12;
    uint8_t h16;
    uint8_t h18;
    uint8_t h20;
    uint8_t h22;
    uint8_t h24;

    uint8_t UhrtypeDef;
    BuildTypeDef buildTypeDef;
    uint8_t Colortype;
    WhiteType wType;

    MqttData mqtt;

    OpenWeatherMapData openWeatherMap;

    uint8_t autoBrightEnabled;
    uint8_t autoBrightOffset;
    uint8_t autoBrightSlope;
    uint8_t transitionType;
    uint8_t transitionDuration;
    uint8_t transitionSpeed;
    uint8_t transitionColorize;
    uint8_t transitionDemo;

    bool bootLedBlink;
    bool bootLedSweep;
    bool bootShowWifi;
    bool bootShowIP;

    Birthday birthday[MAX_BIRTHDAY_COUNT];
};
GLOBAL G = {};

// LDR
float ledGain = 100;

uint8_t _second = 0;
uint8_t _secondFrame = 0;
uint8_t _minute = 0;
uint8_t _hour = 0;
uint8_t lastSecond = 0;
uint8_t lastMinute = 0;

uint32_t frontMatrix[MAX_ROW_SIZE] = {0};
uint32_t lastFrontMatrix[MAX_ROW_SIZE] = {0};
uint8_t minuteArray = 0; /* Using a byte as a per bit array */
uint8_t lastMinuteArray = 0;
uint16_t minutePixelArray[4] = {0};
uint64_t frameArray = 0;
bool parametersChanged = false;
bool layoutChanged = false;
bool colorChangedByWebsite = false;
uint8_t statusAccessPoint = 0;

char str[1024];

bool externalRTC = false;

enum ColorPosition {
    Foreground = 0,
    Background = 1,
    Frame = 2,
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
    EN_ShowAQuarter = 5,
};

enum LayoutVariants {
    ReverseMinDirection = 0,
    MirrorVertical = 1,
    MirrorHorizontal = 2,
};

enum fontSize {
    normalSizeASCII = 0,
    smallSizeNumbers = 1,
};

enum CommandWords {
    COMMAND_IDLE = 0,

    COMMAND_MODE_WORD_CLOCK = 1,
    COMMAND_MODE_SECONDS = 2,
    COMMAND_MODE_SCROLLINGTEXT = 3,
    COMMAND_MODE_RAINBOWCYCLE = 4,
    COMMAND_MODE_RAINBOW = 5,
    COMMAND_MODE_COLOR = 6,
    COMMAND_MODE_DIGITAL_CLOCK = 7,
    COMMAND_MODE_SYMBOL = 8,
    COMMAND_MODE_TRANSITION = 10,

    PLACEHOLDER_MAX_MODE = 19,

    COMMAND_SET_INITIAL_VALUES = 20,
    COMMAND_SET_TIME = 30,

    COMMAND_SET_BIRTHDAYS = 83,
    COMMAND_SET_LANGUAGE_VARIANT = 84,
    COMMAND_SET_MQTT = 85,
    COMMAND_SET_TIME_MANUAL = 86,
    COMMAND_SET_BUILDTYPE = 87,
    COMMAND_SET_COLORTYPE = 88,
    COMMAND_SET_UHRTYPE = 89,
    COMMAND_SET_WEATHER_DATA = 90,
    COMMAND_SET_WHITETYPE = 91,
    COMMAND_SET_HOSTNAME = 92,
    COMMAND_SET_SETTING_SECOND = 93,
    COMMAND_SET_MINUTE = 94,
    COMMAND_SET_BRIGHTNESS = 95,
    COMMAND_SET_SCROLLINGTEXT = 96,
    COMMAND_SET_TIMESERVER = 97,
    COMMAND_SET_WIFI_DISABLED = 98,
    COMMAND_SET_WIFI_AND_RESTART = 99,
    COMMAND_RESET = 100,
    COMMAND_SET_BOOT = 101,
    COMMAND_SET_AUTO_BRIGHT = 102,
    COMMAND_SET_LAYOUT_VARIANT = 103,
    COMMAND_SET_MQTT_HA_DISCOVERY = 104,

    COMMAND_SPEED = 152,

    PLACEHOLDER_MAX_SET = 199,

    COMMAND_REQUEST_CONFIG_VALUES = 200,
    COMMAND_REQUEST_COLOR_VALUES = 201,
    COMMAND_REQUEST_WIFI_LIST = 202,
    COMMAND_REQUEST_AUTO_BRIGHT = 203,
    COMMAND_REQUEST_TRANSITION = 204,
    COMMAND_REQUEST_MQTT_VALUES = 205,
    COMMAND_REQUEST_BIRTHDAYS = 206,

    PLACEHOLDER_MAX_REQUEST = 255,
};

enum ClockType {
    Eng10x11 = 10,
    Ger10x11 = 1,
    Ger10x11Alternative = 2,
    Ger10x11AlternativeFrame = 4,
    Ger10x11Vertical = 12,
    Ger10x11Clock = 6,
    Ger10x11schwaebisch = 20,
    Ger10x11Nero = 11,
    Ger11x11 = 3,
    Ger11x11V2 = 8,
    Ger11x11V3 = 14,
    Ger22x11Weather = 5,
    Ger16x8 = 13,
    Ger16x18 = 7,
    Nl10x11 = 9,
    Es10x11 = 15,
    It10x11 = 16,
    Hu10x10 = 17,
    Ch10x11 = 18,
    Ro10x11 = 19,
    Fr10x11 = 21,
    Se10x11 = 22,
    Ru10x11 = 23,
};

enum Icons {
    WLAN100 = 0,
    WLAN60 = 1,
    WLAN30 = 2,
    FIRE_1 = 3,
    FIRE_2 = 4,
    FIRE_3 = 5,
    FIRE_4 = 6,
    FIRE_5 = 7,
    FIRE_6 = 8,
    HEART = 9,
    SMILEY = 10,
    NOTE = 11,
    SNOW = 12,
    MAIL = 13,
    BELL = 14,
    STOP = 15,
    STBY = 16,
};

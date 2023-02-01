/*
 * This is the default word clock configuration. You can also change it later in
 * the settings menu on the clock's hosted website.
 */

//--------------------------------------------------------------------------
// Front panel layout
//--------------------------------------------------------------------------
/**********************/
/*       German       */
/**********************/
//
#define DEFAULT_LAYOUT Ger10x11
// 10 rows, 11 LEDs per row + 4 LEDs for minutes
//
// #define DEFAULT_LAYOUT Ger10x11Alternative
// 10 rows, each 11 LED's per row + 4 LED's for minutes, with modified
// layout for extra words in the matrix
//
// #define DEFAULT_LAYOUT Ger10x11Clock
// 10 rows, each 11 LED's per row + 4 LED's for minutes, with the layout
// from the original manufacturer
//
// #define DEFAULT_LAYOUT Ger11x11
// 11 rows, each 11 LED's per row + 4 LED's for minutes
//
// #define DEFAULT_LAYOUT Ger10x11Frame
// with additional LED's to illuminate the frame from the side
//
// #define DEFAULT_LAYOUT Ger21x11Weather
// clock with weather forecast 242 LED's
//
// #define DEFAULT_LAYOUT Ger17x17
// Clock with 24 hours display 18x16
//
/**********************/
/*       Dutch        */
/**********************/
//
// #define DEFAULT_LAYOUT  Nl10x11
// 10 rows, each 11 LED's per row + 4 LED's for minutes, with changed
// layout for Dutch language
//
/**********************/
/*       English      */
/**********************/
//
// #define DEFAULT_LAYOUT  Eng10x11
// 10 rows, 11 LED's per row + 4 LED's for minutes, with modified
// layout for the English language

//--------------------------------------------------------------------------
// Define LED Type
//--------------------------------------------------------------------------
/*
 * Currently only WS2812 or SK6812 LEDs are supported.
 * Different manufacturers use different layout, so you can change it
 * accordingly.
 *
 * Valid values [Brg, Grb, Rgb, Rbg, Grbw]
 */
#define DEFAULT_LEDTYPE Brg

//--------------------------------------------------------------------------
// Define External Realtime Clock
//--------------------------------------------------------------------------
/*
 * If you want to use an external RTC it must be defined here, for examle for
 * Wifi less Operation of the clock.
 * However, one must always be defined, even if none is installed.
 *
 * Valid values [RTC_DS1307, RTC_PCF8523, RTC_DS3231]
 */
#define RTC_Type RTC_DS3231

//--------------------------------------------------------------------------
// Serial Number
//--------------------------------------------------------------------------
/*
 * To delete or initialize the EEPROM, enter another serial number here.
 *
 * Valid values [0 ... 255]
 */
#define SERNR 70

//--------------------------------------------------------------------------
// Toggle Serial DEBUG Output
//--------------------------------------------------------------------------
/*
 * To delete or initialize the EEPROM, enter another serial number here.
 *
 * Valid values [true, false]
 */
#define GENERAL_VERBOSE true
#define WEATHER_VERBOSE false

//--------------------------------------------------------------------------
// Manual Wifi settings
//--------------------------------------------------------------------------
/*
 * Here you can set the WiFi connection settings manually. The clock can be
 * paired with a new WiFi, but the setting made here is set again when the
 * clock is restarted.
 *
 * Valid values MANUAL_WIFI_SETTINGS [true, false]
 * Valid values WIFI_SSID [up to 64 Alphanumeric Letters]
 * Valid values WIFI_PASSWORD [up to 64 Alphanumeric Letters]
 * 
 */
#define MANUAL_WIFI_SETTINGS true
#define WIFI_SSID "WIFI SSID"
#define WIFI_PASSWORD "WIFI PWD"

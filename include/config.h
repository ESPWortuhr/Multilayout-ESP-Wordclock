/*
 * This is the default word clock configuration. You can also change it later
 * in the settings menu on the clock's hosted website.
 */

//--------------------------------------------------------------------------
// PIN Configuration for ESP32 (ESP8266 currently unsupported)
//--------------------------------------------------------------------------
#define LED_PIN 3        // Use direct pin number
#define SDA_PIN_ESP32 21 // Use direct pin number
#define SCL_PIN_ESP32 22 // Use direct pin number

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
// layout for extra words in the matrix from the Github user @dbambus
//
// #define DEFAULT_LAYOUT Ger10x11AlternativeFrame
// 10 rows, each 11 LED's per row + 4 LED's (minutes)+ 48 LED's (seconds) in a
// Frame around with modified layout for extra words in the matrix from the
// Github user @dbambus
//
// #define DEFAULT_LAYOUT Ger10x11Clock
// 10 rows, each 11 LED's per row + 4 LED's for minutes, with the layout
// from the original manufacturer
//
// #define DEFAULT_LAYOUT Ger10x11schwaebisch
// 10 rows, each 11 LED's per row + 4 LED's for minutes, with the layout
// from the original manufacturer
//
// #define DEFAULT_LAYOUT Ger10x11Nero
// 10 rows, each 11 LED's per row + 4 LED's for minutes, with the layout
// from the Github user @n3roGit
//
// #define DEFAULT_LAYOUT Ger11x11
// 11 rows, each 11 LED's per row + 4 LED's for minutes
//
// #define DEFAULT_LAYOUT Ger11x11V2
// 11 rows, each 11 LED's per row + 4 LED's for minutes plus twenty word
//
// #define DEFAULT_LAYOUT Ger11x11V3
// 11 rows, each 11 LED's per row. Layout for panbachi/wordclock plate design
//
// #define DEFAULT_LAYOUT Ger11x11Frame
// Same Layout as Ger11x11, but with additional LED's to illuminate the frame
// from the side
//
// #define DEFAULT_LAYOUT Ger13x13
// 13 rows, 13 LEDs per row
//
// #define DEFAULT_LAYOUT Ger22x11Weather
// 22 rows, each 11 LED's per row, with weather forecast on overall 242 LED's
// based on design of Github user @Eisbaeeer
//
// #define DEFAULT_LAYOUT Ger16x8
// 16 rows, each 8 LED's per row, based on a 16x16 presoldered WS2812b matrix
// and a 3D printed case
//
// #define DEFAULT_LAYOUT Ger16x18
// 16 rows, each 18 LED's per row, with the layout WordClock24h of
// mikrocontroller.net
//
// #define DEFAULT_LAYOUT Ger08x08
// 8 rows, each 8 LED's per row in 5 minute steps
//
// #define DEFAULT_LAYOUT Ger08x08Viertel
// 8 rows, each 8 LED's per row, only quarter hours
//
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
//
// #define DEFAULT_LAYOUT Eng08x08
// 8 rows, 8 LED's per row with modified layout for the English language
//
// #define DEFAULT_LAYOUT Eng11x12
// 11 rows, 12 LED's per row with modified layout for the English language

/**********************/
/*       Italian      */
/**********************/
//
// #define DEFAULT_LAYOUT  It10x11
// 10 rows, 11 LED's per row + 4 LED's for minutes, with modified
// layout for the Italian language

/**********************/
/*       Spanish      */
/**********************/
//
// #define DEFAULT_LAYOUT  Es10x11
// 10 rows, 11 LED's per row + 4 LED's for minutes, with modified
// layout for the Spanish language
//
// #define DEFAULT_LAYOUT  Es08x08Cuarto
// 8 rows, each 8 LED's per row, only quarter hours
// layout for the Spanish language

/**********************/
/*      Hungarian     */
/**********************/
//
// #define DEFAULT_LAYOUT  Hu10x10
// 10 rows, 10 LED's per row + 4 LED's for minutes, with modified
// layout for the Hungarian language

/**********************/
/*    Swiss German    */
/**********************/
//
// #define DEFAULT_LAYOUT  Ch10x11
// 10 rows, 11 LED's per row + 4 LED's for minutes, with modified
// layout for the Swiss German language
//
// #define DEFAULT_LAYOUT  Ch10x11V2
// 10 rows, 11 LED's per row + 4 LED's for minutes, with modified
// layout for the Swiss German language with "Gsi" option

/**********************/
/*      Romanian      */
/**********************/
//
// #define DEFAULT_LAYOUT  Ro10x11
// 10 rows, 11 LED's per row + 4 LED's for minutes, with modified
// layout for the Romanian language

/**********************/
/*       French       */
/**********************/
//
// #define DEFAULT_LAYOUT  Fr10x11
// 10 rows, 11 LED's per row + 4 LED's for minutes, with modified
// layout for the French language

/**********************/
/*       Russian      */
/**********************/
//
// #define DEFAULT_LAYOUT  Ru10x11
// 10 rows, 11 LED's per row + 4 LED's for minutes, with modified
// layout for the Russian language

/**********************/
/*       Swedish      */
/**********************/
//
// #define DEFAULT_LAYOUT  Se10x11
// 10 rows, 11 LED's per row + 4 LED's for minutes, with modified
// layout for the Swedish language

//--------------------------------------------------------------------------
// Define LED Type
//--------------------------------------------------------------------------
/*
 * So far, WS2812 or SK6812 LED strips are supported. Different manufacturers of
 * these strips use different colour layouts, so you can adjust it accordingly
 * with DEFAULT_LEDTYPE. If you have an RGBW LED strip, you can also use
 * WHITE_LEDTYPE to specify the type of white LED used (WarmWhite (3000K),
 * NeutralWhite (4300K) or ColdWhite (6500K)), in the case of RGB just leave it
 * at NeutralWhite. With DEFAULT_HUE it is possible to define the hue value
 * of the default color scheme. Use https://colorizer.org to get a hue value
 * of your choice. DEFAULT_BRIGHTNESS defines the default brightness (percent)
 * of the LEDs, if no other value is defined yet.
 *
 * Valid values for DEFAULT_LEDTYPE [Brg, Grb, Rgb, Rbg, Gbr, Grbw]
 * Valid values for WHITE_LEDTYPE [WarmWhite, NeutralWhite, ColdWhite]
 * Valid values for DEFAULT_HUE [integer 0-255]
 * Valid values for DEFAULT_BRIGHTNESS [one of these: 0, 20, 40, 60, 80, 100]
 */
#define DEFAULT_LEDTYPE Brg
#define WHITE_LEDTYPE WhiteType::NeutralWhite
#define DEFAULT_HUE 120
#define DEFAULT_BRIGHTNESS 100

//--------------------------------------------------------------------------
// Define Build Type
//--------------------------------------------------------------------------
/*
 * There are several ways to build wordclocks:
 *
 * For small and mediumsized wordclocks, each LED is used in a strip, such as.
 *
 * [x x x x x x x x] = (Normal)
 *
 * Some variants of wordclocks (mostly midsized - 40x40 cm) rely on Ledstripes
 * with 74LEDs/m with one LED skipped for the Frontmatrix. It is build with 21
 * LEDs per line, for example:
 *
 * [x - x - x - x - x - x - x - x - x] = (DoubleResM1)
 *
 *
 * Valid values:
 * [BuildTypeDef::Normal, BuildTypeDef::DoubleResM1]
 */
#define DEFAULT_BUILDTYPE BuildTypeDef::Normal

//--------------------------------------------------------------------------
// Specify settings for automatic brightness control
//--------------------------------------------------------------------------
/*
 * The wordclock offers the option to automatically adjust the LED
 * brightness to the ambient light. Either an LDR or a BH1750 module is
 * used for this purpose. If both measurement methods are activated and the
 * corresponding hardware is available and functional, the BH1750 is the
 * preferred choice.
 *
 * By setting AUTOBRIGHT_USE_LDR or AUTOBRIGHT_USE_BH1750 to false, the
 * respective variant will be disabled and the program code on the
 * microcontroller can thus be reduced. If both modes are deactivated,
 * the option for automatic brightness control in the web interface is no
 * longer available.
 *
 * For the LDR variant, a voltage divider with an LDR (R1) and a resistor R2
 * is used. As LDR a type 5528 and as R2 a 10k resistor is recommended. In
 * case of a different configuration, it is necessary to adjust these
 * values accordingly:
 * AUTOBRIGHT_LDR_RESBRIGHT resistance of the LDR in bright environment
 *   (10 lux) in KΩ
 * AUTOBRIGHT_LDR_RESDARK resistance of the LDR in a dark environment
 *   (0 lux) in KΩ
 * AUTOBRIGHT_LDR_RESDIVIDER resistance of the divider resistor (R2) in KΩ
 *
 * Valid values:
 * AUTOBRIGHT_USE_BH1750 [true, false]
 * AUTOBRIGHT_USE_LDR [true, false]
 * AUTOBRIGHT_LDR_RESBRIGHT [number]
 * AUTOBRIGHT_LDR_RESDARK [number]
 * AUTOBRIGHT_LDR_RESDIVIDER [number]
 */
#define AUTOBRIGHT_USE_BH1750 true
#define AUTOBRIGHT_USE_LDR true

#define AUTOBRIGHT_LDR_RESBRIGHT 15
#define AUTOBRIGHT_LDR_RESDARK 1000
#define AUTOBRIGHT_LDR_RESDIVIDER 10

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
 * Valid values [0 ... 65536]
 */
#define SERNR 410
//--------------------------------------------------------------------------
// Toggle Serial DEBUG Output
//--------------------------------------------------------------------------
/*
 * Enable or disable debug messages.
 *
 * Valid values [true, false]
 */
#define GENERAL_VERBOSE true
#define WIFI_VERBOSE false
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
#define MANUAL_WIFI_SETTINGS false
#define WIFI_SSID "WIFISSID"
#define WIFI_PASSWORD "Password"

//--------------------------------------------------------------------------
// Captive portal settings
//--------------------------------------------------------------------------
/*
 * The captive portal is used to enter the WiFi credentials when
 * MANUAL_WIFI_SETTINGS is set to false. CP_SSID is used to set a custom
 * SSID for the captive portal. CP_PROTECTED allows you to enable password
 * protection for the captive portal. In this case, remember to change
 * CP_PASSWORD to a secure password.
 *
 * Valid values CP_PROTECTED [true, false]
 * Valid values CP_SSID [Alphanumeric Letters]
 * Valid values CP_PASSWORD [Alphanumeric Letters]
 *
 */
#define CP_PROTECTED false
#define CP_SSID "Connect_to_Wordclock"
#define CP_PASSWORD "CHANGE_THIS_PASSWORD"

//--------------------------------------------------------------------------
// Settings for Boot sequence
//--------------------------------------------------------------------------
/*
 * Show Ip Adress of clock on boot, with BOOT_SHOWIP
 * Light up LEDs on boot, with BOOT_LEDSWEEP
 *
 * Valid values BOOT_SHOWIP [true, false]
 * Valid values BOOT_LEDSWEEP [true, false]
 */
#define BOOT_SHOWIP true
#define BOOT_LEDSWEEP false

//--------------------------------------------------------------------------
// Displaying Option for Minutes
//--------------------------------------------------------------------------
/*
 * Valid values [only one #define option]
 */
// #define MINUTE_Off
#define MINUTE_LED4x
// #define MINUTE_LED7x

//--------------------------------------------------------------------------
// Front Displaying Options
//--------------------------------------------------------------------------
/*
 * Valid values REVERSE_MINUTE_DIR [true, false]
 * Valid values MIRROR_FRONT_VERTICAL [true, false]
 * Valid values MIRROR_FRONT_HORIZONTAL [true, false]
 * Valid values EXTRA_LED_PER_ROW [true, false]
 * Valid values FLIP_HORIZONTAL_VERTICAL [true, false]
 * Valid values MEANDER_ROWS [true, false]
 */
#define REVERSE_MINUTE_DIR false
#define MIRROR_FRONT_VERTICAL false
#define MIRROR_FRONT_HORIZONTAL false
#define EXTRA_LED_PER_ROW false
#define FLIP_HORIZONTAL_VERTICAL false
#define MEANDER_ROWS true
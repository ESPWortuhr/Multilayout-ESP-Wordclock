/*
 * Standard Wortuhr Konfiguration. Kann später in den Einstellungen auf der
 * Webseite geändert werden.
 */

// Layout der Frontplatte:
// - Ger10x11
//   10 Reihen, jeweils 11 LED's pro Reihe + 4 LED's für Minuten
// - Ger10x11Alternative
//   10 Reihen, jeweils 11 LED's pro Reihe + 4 LED's für Minuten, mit geändertem
//   Layout für extra Wörter in der Matrix
// - Ger10x11Clock
//   10 Reihen, jeweils 11 LED's pro Reihe + 4 LED's für Minuten, mit dem Layout
//   vom orginal Hersteller
// - Nl10x11
//   10 Reihen, jeweils 11 LED's pro Reihe + 4 LED's für Minuten, mit geändertem
//   Layout für die niederländische Sprache
// - Eng10x11
//   10 Reihen, jeweils 11 LED's pro Reihe + 4 LED's für Minuten, mit geändertem
//   Layout für die englische Sprache
// - Ger11x11
//   11 Reihen, jeweils 11 LED's pro Reihe + 4 LED's für Minuten
// - Ger10x11Frame
//   mit zusätzlichen LED's um den Rahmen seitlich zu beleuchten
// - Ger21x11Weather
//   Uhr mit Wettervorhersage 242 LED's
// - Ger17x17
//   Uhr mit 24 Stunden Anzeige 18x16
#define DEFAULT_LAYOUT Ger10x11Alternative

// Typ der LEDs:
// - Brg, Grb, Rgb, Rbg (WS2812b)
// - Grbw (SK6812)
#define DEFAULT_LEDTYPE Brg

// External Realtime Clock: RTC_DS1307, RTC_PCF8523 oder RTC_DS3231
#define RTC_Type RTC_DS3231

// um das eeprom zu löschen, bzw. zu initialisieren, hier eine andere
// Seriennummer eintragen!
#define SERNR 70

bool DEBUG = true; // DEBUG ON|OFF wenn auskommentiert
//#define VERBOSE          // DEBUG VERBOSE Openweathermap

#define MANUAL_WIFI_SETTINGS false
#define WIFI_SSID "Wifi-SSID"
#define WIFI_PASSWORD "Wifi-Pwd"

/*--------------------------------------------------------------------------
 * ENDE Hardware Konfiguration. Ab hier nichts mehr aendern!!!
 *--------------------------------------------------------------------------
 */

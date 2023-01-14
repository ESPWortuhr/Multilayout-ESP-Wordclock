/*
 * Standard Wortuhr Konfiguration. Kann später in den Einstellungen auf der
 * Webseite geändert werden.
 */

// Layout der Frontplatte:
// - Uhr_114
//   10 Reihen, jeweils 11 LED's pro Reihe + 4 LED's für Minuten
// - Uhr_114_Alternative
//   10 Reihen, jeweils 11 LED's pro Reihe + 4 LED's für Minuten, mit geändertem
//   Layout für extra Wörter in der Matrix
// - Uhr_114_2Clock
//   10 Reihen, jeweils 11 LED's pro Reihe + 4 LED's für Minuten, mit dem Layout
//   vom orginal Hersteller
// - Uhr_114_Dutch
//   10 Reihen, jeweils 11 LED's pro Reihe + 4 LED's für Minuten, mit geändertem
//   Layout für die niederländische Sprache
// - Uhr_125
//   11 Reihen, jeweils 11 LED's pro Reihe + 4 LED's für Minuten
// - Uhr_169
//   mit zusätzlichen LED's um den Rahmen seitlich zu beleuchten
// - Uhr_242
//   Uhr mit Wettervorhersage 242 LED's
// - Uhr_291
//   Uhr mit 24 Stunden Anzeige 18x16
#define DEFAULT_LAYOUT Uhr_114

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

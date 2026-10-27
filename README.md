![GitHub release (with filter)](https://img.shields.io/github/v/release/ESPWortuhr/Multilayout-ESP-Wordclock)
![GitHub all releases](https://img.shields.io/github/downloads/ESPWortuhr/Multilayout-ESP-Wordclock/total)
![Platform](https://img.shields.io/badge/platform-ESP8266%20%7C%20ESP32%20%7C%20ESP32--C3-blue)
![GitHub stars](https://img.shields.io/github/stars/ESPWortuhr/Multilayout-ESP-Wordclock)
![GitHub contributors](https://img.shields.io/github/contributors/ESPWortuhr/Multilayout-ESP-Wordclock)
![License](https://img.shields.io/github/license/ESPWortuhr/Multilayout-ESP-Wordclock)

[![PlatformIO CI](https://github.com/ESPWortuhr/Multilayout-ESP-Wordclock/actions/workflows/platformio.yml/badge.svg)](https://github.com/ESPWortuhr/Multilayout-ESP-Wordclock/actions/workflows/platformio.yml)
[![Nightly Firmware](https://github.com/ESPWortuhr/Multilayout-ESP-Wordclock/actions/workflows/nightly.yml/badge.svg)](https://github.com/ESPWortuhr/Multilayout-ESP-Wordclock/actions/workflows/nightly.yml)
[![clang-format](https://github.com/ESPWortuhr/Multilayout-ESP-Wordclock/actions/workflows/clang-format.yml/badge.svg)](https://github.com/ESPWortuhr/Multilayout-ESP-Wordclock/actions/workflows/clang-format.yml)

[🇬🇧 English](README.en.md) | 🇩🇪 Deutsch

# ESP Wordclock

<img src="pics/wortuhr-webpage-light.png" alt="Weboberfläche im hellen Modus, links am Desktop, rechts am Smartphone"/>
<img src="pics/wortuhr-webpage-dark.png" alt="Weboberfläche im dunklen Modus, links am Desktop, rechts am Smartphone"/>

Firmware für eine mehrsprachige Wortuhr mit ESP8266, ESP32 oder ESP32-C3 und einem LED-Streifen (WS2812 oder SK6812).

## Funktionen

- **15 Sprachen und Dialekte** mit über 30 Frontlayouts
- **Weboberfläche** im Liquid-Glass-Stil mit hellem und dunklem Modus, Seitenleiste am Desktop und Tab-Leiste am Smartphone, in sieben Sprachen
- **Farben:** Farbrad mit Helligkeit, Farbtemperatur und Favoriten. Farbmodi einfarbig, „Polychrom“ (Verlauf von Haupt- zu Zweitfarbe) und „Zufällig pro Wort“. RGB- und RGBW-LEDs
- **Modi:** Wortuhr, Digitaluhr, Laufschrift, Symbole, Farbe, Regenbogen, Feuer, Sekundenanzeige und Sekundenrahmen
- **Übergänge** zwischen den Minuten, z. B. Rollen, Überblenden, Laser, Matrix-Regen oder Feuerwerk
- **Helligkeit** nach Uhrzeit oder automatisch über LDR oder BH1750
- **Offline einstellen:** Über das Einrichtungsportal lassen sich Uhrzeit (vom Smartphone), Farbe, Frontlayout und Bauart ganz ohne Heimnetz einstellen, eine RTC hält die Zeit bei Stromausfall
- **Home Assistant** über MQTT mit Auto-Discovery
- **Updates** über die Weboberfläche ohne Kabel

<p align="center">
<img src="pics/ModeWordclock.png" alt="Wordclock" width="300"/> <img src="pics/ModeDigitalClock.gif" alt="DigitalClock" width="300"/>
<img src="pics/ModeColors.gif" alt="Colors" width="300"/> <img src="pics/ModeRainbow.gif" alt="Rainbow" width="300"/>
<img src="pics/ModeSymbol.gif" alt="Symbol" width="300"/> <img src="pics/ModeSeconds.gif" alt="Seconds" width="300"/>
<img src="pics/ModeScrollingText.gif" alt="ScrollingText" width="300"/> <img src="pics/Animation.gif" alt="Animation" width="300"/>
<img src="pics/SecondsInFrame.gif" alt="FrameSeconds" width="300"/>
</p>

## Schnellstart

1. Die passende Firmware von der [Releases-Seite](https://github.com/ESPWortuhr/Multilayout-ESP-Wordclock/releases) flashen oder selbst bauen.
2. Mit dem WLAN `Connect_to_Wordclock` verbinden und im Portal das Heim-WLAN eintragen, oder dort ohne WLAN die Uhrzeit, Farbe und Front einstellen.
3. Die Weboberfläche unter der angezeigten IP-Adresse öffnen.

## Dokumentation

Alles Weitere steht im **[Wiki](https://github.com/ESPWortuhr/Multilayout-ESP-Wordclock/wiki)**:

| | |
| :--- | :--- |
| [Installation](https://github.com/ESPWortuhr/Multilayout-ESP-Wordclock/wiki/Installation) | Selbst bauen, fertige Firmware flashen, Updates, Nightly-Builds |
| [Einrichtung](https://github.com/ESPWortuhr/Multilayout-ESP-Wordclock/wiki/Einrichtung) | Erster Start, Offline-Einrichtung über das Portal, Zurücksetzen |
| [Hardware](https://github.com/ESPWortuhr/Multilayout-ESP-Wordclock/wiki/Hardware) | Teile, Aufbau, Verkabelung, Pins, Taster, RTC, Sensoren |
| [Weboberfläche](https://github.com/ESPWortuhr/Multilayout-ESP-Wordclock/wiki/Weboberflaeche) | Alle Funktionen und Einstellungen |
| [Frontlayouts](https://github.com/ESPWortuhr/Multilayout-ESP-Wordclock/wiki/Frontlayouts) | Sprachen, Layouts, Bauarten |
| [Konfiguration](https://github.com/ESPWortuhr/Multilayout-ESP-Wordclock/wiki/Konfiguration) | Einstellungen in `include/Config.h` |
| [MQTT und Home Assistant](https://github.com/ESPWortuhr/Multilayout-ESP-Wordclock/wiki/MQTT-und-Home-Assistant) | Topics, Befehle, Entitäten |
| [Fehlersuche](https://github.com/ESPWortuhr/Multilayout-ESP-Wordclock/wiki/Fehlersuche) | Häufige Probleme |
| [Entwicklung](https://github.com/ESPWortuhr/Multilayout-ESP-Wordclock/wiki/Entwicklung) | Mitwirken, neue Frontlayouts |

Änderungen je Version stehen im [Changelog](CHANGELOG.md).

## Lizenz

BSD-3: Die Software darf frei verwendet, verändert und weitergegeben werden, solange der Copyright-Hinweis erhalten bleibt. Details in [LICENSE](LICENSE).

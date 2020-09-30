# Wortuhr - Version 2.3.0

Hier findet man alle nötigen Informationen zur Worduhr.

![](pics/wortuhr-3-screenshot.png)

### Einleitung 
Es handelt sich hier um die Version 3 der Wortuhr (Wordclock). Aufbauend auf dem Projekt von [Ulrich Radig](https://web.archive.org/web/20180422160812/http://www.ulrichradig.de/home/index.php/projekte/wort-uhr-neu) (Stand 2019, die Website ist mittlerweile offline).

## Für den Nachbau wird folgende Hardware/Software benötigt:
* Hardware
    * NodeMCU (getestetes Board Lolin Version3) oder vergleichbares Board mit einem ESP8266 
    * WS2812B, WS2816 RGB LED Stripes oder SK6812 RGBW Stribes
    * Netzteil 5V 2A
    * 470 Ohm Widerstand
    * Optional: LDR, 10 KOhm Widerstand
* Software
    * Arduio IDE oder Platformio IDE

## Anschluss WS2812 oder SK6812 Stripe
* RX = Data Bus
* GND = Ground
* VV = 5V
* Pegelanpassung 3,3V zu 5V siehe docs

## Anschluss LDR (optional)
![](pics/old/LDR.jpg)
      
## Verzeichnisstruktur

-   `pics` enthält Bilder
-   `svg` enthält SVG Dateien
-   `src` enthält den Sketch für Arduino und das Webinterface
-   `include` enthält die Header-Dateien für Arduino
-   `docs` enthält Anleitungen
-   `bin` enthält die Binary (Für die Uhrenvariante *UHR_114*)

## Arduino IDE vorbereiten
* Datei \> Voreinstellungen \> Zusätzliche Boardverwalter-URLs \> "http://arduino.esp8266.com/stable/package_esp8266com_index.json"
* Werkzeuge \>  Board \> Boardverwalter \> esp8266 Board installieren
    * Weitere Informationen sind auf der [Github Seite des Entwicklers zu finden](https://github.com/esp8266/Arduino)
* Unter Datei \> Sketch \> Include Libraries folgende Bibliotheken hinzufügen:
    * '[NeoPixelBus](https://github.com/Makuna/NeoPixelBus)' von Mankuna
    * '[Time](https://github.com/PaulStoffregen/Time)' von PaulStoffregen
    * '[ArduinoJson](https://github.com/bblanchon/ArduinoJson)' von BBlanchon (Version 5.13.4)
    * '[Timezone](https://github.com/JChristensen/Timezone)' von JChristensen
    * '[WebSockets](https://www.arduino.cc/reference/en/libraries/websockets/)' von Arduino
    * '[NTPClient](https://github.com/arduino-libraries/NTPClient)' von arduino-libraries
    * '[RTClib](https://github.com/adafruit/RTClib)' von adafruit
    
## To DO & (Ideen):
* Symbol zu einem bestimmten Datum/Uhrzeit
* Getrennte Helligkeitsregelung Hintergrundbeleuchtung/Anzeige 
* "The Matrix" effekt
* Minten Effekte
* Silvester Countdown
* HomeKit Support
* Alexa/GoogleHome Support
* HSI Farb Selektor auf der Website
* Finaler Release wäre eine unified Binary mit selektor auf der Webseite

## ChangeLog
### Version 2.3.0 WORK_IN_PROGRESS
* (dbambus)
* Aufräumen der Uhr.h für die Übersichtlichkeit
* Restrukturierung der Worduhr.ino um IDE Clang gerecht zu werden
* Weiteres Entfernen von nicht benötigten #IfDefs im Code
* Weitere Enums und Structs für UHR_Typen hinzugefügt
* Umstellung der NTP Funktion der Uhr auf eine externe Library NTPClient
* Support von RTCs zum Beispiel `DS1307`, `DS3231` und `PCF8523`
### Geplant
* Hinzufügen eine OfflineModus durch Abschalten der WLAN Kommunikation.
* Support von ArduinoJson Version 6.xx
### Version 2.2.2
* (dbambus)
* Aufräumen der Uhr_func.h für die Übersichtlichkeit
* Entfernen der nicht relevanten IfDefs in Uhr_func.h für bessere Lesbarkeit
* Habe die Commits von Wandmalffarbes Branche hinzugefügt (Website betreffend)
* Fixes für ein Großteil der Compiler-Warnungen 
* Umstellung auf eine neue HSV-to-RGB Conversion
* Entfernen der Websockets Header Dateien aus dem Repo, hier ein neuer Verweis auf neue Libs in der Readme
* Ordnerstruktur angepasst für cross-kompatiblität Platformio/Arduino
### Version 2.2.1
* (Eisbaeeer)
* Bugfix Openweathermap (freeze without Wifi connection)
* Enhanced DEBUG uhr_242
* Correction global vars Openweathermap json parser
### Version 2.2.0
* (dbambus)
* Bugfixes für Kohttps://www.arduino.cc/reference/en/libraries/websockets/nfigseite #7
* Support für SK6812 RGBW Stribes
* Hinzufügen eines neunen Layouts UHR_114_Fränkisch     
### Version 2.1.1
* (Pascal Wagler)
* Neues Webinterface hinzugefügt
* Arduino-Projekt umbenannt
* Alten `lib`-Ordner und die Datei `7x5 Font.xlsx` gelöscht
* (Atho95)
* Neueste Version der WebSockets - Bugfix Nullpointer in Debug-Ausgabe (payload)
     Im Directory Uhr_Neu befindet sich noch die alte Version (könnte gelöscht werden)
* Buffer für IP Adressen auf 20 erhöht (sprintf hängt '\0' an)
* uhr_func_114.h erweitert auf umgekehrt angeschlossene LED's
* uhr.h erweitert, um auch RGB LED's konfigurieren zu können
### Version 2.1.0
* (Eisbaeeer)
* Bugfix null pointer
* Code weiter aufgeräumt
* Minuten arrays gefixt
* Neue Uhr Version 242 (Open Weather Map)
### Version 2.0.12
* (Elektron79)
* Bugfix Passwort speichern
### Version 2.0.11
* (Eisbaeeer)
* Bugfix issue #3 (Leerstelle in SSID)
### Version 2.0.10
* (masju)
* Erweiterung Randbeleuchtung ("Ambilight") für die 114-LED-Version
### Version 2.0.9
* (Eisbaeeer)
* Telnet Server für Debugging
### Version 2.0.8
* (Flo455)
* WLAN Scan hinzugefügt
### Version 2.0.7
* (Eisbaeeer)
* LDR für Hintergrundhelligkeit
### Version 2.0.6
* (path83 & masju & Eisbaeeer)
* Over The Air Update Link in Webconfig
### Version 2.0.5
* (Eisbaeeer)
* IP Adresse per Laufschrift ausgeben
* LDR Kalibrierung über WebConfig
### Version 2.0.4
* (path83 & Eisbaeeer)
* LDR für automatische Helligkeitsregelung
### Version 2.0.3
* (Eisbaeeer)
* Neue Variante mit 11x11 Reihen LED´s hinzugefügt
### Version 2.0.2
* (Eisbaeeer)
* fix NTP DNS lookup (falsche IP im DNS lookup)
### Version 2.0.1
* (Markus Aust)
* Minuten LED´s
* (Eisbaeeer)
* Versionierung
### Version 2.0.0 
* (Ulrich Radig)
* inital version   

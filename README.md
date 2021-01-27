# WortUhr

Hier findet man alle nötigen Informationen zur Worduhr.

![](pics/wortuhr-3-screenshot.png)

## Für den Nachbau wird folgende Hardware/Software benötigt:
* Hardware
    * NodeMCU (getestetes Board Lolin Version3) oder vergleichbares Board mit einem ESP8266 
    * WS2812B, WS2816 RGB LED Stripes oder SK6812 RGBW Stribes
    * Netzteil 5V 2A
    * 470 Ohm Widerstand
    * Optional: LDR, 10 KOhm Widerstand
* Software
    * PlatformIO Core oder IDE

## Anschluss WS2812 oder SK6812 Stripe
* RX = DI / Data Bus
* G = GND / Masse
* VU = 5V
* Pegelanpassung 3,3V zu 5V siehe docs

## Anschluss LDR (optional)
![](pics/old/LDR.png)
      
## Verzeichnisstruktur

-   `pics` enthält Bilder
-   `svg` enthält SVG Dateien
-   `include` enthält C Header Dateien
-   `src` enthält C Source Dateien
-   `webpage` enthält das Webinterface
-   `docs` enthält Anleitungen
-   `bin` enthält die Binary (Für die Uhrenvariante *UHR_114*)

## To DO & (Ideen):
* Symbol zu einem bestimmten Datum/Uhrzeit
* Getrennte Helligkeitsregelung Hintergrundbeleuchtung/Anzeige 
* "The Matrix" effekt
* Minten Effekte
* HomeKit Support
* Alexa/GoogleHome Support
* HSI Farb Selektor auf der Website

## Known Bugs

* Bei > 60 Sekunden in der Sekundenanzeige wird die Uhrzeit nach Wechsel erst zur nächsten vollen Minute aktualisiert. 

### BSD-3 Lizenz

Diese Software steht unter der BSD-Lizenz und darf frei verwendet werden. Es ist erlaubt, sie zu kopieren, zu verändern und zu verbreiten.
Einzige Bedingung ist, dass der Copyright-Vermerk des ursprünglichen Programms nicht entfernt werden darf.

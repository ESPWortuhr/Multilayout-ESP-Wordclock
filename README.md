# ESP Wortuhr

English and Dutch description further below.

Dies ist ein Projekt für eine mehrsprachige Wortuhr, basierend auf einem ESP8266 Mikrocontroller und einem programmierbaren LED-Streifen (WS2812 oder SK6812). Eine Wortuhr ist ein einzigartiges Projekt, das Technologie und Design kombiniert, um einen funktionalen und ästhetisch ansprechenden Zeitmesser zu schaffen. Mit der Möglichkeit, die Zeit in mehreren Sprachen anzuzeigen, ist diese Wortuhr nicht nur ein Gesprächsthema, sondern auch eine praktische Lösung für alle, die nach einer modernen und innovativen Möglichkeit suchen, die Zeit im Auge zu behalten.
Egal, ob du ein Anfänger oder ein erfahrener Bastler bist, dieses Projekt ist eine großartige Möglichkeit, deine Fähigkeiten auf die Probe zu stellen und etwas wirklich Besonderes zu schaffen.

![](pics/wortuhr-webpage.png)

## Die folgende Hardware/Software wird für dieses Projekt benötigt:
* Hardware
    * NodeMCU (getestetes Board Lolin Version3 und Wemos D3 mini) oder vergleichbares Board mit einem ESP8266 
    * WS2812B, WS2816 RGB-LED-Streifen oder SK6812 RGBW-Streifen
    * Stromversorgung 5V 2A
    * Optional: LDR, 10 KOhm-Widerstand
* Software
    * PlatformIO [Core](https://docs.platformio.org/en/latest/core/installation.html) oder [IDE](https://platformio.org/install/ide?install=vscode)
    * [Node.js](https://www.nodejs.org/)
    * [Git](https://git-scm.com)

## Installation 
### Windows

* Installieren Sie PlatformIO IDE, Node.js und Git manuell über die oben genannten Links.
* Dadurch wird Visual Studio Code installiert, mit einem PlatformIO-Symbol (Ameisenkopf/Alien) in der Seitenleiste.
* Gehen Sie zu 'Quick Access / Miscellaneous' und geben Sie den Befehl 'Clone Git Project' ein, und geben Sie 'https://github.com/ESPWortuhr/Wortuhr' als URL ein.
* Gehen Sie dann zu 'Projekte', fügen Sie das neue Projekt mit 'Vorhandenes hinzufügen' zur Liste hinzu und klicken Sie auf 'Öffnen'.
* In der PlatformIO-Seitenleiste erscheint nun 'Project Tasks'. Wählen Sie den Befehl 'General / Upload' (dauert ein paar Minuten, die Software wird zuerst erstellt).
* Schließen Sie den ESP-8266 über USB an. Wenn die Wortuhr-Software erstellt ist, wird sie auf dem ESP installiert.

### MacOS

Der einfachste Weg wäre mit [homebrew](https://docs.brew.sh/Installation):

```sh
brew install platformio
brew install node
git clone https://github.com/ESPWortuhr/Wortuhr
cd Wortuhr
pio run -t upload
```

### Linux

```sh
python3 -c "$(curl -fsSL https://raw.githubusercontent.com/platformio/platformio/master/scripts/get-platformio.py)"
sudo apt install npm
git clone https://github.com/ESPWortuhr/Wortuhr
cd Wortuhr
pio run -t upload
```

## Anschließen des WS2812/ SK6812 Stripe
* RX = DI / Datenbus
* G = GND / Masse
* VU = 5V
* Pegelanpassung 3.3V bis 5V siehe Doku

## Anschluss für den LDR (optional)
![](pics/old/LDR.png)
      
## Verzeichnisstruktur

- `pics` enthält Bilder
- `svg` enthält SVG-Dateien
- `include` enthält C-Header-Dateien
- `src` enthält C-Quelldateien
- `webpage` enthält das Webinterface
- `docs` enthält Handbücher
- `templates` enthält Fräs- und Laserdaten

## To DO & (Ideen):
* HomeKit Unterstützung
* Home Assistant Unterstützung

### BSD-3 Lizenz

Diese Software ist unter der BSD-Lizenz lizenziert und darf frei verwendet werden. Es ist erlaubt, sie zu kopieren, zu verändern und zu verbreiten.
Die einzige Bedingung ist, dass der Copyright-Hinweis des Originalprogramms nicht entfernt werden darf.

## ENGLISH Textversion

This is a project for a multilingual wordclock based on an ESP8266 microcontroller and a programmable LED stripe (WS2812 or SK6812). A wordclock is a unique project that combines technology and design to create a functional and aesthetically pleasing timepiece. With the ability to display the time in multiple languages, this wordclock is not only a conversation piece, but also a practical solution for anyone looking for a modern and innovative way to keep track of time.
Whether you're a beginner or an experienced maker, this project is a great way to put your skills to the test and create something truly special.

![](pics/wortuhr-webpage.png)

## The following hardware/software are required for this project:
* Hardware
    * NodeMCU (tested board Lolin Version3 and Wemos D3 mini) or comparable board with an ESP8266 
    * WS2812B, WS2816 RGB LED Stripes or SK6812 RGBW Stribes
    * Power supply 5V 2A
    * Optional: LDR, 10 KOhm resistor
* Software
    * PlatformIO [Core](https://docs.platformio.org/en/latest/core/installation.html) or [IDE](https://platformio.org/install/ide?install=vscode)
    * [Node.js](https://www.nodejs.org/)
    * [Git](https://git-scm.com)

## Installation 
### Windows

* Manually install PlatformIO IDE, Node.js and Git using the links above.
* This will install Visual Studio Code, with a PlatformIO icon (ant head/alien) in the sidebar.
* Go to 'Quick Access / Miscellaneous' and enter the command 'Clone Git Project', and enter 'https://github.com/ESPWortuhr/Wortuhr' as the URL.
* Then go to 'Projects', add the new project to the list by 'Add Existing' and click on 'Open'.
* Then in the PlatformIO sidebar 'Project Tasks' appears. Select the command 'General / Upload' (takes a few minutes, the software will be built first).
* Connect the ESP-8266 via USB. When the word clock software is built, it will be installed on the ESP.

### MacOS

The easiest way would be with [homebrew](https://docs.brew.sh/Installation):

```sh
brew install platformio
brew install node
git clone https://github.com/ESPWortuhr/Wortuhr
cd Wortuhr
pio run -t upload
```

### Linux

```sh
python3 -c "$(curl -fsSL https://raw.githubusercontent.com/platformio/platformio/master/scripts/get-platformio.py)"
sudo apt install npm
git clone https://github.com/ESPWortuhr/Wortuhr
cd Wortuhr
pio run -t upload
```

## Connecting the WS2812/ SK6812 Stripe
* RX = DI / Data Bus
* G = GND / Masse
* VU = 5V
* Level adjustment 3.3V to 5V see docs

## Connection for the LDR (optional)
![](pics/old/LDR.png)
      
## directory structure

- `pics` contains images
- `svg` contains SVG files
- `include` contains C header files
- `src` contains C source files
- `webpage` contains the web interface
- `docs` contains manuals
- `templates` contains milling and laser data

## To DO & (Ideen):
* HomeKit Support
* Home Assistant Support

### BSD-3 License

This software is licensed under the BSD license and may be used freely. It is allowed to copy, modify and distribute it.
The only condition is that the copyright notice of the original program must not be removed.

## Nederlandse versie

Dit is een project voor een meertalige Woordklok op basis van een ESP8266 microcontroller en een programmeerbare LED-strip (WS2812 of SK6812). Een Woordklok is een uniek project dat technologie en design combineert om een functioneel en esthetisch uurwerk te creëren. Met de mogelijkheid om de tijd in meerdere talen weer te geven, is deze Woordklok niet alleen een conversatiestuk, maar ook een praktische oplossing voor iedereen die op zoek is naar een moderne en innovatieve manier om de tijd bij te houden.
Of je nu een beginner of een ervaren maker bent, dit project is een geweldige manier om je vaardigheden op de proef te stellen en iets heel bijzonders te creëren.

![](pics/wortuhr-webpage.png)

## De volgende hardware/software is vereist voor dit project:
* Apparatuur
     * NodeMCU (getest board Lolin Version3 en Wemos D3 mini) of vergelijkbaar board met een ESP8266
     * WS2812B, WS2816 RGB LED-strips of SK6812 RGBW-strips
     * Voeding 5V 2A
     * Optioneel: LDR, 10 KOhm weerstand
* Software
     * PlatformIO [Core](https://docs.platformio.org/en/latest/core/installation.html) of [IDE](https://platformio.org/install/ide?install=vscode)
     * [Node.js](https://www.nodejs.org/)
     * [Git](https://git-scm.com)

## Installatie
### Windows

* Installeer PlatformIO IDE, Node.js en Git handmatig via de bovenstaande links.
* Hiermee wordt Visual Studio Code geïnstalleerd, met een PlatformIO-pictogram (mierhoofd/alien) in de zijbalk.
* Ga naar 'Quick Access / Miscellaneous' en voer het commando 'Clone Git Project' in, en voer 'https://github.com/ESPWortuhr/Wortuhr' in als de URL.
* Ga vervolgens naar 'Projecten', voeg het nieuwe project toe aan de lijst door 'Bestaand toevoegen' en klik op 'Openen'.
* Vervolgens verschijnt in de PlatformIO-zijbalk 'Projecttaken'. Selecteer het commando 'Algemeen / Upload' (duurt enkele minuten, de software wordt eerst gebouwd).
* Sluit de ESP-8266 aan via USB. Wanneer de wordclock-software is gebouwd, wordt deze op de ESP geïnstalleerd.

### MacOS

De makkelijkste manier is met [homebrew](https://docs.brew.sh/Installation):

```sh
brew install platformio
brew install node
git clone https://github.com/ESPWortuhr/Wortuhr
cd Wortuhr
pio run -t upload
```

### Linux

```sh
python3 -c "$(curl -fsSL https://raw.githubusercontent.com/platformio/platformio/master/scripts/get-platformio.py)"
sudo apt install npm
git clone https://github.com/ESPWortuhr/Wortuhr
cd Wortuhr
pio run -t upload
```

## Verbinden van de WS2812/ SK6812 Stripe
* RX = DI / Data 
* G = GND / Massa
* VU = 5V
* Niveauregeling 3.3V tot 5V zie docs

## Verbinden van de LDR (optioneel)
![](pics/old/LDR.png)
      
## mappenstructuur

- `pics` bevat afbeeldingen
- `svg` bevat SVG-bestanden
- `include` bevat C-headerbestanden
- `src` bevat C-bronbestanden
- `webpagina` bevat de webinterface
- `docs` bevat handleidingen
- `Templates` bevat frees- en lasergegevens

## To DO & (Ideen):
* HomeKit Support
* Home Assistant Support

### BSD-3 License

Deze software is gelicentieerd onder de BSD-licentie en mag vrij worden gebruikt. Het is toegestaan deze te kopiëren, aan te passen en te verspreiden.
De enige voorwaarde is dat de copyrightvermelding van het originele programma niet mag worden verwijderd.

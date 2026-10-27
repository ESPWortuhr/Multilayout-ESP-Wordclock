![GitHub release (with filter)](https://img.shields.io/github/v/release/ESPWortuhr/Multilayout-ESP-Wordclock)
![GitHub all releases](https://img.shields.io/github/downloads/ESPWortuhr/Multilayout-ESP-Wordclock/total)
![Platform](https://img.shields.io/badge/platform-ESP8266%20%7C%20ESP32%20%7C%20ESP32--C3-blue)
![GitHub stars](https://img.shields.io/github/stars/ESPWortuhr/Multilayout-ESP-Wordclock)
![GitHub contributors](https://img.shields.io/github/contributors/ESPWortuhr/Multilayout-ESP-Wordclock)
![License](https://img.shields.io/github/license/ESPWortuhr/Multilayout-ESP-Wordclock)

[![PlatformIO CI](https://github.com/ESPWortuhr/Multilayout-ESP-Wordclock/actions/workflows/platformio.yml/badge.svg)](https://github.com/ESPWortuhr/Multilayout-ESP-Wordclock/actions/workflows/platformio.yml)
[![Nightly Firmware](https://github.com/ESPWortuhr/Multilayout-ESP-Wordclock/actions/workflows/nightly.yml/badge.svg)](https://github.com/ESPWortuhr/Multilayout-ESP-Wordclock/actions/workflows/nightly.yml)
[![clang-format](https://github.com/ESPWortuhr/Multilayout-ESP-Wordclock/actions/workflows/clang-format.yml/badge.svg)](https://github.com/ESPWortuhr/Multilayout-ESP-Wordclock/actions/workflows/clang-format.yml)

🇬🇧 English | [🇩🇪 Deutsch](README.md)

# ESP Wordclock

<img src="pics/wortuhr-webpage-light.png" alt="Web interface in light mode, desktop on the left, phone on the right"/>
<img src="pics/wortuhr-webpage-dark.png" alt="Web interface in dark mode, desktop on the left, phone on the right"/>

Firmware for a multilingual word clock with an ESP8266, ESP32 or ESP32-C3 and an LED strip (WS2812 or SK6812).

## Features

- **15 languages and dialects** with more than 30 front layouts, see [Supported languages](#supported-languages)
- **Web interface** in a liquid glass look with light and dark mode, a sidebar on desktop and a tab bar on phones, in seven languages
- **Colours:** colour wheel with brightness, colour temperature and favourites. Colour modes single colour, "Polychrome" (gradient from a primary to a secondary colour) and "Random per word". RGB and RGBW LEDs
- **Modes:** word clock, digital clock, scrolling text, symbols, colour, rainbow, fire, seconds and seconds frame
- **Transitions** between minutes, e.g. roll, fade, laser, matrix rain or fire
- **Brightness** by time of day or automatically via LDR or BH1750
- **Offline setup:** the setup portal sets the time (from your phone), colour, front layout and build type without any home network, an RTC keeps the time through power cuts
- **Home Assistant** via MQTT with auto-discovery, the connection status is shown in the web interface
- **Updates** through the web interface without a cable

## Table of contents

- [Supported languages](#supported-languages)
- [Operating modes](#operating-modes)
- [Hardware](#hardware)
- [Installation](#installation)
- [First start and setup](#first-start-and-setup)
  - [Offline setup through the setup portal](#offline-setup-through-the-setup-portal)
- [Configuration](#configuration)
- [MQTT and Home Assistant](#mqtt-and-home-assistant)
- [Contributing](#contributing)
- [License](#license)

## Supported languages

The language follows from the front layout. All layouts are part of the firmware and can be switched in the web interface under **Display → Front**.

| | Language | Native name | Layouts (rows × columns) |
|---|---|---|---|
| 🇩🇪 | German | Deutsch | 10 × 11 (standard, alternative, clock, Nero, mrrioes), 11 × 11 (standard, version 2, panbachi), 13 × 13, 16 × 8, 16 × 18, 22 × 11 weather, 8 × 8, 8 × 8 quarter hours |
| 🇩🇪 | Bavarian | Boarisch | 10 × 11 |
| 🇩🇪 | Swabian | Schwäbisch | 10 × 11, 11 × 11 |
| 🇬🇧 | English | English | 10 × 11, 11 × 12, 8 × 8 |
| 🇳🇱 | Dutch | Nederlands | 10 × 11 |
| 🇫🇷 | French | Français | 10 × 11 |
| 🇪🇸 | Spanish | Español | 10 × 11, 8 × 8 quarter hours |
| 🇮🇹 | Italian | Italiano | 10 × 11 |
| 🇨🇭 | Swiss German | Schwiizerdütsch | 10 × 11, 10 × 11 version 2, 10 × 11 Züridütsch (Zurich) |
| 🇸🇪 | Swedish | Svenska | 10 × 11 |
| 🇭🇺 | Hungarian | Magyar | 10 × 10 |
| 🇷🇴 | Romanian | Română | 10 × 11 |
| 🇷🇺 | Russian | Русский | 10 × 11 |
| 🇹🇷 | Turkish | Türkçe | 10 × 11 |
| 🇧🇩 | Bengali | বাংলা | 9 × 8 |

## Operating modes

<p align="center">
<img src="pics/ModeWordclock.png" alt="Wordclock" width="300"/> <img src="pics/ModeDigitalClock.gif" alt="DigitalClock" width="300"/>
<img src="pics/ModeColors.gif" alt="Colors" width="300"/> <img src="pics/ModeRainbow.gif" alt="Rainbow" width="300"/>
<img src="pics/ModeSymbol.gif" alt="Symbol" width="300"/> <img src="pics/ModeSeconds.gif" alt="Seconds" width="300"/>
<img src="pics/ModeScrollingText.gif" alt="ScrollingText" width="300"/> <img src="pics/Animation.gif" alt="Animation" width="300"/>
<img src="pics/SecondsInFrame.gif" alt="FrameSeconds" width="300"/>
</p>

## Hardware

- Board with an ESP8266, ESP8285, ESP32 or ESP32-C3, e.g. NodeMCU or Wemos D1 Mini
- LED strip WS2812B (RGB) or SK6812 (RGBW)
- 5 V power supply. With many letters lit in white the matrix draws several amperes, 3 to 5 A is recommended
- Optional: RTC (DS3231 or DS1307), light sensor (LDR with a 10 kΩ resistor or BH1750), level shifter such as a 74AHCT125 for a clean 5 V data signal

![Schematic](pics/Schematic.png)

The +5 V and GND of the power supply go to the ESP and to the LED strip, the ESP's data pin goes to DIN of the first LED. The strips are usually laid out in a zigzag: DOUT of one row goes to DIN of the next row on the same side.

**Layers from front to back:** front panel with the letters, diffuser (frosted film or tracing paper), light grid that separates every LED from its neighbours, and back panel with the LED strips.

| Front | Matrix | LED strip | Note |
| :--- | :--- | :--- | :--- |
| 30 × 30 cm | 25 × 25 cm | 60 LEDs/m | Standard size, every LED is used |
| 40 × 40 cm | 35 × 35 cm | 74 LEDs/m | Every second LED of a row stays unused |
| 50 × 50 cm | 50 × 50 cm | 30 LEDs/m | The LED spacing matches the grid directly |

## Installation

USB boards such as NodeMCU or Wemos D1 Mini usually need a **CH340** or **CP210x** driver. Use a data cable for flashing, not a charge-only cable.

### Build it yourself

This is the only way to adapt the clock to your layout, language and hardware via `include/Config.h`. You need [PlatformIO](https://platformio.org/install), [Node.js](https://www.nodejs.org/) and [Git](https://git-scm.com).

```sh
git clone https://github.com/ESPWortuhr/Multilayout-ESP-Wordclock.git
cd Multilayout-ESP-Wordclock
pio run -e ESP8266 -t upload   # or -e ESP32 / -e ESP32C3
```

- **Windows:** install PlatformIO IDE for VS Code, open the project via **Clone Git Project** and flash under **Project Tasks → General → Upload**.
- **macOS:** `brew install platformio node`
- **Linux:** install PlatformIO with the [installer script](https://docs.platformio.org/en/latest/core/installation/methods/installer-script.html), plus `sudo apt install npm git`. Add your user to the `dialout` group to access the serial port.

The first build takes a few minutes because PlatformIO downloads all libraries.

### Prebuilt firmware

The [releases page](https://github.com/ESPWortuhr/Multilayout-ESP-Wordclock/releases) has one file per chip, e.g. `ESP8266_V5.0.0.bin`, plus `SHA256SUMS`. It contains the default configuration: German layout `Ger10x11Alternative`, GRB LEDs, DS3231 RTC. If you need anything else, build it yourself.

- **Update over WiFi:** open `http://<clock-IP>:81/update` (port 81) and upload the `.bin`. Your settings are kept.
- **Serial flashing** with `esptool` (`pip install esptool`):
  ```sh
  esptool.py --chip esp8266 --port /dev/ttyUSB0 write_flash 0x0 ESP8266_V5.0.0.bin
  esptool.py --chip esp32 --port /dev/ttyUSB0 write_flash 0x10000 ESP32_V5.0.0.bin
  ```
- **In the browser** with [esptool-js](https://espressif.github.io/esptool-js/) or [esp.huhn.me](https://esp.huhn.me/), same offsets. This only works in Chrome, Edge or Opera.

On the ESP32 and ESP32-C3 the offset `0x10000` writes the application only. A completely blank chip also needs the bootloader and the partition table. Flash it once with `pio run -t upload` for that.

**Nightly builds:** for every commit on `main`, [Actions → Nightly Firmware](https://github.com/ESPWortuhr/Multilayout-ESP-Wordclock/actions/workflows/nightly.yml) has ready-made files for all three chips. Downloading requires a GitHub login, the files are deleted after 14 days. These are untested development builds.

## First start and setup

1. The clock opens the WiFi network `Connect_to_Wordclock`. Connect to it.
2. The setup portal opens by itself, otherwise go to `http://192.168.4.1`.
3. Enter your home WiFi. The clock restarts and is then reachable at its IP address.

### Offline setup through the setup portal

The clock can be put into operation and set up in its basics without any home network, using just your phone:

1. Connect to the WiFi network `Connect_to_Wordclock`. The clock immediately takes the time from your phone and sets an installed RTC as well.
2. Open **Clock** in the portal. There you can set:
   - foreground colour
   - clock variant (front layout)
   - the six layout switches
   - matrix build type
   - the time by hand, if taking it over automatically does not work
3. Save with **Save settings**. A changed matrix size only applies after a restart.
4. **Continue without WiFi** leaves the portal, the clock now shows colour and time.

- Without an RTC the time is lost on a power cut and has to be taken over again through the portal.
- With an RTC the portal closes by itself after 120 seconds without input and the clock runs on the RTC time.

### Portal, RTC and I2C

**Opening the portal again:** cut the power three times in a row, each time before the clock has finished starting. On the **sixth** start in a row all settings except the WiFi credentials are reset.

**RTC HW-111 (DS1307):** set `RTC_Type` to `RTC_DS1307` in `include/Config.h` and build it yourself. With a regular CR2032 instead of an LIR2032 the charging circuit has to be removed: desolder D1, R4, R5 and R6 and bridge the pads of R6 ([Arduino forum](https://forum.arduino.cc/t/tiny-rtc-i2c-module-issue/173162/22)).

**I2C pins** for the RTC and BH1750 are entered in the web interface under **Settings → Hardware pins**. **Scan I2C addresses** shows what is on the bus: DS3231 and DS1307 at `0x68`, BH1750 at `0x23` or `0x5C`.

## Configuration

The defaults live in `include/Config.h`. Almost everything can be changed later in the web interface.

| Setting | Meaning | Default |
| :--- | :--- | :--- |
| `DEFAULT_LAYOUT` | Front layout and therefore language | `Ger10x11Alternative` |
| `DEFAULT_LEDTYPE` | Colour order of the LED strip | `Grb` |
| `WHITE_LEDTYPE` | Colour temperature of the white LED on RGBW | `NeutralWhite` |
| `DEFAULT_HUE` / `DEFAULT_BRIGHTNESS` | Hue (0–360) and brightness in % on first start | `120` / `100` |
| `DEFAULT_BUILDTYPE` | Matrix build type | `Normal` |
| `LED_PIN` | GPIO for the LED data | `3` (RX) |
| `SDA_PIN_…` / `SCL_PIN_…` | I2C pins, `255` = off | ESP8266: `D2` / `D1` |
| `RTC_Type` | RTC module | `RTC_DS3231` |
| `AUTOBRIGHT_LDR_…` | Resistor values for the LDR voltage divider | `15`, `1000`, `10` |
| `MINUTE_…` | Minute display type | `MINUTE_LED4x` |
| `MANUAL_WIFI_SETTINGS`, `WIFI_SSID`, `WIFI_PASSWORD` | Compile in fixed WiFi credentials | off |
| `CP_PROTECTED`, `CP_SSID`, `CP_PASSWORD` | Setup portal | open, `Connect_to_Wordclock` |
| `WEB_PROTECTED`, `WEB_USER`, `WEB_PASSWORD` | Password for the update port 81 | off |
| `REVERSE_MINUTE_DIR`, `MIRROR_FRONT_…`, `FLIP_HORIZONTAL_VERTICAL`, `EXTRA_LED_PER_ROW`, `MEANDER_ROWS` | Matrix wiring | zigzag, not mirrored |

## MQTT and Home Assistant

The clock uses the Home Assistant **JSON light schema**. `<TOPIC>` is the base topic from the web interface, e.g. `ESPWordclock`.

| Topic | Content |
| :--- | :--- |
| `<TOPIC>/cmd` | Commands to the clock |
| `<TOPIC>/status` | State after every change |
| `<TOPIC>/availability` | `online` / `offline` |

| Field | Values |
| :--- | :--- |
| `state` | `"ON"`, `"OFF"` |
| `brightness` | `0`–`255` |
| `color` | `{"h": 0–360, "s": 0–100}`, sets the foreground colour |
| `effect` | `Wordclock`, `Seconds`, `Digitalclock`, `Scrollingtext`, `Rainbowcycle`, `Rainbow`, `Fire`, `Color`, `Symbol` |
| `scrolling_text` | Text for `Scrollingtext` |

```json
{ "state": "ON", "brightness": 255, "color": { "h": 0, "s": 100 } }
{ "effect": "Scrollingtext", "scrolling_text": "Hello World!" }
```

**Auto-discovery:** when it connects, after Home Assistant restarts or via **MQTT Discovery** in the web interface, the clock announces every entity on its own under `homeassistant/<type>/<MAC>…/config`. Home Assistant groups them into one device:

- Light with on/off, brightness, colour and all effects
- Scrolling text, message (shows a text as scrolling text), effect speed, auto brightness
- Selects for transition, transition colour and transition duration
- Diagnostics: illuminance in lux, LED gain, WiFi signal, IP address, uptime

Systems that mimic the Home Assistant format (e.g. openHAB) find the clock as well. Changes from Home Assistant or the buttons show up immediately in every open web interface.

## Contributing

Pull requests for new languages, front layouts or bug fixes are welcome.

- **Formatting:** `clang-format -i $(git ls-files '*.h' '*.hpp' '*.cpp' '*.ino')`. CI checks with clang-format 11.
- **Build:** `pio run` builds all three environments, just like CI. The workflows [PlatformIO CI](.github/workflows/platformio.yml), [clang-format](.github/workflows/clang-format.yml) and [Nightly Firmware](.github/workflows/nightly.yml) must be green.
- **New front layout:**
  1. Put the layout file as a `.hpp` into `include/WordClockTypes/`. It is included automatically.
  2. Register the layout in `CLOCK_TYPES_LIST` in [include/WordClockState.h](include/WordClockState.h) with the next free ID, e.g. `X(Ger10x11Alternative, 12, _de10x11Alternative, "de-10-11-alt")`.
  3. Add the display name under its i18n key in `webpage/language/*.js` (section `view.front`). The web interface's selection list is generated automatically.

## License

BSD-3: the software may be used, modified and redistributed freely as long as the copyright notice is kept. Details in [LICENSE](LICENSE).

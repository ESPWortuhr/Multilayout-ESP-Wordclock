# Changelog

## V5.0.0-rc.1

Release candidate for 5.0.0. This release brings a new web interface, gradient and per-word colouring, a new render pipeline for colours and transitions, a reworked OpenWeatherMap integration and many stability fixes.

### Upgrade notes

- All settings are reset to default values after the update (`SERNR` changed from 440 to 500). Note your hardware pins, MQTT, OpenWeatherMap and layout settings before updating.
- The minute variant "Corners" was removed. The 11x11 layouts now use the standard minute LEDs.

### Added

- New web interface with a liquid glass look, sidebar on desktop and a floating tab bar on phones.
- The web interface follows the system light or dark mode.
- New lightweight colour wheel with brightness and colour temperature, `iro.js` is no longer needed.
- New colour mode "Polychrom": gradient from a primary to a secondary colour.
- New colour mode "Random per word": every word gets its own colour.
- The gradient also works for digital clock, scrolling text, plain colour and symbols.
- New mode "Fire" with adjustable cooling and sparking.
- Brightness control for the symbol mode.
- New Swiss German layout `CH10x11ZH` (Zurich dialect).
- The seconds frame is now a generic feature with a configurable LED count.
- The web page is generated per build environment.

### Improved

- Colours and transitions are separate render stages. Colour changes are applied to a running transition without restarting it.
- The LED strip length is taken from the active layout instead of a fixed 300 LEDs.
- Language variants default to what the layout offers.
- A new hostname is applied at runtime without a reboot.
- MQTT dims both gradient colours with the brightness.
- MQTT reports the current effect and publishes the state once per Home Assistant command.
- MQTT reconnect attempts no longer freeze the display.
- OpenWeatherMap data is fetched without blocking the main loop, 8-digit city IDs are allowed and outdated forecasts are hidden.
- Mist, haze and fog are shown as clouds, cloudy forecasts as "Wolken".
- Settings from the websocket and EEPROM are validated, invalid values are rejected or reset.
- Auto brightness limits are clamped, the 10 bit ADC readout works on every ESP.
- The BH1750 sensor is only used if it is found.
- The manual is updated for 5.0.0 and lists all languages.

### Fixed

- Transitions ran upside down or mirrored on layouts with horizontal or vertical mirroring (#809).
- Mirrored symbols (e.g. note) were shown left-right swapped.
- Auto brightness never fell back to the LDR.
- The countdown ignored the selected font size.
- "vor" lit "Uhr" in the weather layout, and a translation bug in the weather layout.
- `ES08x08` layout.
- Short text payloads wiped the setting.
- Text fields in the web interface are padded by byte length, so umlauts no longer cut off text.
- The secondary colour did not turn off together with the clock.
- Minute LED colour, frame handling and several transition buffer issues after a layout change.
- OpenWeatherMap DNS lookups and header timeouts could freeze the display on ESP32.
- `RTC.begin()` is skipped when I2C is disabled.

### Internal / Maintenance

- Many classes moved out of the unity build into real `.h`/`.cpp` files (Led, Network, WiFi, i2cBus, MQTT, WebPageAdapter, HardwareButtonController, OpenWeatherMap).
- Transitions moved to `include/Transitions/`, new `ColorMatrix` buffers and word identity for colouring.
- Host unit tests for colour changes, display policy, transitions, weather mapping and more.
- Nightly firmware build on GitHub Actions.
- Removed dead code and unused settings (`transitionSpeed`, `colordrift`, `networkReboot` and more).
- Updated dependencies and fixed all open Dependabot alerts except `sprintf-js` (no fixed version available).
- Bumped version in `package.json` to `5.0.0-rc.1`.

### Known Issues

- Not yet tested on real hardware with every layout and all transitions.
- In narrow phone screens some tab labels are shortened (e.g. "Funktio…").

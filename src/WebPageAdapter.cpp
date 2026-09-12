#include "WebPageAdapter.h"

#include "Render/ColorContext.h"        // Colorize
#include "Transitions/TransitionType.h" // isValidTransitionType()
#include "WordClock.h"                  // sendMQTTUpdate()
#include <Arduino.h>

const char favicon[] PROGMEM = {
    0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A, 0x00, 0x00, 0x00, 0x0D,
    0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x10,
    0x04, 0x03, 0x00, 0x00, 0x00, 0xED, 0xDD, 0xE2, 0x52, 0x00, 0x00, 0x00,
    0x21, 0x50, 0x4C, 0x54, 0x45, 0x1B, 0x1B, 0x1B, 0x1A, 0x1A, 0x1A, 0x1A,
    0x1A, 0x1A, 0x1C, 0x1C, 0x1C, 0x1A, 0x1A, 0x1A, 0x18, 0x18, 0x18, 0x1A,
    0x1A, 0x1A, 0xFF, 0xEB, 0x3B, 0x96, 0xDE, 0x42, 0xFF, 0x40, 0x81, 0x34,
    0x9A, 0xFF, 0x93, 0xA0, 0x21, 0x41, 0x00, 0x00, 0x00, 0x06, 0x74, 0x52,
    0x4E, 0x53, 0x4B, 0xE4, 0xE3, 0x4A, 0xE2, 0x49, 0x0C, 0xE2, 0x12, 0xBB,
    0x00, 0x00, 0x00, 0x41, 0x49, 0x44, 0x41, 0x54, 0x08, 0xD7, 0x63, 0x60,
    0x4C, 0x03, 0x03, 0x65, 0x06, 0xB5, 0xF2, 0x72, 0x10, 0x23, 0x85, 0x21,
    0x0D, 0xC2, 0x48, 0x63, 0x48, 0x83, 0x02, 0x10, 0x23, 0xA3, 0xA3, 0xA3,
    0xA3, 0x0D, 0x85, 0x01, 0x93, 0x9A, 0x39, 0x73, 0x26, 0x1A, 0x23, 0x0D,
    0x53, 0x7B, 0xD6, 0xAA, 0x55, 0xAB, 0x96, 0xA1, 0x30, 0x20, 0x52, 0x6A,
    0x10, 0x3A, 0x85, 0xC1, 0x08, 0xC2, 0x70, 0x05, 0x00, 0xFC, 0xF5, 0x36,
    0x26, 0x21, 0xD5, 0x10, 0xB0, 0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4E,
    0x44, 0xAE, 0x42, 0x60, 0x82};

const uint32_t faviconSize = sizeof(favicon);

WebPageAdapter webSocket = WebPageAdapter(80);

//------------------------------------------------------------------------------

namespace {

constexpr size_t COLOR_PAYLOAD_LENGTH = 21;
constexpr size_t EFFECT_PAYLOAD_LENGTH = 27;
constexpr size_t FIRE_PAYLOAD_LENGTH = 12;
constexpr size_t BRIGHTNESS_PAYLOAD_LENGTH = 30;
constexpr size_t COMMAND_PAYLOAD_LENGTH = 3;

bool requirePayloadLength(size_t length, size_t required, const char *command) {
    if (length >= required) {
        return true;
    }

    Serial.print(command);
    Serial.println(" command ignored - incomplete payload");
    return false;
}

//------------------------------------------------------------------------------

uint32_t split(const uint8_t *payload, size_t payloadLength, uint8_t start,
               uint8_t length = 3) {
    if (length > 15)
        length = 15;
    if (static_cast<size_t>(start) + length > payloadLength)
        return 0;

    char buf[16] = {0};
    memcpy(buf, payload + start, length);
    return strtoul(buf, nullptr, 10);
}

//------------------------------------------------------------------------------

uint32_t clampToRange(uint32_t value, uint32_t low, uint32_t high) {
    if (value < low)
        return low;
    if (value > high)
        return high;
    return value;
}

//------------------------------------------------------------------------------

void payloadTextHandling(const uint8_t *payload, size_t payloadLength,
                         char *text, uint8_t start = 3) {
    uint8_t len = PAYLOAD_LENGTH - 1;
    if (static_cast<size_t>(start) + len > payloadLength) {
        return;
    }

    memcpy(text, payload + start, len);
    text[len] = '\0';
    for (int8_t i = len - 1; i >= 0; i--) {
        if (isSpace(text[i]))
            text[i] = '\0';
        else
            break;
    }
}

//------------------------------------------------------------------------------

bool compareEffBriAndSpeedToOld(uint8_t *payload, size_t length) {
    if (length < EFFECT_PAYLOAD_LENGTH) {
        return false;
    }

    return ((G.effectBri != split(payload, length, 21)) ||
            (G.effectSpeed != split(payload, length, 24)));
}

//------------------------------------------------------------------------------

bool modeNeedsInit(uint8_t command, uint8_t *payload, size_t length) {
    switch (command) {
    case COMMAND_MODE_WORD_CLOCK:
        return G.prog != COMMAND_IDLE && G.prog != COMMAND_MODE_WORD_CLOCK;
    case COMMAND_MODE_SECONDS:
    case COMMAND_MODE_DIGITAL_CLOCK:
    case COMMAND_MODE_COLOR:
        return G.prog != command;
    case COMMAND_MODE_RAINBOW:
    case COMMAND_MODE_RAINBOWCYCLE:
    case COMMAND_MODE_FIRE:
    case COMMAND_MODE_SCROLLINGTEXT:
    case COMMAND_MODE_SYMBOL:
        return G.prog != command || compareEffBriAndSpeedToOld(payload, length);
    case COMMAND_MODE_TRANSITION:
        return true;
    default:
        return false;
    }
}

//------------------------------------------------------------------------------

bool parseColor(uint8_t *payload, size_t length) {
    if (!requirePayloadLength(length, COLOR_PAYLOAD_LENGTH, "Color")) {
        return false;
    }

    uint32_t position = split(payload, length, 3);
    uint32_t hue = split(payload, length, 6);
    uint32_t saturation = split(payload, length, 9);
    uint32_t value = split(payload, length, 12);
    uint32_t effectBrightness = split(payload, length, 15);
    uint32_t effectSpeed = split(payload, length, 18);

    if (position > GradientEnd || hue > 360 || saturation > 100 ||
        value > 100 || effectBrightness > 100 ||
        !effectSpeedIsValid(effectSpeed)) {
        Serial.println("Invalid color payload ignored");
        return false;
    }

    G.color[static_cast<ColorPosition>(position)] = {
        HsbColor(hue / 360.f, saturation / 100.f, value / 100.f)};
    colorChangedByWebsite = true;

    G.effectBri = effectBrightness;
    G.effectSpeed = effectSpeed;
    return true;
}

//------------------------------------------------------------------------------

bool parseBrightness(const uint8_t *payload, size_t length) {
    if (!requirePayloadLength(length, BRIGHTNESS_PAYLOAD_LENGTH,
                              "Brightness")) {
        return false;
    }

    uint32_t brightness[9];
    for (uint8_t i = 0; i < 9; i++) {
        brightness[i] = split(payload, length, 3 + i * 3);
        if (brightness[i] > 100) {
            Serial.println("Invalid brightness payload ignored");
            return false;
        }
    }

    G.h6 = brightness[0];
    G.h8 = brightness[1];
    G.h12 = brightness[2];
    G.h16 = brightness[3];
    G.h18 = brightness[4];
    G.h20 = brightness[5];
    G.h22 = brightness[6];
    G.h24 = brightness[7];
    G.effectBri = brightness[8];
    return true;
}

//------------------------------------------------------------------------------

const char *loggablePayload(WStype_t type, const uint8_t *payload,
                            size_t length) {
    if (type == WStype_TEXT && length >= COMMAND_PAYLOAD_LENGTH) {
        switch (split(payload, length, 0)) {
        case COMMAND_SET_MQTT:
        case COMMAND_SET_WEATHER_DATA:
            return "<redacted>";
        default:
            break;
        }
    }

    return reinterpret_cast<const char *>(payload);
}

} // namespace

//------------------------------------------------------------------------------

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload,
                    size_t length) {
    payload = (payload == NULL) ? (uint8_t *)"" : payload;
    Serial.printf("Client-Nr.: [%u]  WStype: %u length: %u payload: %s\n", num,
                  type, static_cast<unsigned>(length),
                  loggablePayload(type, payload, length));

    switch (type) {
    case WStype_DISCONNECTED: {
        Serial.printf("[%u] Disconnected!\n", num);
        break;
    }
    case WStype_CONNECTED: {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0],
                      ip[1], ip[2], ip[3], payload);
        break;
    }
    case WStype_TEXT: {
        if (!requirePayloadLength(length, COMMAND_PAYLOAD_LENGTH,
                                  "WebSocket")) {
            break;
        }

        uint8_t command = split(payload, length, 0);
        G.param1 = 0;

        if (modeNeedsInit(command, payload, length)) {
            G.progInit = true;
        }

        switch (command) {
        case COMMAND_MODE_WORD_CLOCK:
        case COMMAND_MODE_DIGITAL_CLOCK:
        case COMMAND_MODE_COLOR: {
            parametersChanged = true;
            parseColor(payload, length);
            break;
        }

            //------------------------------------------------------------------------------

        case COMMAND_MODE_SECONDS:
        case COMMAND_MODE_SCROLLINGTEXT:
        case COMMAND_MODE_SYMBOL: {
            parseColor(payload, length);
            break;
        }

            //------------------------------------------------------------------------------

        case COMMAND_MODE_TRANSITION: {
            const uint32_t transitionType = split(payload, length, 3);
            if (isValidTransitionType(transitionType)) {
                G.transitionType = transitionType;
            } else {
                Serial.printf("Ignoring invalid transition type: %lu\n",
                              static_cast<unsigned long>(transitionType));
            }
            G.transitionDuration = split(payload, length, 6);
            G.transitionDemo = split(payload, length, 9);
            break;
        }

            //------------------------------------------------------------------------------

        case COMMAND_SET_COLORIZE: {
            const uint32_t mode = split(payload, length, 3);
            if (mode <= WORD_RANDOM) {
                G.colorize = static_cast<uint8_t>(mode);
            }
            break;
        }

            //------------------------------------------------------------------------------

        case COMMAND_SPEED: {
            const uint32_t speed = split(payload, length, 3);
            if (effectSpeedIsValid(speed)) {
                G.effectSpeed = static_cast<uint8_t>(speed);
            }
            break;
        }

            //------------------------------------------------------------------------------

        case COMMAND_SET_INITIAL_VALUES: {
            Serial.println("Startwerte gespeichert");

            if (length > 3) {
                parseColor(payload, length);
            }
            break;
        }

            //------------------------------------------------------------------------------

        case COMMAND_SET_TIME: {
            struct timeval tv;
            tv.tv_sec = split(payload, length, 6, 16);
            tv.tv_usec = 0;
            settimeofday(&tv, nullptr);
            break;
        }

            //------------------------------------------------------------------------------

        case COMMAND_SET_HOSTNAME: {
            payloadTextHandling(payload, length, G.hostname);
            break;
        }

            //------------------------------------------------------------------------------

        case COMMAND_SET_SETTING_SECOND: {
            G.progInit = true;

            const uint32_t secondVariant = split(payload, length, 3);
            if (secondVariantIsValid(secondVariant)) {
                G.secondVariant = static_cast<SecondVariant>(secondVariant);
            } else {
                Serial.printf("Ignoring invalid second variant: %lu\n",
                              static_cast<unsigned long>(secondVariant));
            }
            break;
        }

            //------------------------------------------------------------------------------

        case COMMAND_SET_MINUTE: {
            const uint32_t minuteVariant = split(payload, length, 3);
            if (minuteVariantIsValid(minuteVariant)) {
                G.minuteVariant = static_cast<MinuteVariant>(minuteVariant);
            } else {
                Serial.printf("Ignoring invalid minute variant: %lu\n",
                              static_cast<unsigned long>(minuteVariant));
            }
            break;
        }

            //------------------------------------------------------------------------------

        case COMMAND_SET_SECONDS_FRAME: {
            // Staged in G.param1 rather than written straight into
            // G.secondsFrameLedCount: the ClockWork handler needs the OLD
            // count still in place when it clears the frame, so that
            // shrinking the count doesn't leave the now out-of-range LEDs
            // permanently lit.
            const uint32_t ledCount = split(payload, length, 3);
            G.param1 = static_cast<uint8_t>(min(
                ledCount, static_cast<uint32_t>(MAX_SECONDS_FRAME_LED_COUNT)));
            break;
        }

            //------------------------------------------------------------------------------

        case COMMAND_SET_FIRE: {
            if (!requirePayloadLength(length, FIRE_PAYLOAD_LENGTH, "Fire")) {
                break;
            }

            const uint32_t cooling = split(payload, length, 3);
            const uint32_t sparking = split(payload, length, 6);

            if (cooling < FIRE_COOLING_MIN || cooling > FIRE_COOLING_MAX ||
                sparking < FIRE_SPARKING_MIN || sparking > FIRE_SPARKING_MAX) {
                Serial.printf("Ignoring invalid fire settings: %lu %lu\n",
                              static_cast<unsigned long>(cooling),
                              static_cast<unsigned long>(sparking));
                break;
            }

            G.fireCooling = static_cast<uint8_t>(cooling);
            G.fireSparking = static_cast<uint8_t>(sparking);
            G.param1 = split(payload, length, 9) ? 1 : 0;
            break;
        }

            //------------------------------------------------------------------------------

        case COMMAND_SET_AUTO_BRIGHT: {
            G.autoBrightEnabled = split(payload, length, 3);
            G.autoBrightMin = clampToRange(split(payload, length, 6), 0, 100);
            G.autoBrightMax = clampToRange(split(payload, length, 9), 10, 100);
            G.autoBrightPeak =
                clampToRange(split(payload, length, 12, 4), 10, 1500);
            G.param1 = 1;
            break;
        }

            //------------------------------------------------------------------------------

        case COMMAND_SET_IT_IS_VARIANT: {
            const uint32_t itIsVariant = split(payload, length, 3);
            if (itIsVariantIsValid(itIsVariant)) {
                G.itIsVariant = static_cast<ItIsVariant>(itIsVariant);
            } else {
                Serial.printf("Ignoring invalid it-is variant: %lu\n",
                              static_cast<unsigned long>(itIsVariant));
            }
            break;
        }

            //------------------------------------------------------------------------------

        case COMMAND_SET_LANGUAGE_VARIANT: {
            G.languageVariant[ItIs15] = split(payload, length, 3);
            G.languageVariant[ItIs20] = split(payload, length, 6);
            G.languageVariant[ItIs40] = split(payload, length, 9);
            G.languageVariant[ItIs45] = split(payload, length, 12);
            G.languageVariant[EN_ShowAQuarter] = split(payload, length, 15);
            break;
        }

            //------------------------------------------------------------------------------

        case COMMAND_SET_LAYOUT_VARIANT: {
            G.layoutVariant[ReverseMinDirection] = split(payload, length, 3);
            G.layoutVariant[MirrorVertical] = split(payload, length, 6);
            G.layoutVariant[MirrorHorizontal] = split(payload, length, 9);
            G.layoutVariant[FlipHorzVert] = split(payload, length, 12);
            G.layoutVariant[ExtraLedPerRow] = split(payload, length, 15);
            G.layoutVariant[MeanderRows] = split(payload, length, 18);
            break;
        }

            //------------------------------------------------------------------------------

        case COMMAND_SET_MQTT: {
            uint8_t newState = split(payload, length, 3);

            if (newState && !G.mqtt.state) {
                G.progInit = true;
            }

            G.mqtt.state = newState;
            G.mqtt.port = split(payload, length, 6, 5);
            uint8_t index_start = 11;
            payloadTextHandling(payload, length, G.mqtt.serverAdress,
                                index_start);
            index_start += PAYLOAD_LENGTH;
            payloadTextHandling(payload, length, G.mqtt.user, index_start);

            // check if submitted password has changed compared to masked
            // password
            index_start += PAYLOAD_LENGTH;
            char passSubmitted[sizeof(G.mqtt.password)] = {0};
            payloadTextHandling(payload, length, passSubmitted, index_start);
            if (!sensitive::matchesMaskedValue(passSubmitted,
                                               G.mqtt.password)) {
                payloadTextHandling(payload, length, G.mqtt.password,
                                    index_start);
            }

            index_start += PAYLOAD_LENGTH;
            payloadTextHandling(payload, length, G.mqtt.clientId, index_start);
            index_start += PAYLOAD_LENGTH;
            payloadTextHandling(payload, length, G.mqtt.topic, index_start);
            break;
        }

            //------------------------------------------------------------------------------

        case COMMAND_SET_TIME_MANUAL: {
            time_t old = time(nullptr);
            struct tm tm;
            localtime_r(&old, &tm);
            tm.tm_hour = split(payload, length, 3);
            tm.tm_min = split(payload, length, 6);
            tm.tm_sec = 0;
            struct timeval tv;
            tv.tv_sec = mktime(&tm);
            tv.tv_usec = 0;
            Serial.println("Time manually set");
            settimeofday(&tv, nullptr);
            break;
        }

            //------------------------------------------------------------------------------

        case COMMAND_SET_HARDWARE_PINS: {
            G.hardwarePins.led = split(payload, length, 3);
            G.hardwarePins.powerButton = split(payload, length, 6);
            G.hardwarePins.modeButton = split(payload, length, 9);
            G.hardwarePins.speedButton = split(payload, length, 12);
            G.i2cSdaPin = split(payload, length, 15);
            G.i2cSclPin = split(payload, length, 18);
            break;
        }

            //------------------------------------------------------------------------------

        case COMMAND_SET_BIRTHDAYS: {

            for (uint8_t i = 0; i < MAX_BIRTHDAY_COUNT; i++) {
                G.birthday[i].month = split(payload, length, 3 + i * 5, 2);
                G.birthday[i].day = split(payload, length, 6 + i * 5, 2);
            }
            break;
        }

            //------------------------------------------------------------------------------

        case COMMAND_SET_SYMBOL: {
            G.bitmapSymbol =
                static_cast<BitmapSymbol>(split(payload, length, 3));
            if (G.bitmapSymbol >= BitmapSymbol::MAX_BITMAP_SYMBOLS) {
                G.bitmapSymbol = BitmapSymbol::HEART;
            }
            G.progInit = true;
            break;
        }
            //------------------------------------------------------------------------------

        case COMMAND_SET_COLORTYPE: {
            G.progInit = true;

            const uint32_t colorType = split(payload, length, 3);
            if (colorTypeIsValid(colorType)) {
                G.param1 = static_cast<uint8_t>(colorType);
            } else {
                Serial.printf("Ignoring invalid color type: %lu\n",
                              static_cast<unsigned long>(colorType));
                G.param1 = G.Colortype;
            }
            break;
        }

            //------------------------------------------------------------------------------

        case COMMAND_SET_BUILDTYPE: {
            G.progInit = true;

            const uint32_t buildType = split(payload, length, 3);
            if (buildTypeIsValid(buildType)) {
                G.param1 = static_cast<uint8_t>(buildType);
            } else {
                Serial.printf("Ignoring invalid build type: %lu\n",
                              static_cast<unsigned long>(buildType));
                G.param1 = static_cast<uint8_t>(G.buildTypeDef);
            }
            break;
        }

            //------------------------------------------------------------------------------

        case COMMAND_SET_WHITETYPE: {
            const uint32_t whiteType = split(payload, length, 3);
            if (whiteTypeIsValid(whiteType)) {
                G.wType = static_cast<WhiteType>(whiteType);
            } else {
                Serial.printf("Ignoring invalid white type: %lu\n",
                              static_cast<unsigned long>(whiteType));
            }
            break;
        }

            //------------------------------------------------------------------------------

        case COMMAND_SET_CLOCK_TYPE: {
            uint32_t clockTypeDef = split(payload, length, 3);
            if (clockTypeDef <= UINT8_MAX &&
                isValidClockTypeDef(static_cast<uint8_t>(clockTypeDef))) {
                G.clockTypeDef = static_cast<uint8_t>(clockTypeDef);
            } else {
                Serial.printf("Ignoring invalid ClockType: %lu\n",
                              static_cast<unsigned long>(clockTypeDef));
            }
            break;
        }

            //------------------------------------------------------------------------------

        case COMMAND_SET_WEATHER_DATA: {
            uint8_t ii = 0;
            size_t cityEnd = 3;
            while (cityEnd < length && payload[cityEnd] != ' ') {
                if (ii < sizeof(G.openWeatherMap.cityid) - 1) {
                    G.openWeatherMap.cityid[ii++] = payload[cityEnd];
                }
                cityEnd++;
            }
            G.openWeatherMap.cityid[ii] = '\0';

            char submittedApiKey[sizeof(G.openWeatherMap.apikey)] = {0};
            uint8_t jj = 0;
            const size_t apiKeyStart = cityEnd + 1;
            if (length > apiKeyStart) {
                const size_t apiKeyEnd =
                    (length < apiKeyStart + 32) ? length : apiKeyStart + 32;
                for (size_t l = apiKeyStart; l < apiKeyEnd; l++) {
                    if (payload[l] != ' ' && jj < sizeof(submittedApiKey) - 1) {
                        submittedApiKey[jj++] = payload[l];
                    }
                }
                submittedApiKey[jj] = '\0';
                if (!sensitive::matchesMaskedValue(submittedApiKey,
                                                   G.openWeatherMap.apikey)) {
                    sensitive::copyBoundedString(G.openWeatherMap.apikey,
                                                 submittedApiKey);
                }
            }
            Serial.println("write EEPROM!");
            Serial.print("CityID : ");
            Serial.println(G.openWeatherMap.cityid);
            Serial.print("APIkey : ");
            char apiKeyMasked[sizeof(G.openWeatherMap.apikey) + 1] = {0};
            sensitive::maskPreservingSuffix(apiKeyMasked,
                                            G.openWeatherMap.apikey);
            Serial.println(apiKeyMasked);
            break;
        }

            //------------------------------------------------------------------------------

        case COMMAND_SET_BRIGHTNESS: {
            parseBrightness(payload, length);
            break;
        }

            //------------------------------------------------------------------------------

        case COMMAND_SET_SCROLLINGTEXT: {
            payloadTextHandling(payload, length, G.scrollingText);
            break;
        }

            //------------------------------------------------------------------------------

        case COMMAND_SET_TIMESERVER: {
            payloadTextHandling(payload, length, G.timeserver);
            break;
        }

            //------------------------------------------------------------------------------

        case COMMAND_SET_TIMEZONE: {
            payloadTextHandling(payload, length, G.timezone);
            break;
        }

            //------------------------------------------------------------------------------

        case COMMAND_SET_BOOT: {
            G.bootLedBlink = split(payload, length, 3);
            G.bootLedSweep = split(payload, length, 6);
            G.bootShowWifi = split(payload, length, 9);
            G.bootShowIP = split(payload, length, 12);
            break;
        }

            //------------------------------------------------------------------------------

        case COMMAND_REQUEST_I2C_SCAN: {
            G.client_nr = num;
            i2cScanSdaPin = split(payload, length, 3);
            i2cScanSclPin = split(payload, length, 6);
            break;
        }

            //------------------------------------------------------------------------------

        case COMMAND_REQUEST_BIRTHDAYS:
        case COMMAND_REQUEST_MQTT_VALUES:
        case COMMAND_REQUEST_CONFIG_VALUES:
        case COMMAND_REQUEST_COLOR_VALUES:
        case COMMAND_REQUEST_TRANSITION:
        case COMMAND_REQUEST_AUTO_BRIGHT: {
            G.client_nr = num;
            break;
        }

            //------------------------------------------------------------------------------

        default:
            break;
        }

        if (command < PLACEHOLDER_MAX_MODE) {
            if (isValidModeCommand(command)) {
                G.prog = command;
            } else {
                Serial.printf("Ignoring unknown mode command: %u\n", command);
            }
        } else if (command < PLACEHOLDER_MAX_SET) {
            G.conf = command;
        } else if (command < PLACEHOLDER_MAX_REQUEST) {
            G.conf = command;
        }

        break;
    }
    case WStype_BIN: {
        Serial.printf("[%u] get binary length: %u\n", num, length);
        break;
    }
    default:
        break;
    }

    sendMQTTUpdate();
}

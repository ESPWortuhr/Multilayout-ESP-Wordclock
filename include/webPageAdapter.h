#pragma once

#include "Uhr.h"
#include "WebPageContent.h"
#include "WebSocketsServer.h"

#define RESPONSE_SIZE 900
#define SIZE_OF_FAVICON 185

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

class WebPageAdapter : public WebSocketsServer {

public:
    // forward the port to the parent class constructor
    WebPageAdapter(int port) : WebSocketsServer(port) {}

    /**
     * @param client WSclient_t *  ptr to the client struct
     */
    void handleNonWebsocketConnection(WSclient_t *client) override {
        DEBUG_WEBSOCKETS(
            "[WS-Server][%d][handleHeader] no Websocket connection close.\n",
            client->num);
        char buf[200];
        int index = client->cUrl.indexOf('?');
        int length = index >= 0 ? index : client->cUrl.length();
        // remove GET request data from URL
        String url = client->cUrl.substring(0, length);
        // ----------------------------------------
        if (url.endsWith("favicon.ico")) {
            sprintf(buf,
                    "HTTP/1.1 200 OK\r\n"
                    "Content-Type: image/x-ico\r\n"
                    "Content-Length: %d\r\n"
                    "Connection: close\r\n"
                    "\r\n",
                    SIZE_OF_FAVICON);
            client->tcp->write(buf);
            for (int i = 0; i < SIZE_OF_FAVICON; i++) {
                buf[i] = pgm_read_byte(&favicon[i]);
            }
            client->tcp->write(buf, SIZE_OF_FAVICON);

        } else {
            // ------------------------------------
            if (url.equals("/")) {
                client->tcp->write(
                    "HTTP/1.1 200 OK\r\n"
                    "Server: arduino-WebSocket-Server\r\n"
                    "Content-Type: text/html\r\n"
                    //--                    "Content-Length: 32\r\n"
                    "Connection: close\r\n"
                    //--                    "Sec-WebSocket-Version: 13\r\n"
                    "\r\n");
                sendHtmlCode(client, html_code, html_size);
            } else {

                // --------------------------------
                int len = sprintf(buf, "HTTP/1.1 404 Not Found\r\n"
                                       "Content-Type: text/plain\r\n\r\n"
                                       "Seite ");
                client->cUrl.toCharArray(buf + len, sizeof(buf) - len);
                strcat(buf, " nicht gefunden\n");
                client->tcp->write(buf);
            }
        }

        clientDisconnect(client);
    }

    void sendHtmlCode(const WSclient_t *client, const char *data,
                      uint32_t size) const {
        char buf[RESPONSE_SIZE];
        unsigned sent = 0;
        unsigned blen = 0;
        while (sent < size) {
            buf[blen] = pgm_read_byte(&data[sent]);
            blen++;
            if (blen == RESPONSE_SIZE) {
                client->tcp->write(buf, blen);
                blen = 0;
            }
            sent++;
        }
        if (blen > 0) {
            client->tcp->write(buf, blen);
        }
    }
};

//-- WebSocketserver
WebPageAdapter webSocket = WebPageAdapter(80);

//------------------------------------------------------------------------------

uint16_t split(uint8_t *payload, uint8_t start, uint8_t length = 3) {
    String value;
    for (uint16_t k = start; k < start + length; k++) {
        value += char(payload[k]);
    }
    return value.toInt();
}

//------------------------------------------------------------------------------

void payloadTextHandling(const uint8_t *payload, char *text,
                         uint8_t start = 3) {
    uint8_t ii = 0;
    for (uint8_t k = start; k < start + PAYLOAD_LENGTH - 1;
         k++) // need space for  '\0'
    {
        text[ii] = payload[k];
        ii++;
    }
    uint8_t index = PAYLOAD_LENGTH - 1;
    for (int8_t counter = PAYLOAD_LENGTH - 2; counter > -1; counter--) {
        if (!isSpace(text[counter])) {
            index = counter + 1;
            break;
        }
    }
    text[index] = '\0';
}

//------------------------------------------------------------------------------

bool compareEffBriAndSpeedToOld(uint8_t *payload) {
    return ((G.effectBri != split(payload, 21)) ||
            (G.effectSpeed != split(payload, 24)));
}

//------------------------------------------------------------------------------

void parseColor(uint8_t *payload, ColorPosition position = Foreground) {
    if (position == Background) {
        G.color[position] = {HsbColor(split(payload, 12) / 360.f,
                                      split(payload, 15) / 100.f,
                                      split(payload, 18) / 100.f)};
    } else {
        G.color[position] = {HsbColor(split(payload, 3) / 360.f,
                                      split(payload, 6) / 100.f,
                                      split(payload, 9) / 100.f)};
    }
    colorChangedByWebsite = true;
}

//------------------------------------------------------------------------------

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload,
                    size_t length) {
    // Disable Accesspoint Mode Disable Timer on Web Event
    if (statusAccessPoint > 0) {
        statusAccessPoint = 0;
    }
    int ii;
    int jj;
    char tmp[30];
    payload = (payload == NULL) ? (uint8_t *)"" : payload;
    Serial.printf("Client-Nr.: [%u]  WStype: %u payload: %s\n", num, type,
                  payload);

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
        Serial.printf("[%u] get Text: %s\n", length, payload);

        uint8_t command = split(payload, 0);
        G.param1 = 0;

        switch (command) {
        case COMMAND_MODE_WORD_CLOCK: {
            if (G.prog != COMMAND_IDLE && G.prog != COMMAND_MODE_WORD_CLOCK) {
                G.progInit = true;
            }
            parametersChanged = true;
            parseColor(payload);
            parseColor(payload, Background);
            break;
        }

            //------------------------------------------------------------------------------

        case COMMAND_MODE_SECONDS: {
            if (G.prog != command) {
                G.progInit = true;
            }

            parseColor(payload);
            break;
        }

            //------------------------------------------------------------------------------

        case COMMAND_MODE_DIGITAL_CLOCK: {
            if (G.prog != command) {
                G.progInit = true;
            }

            parseColor(payload);
            parametersChanged = true;
            break;
        }

            //------------------------------------------------------------------------------

        case COMMAND_MODE_SCROLLINGTEXT: {
            if ((G.prog != command) || compareEffBriAndSpeedToOld(payload)) {
                G.progInit = true;
            }

            parseColor(payload);
            G.effectSpeed = split(payload, 24);
            break;
        }

            //------------------------------------------------------------------------------

        case COMMAND_MODE_RAINBOWCYCLE: {
            if ((G.prog != command) || compareEffBriAndSpeedToOld(payload)) {
                G.progInit = true;
            }

            G.effectBri = split(payload, 21);
            G.effectSpeed = split(payload, 24);
            break;
        }

            //------------------------------------------------------------------------------

        case COMMAND_MODE_RAINBOW: {
            if ((G.prog != command) || compareEffBriAndSpeedToOld(payload)) {
                G.progInit = true;
            }

            G.effectBri = split(payload, 21);
            G.effectSpeed = split(payload, 24);
            break;
        }

            //------------------------------------------------------------------------------

        case COMMAND_MODE_COLOR: {
            if (G.prog != command) {
                G.progInit = true;
            }

            parametersChanged = true;
            parseColor(payload);
            break;
        }
            //------------------------------------------------------------------------------

        case COMMAND_MODE_SYMBOL: {
            if ((G.prog != command) || compareEffBriAndSpeedToOld(payload)) {
                G.progInit = true;
            }

            parseColor(payload);
            G.effectBri = split(payload, 21);
            G.effectSpeed = split(payload, 24);
            break;
        }
            //------------------------------------------------------------------------------

        case COMMAND_MODE_TRANSITION: {
            G.progInit = true;

            G.transitionType = split(payload, 3);
            G.transitionDuration = split(payload, 6);
            G.transitionSpeed = split(payload, 9);
            G.transitionColorize = split(payload, 12);
            G.transitionDemo = split(payload, 15);
            ;
            break;
        }

            //------------------------------------------------------------------------------

        case COMMAND_SPEED: {
            G.effectSpeed = split(payload, 3);
            break;
        }

            //------------------------------------------------------------------------------

        case COMMAND_SET_INITIAL_VALUES: {
            Serial.println("Startwerte gespeichert");

            parseColor(payload);
            break;
        }

            //------------------------------------------------------------------------------

        case COMMAND_SET_TIME: {
            ii = 0;
            tmp[0] = '\0';
            uint32_t tt = split(payload, 3, 16);
            Serial.println(tt);
            for (uint8_t k = 12; k < 28; k++) {
                tmp[ii] = payload[k];
                ii++;
            }
            struct timeval tv;
            tv.tv_sec = atoi(tmp);
            tv.tv_usec = 0;
            Serial.printf("Conf: Time: %lld\n", tv.tv_sec);
            settimeofday(&tv, nullptr);
            break;
        }

            //------------------------------------------------------------------------------

        case COMMAND_SET_HOSTNAME: {
            payloadTextHandling(payload, G.hostname);
            break;
        }

            //------------------------------------------------------------------------------

        case COMMAND_SET_SETTING_SECOND: {
            G.progInit = true;

            G.secondVariant = static_cast<SecondVariant>(split(payload, 3));
            break;
        }

            //------------------------------------------------------------------------------

        case COMMAND_SET_MINUTE: {
            G.minuteVariant = static_cast<MinuteVariant>(split(payload, 3));
            break;
        }

            //------------------------------------------------------------------------------

        case COMMAND_SET_AUTO_BRIGHT: {
            G.autoBrightEnabled = split(payload, 3);
            G.autoBrightOffset = split(payload, 6);
            G.autoBrightSlope = split(payload, 9);
            break;
        }

            //------------------------------------------------------------------------------

        case COMMAND_SET_LANGUAGE_VARIANT: {
            G.languageVariant[ItIs15] = split(payload, 3);
            G.languageVariant[ItIs20] = split(payload, 6);
            G.languageVariant[ItIs40] = split(payload, 9);
            G.languageVariant[ItIs45] = split(payload, 12);
            G.languageVariant[NotShowItIs] = split(payload, 15);
            G.languageVariant[EN_ShowAQuarter] = split(payload, 18);
            break;
        }

            //------------------------------------------------------------------------------

        case COMMAND_SET_LAYOUT_VARIANT: {
            G.layoutVariant[ReverseMinDirection] = split(payload, 3);
            G.layoutVariant[MirrorVertical] = split(payload, 6);
            G.layoutVariant[MirrorHorizontal] = split(payload, 9);
            break;
        }

            //------------------------------------------------------------------------------

        case COMMAND_SET_MQTT: {
            uint8_t newState = split(payload, 3);

            if (newState && !G.mqtt.state) {
                G.progInit = true;
            }

            G.mqtt.state = newState;
            G.mqtt.port = split(payload, 6, 5);
            uint8_t index_start = 11;
            payloadTextHandling(payload, G.mqtt.serverAdress, index_start);
            index_start += PAYLOAD_LENGTH;
            payloadTextHandling(payload, G.mqtt.user, index_start);
            index_start += PAYLOAD_LENGTH;
            payloadTextHandling(payload, G.mqtt.password, index_start);
            index_start += PAYLOAD_LENGTH;
            payloadTextHandling(payload, G.mqtt.clientId, index_start);
            index_start += PAYLOAD_LENGTH;
            payloadTextHandling(payload, G.mqtt.topic, index_start);
            break;
        }

            //------------------------------------------------------------------------------

        case COMMAND_SET_TIME_MANUAL: {
            time_t old = time(nullptr);
            struct tm tm;
            localtime_r(&old, &tm);
            tm.tm_hour = split(payload, 3);
            tm.tm_min = split(payload, 6);
            tm.tm_sec = 0;
            struct timeval tv;
            tv.tv_sec = mktime(&tm);
            tv.tv_usec = 0;
            Serial.println("Time manually set");
            Serial.printf("Conf: Time: %lld\n", tv.tv_sec);
            settimeofday(&tv, nullptr);
            break;
        }

            //------------------------------------------------------------------------------

        case COMMAND_SET_BIRTHDAYS: {

            for (uint8_t i = 0; i < MAX_BIRTHDAY_COUNT; i++) {
                G.birthday[i].year = split(payload, 3 + i * 10, 4);
                G.birthday[i].month = split(payload, 9 + i * 10, 2);
                G.birthday[i].day = split(payload, 11 + i * 10, 2);
            }
        }
            //------------------------------------------------------------------------------

        case COMMAND_SET_COLORTYPE: {
            G.progInit = true;

            G.param1 = split(payload, 3);
            break;
        }

            //------------------------------------------------------------------------------

        case COMMAND_SET_BUILDTYPE: {
            G.progInit = true;

            G.param1 = split(payload, 3);
            break;
        }

            //------------------------------------------------------------------------------

        case COMMAND_SET_WHITETYPE: {
            G.wType = static_cast<WhiteType>(split(payload, 3));
            break;
        }

            //------------------------------------------------------------------------------

        case COMMAND_SET_UHRTYPE: {
            G.UhrtypeDef = split(payload, 3);
            break;
        }

            //------------------------------------------------------------------------------

        case COMMAND_SET_WEATHER_DATA: {
            ii = 0;
            for (uint8_t k = 3; k < 10; k++) {
                if (payload[k] != ' ') {
                    G.openWeatherMap.cityid[ii] = payload[k];
                    ii++;
                }
            }
            G.openWeatherMap.cityid[ii] = '\0';
            //
            jj = 0;
            for (uint8_t l = 11; l < 43; l++) {
                if (payload[l] != ' ') {
                    G.openWeatherMap.apikey[jj] = payload[l];
                    jj++;
                }
            }
            G.openWeatherMap.apikey[jj] = '\0';
            Serial.println("write EEPROM!");
            Serial.print("CityID : ");
            Serial.println(G.openWeatherMap.cityid);
            Serial.print("APIkey : ");
            Serial.println(G.openWeatherMap.apikey);
            break;
        }

            //------------------------------------------------------------------------------

        case COMMAND_SET_BRIGHTNESS: {
            G.h6 = split(payload, 3);
            G.h8 = split(payload, 6);
            G.h12 = split(payload, 9);
            G.h16 = split(payload, 12);
            G.h18 = split(payload, 15);
            G.h20 = split(payload, 18);
            G.h22 = split(payload, 21);
            G.h24 = split(payload, 24);
            G.effectBri = split(payload, 27);
            break;
        }

            //------------------------------------------------------------------------------

        case COMMAND_SET_SCROLLINGTEXT: {
            payloadTextHandling(payload, G.scrollingText);
            break;
        }

            //------------------------------------------------------------------------------

        case COMMAND_SET_TIMESERVER: {
            payloadTextHandling(payload, G.timeserver);
            break;
        }

            //------------------------------------------------------------------------------

        case COMMAND_SET_BOOT: {
            G.bootLedBlink = split(payload, 3);
            G.bootLedSweep = split(payload, 6);
            G.bootShowWifi = split(payload, 9);
            G.bootShowIP = split(payload, 12);
            break;
        }
            //------------------------------------------------------------------------------

        case COMMAND_SET_WIFI_DISABLED:
        case COMMAND_SET_MQTT_HA_DISCOVERY:
        case COMMAND_SET_WIFI_AND_RESTART:
        case COMMAND_RESET: {
            break;
        }

            //------------------------------------------------------------------------------

        case COMMAND_REQUEST_BIRTHDAYS:
        case COMMAND_REQUEST_MQTT_VALUES:
        case COMMAND_REQUEST_CONFIG_VALUES:
        case COMMAND_REQUEST_COLOR_VALUES:
        case COMMAND_REQUEST_WIFI_LIST:
        case COMMAND_REQUEST_TRANSITION: {
            G.client_nr = num;
            break;
        }

            //------------------------------------------------------------------------------

        case COMMAND_REQUEST_AUTO_BRIGHT: {

            G.param1 = split(payload, 3);
            G.client_nr = num;
            break;
        }

            //------------------------------------------------------------------------------

        default:
            break;
        }

        if (command < PLACEHOLDER_MAX_MODE) {
            G.prog = command;
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
}

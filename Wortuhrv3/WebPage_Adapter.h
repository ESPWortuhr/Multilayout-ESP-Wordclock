#pragma once

#include "WebSocketsServer.h"

#ifdef Grbw
#include "WebPageWortuhr_RGBW.h"
#else

#include "WebPageWortuhr.h"

#endif

#define RESPONSE_SIZE    900

class WebPage_Adapter : public WebSocketsServer {

public:
    // forward the port to the parent class constructor
    WebPage_Adapter(int port) : WebSocketsServer(port) {
    }

    /**
     * @param client WSclient_t *  ptr to the client struct
     */
    void handleNonWebsocketConnection(WSclient_t *client) override {
        char str[RESPONSE_SIZE + 4];
        unsigned ww = 0;
        unsigned yy = 0;
        int j;
        DEBUG_WEBSOCKETS("[WS-Server][%d][handleHeader] no Websocket connection close.\n", client->num);
        client->tcp->write("HTTP/1.1 200 OK\r\n"
                           "Server: arduino-WebSocket-Server\r\n"
                           "Content-Type: text/html\r\n"
                           //--                    "Content-Length: 32\r\n"
                           "Connection: close\r\n"
                           //--                    "Sec-WebSocket-Version: 13\r\n"
                           "\r\n");
        while (ww < sizeof(index_html_head)) {
            str[yy] = pgm_read_byte(&index_html_head[ww]);
            str[yy + 1] = '\0';
            yy++;
            if (yy == RESPONSE_SIZE) {
                j = strlen(str);
                client->tcp->write(&str[0], j);
                str[0] = '\0';
                yy = 0;
            }
            ww++;
        }
        if (yy > 0) {
            j = strlen(str);
            client->tcp->write(&str[0], j);
        }
        ww = 0;
        yy = 0;
        while (ww < sizeof(index_html_body_RGB)) {
            str[yy] = pgm_read_byte(&index_html_body_RGB[ww]);
            str[yy + 1] = '\0';
            yy++;
            if (yy == RESPONSE_SIZE) {
                j = strlen(str);
                client->tcp->write(&str[0], j);
                str[0] = '\0';
                yy = 0;
            }
            ww++;
        }
        if (yy > 0) {
            j = strlen(str);
            client->tcp->write(&str[0], j);
        }
        clientDisconnect(client);
    }

};

//-- WebSocketserver
WebPage_Adapter webSocket = WebPage_Adapter(80);

//------------------------------------------------------------------------------

uint32_t split(uint8_t i, uint8_t j) {
    char dest[3];
    int m;
    m = 0;
    for (int k = i; k < (i + j); k++) {
        dest[m] = str[k];
        m++;
    }
    return atoi(dest);
}

//------------------------------------------------------------------------------

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t lenght) {
    int ii;
    int jj;
    char tmp[30];
    payload = (payload == NULL) ? (uint8_t *) "" : payload;
    Serial.printf("Client-Nr.: [%u]  WStype: %u payload: %s\n", num, type, payload);

    switch (type) {
        case WStype_DISCONNECTED: {
            Serial.printf("[%u] Disconnected!\n", num);
            break;
        }
        case WStype_CONNECTED: {
            IPAddress ip = webSocket.remoteIP(num);
            Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
            break;
        }
        case WStype_TEXT: {
            Serial.printf("[%u] get Text: %s\n", lenght, payload);
            for (unsigned int k = 0; k < lenght; k++) {
                str[k] = payload[k];  //does this "copy" is buggy code?
            }

            int cc = split(0, 3);
            G.param1 = split(3, 3);
            G.param2 = split(6, 3);

            if (cc == COMMAND_MODE_WORD_CLOCK) {        // Uhrzeit Vordergrund Farbe einstellen
                G.prog = COMMAND_MODE_WORD_CLOCK;
                G.rgb[Foreground][0] = split(9, 3);
                G.rgb[Foreground][1] = split(12, 3);
                G.rgb[Foreground][2] = split(15, 3);
                G.rgb[Foreground][3] = split(18, 3);

                G.rgb[Background][0] = split(21, 3);
                G.rgb[Background][1] = split(24, 3);
                G.rgb[Background][2] = split(27, 3);
                G.rgb[Background][3] = split(30, 3);

                G.rgb[Frame][0] = split(33, 3);
                G.rgb[Frame][1] = split(36, 3);
                G.rgb[Frame][2] = split(39, 3);
                G.rgb[Frame][3] = split(42, 3);

                G.rgb[Effect][0] = split(45, 3);
                G.rgb[Effect][1] = split(48, 3);
                G.rgb[Effect][2] = split(51, 3);
                G.rgb[Effect][3] = split(54, 3);
                break;
            }

            //------------------------------------------------------------------------------

            if (cc == COMMAND_MODE_SECONDS) {      // Sekunden
                G.prog = COMMAND_MODE_SECONDS;
                if (G.param1 == 0) { G.prog_init = 1; }

                G.rgb[Effect][0] = split(45, 3);
                G.rgb[Effect][1] = split(48, 3);
                G.rgb[Effect][2] = split(51, 3);
                G.rgb[Effect][3] = split(54, 3);
                G.hell = split(57, 3);
                G.geschw = split(60, 3);
                break;
            }

            if (cc == COMMAND_MODE_MARQUEE) {      // Laufschrift
                G.prog = COMMAND_MODE_MARQUEE;
                if (G.param1 == 0) { G.prog_init = 1; }

                G.rgb[Effect][0] = split(45, 3);
                G.rgb[Effect][1] = split(48, 3);
                G.rgb[Effect][2] = split(51, 3);
                G.rgb[Effect][3] = split(54, 3);
                G.hell = split(57, 3);
                G.geschw = split(60, 3);
                break;
            }

            if (cc == COMMAND_MODE_RAINBOW) {      // Regenbogen
                G.prog = COMMAND_MODE_RAINBOW;
                G.prog_init = 1;

                G.hell = split(57, 3);
                G.geschw = split(60, 3);
                break;
            }

            if (cc == COMMAND_MODE_CHANGE) {      // Farbwechsel
                G.prog = COMMAND_MODE_CHANGE;
                G.prog_init = 1;

                G.hell = split(57, 3);
                G.geschw = split(60, 3);
                break;
            }

            if (cc == COMMAND_MODE_COLOR) {      // Farbe
                G.prog = COMMAND_MODE_COLOR;
                G.prog_init = 1;

                G.rgb[Effect][0] = split(45, 3);
                G.rgb[Effect][1] = split(48, 3);
                G.rgb[Effect][2] = split(51, 3);
                G.rgb[Effect][3] = split(54, 3);
                break;
            }

            //------------------------------------------------------------------------------

            if (cc == COMMAND_BRIGHTNESS) {      // Helligkeit
                G.hell = split(57, 3);
                break;
            }

            if (cc == COMMAND_SPEED) {       // Geschwindigkeit
                G.geschw = split(60, 3);
                break;
            }

            //------------------------------------------------------------------------------

            if (cc == COMMAND_SET_INITIAL_VALUES) {       // Startwerte speichern
                G.conf = COMMAND_SET_INITIAL_VALUES;
                break;
            }

            if (cc == COMMAND_SET_TIME) {       // Uhrzeit setzen
                G.conf = COMMAND_SET_TIME;
                ii = 0;
                tmp[0] = '\0';
                for (uint8_t k = 12; k < 28; k++) {
                    tmp[ii] = str[k];
                    ii++;
                }
                uint32_t tt = atoi(tmp);
                Serial.printf("Conf: Time: %d\n", tt);
                setTime(tt);
                break;
            }

            if (cc == COMMAND_SET_HOSTNAME) {         // Hostname speichern
                G.conf = COMMAND_SET_HOSTNAME;
                ii = 0;
                for (uint8_t k = 9; k < 25; k++) {
                    if (str[k] != ' ') {
                        G.hostname[ii] = str[k];
                        ii++;
                    }
                }
                G.hostname[ii] = '\0';
                break;
            }

            if (cc == COMMAND_SET_SETTING_SECOND) {       // Anzeige Sekunden speichern
                G.conf = COMMAND_SET_SETTING_SECOND;
                G.zeige_sek = split(9, 3);
                break;
            }

            if (cc == COMMAND_SET_MINUTE) {       // Anzeige Minuten speichern
                G.conf = COMMAND_SET_MINUTE;
                G.zeige_min = split(9, 3);
                break;
            }

            if (cc == COMMAND_SET_LDR) {       // LDR speichern
                G.conf = COMMAND_SET_LDR;
                G.ldr = split(9, 3);
                G.ldrCal = split(12, 3);
                break;
            }

            if (cc == COMMAND_SET_UHRTYPE) {       // UhrType speichern
                G.conf = COMMAND_SET_UHRTYPE;
                G.UhrtypeDef = split(9, 3);
                break;
            }

            if (cc == COMMAND_SET_WEATHER_DATA) {       // Openweathermap speichern
                G.conf = COMMAND_SET_WEATHER_DATA;
                ii = 0;
                for (uint8_t k = 9; k < 16; k++) {
                    if (str[k] != ' ') {
                        G.cityid[ii] = str[k];
                        ii++;
                    }
                }
                G.cityid[ii] = '\0';
                //
                jj = 0;
                for (uint8_t l = 17; l < 49; l++) {
                    if (str[l] != ' ') {
                        G.apikey[jj] = str[l];
                        jj++;
                    }
                }
                G.apikey[jj] = '\0';
                break;
            }

            if (cc == COMMAND_SET_BRIGHTNESS) {       // Helligkeit speichern
                G.conf = COMMAND_SET_BRIGHTNESS;
                G.h6 = split(9, 3);
                G.h8 = split(12, 3);
                G.h12 = split(15, 3);
                G.h16 = split(18, 3);
                G.h18 = split(21, 3);
                G.h20 = split(24, 3);
                G.h22 = split(27, 3);
                G.h24 = split(30, 3);
                break;
            }

            if (cc == COMMAND_SET_MARQUEE_TEXT) {       // Lauftext speichern
                G.conf = COMMAND_SET_MARQUEE_TEXT;
                ii = 0;
                for (uint8_t k = 9; k < 39; k++) {
                    if (str[k] != ' ') {
                        G.ltext[ii] = str[k];
                        ii++;
                    }
                }
                G.ltext[ii] = '\0';
                break;
            }

            if (cc == COMMAND_SET_TIMESERVER) {       // Zeitserver speichern
                G.conf = COMMAND_SET_TIMESERVER;
                ii = 0;
                for (int k = 9; k < 24; k++) {
                    G.zeitserver[ii] = str[k];
                    ii++;
                }
                G.zeitserver[ii] = '\0';
                break;
            }

            if (cc == COMMAND_SET_WIFI_DISABLED) {       // Zeitserver speichern
                G.conf = COMMAND_SET_WIFI_DISABLED;
                break;
            }

            if (cc == COMMAND_SET_WIFI_AND_RESTART) {       // WLAN-Daten speichern und neu starten
                G.conf = COMMAND_SET_WIFI_AND_RESTART;
                ii = 0;
                for (uint8_t k = 9; k < 34; k++) {
                    G.ssid[ii] = str[k];
                    ii++;
                }
                uint8_t index = 0;
                for (int8_t counter = sizeof(G.ssid) / sizeof(G.ssid[0]) - 1; counter > -1; counter--) {
                    if (!isSpace(G.ssid[counter])) {
                        index = counter;
                        break;
                    }
                }
                G.ssid[index + 1] = '\0';

                ii = 0;
                for (int k = 34; k < 59; k++) {
                    if (str[k] != ' ') {
                        G.passwd[ii] = str[k];
                        ii++;
                    }
                }
                G.passwd[ii] = '\0';
                break;
            }

            if (cc == COMMAND_RESET) {      // Reset
                G.conf = COMMAND_RESET;
                break;
            }

            //------------------------------------------------------------------------------

            if (cc == COMMAND_REQUEST_CONFIG_VALUES) {      // Config anfordern
                G.conf = COMMAND_REQUEST_CONFIG_VALUES;
                G.client_nr = num;
                break;
            }

            if (cc == COMMAND_REQUEST_COLOR_VALUES) {      // Farbwerte anfordern
                G.conf = COMMAND_REQUEST_COLOR_VALUES;
                G.client_nr = num;
                break;
            }

            if (cc == COMMAND_REQUEST_WIFI_LIST) {      // Wlan Liste anfordern
                G.conf = COMMAND_REQUEST_WIFI_LIST;
                G.client_nr = num;
                break;
            }

            //--echo data back to browser
            //webSocket.sendTXT(num, payload, lenght);
            //-send data to all connected clients
            //webSocket.broadcastTXT(payload, lenght);
            break;
        }
        case WStype_BIN: {
            Serial.printf("[%u] get binary lenght: %u\n", num, lenght);
            hexdump(payload, lenght);
            //--echo data back to browser
            //webSocket.sendBIN(num, payload, lenght);
            break;
        }
        default:
            break;
    }
}


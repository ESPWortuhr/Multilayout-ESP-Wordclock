#pragma once

#include "SensitiveData.h"
#include "WebPageContent.h"
#include "WebSocketsServer.h"
#include "WordClockState.h"

#define SIZE_OF_FAVICON 185

// Defined in WebPageAdapter.cpp.
extern const char favicon[];

class WebPageAdapter : public WebSocketsServer {

public:
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
                    "Content-Type: image/png\r\n"
                    "Content-Length: %d\r\n"
                    "Connection: close\r\n"
                    "\r\n",
                    SIZE_OF_FAVICON);
            sendHtmlCode(client, (const uint8_t *)favicon, SIZE_OF_FAVICON);
        } else if (url.equals("/")) {
            // ------------------------------------
            client->tcp->write(
                "HTTP/1.1 200 OK\r\n"
                "Server: arduino-WebSocket-Server\r\n"
                "Content-Type: text/html\r\n"
                "Content-Encoding: gzip\r\n" // <--- CRITICAL FOR GZIP
                //--                    "Content-Length: 32\r\n"
                "Connection: close\r\n"
                //--                    "Sec-WebSocket-Version: 13\r\n"
                "\r\n");
            sendHtmlCode(client, html_code, html_size);
        } else {
            snprintf(buf, sizeof(buf),
                     "HTTP/1.1 404 Not Found\r\n"
                     "Content-Type: text/plain\r\n\r\n"
                     "Page %s not found\n",
                     client->cUrl.c_str());
            client->tcp->write(buf);
        }

        clientDisconnect(client);
    }

    //------------------------------------------------------------------------------

    void sendHtmlCode(const WSclient_t *client, const uint8_t *data,
                      uint32_t size) const {
        const uint16_t CHUNK_SIZE = 256;
        uint8_t buf[CHUNK_SIZE];
        uint32_t sent = 0;

        while (sent < size) {
            uint32_t bytesToCopy = min((uint32_t)CHUNK_SIZE, size - sent);
            memcpy_P(buf, data + sent, bytesToCopy);
            client->tcp->write(buf, bytesToCopy);
            sent += bytesToCopy;
            yield();
        }
    }
};

// Defined in WebPageAdapter.cpp.
extern WebPageAdapter webSocket;

// WebSocket event handler; implemented (with its payload helpers) in
// WebPageAdapter.cpp.
void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload,
                    size_t length);

#pragma once

#include "WebSocketsServer.h"
#ifdef Grbw
#include "WebPageWortuhr_RGBW.h"
#else
#include "WebPageWortuhr.h"
#endif

#define RESPONSE_SIZE	900

class WebPage_Adapter : public WebSocketsServer {

	public:
	// forward the port to the parent class constructor
	WebPage_Adapter(int port) : WebSocketsServer(port) {
	}

    /**
     * @param client WSclient_t *  ptr to the client struct
     */
     void handleNonWebsocketConnection(WSclient_t * client) override {
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
        	while (ww < sizeof(index_html)) {
        		str[yy]= pgm_read_byte( &index_html[ww]);
        		str[yy+1] = '\0';
        		yy++;
        		if (yy == RESPONSE_SIZE) {
        			j = strlen(str);
        			client->tcp->write(&str[0], j);
        			str[0]='\0';
        			yy = 0;
        		}
        		ww++;
        	}
        if (yy > 0){
        	j = strlen(str);
        	client->tcp->write(&str[0], j);
        }
        clientDisconnect(client);
    }

};

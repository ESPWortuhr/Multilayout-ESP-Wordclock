/*--------------------------------------------------------------------------
 * Hier wird definiert, welche Anzahl von LED's bzw. Reihen verwendet werden
 */
#define UHR_114                       // Uhr mit 10 Reihen, jeweils 11 LED's pro Reihe + 4 LED's für Minuten
//#define UHR_114_Alternative         // Uhr mit 10 Reihen, jeweils 11 LED's pro Reihe + 4 LED's für Minuten, mit geändertem Layout für extra Wörter in der Matrix
//#define UHR_114_2Clock			  // Uhr mit 10 Reihen, jeweils 11 LED's pro Reihe + 4 LED's für Minuten, mit dem Layout vom orginal Hersteller
//#define UHR_125                       // Uhr mit 11 Reihen, jeweils 11 LED's pro Reihe + 4 LED's für Minuten
//#define UHR_169                     // Uhr mit zusätzlichen LED's um den Rahmen seitlich zu beleuchten
//#define UHR_242                       // Uhr mit Wettervorhersage 242 LED's --> Bitte die Library "ArduinoJson" im Library Manager installieren!

#define SERNR 101             //um das eeprom zu löschen, bzw. zu initialisieren, hier eine andere Seriennummer eintragen!

// Wenn die Farben nicht passen können sie hier angepasst werden:
#define Brg_Color   // RGB-Stripe mit dem Chip WS2812b und dem Layout Brg
//#define Grb_Color      // RGB-Stripe mit dem Chip WS2812b und dem Layout Grb
//#define Rgb_Color    // RGB-Stripe mit dem Chip WS2812b und dem Layout Rgb
//#define Rbg_Color    // RGB-Stripe mit dem Chip WS2812b und dem Layout Rbg
//#define Grbw_Color   // RGBW-Stripe mit dem Chip SK6812 und dem Layout Grbw

#define RTC_Type RTC_DS3231        // External Realtime Clock: RTC_DS1307, RTC_PCF8523 oder RTC_DS3231

bool DEBUG = true;       // DEBUG ON|OFF wenn auskommentiert
//#define VERBOSE          // DEBUG VERBOSE Openweathermap
/*--------------------------------------------------------------------------
 * ENDE Hardware Konfiguration. Ab hier nichts mehr aendern!!!
 *--------------------------------------------------------------------------
 */

#include <Arduino.h>
#include <NeoPixelBus.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>
#include <Hash.h>
#include <Wire.h>
#include <RTClib.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <coredecls.h>
#include <sntp.h>

#include "Uhr.h"
#include "WebPage_Adapter.h"
#include "EEPROMAnything.h"
#include "icons.h"

#include "Uhrtypes/uhr_func_114.hpp"
#include "Uhrtypes/uhr_func_114_Alternative.hpp"
#include "Uhrtypes/uhr_func_114_2Clock.hpp"
#include "Uhrtypes/uhr_func_125.hpp"
#include "Uhrtypes/uhr_func_169.hpp"
#include "Uhrtypes/uhr_func_242.hpp"

UHR_114_t Uhr_114_type;
UHR_114_Alternative_t Uhr_114_Alternative_type;
UHR_114_2Clock_t Uhr_114_2Clock_type;
UHR_125_t Uhr_125_type;
UHR_169_t Uhr_169_type;
UHR_242_t Uhr_242_type;

iUhrType *usedUhrType = nullptr;

NeoPixelBus<NeoBrgFeature, Neo800KbpsMethod> *strip_RGB = NULL;
NeoPixelBus<NeoGrbwFeature, Neo800KbpsMethod> *strip_RGBW = NULL;

WiFiClient client;
WiFiUDP ntpUDP;
PubSubClient mqttClient(client);

// Timezone from https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv
const char TZ_Europe_Berlin[] = "CET-1CEST,M3.5.0,M10.5.0/3";

#ifndef RTC_Type
RTC_DS3231 RTC;
#else
RTC_Type RTC;
#endif

#include "font.h"
#include "mqtt_func.hpp"
#include "uhr_func.hpp"
#include "wifi_func.hpp"
#include "openwmap.h"

//------------------------------------------------------------------------------

iUhrType *getPointer(uint8_t num){
	switch (num){
		case 1:
			return reinterpret_cast<iUhrType *>(&Uhr_114_type);
		case 2:
			return reinterpret_cast<iUhrType *>(&Uhr_114_Alternative_type);
		case 6:
			return reinterpret_cast<iUhrType *>(&Uhr_114_2Clock_type);
		case 3:
			return reinterpret_cast<iUhrType *>(&Uhr_125_type);
		case 4:
			return reinterpret_cast<iUhrType *>(&Uhr_169_type);
		case 5:
			return reinterpret_cast<iUhrType *>(&Uhr_242_type);
		default:
			return nullptr;
	}
}

//------------------------------------------------------------------------------

void InitLedStrip(uint8_t num){
	if (num == Grbw){
		if (strip_RGBW != NULL){
			delete strip_RGBW; // delete the previous dynamically created strip
		}
		strip_RGBW = new NeoPixelBus<NeoGrbwFeature, Neo800KbpsMethod>(usedUhrType->NUM_PIXELS());
		strip_RGBW->Begin();
	}
	else{
		if (strip_RGB != NULL){
			delete strip_RGB; // delete the previous dynamically created strip
		}
		strip_RGB = new NeoPixelBus<NeoBrgFeature, Neo800KbpsMethod>(usedUhrType->NUM_PIXELS());
		strip_RGB->Begin();
	}
}

uint32_t sntp_startup_delay_MS_rfc_not_less_than_60000 () {
	if (externalRTC) {
		Serial.println("SNTP startup delay 1min");
		return 60000;
	} else {
		// yes this is against the RFC, but we don't have an RTC and want the time now.
		Serial.println("no RTC clock - disable SNTP startup delay");
		return 500;
	}
}

void time_is_set() {
	Serial.printf("settimeofday");

	time_t utc = time(nullptr);
	if (externalRTC) {
		RTC.adjust(DateTime(utc));
	}

	struct tm tm;
	localtime_r(&utc, &tm);
	_sekunde = tm.tm_sec;
	_minute = tm.tm_min;
	_stunde = tm.tm_hour;
	if (G.UhrtypeDef == Uhr_169){
		_sekunde48 = _sekunde * 48 / 60;
	}
}

//------------------------------------------------------------------------------
// Start setup()
//------------------------------------------------------------------------------

void setup(){
	//-------------------------------------
	// EEPROM lesen / initialisieren
	//-------------------------------------
	EEPROM.begin(512);

	eeprom_read();

	if (G.sernr != SERNR){
		for (uint16_t i = 0; i < 512; i++) { EEPROM.write(i, i); }
		EEPROM.commit();

		G.sernr = SERNR;
		G.prog = 1;
		G.param1 = 0;
		G.param2 = 0;
		G.prog_init = 1;
		G.conf = COMMAND_IDLE;
		for (uint8_t i = 0; i < 4; i++) { for (uint8_t ii = 0; ii < 4; ii++) { G.rgb[i][ii] = 0; }}
		G.rgb[Foreground][2] = 100;
		G.rgb[Effect][1] = 100;
		G.rr = 0;
		G.gg = 0;
		G.bb = 0;
		G.ww = 0;
		G.hell = 2;
		G.geschw = 10;
		G.client_nr = 0;
		G.zeige_sek = 0;
		G.zeige_min = 1;
		G.ldr = 0;
		G.ldrCal = 0;
		strcpy(G.cityid, "");
		strcpy(G.apikey, "");
		strcpy(G.zeitserver, "europe.pool.ntp.org");
		strcpy(G.hostname, "uhr");
		strcpy(G.ltext, "HELLO WORLD ");

		G.hh = 100;
		G.h6 = 100;
		G.h8 = 100;
		G.h12 = 100;
		G.h16 = 100;
		G.h18 = 100;
		G.h20 = 100;
		G.h22 = 100;
		G.h24 = 100;
		for (uint8_t i = 0; i < 4; i++) { G.Sprachvariation[i] = 0;}

		G.MQTT_State = 0;
		G.MQTT_Port = 1883;
		strcpy(G.MQTT_Server, "192.168.4.1");

#ifdef UHR_114
		G.UhrtypeDef = Uhr_114;
#endif

#ifdef UHR_114_Alternative
		G.UhrtypeDef = Uhr_114_Alternative;
#endif

#ifdef UHR_114_2Clock
		G.UhrtypeDef = Uhr_114_2Clock;
#endif

#ifdef UHR_125
		G.UhrtypeDef = Uhr_125;
#endif

#ifdef UHR_169
		G.UhrtypeDef = Uhr_169;
#endif

#ifdef UHR_242
		G.UhrtypeDef = Uhr_242;
#endif

#ifdef Brg_Color
		G.Colortype = Brg;
#endif

#ifdef Grb_Color
		G.Colortype = Grb;
#endif

#ifdef Rgb_Color
		G.Colortype = Rgb;
#endif

#ifdef Rbg_Color
		G.Colortype = Rbg;
#endif

#ifdef Grbw_Color
		G.Colortype = Grbw;
#endif
		G.bootLedBlink = false;
		G.bootLedSweep = false;
		G.bootShowWifi = true;
		G.bootShowIP = false;

		eeprom_write();
		Serial.println("eeprom schreiben");
	}
	//-------------------------------------
	// Start Serielle Schnittstelle bei Bedarf
	//-------------------------------------
	if (DEBUG == true){
		Serial.begin(115200);
		Serial.println("");
		Serial.println("--------------------------------------");
		Serial.println("Begin Setup");
		Serial.println("--------------------------------------");
	}

	//-------------------------------------
	Serial.println("Starting Telnet server");
	TelnetServer.begin();
	TelnetServer.setNoDelay(true);


	//-------------------------------------
	Serial.println("--------------------------------------");
	Serial.println("ESP Uhr");
	Serial.print("Version         : "), Serial.println(VER);
	Serial.printf("Chip ID         : %08X\n", ESP.getChipId());
	Serial.printf("Flash ID        : %08X\n\n", ESP.getFlashChipId());
	Serial.printf("CPU Speed       : %u MHz \n\n", ESP.getCpuFreqMHz());

	Serial.printf("Flash real Size : %u KByte\n", ESP.getFlashChipRealSize() / 1024);
	Serial.printf("Flash ide  Size : %u KByte\n", ESP.getFlashChipSize() / 1024);
	Serial.printf("Flash ide Speed : %u\n\n", ESP.getFlashChipSpeed());

	Serial.printf("Free Heap Size  : %u Byte\n", ESP.getFreeHeap());
	Serial.printf("Sketch Size     : %u Byte \n", ESP.getSketchSize());
	Serial.printf("Free Sketch Size: %u Byte \n\n", ESP.getFreeSketchSpace());

	Serial.printf("SDK Version     : %s\n", ESP.getSdkVersion());
	Serial.print("RESET Info      : ");
	Serial.println(ESP.getResetInfo());
	Serial.print("COMPILED        : ");
	Serial.print(__DATE__);
	Serial.print(" ");
	Serial.println(__TIME__);

	//-------------------------------------
	// Get Pointer for Uhrtype
	//-------------------------------------

	usedUhrType = getPointer(G.UhrtypeDef);

	//-------------------------------------
	// LEDs initialisieren
	//-------------------------------------
	Serial.println("LED Init");
	InitLedStrip(G.Colortype);
	if (G.bootLedBlink) {
		led_set_all(0x40, 0x40, 0x40, 0x40);
		led_show();
		delay(20);
	}
	if (G.bootLedSweep) {
		led_single(20);
	}
	led_clear();
	led_show();

	G.conf = COMMAND_IDLE;

	//-------------------------------------
	// Start External RealtimeClock
	//-------------------------------------

	if (RTC.begin() == true){
		Serial.println("External RealtimeClock found");
		externalRTC = true;
	}else{
		Serial.println("No external RealtimeClock found");
		externalRTC = false;
	}
	settimeofday_cb(time_is_set);

    //-------------------------------------
    // Init Array
    //-------------------------------------

    for (uint16_t i = 0; i < usedUhrType->NUM_PIXELS(); i++) {
        Word_array_old[i] = 255;
    }

	//-------------------------------------
	// Start WiFi
	//-------------------------------------
	if (G.bootShowWifi) {
		show_icon_wlan(0);
	}
	Network_setup(G.hostname);
	int strength = Network_getQuality();
	Serial.printf("Signal strength: %i\n", strength);
	if (G.bootShowWifi) {
		show_icon_wlan(strength);
	}
	WlanStart();
	configTime(0, 0, G.zeitserver);
	setenv("TZ", TZ_Europe_Berlin, true);
	tzset();

	//-------------------------------------
	// mDNS--
	//-------------------------------------
	MDNS.begin((const char *) G.hostname);
	MDNS.addService("http", "tcp", 80);
	MDNS.addService("http", "tcp", 81);

	//-------------------------------------
	// OTA--
	//-------------------------------------
	httpUpdater.setup(&httpServer);
	httpServer.begin();

	//-------------------------------------
	// MQTT
	//-------------------------------------

	if (G.MQTT_State == 1)
	{
		mqttClient.setServer(G.MQTT_Server, G.MQTT_Port);
		mqttClient.setCallback(MQTT_callback);
		mqttClient.connect("Wortuhr");
		mqttClient.subscribe("/Wortuhr");
	}

	//-------------------------------------
	// Start Websocket
	//-------------------------------------
	webSocket.begin();
	webSocket.onEvent(webSocketEvent);

	Serial.println("Websockest started");
	Serial.println("--------------------------------------");
	Serial.println("Ende Setup");
	Serial.println("--------------------------------------");
	Serial.println("");

	// setup frame
	if (G.UhrtypeDef == Uhr_169 && G.zeige_sek < 1 && G.zeige_min < 2){
		set_farbe_rahmen();
	}
}

//------------------------------------------------------------------------------
// Start loop()
//------------------------------------------------------------------------------

void loop(){
	unsigned long currentMillis = millis();
	previousMillis = currentMillis;
	count_delay += currentMillis - previousMillis;
	if (G.UhrtypeDef == Uhr_169){
		count_millis48 += currentMillis - previousMillis;
	}

	time_t utc = time(nullptr);
	if (utc > 100000000) {
		struct tm tm;
		localtime_r(&utc, &tm);
		_sekunde = tm.tm_sec;
		_minute = tm.tm_min;
		_stunde = tm.tm_hour;
	}

	if (G.UhrtypeDef == Uhr_169){
		if (count_millis48 >= interval48){
			count_millis48 = 0;
			_sekunde48++;
			if (_sekunde48 > 47) { _sekunde48 = 0; }
		}
	}

	//------------------------------------------------
	Telnet();  // Handle telnet connections

	httpServer.handleClient();

	webSocket.loop();

	//------------------------------------------------
	// MQTT
	//------------------------------------------------
	if (G.MQTT_State == 1 && WiFi.status() == WL_CONNECTED)
	{
        if (!mqttClient.connected()) {MQTT_reconnect();}
        mqttClient.loop();
	}

	//------------------------------------------------
	// Sekunde48
	//------------------------------------------------
	if (G.UhrtypeDef == Uhr_169){
		if (last_sekunde48 != _sekunde48){
			if (G.prog == 0 && G.conf == 0){
				if (G.zeige_sek == 1 || G.zeige_min == 2){
					rahmen_clear();
				}
				if (G.zeige_sek > 0){
					show_sekunde();
				}
				led_show();
			}
			last_sekunde48 = _sekunde48;
		}
	}

	//------------------------------------------------
	// Sekunde und LDR Regelung
	//------------------------------------------------
	if (last_sekunde != _sekunde){

		// Wetteruhr
		if (G.UhrtypeDef == Uhr_242){
			weather_tag++;
		}

		//--- LDR Regelung
		if (G.ldr == 1){
			doLDRLogic();
		}

		if (G.prog == 0 && G.conf == 0){
			led_clear();
			show_zeit();
		}
		last_sekunde = _sekunde;

		if (G.UhrtypeDef == Uhr_242){
			if ((_sekunde == 0) | (_sekunde == 10) | (_sekunde == 20) | (_sekunde == 30) | (_sekunde == 40) | (_sekunde == 50)){
				wetterswitch++;
				led_clear();
				if (wetterswitch > 4){
					wetterswitch = 1;
				}
				Serial.print("Wetterswitch: ");
				Serial.println(wetterswitch);
				Serial.print("WStunde: ");
				Serial.println(wstunde);
			}
		}

		if (sntp_getreachability(0)) {
			Serial.printf("%s (%s)\n", ctime(&utc), sntp_getservername(0));
		} else {
			Serial.printf("%s (SNTP not reachable)\n", ctime(&utc));
		}
	}

	//------------------------------------------------
	// Minute
	//------------------------------------------------
	if (last_minute != _minute){
		_sekunde48 = 0;
		last_minute = _minute;
	}

	//------------------------------------------------
	// Wetterdaten abrufen
	//------------------------------------------------
	if (G.UhrtypeDef == Uhr_242 && weather_tag >= 600){    // Eisbaeeer changed for Debug (soll 600)
		weather_tag = 0;
		if (WiFi.status() == WL_CONNECTED)
		{
			getweather();
		}
	}

	Network_loop();

	switch (G.prog)
	{
        //------------------------------------------------
        // Farbe Uhr / Hintergrund / Rahmen einstellen
        //------------------------------------------------
	    case COMMAND_MODE_WORD_CLOCK:
	        {
            show_zeit();
            if (G.UhrtypeDef == Uhr_169 && G.zeige_sek < 1 && G.zeige_min < 2){
                set_farbe_rahmen();
            }
            G.prog = COMMAND_IDLE;
            break;
        }
			//------------------------------------------------
			// Sekunden
			//------------------------------------------------
		case COMMAND_MODE_SECONDS:
		{
			if (G.prog_init == 1)
			{
				led_clear();
				G.prog_init = 0;
			}
			char d1[5];
			char d2[5];
			sprintf(d1, "%d", (int) (_sekunde / 10));
			sprintf(d2, "%d", (int) (_sekunde % 10));
			zahlen(d1[0], d2[0]);
			break;
		}

			//------------------------------------------------
			// Laufschrift
			//------------------------------------------------
		case COMMAND_MODE_MARQUEE:
		{
			if (G.prog_init == 1)
			{
				G.prog_init = 0;
				led_clear();
				count_delay = (G.geschw + 1) * 20;
			}
			if (count_delay >= (G.geschw + 1u) * 20u)
			{
				laufschrift(G.ltext);
				count_delay = 0;
			}
			break;
		}

			//------------------------------------------------
			// Regenbogen
			//------------------------------------------------
		case COMMAND_MODE_RAINBOW:
		{
			if (G.prog_init == 1)
			{
				G.prog_init = 0;
				uhr_clear();
				count_delay = G.geschw * 7 + 1;
			}
			if (count_delay >= G.geschw * 7u + 1u)
			{
				rainbowCycle();
				count_delay = 0;
			}
			break;
		}

			//------------------------------------------------
			// Farbwechsel
			//------------------------------------------------
		case COMMAND_MODE_CHANGE:
		{
			if (G.prog_init == 1)
			{
				G.prog_init = 0;
				led_clear();
				count_delay = G.geschw * 7 + 1;
			}
			if (count_delay >= G.geschw * 7u + 1u)
			{
				rainbow();
				count_delay = 0;
			}
			break;
		}

			//------------------------------------------------
			// Farbe Rahmen
			//------------------------------------------------
		case COMMAND_MODE_COLOR:
		{
			if (G.prog_init == 1)
			{
				G.prog_init = 0;
				set_farbe();
				led_show();
			}
			break;
		}

			//------------------------------------------------
		default:
			break;
	}

	switch (G.conf)
	{
			//------------------------------------------------
			// Reset
			//------------------------------------------------
		case COMMAND_RESET:
		{
			delay(500);
			ESP.reset();
			ESP.restart();
			while (true) {}
			break;
		}

			//------------------------------------------------
			// Config Senden
			//------------------------------------------------
		case COMMAND_REQUEST_CONFIG_VALUES:
		{
			DynamicJsonDocument config(1024);
			config["command"] = "config";
			config["ssid"] = Network_getSSID();
			config["zeitserver"] = G.zeitserver;
			config["hostname"] = G.hostname;
			config["ltext"] = G.ltext;
			config["h6"] = G.h6;
			config["h8"] = G.h8;
			config["h12"] = G.h12;
			config["h16"] = G.h16;
			config["h18"] = G.h18;
			config["h20"] = G.h20;
			config["h22"] = G.h22;
			config["h24"] = G.h24;
			for (uint8_t i = 0; i < 4; i++)
			{
				sprintf(s, "spv%d", i);
				config[s] = G.Sprachvariation[i];
			}
			config["hell"] = G.hell;
			config["zeige_sek"] = G.zeige_sek;
			config["zeige_min"] = G.zeige_min;
			config["ldr"] = G.ldr;
			config["ldrCal"] = G.ldrCal;
			config["cityid"] = G.cityid;
			config["apikey"] = G.apikey;
            config["colortype"] = G.Colortype;
			config["UhrtypeDef"] = G.UhrtypeDef;
			config["MQTT_State"] = G.MQTT_State;
			config["MQTT_Port"] = G.MQTT_State;
			config["MQTT_Server"] = G.MQTT_State;
			config["bootLedBlink"] = G.bootLedBlink;
			config["bootLedSweep"] = G.bootLedSweep;
			config["bootShowWifi"] = G.bootShowWifi;
			config["bootShowIP"] = G.bootShowIP;
			serializeJson(config, str);
			Serial.print("Sending Payload:");
			Serial.println(str);
			webSocket.sendTXT(G.client_nr, str, strlen(str));
			G.conf = COMMAND_IDLE;
			break;
		}

			//------------------------------------------------
			// conf Farbwerte senden
			//------------------------------------------------
		case COMMAND_REQUEST_COLOR_VALUES:
		{
			DynamicJsonDocument config(1024);
			config["command"] = "set";
			for (uint8_t i = 0; i < 4; i++)
			{
				for (uint8_t ii = 0; ii < 4; ii++)
				{
					sprintf(s, "rgb%d%d", i, ii);
					config[s] = G.rgb[i][ii];
				}
			}
			config["hell"] = G.hell;
			config["geschw"] = G.geschw;
			config["colortype"] = G.Colortype;
			serializeJson(config, str);
			webSocket.sendTXT(G.client_nr, str, strlen(str));
			G.conf = COMMAND_IDLE;
			break;
		}

            //------------------------------------------------
            // Uhrzeit setzen
            //------------------------------------------------
        case COMMAND_SET_TIME:
        {
            eeprom_write();
            delay(100);
            G.conf = COMMAND_IDLE;
            break;
        }

            //------------------------------------------------
            // Startwerte speichern
            //------------------------------------------------
        case COMMAND_SET_INITIAL_VALUES:
        {
            Serial.println("Startwerte gespeichert");
            Serial.println(G.rgb[Foreground][0]);
            Serial.println(G.rgb[Foreground][1]);
            Serial.println(G.rgb[Foreground][2]);
            Serial.println(G.rgb[Foreground][3]);
            eeprom_write();
            delay(100);
            G.conf = COMMAND_IDLE;
            break;
        }

            //------------------------------------------------
            // Helligkeit speichern
            //------------------------------------------------
        case COMMAND_SET_BRIGHTNESS:
        {
            eeprom_write();
            delay(100);
            G.conf = COMMAND_IDLE;
            break;
        }

            //------------------------------------------------
            // Anzeige Minuten speichern
            //------------------------------------------------
        case COMMAND_SET_MINUTE:
        {
            eeprom_write();
            delay(100);
            G.conf = COMMAND_IDLE;
            break;
        }

            //------------------------------------------------
            // LDR Einstellung speichern
            //------------------------------------------------
        case COMMAND_SET_LDR:
        {
            eeprom_write();
            delay(100);
            Serial.printf("LDR : %u\n\n", G.ldr);
            Serial.printf("LDR Kalibrierung: %u\n\n", G.ldrCal);
            G.conf = COMMAND_IDLE;
            break;
        }

			//------------------------------------------------
			// Sprachvarianten Einstellungen
			//------------------------------------------------
		case COMMAND_SET_LANGUAGE_VARIANT:
		{
			eeprom_write();
			led_clear();
			show_zeit();
			G.conf = COMMAND_IDLE;
			break;
		}

			//------------------------------------------------
			// MQTT Einstellungen
			//------------------------------------------------
		case COMMAND_SET_MQTT:
		{
			if (!mqttClient.connected()) {
				mqttClient.connect("Wortuhr");
				MQTT_reconnect();
			}
			eeprom_write();
			G.conf = COMMAND_IDLE;
			break;
		}

			//------------------------------------------------
			// Uhrzeit manuell einstellen
			//------------------------------------------------
		case COMMAND_SET_TIME_MANUAL:
		{
			Serial.println("Uhrzeit manuell eingstellt");
			led_clear();
			show_zeit();
			G.conf = COMMAND_IDLE;
			break;
		}

            //------------------------------------------------
            // Colortype der LED einstellen
            //------------------------------------------------
        case COMMAND_SET_COLORTYPE:
        {
            eeprom_write();
            Serial.printf("LED Colortype: %u\n", G.Colortype);
            if (G.Colortype == Grbw)
            {
                G.conf = COMMAND_RESET;
            }
            else
            {
                G.conf = COMMAND_IDLE;
            }
            break;
        }

            //------------------------------------------------
            // Uhrtype Layout einstellen
            //------------------------------------------------
        case COMMAND_SET_UHRTYPE:
        {
            eeprom_write();
            Serial.printf("Uhrtype: %u\n", G.UhrtypeDef);
            G.conf = COMMAND_RESET;
            break;
        }

            //------------------------------------------------
            // OpenWeathermap Einstellung speichern
            //------------------------------------------------
        case COMMAND_SET_WEATHER_DATA:
        {
            Serial.println("write EEPROM!");
            Serial.print("CityID : ");
            Serial.println(G.cityid);
            Serial.print("APIkey : ");
            Serial.println(G.apikey);
            eeprom_write();
            delay(100);
            G.conf = COMMAND_IDLE;
            break;
        }

            //------------------------------------------------
            // Hostname speichern
            //------------------------------------------------
        case COMMAND_SET_HOSTNAME:
        {
            Serial.print("Hostname: ");
            Serial.println(G.hostname);
            eeprom_write();
            Network_reboot();
            G.conf = COMMAND_IDLE;
            break;
        }

            //------------------------------------------------
            // Anzeige Sekunde speichern
            //------------------------------------------------
        case COMMAND_SET_SETTING_SECOND:
        {
            show_zeit();
            eeprom_write();
            delay(100);
            G.conf = COMMAND_IDLE;
            break;
        }

            //------------------------------------------------
            // Lauftext speichern
            //------------------------------------------------
        case COMMAND_SET_MARQUEE_TEXT:
        {
            eeprom_write();
            delay(100);
            G.conf = COMMAND_IDLE;
            break;
        }

            //------------------------------------------------
            // Zeitserver speichern
            //------------------------------------------------
        case COMMAND_SET_TIMESERVER:
        {
            eeprom_write();
            configTime(0, 0, G.zeitserver);
            G.conf = COMMAND_SET_TIME;
            break;
        }

            //------------------------------------------------
            // Bootoptionen speichern
            //------------------------------------------------
        case COMMAND_SET_BOOT:
        {
            eeprom_write();
            G.conf = COMMAND_IDLE;
            break;
        }

            //------------------------------------------------
            // WLAN-Daten speichern und neu starten
            //------------------------------------------------
        case COMMAND_SET_WIFI_DISABLED:
        {
            eeprom_write();
            delay(100);
            Serial.println("Conf: WLAN Abgeschaltet");
            Network_disable();
            G.conf = COMMAND_IDLE;
            break;
        }

            //------------------------------------------------
            // WLAN-Daten speichern und neu starten
            //------------------------------------------------
        case COMMAND_SET_WIFI_AND_RESTART:
        {
            Serial.println("Conf: WLAN neu konfiguriert");
            Network_resetSettings();
            G.conf = COMMAND_IDLE;
            break;
        }

            //------------------------------------------------
        default:
            break;

	}

	if (count_delay > 10000) { count_delay = 0; }
}

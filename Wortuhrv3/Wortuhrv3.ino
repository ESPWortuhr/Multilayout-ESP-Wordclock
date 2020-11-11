/*--------------------------------------------------------------------------
 * Hier wird definiert, welche Anzahl von LED's bzw. Reihen verwendet werden
 */
#define UHR_114                       // Uhr mit 10 Reihen, jeweils 11 LED's pro Reihe + 4 LED's für Minuten
//#define UHR_114_Alternative         // Uhr mit 10 Reihen, jeweils 11 LED's pro Reihe + 4 LED's für Minuten, mit geändertem Layout für extra Wörter in der Matrix
//#define UHR_125                       // Uhr mit 11 Reihen, jeweils 11 LED's pro Reihe + 4 LED's für Minuten
//#define UHR_169                     // Uhr mit zusätzlichen LED's um den Rahmen seitlich zu beleuchten
//#define UHR_242                       // Uhr mit Wettervorhersage 242 LED's --> Bitte die Library "ArduinoJson" im Library Manager installieren!

#define SERNR 100             //um das eeprom zu löschen, bzw. zu initialisieren, hier eine andere Seriennummer eintragen!

// Wenn die Farben nicht passen können sie hier angepasst werden:
#define Brg_Color   // RGB-Stripe mit dem Chip WS2812b und dem Layout Brg
//#define Grb_Color      // RGB-Stripe mit dem Chip WS2812b und dem Layout Grb
//#define Rgb_Color    // RGB-Stripe mit dem Chip WS2812b und dem Layout Rgb
//#define Rbg_Color    // RGB-Stripe mit dem Chip WS2812b und dem Layout Rbg
//#define Grbw_Color   // RGBW-Stripe mit dem Chip SK6812 und dem Layout Grbw

#define RTC_Type RTC_DS3231        // External Realtime Clock: RTC_DS1307, RTC_PCF8523 oder RTC_DS3231

bool DEBUG = true;       // DEBUG ON|OFF wenn auskommentiert
//#define VERBOSE          // DEBUG VERBOSE Openweathermap
bool show_ip = true;      // Zeige IP Adresse beim Start
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
#include <NTPClient.h>
#include <Hash.h>
#include <TimeLib.h>
#include <Timezone.h>
#include <Wire.h>
#include <RTClib.h>

#include "Uhr.h"
#include "WebPage_Adapter.h"
#include "EEPROMAnything.h"

#include "Uhrtypes/uhr_func_114_Alternative.hpp"
#include "Uhrtypes/uhr_func_114.hpp"
#include "Uhrtypes/uhr_func_125.hpp"
#include "Uhrtypes/uhr_func_169.hpp"
#include "Uhrtypes/uhr_func_242.hpp"

UHR_114_Alternative_t Uhr_114_Alternative_type;
UHR_114_t Uhr_114_type;
UHR_125_t Uhr_125_type;
UHR_169_t Uhr_169_type;
UHR_242_t Uhr_242_type;

iUhrType *usedUhrType = nullptr;

NeoPixelBus<NeoBrgFeature, Neo800KbpsMethod> *strip_RGB = NULL;
NeoPixelBus<NeoGrbwFeature, Neo800KbpsMethod> *strip_RGBW = NULL;

TimeChangeRule CEST = {"CEST", Last, Sun, Mar, 2, 120};     //Central European Summer Time
TimeChangeRule CET = {"CET ", Last, Sun, Oct, 3, 60};       //Central European Standard Time
Timezone tzc(CEST, CET);

time_t ltime, utc;
TimeChangeRule *tcr;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, G.zeitserver);

#ifndef RTC_Type
RTC_DS3231 RTC;
#else
RTC_Type RTC;
#endif

#include "font.h"
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
		for (int i = 0; i < 512; i++) { EEPROM.write(i, i); }
		EEPROM.commit();

		G.sernr = SERNR;
		strcpy(G.ssid, "Enter_Your_SSID");
		strcpy(G.passwd, "Enter_Your_PASSWORD");
		G.prog = 1;
		G.param1 = 0;
		G.param2 = 0;
		G.prog_init = 1;
		G.conf = COMMAND_IDLE;
		for (int i = 0; i < 4; i++) { for (int ii = 0; ii < 4; ii++) { G.rgb[i][ii] = 0; }}
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

#ifdef UHR_114_Alternative
		G.UhrtypeDef = Uhr_114_Alternative;
#endif

#ifdef UHR_114
		G.UhrtypeDef = Uhr_114;
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
	led_single(20);
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

	//-------------------------------------
	// Start WiFi
	//-------------------------------------
	WlanStart();

	//-------------------------------------
	// Zeit setzen
	//-------------------------------------
	utc = now();    //current time from the Time Library
	ltime = tzc.toLocal(utc, &tcr);
	_sekunde = second(ltime);
	_minute = minute(ltime);
	_stunde = hour(ltime);
	if (G.UhrtypeDef == Uhr_169){
		_sekunde48 = _sekunde * 48 / 60;
	}
	show_zeit(1);
	if (G.UhrtypeDef == Uhr_169 && G.zeige_sek < 1 && G.zeige_min < 2){
		set_farbe_rahmen();
	}

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
}

//------------------------------------------------------------------------------
// Start loop()
//------------------------------------------------------------------------------

void loop(){
	unsigned long currentMillis = millis();
	count_millis += currentMillis - previousMillis;
	count_delay += currentMillis - previousMillis;
	if (G.UhrtypeDef == Uhr_169){
		count_millis48 += currentMillis - previousMillis;
	}
	previousMillis = currentMillis;
	if (count_millis >= interval){
		count_millis = 0;
		utc = now();    //current time from the Time Library
		ltime = tzc.toLocal(utc, &tcr);
		_sekunde = second(ltime);
		_minute = minute(ltime);
		_stunde = hour(ltime);
		count_tag++;
		// Wetteruhr
		if (G.UhrtypeDef == Uhr_242){
			weather_tag++;
		}

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

		//--- LDR Regelung
		//
		if (G.ldr == 1){
			doLDRLogic();
		}
		//--- LDR Regelung

		if (G.prog == 0 && G.conf == 0){
			show_zeit(0); // Anzeige Uhrzeit ohne Config
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
	}

	//------------------------------------------------
	// Minute
	//------------------------------------------------
	if (last_minute != _minute){
		Serial.println(">>>> Begin Minute <<<<");
		TelnetMsg(">>>> Begin Minute <<<<");

		if (G.prog == 0 && G.conf == 0){
			led_clear();
			show_zeit(1); // Anzeige Uhrzeit mit Config
		}

		//Deaktiviere WLAN nach 5 Minuten AP Zeit
		WiFiCheck_AP_Status();

		// WLAN reconnect
		wlan_status = WiFi.status();
		Serial.printf("WLAN-Status: %s\n", wstatus[wlan_status]);
		TelnetMsg("WLAN-Status: ");
		TelnetMsg(wstatus[wlan_status]);

		_sekunde48 = 0;
		last_minute = _minute;

		Serial.printf("%u.%u.%u %u:%u:%u \n", day(ltime), month(ltime), year(ltime), hour(ltime), minute(ltime), second(ltime));
		Serial.println(">>>> Ende  Minute <<<<");
		TelnetMsg(">>>> Ende  Minute <<<<");
	}

	//------------------------------------------------
	// Stunde
	//------------------------------------------------
	if (last_stunde != _stunde){
		Serial.println(">>>> Begin Stunde <<<<");
		TelnetMsg(">>>> Begin Stunde <<<<");
		// WLAN testen
		wlan_status = WiFi.status();
		Serial.printf("WLAN-Status: %s\n", wstatus[wlan_status]);
		TelnetMsg("WLAN-Status: ");
		TelnetMsg(wstatus[wlan_status]);
		if (wlan_client == false && wlan_ssid == true && AP_Status != 6){
			WlanStart();
		}

		last_stunde = _stunde;
		Serial.println(">>>> Ende  Stunde <<<<");
		TelnetMsg(">>>> Ende  Stunde <<<<");
	}

	//------------------------------------------------
	// Tag
	//------------------------------------------------
	if (count_tag >= 86400){
		count_tag = 0;
		ntp_flag = true;
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

	//------------------------------------------------
	// NTP Zeit neu holen
	//------------------------------------------------
	if (ntp_flag == true){
		Serial.println("npt: Neue Zeit holen");
		TelnetMsg("npt: Neue Zeit holen");
		ntp_flag = false;
		wlan_status = WiFi.status();
		if (wlan_status == 3){
			timeClient.update();
			unix_time = timeClient.getEpochTime();
			if (unix_time > 0){
				setTime(unix_time);
				Serial.println(unix_time);
				utc = now();    //current time from the Time Library
				ltime = tzc.toLocal(utc, &tcr);
				Serial.print(hour(ltime));
				Serial.print(":");
				Serial.print(minute(ltime));
				Serial.print(":");
				Serial.print(second(ltime));
				Serial.print(" - ");
				Serial.print(day(ltime));
				Serial.print(".");
				Serial.print(month(ltime));
				Serial.print(".");
				Serial.println(year(ltime));
			}
		}
	}

	//------------------------------------------------
	// Farbe Uhr / Hintergrund / Rahmen einstellen
	//------------------------------------------------
	if (G.prog == COMMAND_MODE_WORD_CLOCK){
		show_zeit(0); // Anzeige Uhrzeit ohne Config
		if (G.UhrtypeDef == Uhr_169 && G.zeige_sek < 1 && G.zeige_min < 2){
			set_farbe_rahmen();
		}
		G.prog = COMMAND_IDLE;
	}

	switch (G.conf)
	{
		//------------------------------------------------
		// Uhrzeit setzen
		//------------------------------------------------
		case COMMAND_SET_TIME:
		{
			utc = now();    //current time from the Time Library
			ltime = tzc.toLocal(utc, &tcr);
			_sekunde = second(ltime);
			_minute = minute(ltime);
			_stunde = hour(ltime);
			if (G.UhrtypeDef == Uhr_169)
			{
				_sekunde48 = _sekunde * 48 / 60;
			}
			show_zeit(1); // Anzeige Uhrzeit mit Config
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
			show_zeit(1); // Anzeige Uhrzeit mit Config
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
			show_zeit(1); // Anzeige Uhrzeit mit Config
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
			// Colortype der LED einstellen
			//------------------------------------------------
		case COMMAND_SET_WPS_MODE:
		{
			Serial.printf("WiFi WPS Mode aktiviert");
			WiFiStart_WPS();
			eeprom_write();
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
			MDNS.setHostname((const char *) G.hostname);
			eeprom_write();
			delay(100);
			G.conf = COMMAND_IDLE;
			break;
		}

			//------------------------------------------------
			// Anzeige Sekunde speichern
			//------------------------------------------------
		case COMMAND_SET_SETTING_SECOND:
		{
			show_zeit(1); // Anzeige Uhrzeit mit Config
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
			timeClient.end();
			NTPClient timeClient(ntpUDP, G.zeitserver);
			timeClient.begin();
			delay(100);
			timeClient.update();
			eeprom_write();
			delay(100);
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
			WiFi.forceSleepBegin();
			G.conf = COMMAND_IDLE;
			break;
		}

			//------------------------------------------------
			// WLAN-Daten speichern und neu starten
			//------------------------------------------------
		case COMMAND_SET_WIFI_AND_RESTART:
		{
			eeprom_write();
			delay(100);
			Serial.println("Conf: WLAN neu konfiguriert");
			WlanStart();
			G.conf = COMMAND_IDLE;
			break;
		}

			//------------------------------------------------
		default:
			break;
	}

	switch (G.prog)
	{
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
			delay(1000);
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
			strcpy(str, R"({"command":"config")");
			strcat(str, ",\"ssid\":");
			strcat(str, "\"");
			strcat(str, G.ssid);
			strcat(str, R"(","zeitserver":")");
			strcat(str, G.zeitserver);
			strcat(str, R"(","hostname":")");
			strcat(str, G.hostname);
			strcat(str, R"(","ltext":")");
			strcat(str, G.ltext);
			strcat(str, R"(","h6":")");
			sprintf(s, "%d", G.h6);
			strcat(str, s);
			strcat(str, R"(","h8":")");
			sprintf(s, "%d", G.h8);
			strcat(str, s);
			strcat(str, R"(","h12":")");
			sprintf(s, "%d", G.h12);
			strcat(str, s);
			strcat(str, R"(","h16":")");
			sprintf(s, "%d", G.h16);
			strcat(str, s);
			strcat(str, R"(","h18":")");
			sprintf(s, "%d", G.h18);
			strcat(str, s);
			strcat(str, R"(","h20":")");
			sprintf(s, "%d", G.h20);
			strcat(str, s);
			strcat(str, R"(","h22":")");
			sprintf(s, "%d", G.h22);
			strcat(str, s);
			strcat(str, R"(","h24":")");
			sprintf(s, "%d", G.h24);
			strcat(str, s);
			strcat(str, R"(","hell":")");
			sprintf(s, "%d", G.hell);
			strcat(str, s);
			strcat(str, R"(","zeige_sek":")");
			sprintf(s, "%d", G.zeige_sek);
			strcat(str, s);
			strcat(str, R"(","zeige_min":")");
			sprintf(s, "%d", G.zeige_min);
			strcat(str, s);
			strcat(str, R"(","ldr":")");
			sprintf(s, "%d", G.ldr);
			strcat(str, s);
			strcat(str, R"(","ldrCal":")");
			sprintf(s, "%d", G.ldrCal);
			strcat(str, s);
			strcat(str, R"(","cityid":")");
			strcat(str, G.cityid);
			strcat(str, R"(","apikey":")");
			strcat(str, G.apikey);
			strcat(str, R"(","UhrtypeDef":")");
			sprintf(s, "%d", G.UhrtypeDef);
			strcat(str, s);
			strcat(str, R"(","colortype":")");
			sprintf(s, "%d", G.Colortype);
			strcat(str, s);
			strcat(str, "\"}");
			webSocket.sendTXT(G.client_nr, str, strlen(str));
			G.conf = COMMAND_IDLE;
			break;
		}

			//------------------------------------------------
			// conf Farbwerte senden
			//------------------------------------------------
		case COMMAND_REQUEST_COLOR_VALUES:
		{
			strcpy(str, R"({"command":"set")");
			for (uint8_t i = 0; i < 4; i++)
			{
				for (uint8_t ii = 0; ii < 4; ii++)
				{
					strcat(str, ",\"rgb");
					sprintf(s, "%d", i);
					strcat(str, s);
					sprintf(s, "%d", ii);
					strcat(str, s);
					strcat(str, "\":\"");
					sprintf(s, "%d", G.rgb[i][ii]);
					strcat(str, s);
					strcat(str, "\"");
				}
			}
			strcat(str, R"(,"hell":")");
			sprintf(s, "%d", G.hell);
			strcat(str, s);
			strcat(str, R"(","geschw":")");
			sprintf(s, "%d", G.geschw);
			strcat(str, s);
			strcat(str, R"(","colortype":")");
			sprintf(s, "%d", G.Colortype);
			strcat(str, s);
			strcat(str, "\"}");
			webSocket.sendTXT(G.client_nr, str, strlen(str));
			G.conf = COMMAND_IDLE;
			break;
		}

			//------------------------------------------------
			// Wlan Liste
			//------------------------------------------------

		case COMMAND_REQUEST_WIFI_LIST:
		{
			String strs = R"({"command":"wlan")";
			strs += R"(,"list":")";
			strs += WiFiScan(true);
			strs += "\"}";
			webSocket.sendTXT(G.client_nr, strs);
			G.conf = COMMAND_IDLE;
			break;
		}

			//------------------------------------------------
		default:
			break;
	}

	if (count_delay > 10000) { count_delay = 0; }
}

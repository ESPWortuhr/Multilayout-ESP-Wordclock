/*--------------------------------------------------------------------------
 * Hier wird definiert, welche Anzahl von LED's bzw. Reihen verwendet werden
 */
//#define UHR_114                       // Uhr mit 10 Reihen, jeweils 11 LED's pro Reihe + 4 LED's für Minuten
#define UHR_114_Alternative         // Uhr mit 10 Reihen, jeweils 11 LED's pro Reihe + 4 LED's für Minuten, mit geändertem Layout für extra Wörter in der Matrix
//#define UHR_125                       // Uhr mit 11 Reihen, jeweils 11 LED's pro Reihe + 4 LED's für Minuten
//#define UHR_169                     // Uhr mit zusätzlichen LED's um den Rahmen seitlich zu beleuchten
//#define UHR_242                       // Uhr mit Wettervorhersage 242 LED's --> Bitte die Library "ArduinoJson" im Library Manager installieren!

#define SERNR 100             //um das eeprom zu löschen, bzw. zu initialisieren, hier eine andere Seriennummer eintragen!

// Wenn die Farben nicht passen können sie hier angepasst werden:
//#define Brg   // RGB-Stripe mit dem Chip WS2812b und dem Layout Brg
#define Grb      // RGB-Stripe mit dem Chip WS2812b und dem Layout Grb
//#define Rgb    // RGB-Stripe mit dem Chip WS2812b und dem Layout Rgb
//#define Rbg    // RGB-Stripe mit dem Chip WS2812b und dem Layout Rbg
//#define Grbw   // RGBW-Stripe mit dem Chip SK6812 und dem Layout Grbw

#define RTC_Type RTC_DS3231    	// External Realtime Clock: RTC_DS1307, RTC_PCF8523 oder RTC_DS3231

bool DEBUG = true;       // DEBUG ON|OFF wenn auskommentiert
bool show_ip = false;      // Zeige IP Adresse beim Start
unsigned int NTP_port = 123;  // Standartport für den NTP Server
const char *NtpServerName = "europe.pool.ntp.org";  // NTP Zeitserver
/*--------------------------------------------------------------------------
 * ENDE Hardware Konfiguration. Ab hier nichts mehr aendern!!!
 *--------------------------------------------------------------------------
 */

//---------------------------------------------------------
// WLAN-Status
//---------------------------------------------------------
char wstatus[7][25] = {

		"WL_IDLE_STATUS", "WL_NO_SSID_AVAIL", "WL_SCAN_COMPLETED", "WL_CONNECTED", "WL_CONNECT_FAILED", "WL_CONNECTION_LOST", "WL_DISCONNECTED"};
// WL_NO_SHIELD        = 255,   // for compatibility with WiFi Shield library
// WL_IDLE_STATUS      = 0,
// WL_NO_SSID_AVAIL    = 1,
// WL_SCAN_COMPLETED   = 2,
// WL_CONNECTED        = 3,
// WL_CONNECT_FAILED   = 4,
// WL_CONNECTION_LOST  = 5,
// WL_DISCONNECTED     = 6
//---------------------------------------------------------
// WIFI_EVENT_STAMODE_CONNECTED = 0,
// WIFI_EVENT_STAMODE_DISCONNECTED
// WIFI_EVENT_STAMODE_AUTHMODE_CHANGE
// WIFI_EVENT_STAMODE_GOT_IP
// WIFI_EVENT_STAMODE_DHCP_TIMEOUT
// WIFI_EVENT_SOFTAPMODE_STACONNECTED
//---------------------------------------------------------

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

#include "EEPROMAnything.h"
#include "WebPage_Adapter.h"
#include "Uhr.h"
#include "font.h"
#include "uhr_func.hpp"

//--OpenWeatherMapOrg
#include "openwmap.h"

TimeChangeRule CEST = {"CEST", Last, Sun, Mar, 2, 120};     //Central European Summer Time
TimeChangeRule CET = {"CET ", Last, Sun, Oct, 3, 60};       //Central European Standard Time
Timezone tzc(CEST, CET);

time_t ltime, utc;
TimeChangeRule *tcr;
WiFiUDP ntpUDP;

#ifndef RTC_Type
	RTC_DS3231 RTC;
#else
	RTC_Type RTC;
#endif

NTPClient timeClient(ntpUDP);

bool externalRTC = false;

uint32_t split(uint8_t i, uint8_t j)
{
	char dest[3];
  int m;
  m = 0;
  for (int k = i; k < (i + j); k++)
  {
    dest[m] = str[k];
    m++;
  }
  return atoi(dest);
}

//------------------------------------------------------------------------------

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t lenght)
{
	int cc = 0;
	int ii;
	int jj;
	char tmp[30];
	payload = (payload == NULL) ? (uint8_t *) "" : payload;
	Serial.printf("Client-Nr.: [%u]  WStype: %u payload: %s\n", num, type, payload);

	switch (type)
	{
		case WStype_DISCONNECTED:
		{
			Serial.printf("[%u] Disconnected!\n", num);
			break;
		}
		case WStype_CONNECTED:
		{
			IPAddress ip = webSocket.remoteIP(num);
			Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
			break;
		}
		case WStype_TEXT:
		{
			Serial.printf("[%u] get Text: %s\n", lenght, payload);
			for (unsigned int k = 0; k < lenght; k++)
			{
				str[k] = payload[k];  //does this "copy" is buggy code?
			}

			cc = split(0, 3);
			G.param1 = split(3, 3);
			G.param2 = split(6, 3);
			if (cc == 1)
			{        // Uhrzeit Vordergrund Farbe einstellen
				G.prog = 1;
#ifdef Grbw
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

				G.rgb[SpecialFunction][0] = split(45, 3);
				G.rgb[SpecialFunction][1] = split(48, 3);
				G.rgb[SpecialFunction][2] = split(51, 3);
				G.rgb[SpecialFunction][3] = split(54, 3);
#else
				G.rgb[Foreground][0] = split(9, 3);
				G.rgb[Foreground][1] = split(12, 3);
				G.rgb[Foreground][2] = split(15, 3);

				G.rgb[Background][0] = split(18, 3);
				G.rgb[Background][1] = split(21, 3);
				G.rgb[Background][2] = split(24, 3);

				G.rgb[Frame][0] = split(27, 3);
				G.rgb[Frame][1] = split(30, 3);
				G.rgb[Frame][2] = split(33, 3);

				G.rgb[SpecialFunction][0] = split(36, 3);
				G.rgb[SpecialFunction][1] = split(39, 3);
				G.rgb[SpecialFunction][2] = split(42, 3);
#endif
				break;
			}

			if (cc == 200)
			{      // Sekunden
				G.prog = 200;
				if (G.param1 == 0) { G.prog_init = 1; }
#ifdef Grbw
				G.rgb[SpecialFunction][0] = split(45, 3);
				G.rgb[SpecialFunction][1] = split(48, 3);
				G.rgb[SpecialFunction][2] = split(51, 3);
				G.rgb[SpecialFunction][3] = split(54, 3);
				G.hell = split(57, 3);
				G.geschw = split(60, 3);
#else
				G.rgb[SpecialFunction][0] = split(36, 3);
				G.rgb[SpecialFunction][1] = split(39, 3);
				G.rgb[SpecialFunction][2] = split(42, 3);
				G.hell = split(45, 3);
				G.geschw = split(48, 3);
#endif
				break;
			}

			if (cc == 201)
			{      // Laufschrift
				G.prog = 201;
				if (G.param1 == 0) { G.prog_init = 1; }
#ifdef Grbw
				G.rgb[SpecialFunction][0] = split(45, 3);
				G.rgb[SpecialFunction][1] = split(48, 3);
				G.rgb[SpecialFunction][2] = split(51, 3);
				G.rgb[SpecialFunction][3] = split(54, 3);
				G.hell = split(57, 3);
				G.geschw = split(60, 3);
#else
				G.rgb[SpecialFunction][0] = split(36, 3);
				G.rgb[SpecialFunction][1] = split(39, 3);
				G.rgb[SpecialFunction][2] = split(42, 3);
				G.hell = split(45, 3);
				G.geschw = split(48, 3);
#endif
				break;
			}
			if (cc == 202)
			{      // Regenbogen
				G.prog = 202;
				G.prog_init = 1;
#ifdef Grbw
				G.hell = split(57, 3);
				G.geschw = split(60, 3);
#else
				G.hell = split(45, 3);
				G.geschw = split(48, 3);
#endif
				break;
			}
			if (cc == 203)
			{      // Farbwechsel
				G.prog = 203;
				G.prog_init = 1;
#ifdef Grbw
				G.hell = split(57, 3);
				G.geschw = split(60, 3);
#else
				G.hell = split(45, 3);
				G.geschw = split(48, 3);
#endif
				break;
			}
			if (cc == 204)
			{      // Farbe
				G.prog = 204;
				G.prog_init = 1;
#ifdef Grbw
				G.rgb[SpecialFunction][0] = split(45, 3);
				G.rgb[SpecialFunction][1] = split(48, 3);
				G.rgb[SpecialFunction][2] = split(51, 3);
				G.rgb[SpecialFunction][3] = split(54, 3);
#else
				G.rgb[SpecialFunction][0] = split(36, 3);
				G.rgb[SpecialFunction][1] = split(39, 3);
				G.rgb[SpecialFunction][2] = split(42, 3);
#endif
				break;
			}

			if (cc == 251)
			{      // Helligkeit
#ifdef Grbw
				G.hell = split(57, 3);
#else
				G.hell = split(45, 3);
#endif
				break;
			}
			if (cc == 252)
			{       // Geschwindigkeit
#ifdef Grbw
				G.geschw = split(60, 3);
#else
				G.geschw = split(48, 3);
#endif
				break;
			}
			if (cc == 20)
			{       // Startwerte speichern
				G.conf = 20;
				break;
			}

			if (cc == 30)
			{       // Uhrzeit setzen
				G.conf = 30;
				ii = 0;
				tmp[0] = '\0';
				for (int k = 12; k < 28; k++)
				{
					tmp[ii] = str[k];
					ii++;
				}
				uint32_t tt = atoi(tmp);
				Serial.printf("Conf: Time: %d\n", tt);
				setTime(tt);
				break;
			}
			if (cc == 92)
			{         // Hostname speichern
				G.conf = 92;
				ii = 0;
				for (int k = 9; k < 25; k++)
				{
					if (str[k] != ' ')
					{
						G.hostname[ii] = str[k];
						ii++;
					}
				}
				G.hostname[ii] = '\0';
				break;
			}
			if (cc == 93)
			{       // Anzeige Sekunden speichern
				G.conf = 93;
				G.zeige_sek = split(9, 3);
				break;
			}
			if (cc == 94)
			{       // Anzeige Minuten speichern
				G.conf = 94;
				G.zeige_min = split(9, 3);
				break;
			}
			if (cc == 91)
			{       // LDR speichern
				G.conf = 91;
				G.ldr = split(9, 3);
				G.ldrCal = split(12, 3);
				break;
			}
			if (cc == 90)
			{       // Openweathermap speichern
				G.conf = 90;
				ii = 0;
				for (int k = 9; k < 16; k++)
				{
					if (str[k] != ' ')
					{
						G.cityid[ii] = str[k];
						ii++;
					}
				}
				G.cityid[ii] = '\0';
				//
				jj = 0;
				for (int l = 17; l < 49; l++)
				{
					if (str[l] != ' ')
					{
						G.apikey[jj] = str[l];
						jj++;
					}
				}
				G.apikey[jj] = '\0';
				break;
			}
			if (cc == 95)
			{       // Helligkeit speichern
				G.conf = 95;
				G.conf = 95;
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
			if (cc == 96)
			{       // Lauftext speichern
				G.conf = 96;
				ii = 0;
				for (int k = 9; k < 39; k++)
				{
					if (str[k] != ' ')
					{
						G.ltext[ii] = str[k];
						ii++;
					}
				}
				G.ltext[ii] = '\0';
				break;
			}
			if (cc == 97)
			{       // Zeitserver speichern
				G.conf = 97;
				ii = 0;
				for (int k = 9; k < 24; k++)
				{
					G.zeitserver[ii] = str[k];
					ii++;
				}
				G.zeitserver[ii] = '\0';
				break;
			}
			if (cc == 99)
			{       // WLAN-Daten speichern und neu starten
				G.conf = 99;
				ii = 0;
				for (int k = 9; k < 34; k++)
				{
					if (str[k] != ' ')
					{
						G.ssid[ii] = str[k];
						ii++;
					}
				}
				G.ssid[ii] = '\0';
				ii = 0;
				for (int k = 34; k < 59; k++)
				{
					if (str[k] != ' ')
					{
						G.passwd[ii] = str[k];
						ii++;
					}
				}
				G.passwd[ii] = '\0';
				break;
			}
			if (cc == 100)
			{      // Reset
				G.conf = 100;
				break;
			}

			if (cc == 300)
			{      // Config anfordern
				G.conf = 300;
				G.client_nr = num;
				break;
			}
			if (cc == 301)
			{      // Farbwerte anfordern
				G.conf = 301;
				G.client_nr = num;
				break;
			}
			if (cc == 302)
			{      // Wlan Liste anfordern
				G.conf = 302;
				G.client_nr = num;
				break;
			}

			//--echo data back to browser
			//webSocket.sendTXT(num, payload, lenght);
			//-send data to all connected clients
			//webSocket.broadcastTXT(payload, lenght);
			break;
		}
		case WStype_BIN:
		{
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

//------------------------------------------------------------------------------

String WiFiScan(bool html)
{
	//-------------------------------------
	// Scan Network
	//-------------------------------------

	uint8_t n = WiFi.scanNetworks();
	String htmlwlan, linewlan, cssid;
	int indices[n], swap, quality;

	if (n == 0)
	{
		Serial.println("no networks found");
		if (html == true)
		{
			htmlwlan += F("<div>Es wurden keine WLAN Netzwerke gefunden</div>");
		}
		else
		{
			delay(10);
		}
	}
	else
	{
		Serial.print(n);
		Serial.println(" networks found");
		if (html == true)
		{
			//sort networks
			for (uint8_t i = 0; i < n; i++)
			{
				indices[i] = i;
			}
			for (uint8_t i = 0; i < n; i++)
			{
				for (uint8_t j = i + 1; j < n; j++)
				{
					if (WiFi.RSSI(indices[j]) > WiFi.RSSI(indices[i]))
					{
						swap = indices[i];
						indices[i] = indices[j];
						indices[j] = swap; //hab den Swap Befehl nicht gefunden
					}
				}
			}
			// remove duplicates ( must be RSSI sorted )
			for (uint8_t i = 0; i < n; i++)
			{
				if (indices[i] == -1) { continue; }
				cssid = WiFi.SSID(indices[i]);
				for (int j = i + 1; j < n; j++)
				{
					if (cssid == WiFi.SSID(indices[j]))
					{
						indices[j] = -1; // set dup aps to index -1
					}
				}
			}
		}
		for (uint8_t i = 0; i < n; ++i)
		{
			// Print SSID and RSSI for each network found
			Serial.print(i + 1);
			Serial.print(": ");
			Serial.print(WiFi.SSID(i));
			Serial.print(" (");
			Serial.print(WiFi.RSSI(i));
			Serial.print(")");
			Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
			if (html == true)
			{
				if (indices[i] == -1) { continue; } // skip dups
				linewlan = F("<div><a href='#' onclick='wlanlists(this)'>{s}</a>&nbsp;<span class='wlanlistr {l}'>{r}%</span></div>");
				linewlan.replace("{s}", WiFi.SSID(indices[i]));
				if (WiFi.encryptionType(indices[i]) != ENC_TYPE_NONE)
				{
					linewlan.replace("{l}", "wlanlistl");
				}
				else
				{
					linewlan.replace("{l}", "");
				}
				quality = WiFi.RSSI(indices[i]);
				if (quality <= -100)
				{
					quality = 0;
				}
				else if (quality >= -50)
				{
					quality = 100;
				}
				else
				{
					quality = 2 * (quality + 100);
				}
				linewlan.replace("{r}", String(quality));
				htmlwlan += linewlan;
			}
			else
			{

				delay(100);
				if (wlan_ssid == false)
				{
					wlan_ssid = true;
					for (int k = 0; WiFi.SSID(i)[k] != '\0'; k++)
					{
						if (WiFi.SSID(i)[k] != G.ssid[k]) { wlan_ssid = false; }
					}
				}
			}
		}
	}
	wlan_status = WiFi.status();
	Serial.printf("WLAN-Status: %s\n", wstatus[wlan_status]);
	Serial.printf("WLAN-SSID vorhanden: %d\n", wlan_ssid);
	return htmlwlan;
	//-------------------------------------
}

//------------------------------------------------------------------------------

void WiFiStart_Client()
{
	unsigned int count = 0;
	// Connect to WiFi network
	Serial.println();
	Serial.print("Connecting to ");
	Serial.println(G.ssid);
	Serial.println(G.passwd);

	WiFi.disconnect();
	// WLAN-Mode Clinet stetzen
	WiFi.mode(WIFI_STA);
	WiFi.begin((const char *) G.ssid, (const char *) G.passwd);
	while (count < 30)
	{
		if (WiFi.status() != WL_CONNECTED)
		{
			count++;
			delay(500);
			Serial.print(".");
		}
		else
		{
			break;
		}
	}
	if (WiFi.status() == WL_CONNECTED)
	{  // Als Clinet connected
		Serial.println("");
		Serial.println("WiFi-Client connected");

		char ip_adress[20];
		sprintf(ip_adress, "IP:%d.%d.%d.%d", WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], WiFi.localIP()[3]);
		Serial.println(ip_adress);
		Serial.println("");
		// IP-Adresse als Laufschrift anzeigen
		if (show_ip == true) { zeigeip(ip_adress); }

		wlan_client = true;
	}

}

//------------------------------------------------------------------------------

void WiFiStart_AP()
{
	// WLAN-Mode AccessPoint stetzen
	Serial.println("");
	Serial.println("Setup WiFi Access-Point");

	WiFi.disconnect();
	delay(300);
	WiFi.mode(WIFI_AP);
	WiFi.softAP(ssid_ap, password_ap);

	char buf[20];
	sprintf(buf, "IP:%d.%d.%d.%d", WiFi.softAPIP()[0], WiFi.softAPIP()[1], WiFi.softAPIP()[2], WiFi.softAPIP()[3]);
	Serial.print("AccessPoint ");
	Serial.println(buf);
	Serial.println("");
	zeigeip(buf);

	wlan_client = false;
}

//------------------------------------------------------------------------------

void WiFiReconnect()
{
	// WLAN reconnect
	wlan_status = WiFi.status();
	Serial.printf("WLAN-Status: %s\n", wstatus[wlan_status]);
	//WiFi.reconnect();
	WiFi.disconnect();
	delay(300);
	WiFi.mode(WIFI_STA);
	WiFi.begin((const char *) G.ssid, (const char *) G.passwd);
	delay(300);

	Serial.printf("WLAN-Restart-Status: %s\n", wstatus[wlan_status]);
}

//------------------------------------------------------------------------------

void WlanStart()
{
	Serial.printf("\n-- Begin WlanStart -- \n");

	WiFi.disconnect();
	WiFi.mode(WIFI_STA);
	delay(300);
	WiFiScan(false);
	if (wlan_ssid == true)
	{
		WiFiStart_Client();
	}
	//-------------------------------------
	// Zeitanfrage beim NTP-Server
	//-------------------------------------
	if (wlan_client == true)
	{
		timeClient.begin();
    delay(100);
    timeClient.update();
    delay(100);
    Serial.println(timeClient.getFormattedTime());

		unix_time = timeClient.getEpochTime();
		if(externalRTC == true){RTC.adjust(DateTime(unix_time));}
	}
	else if (wlan_client == false)
	{
		if(externalRTC == true){
			unix_time = RTC.now().unixtime();
		}
    WiFiStart_AP();
	}
	// Zeit setzen
  setTime(unix_time);
	Serial.printf("-- Ende  WlanStart -- \n\n");
}

//------------------------------------------------------------------------------

void eeprom_write()
{
	EEPROM_writeAnything(0, G);
	EEPROM.commit();
}

//------------------------------------------------------------------------------

void eeprom_read()
{
	EEPROM_readAnything(0, G);

	Serial.print("Version   : ");
	Serial.println(VER);
	Serial.printf("Sernr     : %u\n", G.sernr);
	Serial.printf("SSID      : %s\n", G.ssid);
	Serial.printf("Passwd    : %s\n", G.passwd);
	Serial.printf("Programm  : %u\n", G.prog);
	Serial.printf("Conf      : %u\n", G.conf);
	Serial.printf("rgb.0.0   : %u\n", G.rgb[Foreground][0]);
	Serial.printf("rgb.0.1   : %u\n", G.rgb[Foreground][1]);
	Serial.printf("rgb.0.2   : %u\n", G.rgb[Foreground][2]);
	Serial.printf("rgb.0.2   : %u\n", G.rgb[Foreground][3]);
	Serial.printf("rgb.1.0   : %u\n", G.rgb[Background][0]);
	Serial.printf("rgb.1.1   : %u\n", G.rgb[Background][1]);
	Serial.printf("rgb.1.2   : %u\n", G.rgb[Background][2]);
	Serial.printf("rgb.1.3   : %u\n", G.rgb[Background][3]);
	Serial.printf("rgb.2.0   : %u\n", G.rgb[Frame][0]);
	Serial.printf("rgb.2.1   : %u\n", G.rgb[Frame][1]);
	Serial.printf("rgb.2.2   : %u\n", G.rgb[Frame][2]);
	Serial.printf("rgb.2.3   : %u\n", G.rgb[Frame][3]);
	Serial.printf("rgb.3.0   : %u\n", G.rgb[SpecialFunction][0]);
	Serial.printf("rgb.3.1   : %u\n", G.rgb[SpecialFunction][1]);
	Serial.printf("rgb.3.2   : %u\n", G.rgb[SpecialFunction][2]);
	Serial.printf("rgb.3.3   : %u\n", G.rgb[SpecialFunction][3]);
	Serial.printf("Zeitserver: %s\n", G.zeitserver);
	Serial.printf("Text      : %s\n", G.ltext);
	Serial.printf("H6        : %u\n", G.h6);
	Serial.printf("H8        : %u\n", G.h8);
	Serial.printf("H12       : %u\n", G.h12);
	Serial.printf("H16       : %u\n", G.h16);
	Serial.printf("H18       : %u\n", G.h18);
	Serial.printf("H22       : %u\n", G.h22);
	Serial.printf("H24       : %u\n", G.h24);
	Serial.printf("LDR       : %u\n", G.ldr);
	Serial.printf("LDRCal    : %u\n", G.ldrCal);
	Serial.print("OWM_apikey: ");
	Serial.println(G.apikey);
	Serial.print("OWM_city  : ");
	Serial.println(G.cityid);

	delay(100);
}

//------------------------------------------------------------------------------

void WiFiEvent(WiFiEvent_t event)
{
	Serial.printf("[WiFi-event] event: %d\n", event);

	switch (event)
	{
		case WIFI_EVENT_STAMODE_GOT_IP:
			Serial.println("WiFi connected");
			Serial.println("IP address: ");
			Serial.println(WiFi.localIP());
			break;
		case WIFI_EVENT_STAMODE_DISCONNECTED:
			Serial.println("WiFi lost connection");
			break;
		default:
			break;
	}
}

//------------------------------------------------------------------------------
// Start setup()
//------------------------------------------------------------------------------

void setup()
{
	//-------------------------------------
	// EEPROM lesen / initialisieren
	//-------------------------------------
	EEPROM.begin(512);

	eeprom_read();

	if (G.sernr != SERNR)
	{
		for (int i = 0; i < 512; i++) { EEPROM.write(i, i); }
		EEPROM.commit();

		G.sernr = SERNR;
    strcpy(G.ssid, "Enter_Your_SSID");
    strcpy(G.passwd, "Enter_Your_PASSWORD");
		G.prog = 1;
		G.param1 = 0;
		G.param2 = 0;
		G.prog_init = 1;
		G.conf = 0;
		for (int i = 0; i < 4; i++) { for (int ii = 0; ii < 4; ii++) { G.rgb[i][ii] = 0; }}
		G.rgb[Foreground][2] = 100;
		G.rgb[SpecialFunction][1] = 100;
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
		strcpy(G.zeitserver, "ptbtime1.ptb.de");
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

		eeprom_write();
		Serial.println("eeprom schreiben");
	}
	//-------------------------------------
	// Start Serielle Schnittstelle bei Bedarf
	//-------------------------------------
	if (DEBUG == true)
	{
		Serial.begin(38400);
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
	// LEDs initialisieren
	//-------------------------------------
#ifdef Grbw
	Serial.println("SK6812 LED Init");
#else
	Serial.println("WS2812 LED Init");
#endif
	strip.Begin();
	led_single(20);
	led_clear();
	led_show();

	G.conf = 0;

	//-------------------------------------
	// Start External RealtimeClock
	//-------------------------------------

	if (RTC.begin() == true ) {
		Serial.println("External RealtimeClock found");
		externalRTC = true;
	}
	else {
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
#ifdef UHR_169
	_sekunde48 = _sekunde * 48 / 60;
#endif
	show_zeit(1);
#ifdef UHR_169
	if (G.zeige_sek <1 && G.zeige_min < 2){
	  set_farbe_rahmen();
	}
#endif

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

void loop()
{
	unsigned long currentMillis = millis();
	count_millis += currentMillis - previousMillis;
	count_delay += currentMillis - previousMillis;
#ifdef UHR_169
	count_millis48 += currentMillis - previousMillis;
#endif
	previousMillis = currentMillis;
	if (count_millis >= interval)
	{
		count_millis = 0;
		utc = now();    //current time from the Time Library
		ltime = tzc.toLocal(utc, &tcr);
		_sekunde = second(ltime);
		_minute = minute(ltime);
		_stunde = hour(ltime);
		count_tag++;
		// Wetteruhr
#ifdef UHR_242
		weather_tag++;
#endif

	}
#ifdef UHR_169
	if (count_millis48 >= interval48) {
	  count_millis48 = 0;
	  _sekunde48++;
	  if (_sekunde48 >47){ _sekunde48 = 0; }
	}
#endif

	//------------------------------------------------
	Telnet();  // Handle telnet connections

	//------------------------------------------------
	//--OTA--
	//------------------------------------------------
	//  if (G.prog == 0 && G.conf == 0) {
	httpServer.handleClient();
	//  }
	//------------------------------------------------

	webSocket.loop();

	//------------------------------------------------
	// Sekunde48
	//------------------------------------------------
#ifdef UHR_169
	if (last_sekunde48 != _sekunde48) {
	  if (G.prog == 0 && G.conf == 0) {
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
#endif

	//------------------------------------------------
	// Sekunde und LDR Regelung
	//------------------------------------------------
	if (last_sekunde != _sekunde)
	{

		//--- LDR Regelung
		//
		if (G.ldr == 1)
		{
			doLDRLogic();
		}
		//--- LDR Regelung

		if (G.prog == 0 && G.conf == 0)
		{
			show_zeit(0); // Anzeige Uhrzeit ohne Config
		}
		last_sekunde = _sekunde;

#ifdef UHR_242
		if ((_sekunde == 30) | (_sekunde == 0))  {
			wetterswitch ++;
			if (wetterswitch > 4) {
			wetterswitch = 1;
			}
			  Serial.print("Wetterswitch: ");
			  Serial.println(wetterswitch);
			  Serial.print("WStunde: ");
			  Serial.println(wstunde);
		  }
#endif
	}

	//------------------------------------------------
	// Minute
	//------------------------------------------------
	if (last_minute != _minute)
	{
		Serial.println(">>>> Begin Minute <<<<");
		TelnetMsg(">>>> Begin Minute <<<<");

		if (G.prog == 0 && G.conf == 0)
		{
			show_zeit(1); // Anzeige Uhrzeit mit Config
		}

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
	if (last_stunde != _stunde)
	{
		Serial.println(">>>> Begin Stunde <<<<");
		TelnetMsg(">>>> Begin Stunde <<<<");
		// WLAN testen
		wlan_status = WiFi.status();
		Serial.printf("WLAN-Status: %s\n", wstatus[wlan_status]);
		TelnetMsg("WLAN-Status: ");
		TelnetMsg(wstatus[wlan_status]);
		if (wlan_client == false && wlan_ssid == true)
		{
			WlanStart();
		}

		last_stunde = _stunde;
		Serial.println(">>>> Ende  Stunde <<<<");
		TelnetMsg(">>>> Ende  Stunde <<<<");
	}

	//------------------------------------------------
	// Tag
	//------------------------------------------------
	if (count_tag >= 86400)
	{
		count_tag = 0;
		ntp_flag = true;
	}

	//------------------------------------------------
	// Wetterdaten abrufen
	//------------------------------------------------
#ifdef UHR_242
	if (weather_tag >= 600) {
	  weather_tag = 0;
	  if (WiFi.status() == WL_CONNECTED)
	   {
		getweather();
		  }
	}
#endif

	//------------------------------------------------
	// NTP Zeit neu holen
	//------------------------------------------------
	if (ntp_flag == true)
	{
		Serial.println("npt: Neue Zeit holen");
		TelnetMsg("npt: Neue Zeit holen");
		ntp_flag = false;
		wlan_status = WiFi.status();
		if (wlan_status == 3)
		{
			timeClient.update();
			unix_time = timeClient.getEpochTime();
			if (unix_time > 0)
			{
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
	if (G.prog == 1)
	{
		//uhr_clear();;
		show_zeit(0); // Anzeige Uhrzeit ohne Config
#ifdef UHR_169
		if (G.zeige_sek <1 && G.zeige_min < 2){
		  set_farbe_rahmen();
		}
#endif
		G.prog = 0;
	}

	//------------------------------------------------
	// Uhrzeit setzen
	//------------------------------------------------
	if (G.conf == 30)
	{
		utc = now();    //current time from the Time Library
		ltime = tzc.toLocal(utc, &tcr);
		_sekunde = second(ltime);
		_minute = minute(ltime);
		_stunde = hour(ltime);
#ifdef UHR_169
		_sekunde48 = _sekunde * 48 / 60;
#endif
		show_zeit(1); // Anzeige Uhrzeit mit Config
		eeprom_write();
		delay(100);
		G.conf = 0;
	}

	//------------------------------------------------
	// Startwerte speichern
	//------------------------------------------------
	if (G.conf == 20)
	{
		Serial.println("Startwerte gespeichert");
		Serial.println(G.rgb[Foreground][0]);
		Serial.println(G.rgb[Foreground][1]);
		Serial.println(G.rgb[Foreground][2]);
		Serial.println(G.rgb[Foreground][3]);
		eeprom_write();
		delay(100);
		G.conf = 0;
	}

	//------------------------------------------------
	// Helligkeit speichern
	//------------------------------------------------
	if (G.conf == 95)
	{
		show_zeit(1); // Anzeige Uhrzeit mit Config
		eeprom_write();
		delay(100);
		G.conf = 0;
	}

	//------------------------------------------------
	// Anzeige Minuten speichern
	//------------------------------------------------
	if (G.conf == 94)
	{
		show_zeit(1); // Anzeige Uhrzeit mit Config
		eeprom_write();
		delay(100);
		G.conf = 0;
	}

	//------------------------------------------------
	// LDR Einstellung speichern
	//------------------------------------------------
	if (G.conf == 91)
	{
		eeprom_write();
		delay(100);
		Serial.printf("LDR : %u\n\n", G.ldr);
		Serial.printf("LDR Kalibrierung: %u\n\n", G.ldrCal);
		G.conf = 0;
	}

	//------------------------------------------------
	// OpenWeathermap Einstellung speichern
	//------------------------------------------------
	if (G.conf == 90)
	{
		Serial.println("write EEPROM!");
		Serial.print("CityID : ");
		Serial.println(G.cityid);
		Serial.print("APIkey : ");
		Serial.println(G.apikey);
		eeprom_write();
		delay(100);
		G.conf = 0;
	}

	//------------------------------------------------
	// Hostname speichern
	//------------------------------------------------
	if (G.conf == 92)
	{
		eeprom_write();
		delay(100);
		G.conf = 0;
	}

	//------------------------------------------------
	// Anzeige Sekunde speichern
	//------------------------------------------------
	if (G.conf == 93)
	{
		show_zeit(1); // Anzeige Uhrzeit mit Config
		eeprom_write();
		delay(100);
		G.conf = 0;
	}

	//------------------------------------------------
	// Lauftext speichern
	//------------------------------------------------
	if (G.conf == 96)
	{
		eeprom_write();
		delay(100);
		G.conf = 0;
	}

	//------------------------------------------------
	// Zeitserver speichern
	//------------------------------------------------
	if (G.conf == 97)
	{
		eeprom_write();
		delay(100);
		G.conf = 0;
	}

	//------------------------------------------------
	// WLAN-Daten speichern und neu starten
	//------------------------------------------------
	if (G.conf == 99)
	{
		eeprom_write();
		delay(1000);
		Serial.println("Conf: WLAN neu konfiguriert");
		WlanStart();
		G.conf = 0;
	}

	//------------------------------------------------
	// Sekunden
	//------------------------------------------------
	if (G.prog == 200)
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
	}

	//------------------------------------------------
	// Laufschrift
	//------------------------------------------------
	if (G.prog == 201)
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
	}

	//------------------------------------------------
	// Regenbogen
	//------------------------------------------------
	if (G.prog == 202)
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
	}

	//------------------------------------------------
	// Farbwechsel
	//------------------------------------------------
	if (G.prog == 203)
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

	}

	//------------------------------------------------
	// Farbe Rahmen
	//------------------------------------------------

	if (G.prog == 204)
	{
		if (G.prog_init == 1)
		{
			G.prog_init = 0;
			set_farbe();
			led_show();
		}

	}

	//------------------------------------------------
	// Reset
	//------------------------------------------------
	if (G.conf == 100)
	{
		delay(1000);
		ESP.reset();
		ESP.restart();
		while (true) {}
	}

	//------------------------------------------------
	// Config Senden
	//------------------------------------------------
	if (G.conf == 300)
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
		strcat(str, "\"}");
		webSocket.sendTXT(G.client_nr, str, strlen(str));
		G.conf = 0;
	}

	//------------------------------------------------
	// conf Farbwerte senden
	//------------------------------------------------
	if (G.conf == 301)
	{
		strcpy(str, R"({"command":"set")");
		for (uint8_t i = 0; i < 4; i++)
		{
#ifdef Grbw
			for (uint8_t ii = 0; ii < 4; ii++)
			{
#else
			for (uint8_t ii = 0; ii < 3; ii++)
			{
#endif
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
		strcat(str, "\"}");
		webSocket.sendTXT(G.client_nr, str, strlen(str));
		G.conf = 0;
	}
	//------------------------------------------------

	//------------------------------------------------
	// Wlan Liste
	//------------------------------------------------
	if (G.conf == 302)
	{
		String strs = R"({"command":"wlan")";
		strs += R"(,"list":")";
		strs += WiFiScan(true);
		strs += "\"}";
		webSocket.sendTXT(G.client_nr, strs);
		G.conf = 0;
	}
	//------------------------------------------------

	if (count_delay > 10000) { count_delay = 0; }
}

/*--------------------------------------------------
Author: Ulrich Radig
Wort Uhr Version 2
www.ulrich-radig.de

Version 2.0.0
* (Ulrich Radig)
* inital version
Version 2.0.1
* (Markus Aust)
* Minuten LED´s
Version 2.0.2
* (Eisbaeeer)
* Fix NTP issue
Version 2.0.3
* (Eisbaeeer)
* Konfig für 125 LED´s (11 Reihen) hinzugefügt
Version 2.0.4
* (path83 & Eisbaeeer)
* LDR für automatische Helligkeitsregelung
Version 2.0.5
* IP Adresse per Laufschrift ausgeben
* LDR Kalibrierung über WebConfig
* Version 2.0.6
* (path83 & masju & Eisbaeeer)
* Over The Air Update Link in Webconfig
Version 2.0.7
* LDR Helligkeitsregelung Hintergrund
Version 2.0.8
* (Flo455)
* WLAN Scan hinzugefügt
Version 2.0.9
* (Eisbaeeer)
* Telnet Server für Debugging
Version 2.1.0
* OpenWeatherMap für Uhr 242
Version 2.1.1
* (Atho95)
* Neueste Version der WebSockets - Bugfix Nullpointer in Debug-Ausgabe (payload)
     Im Directory Uhr_Neu befindet sich noch die alte Version (könnte gelöscht werden)
* Buffer für IP Adressen auf 20 erhöht (sprintf hängt '\0' an)
* uhr_func_114.h erweitert auf umgekehrt angeschlossene LED's
* uhr.h erweitert, um auch RGB LED's konfigurieren zu können

Ideen / Todo
- Zeitverlauf Farben konfigurierbar
  Sommer-Zeit: Letzter Sonntag März um 2 Uhr
  Winter-Zeit: Letzter Sonntag Oktober um 3 Uhr
- Server-IP eintragen
- Überblenden
- Tretis
- MP3 Gong Geläut


--------------------------------------------------*/

//---------------------------------------------------------
// WLAN-Status
//---------------------------------------------------------
char wstatus[7][25]=
{

  "WL_IDLE_STATUS",
  "WL_NO_SSID_AVAIL",
  "WL_SCAN_COMPLETED",
  "WL_CONNECTED",
  "WL_CONNECT_FAILED",
  "WL_CONNECTION_LOST",
  "WL_DISCONNECTED"
};
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

//--OTA--
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>
//--OTA--
#include <WiFiUdp.h>
#include "WebPage_Adapter.h"
#include <Hash.h>
#include <TimeLib.h>
#include <Timezone.h>
#include <EEPROM.h>
#include "EEPROMAnything.h"
#include "Uhr.h"
#include "font.h"

extern "C"{
#include "uhr_func.h"
};
#include "ntp_func.h"

//--OpenWeatherMapOrg
#include "openwmap.h"

TimeChangeRule CEST = {"CEST", Last, Sun, Mar, 2, 120};     //Central European Summer Time
TimeChangeRule CET = {"CET ", Last, Sun, Oct, 3, 60};       //Central European Standard Time
Timezone tzc(CEST, CET);

time_t ltime, utc;
TimeChangeRule *tcr;

//------------------------------------------------------------------------------
// Start setup()
//------------------------------------------------------------------------------

void setup()
{
  //-------------------------------------
  // Start Serielle Schnittstelle
  //-------------------------------------
  #ifdef DEBUG
   USE_SERIAL.begin(38400);
   USE_SERIAL.println("");
  #endif
  delay(100);
  //-------------------------------------
  #ifdef DEBUG
   USE_SERIAL.println("");
   USE_SERIAL.println("--------------------------------------");
   USE_SERIAL.println ( "Begin Setup" );
   USE_SERIAL.println("--------------------------------------");
  #endif

  //-------------------------------------
  USE_SERIAL.println("Starting Telnet server");
  TelnetServer.begin();
  TelnetServer.setNoDelay(true);

  //-------------------------------------
  // EEPROM lesen / initialisieren
  //-------------------------------------
  EEPROM.begin(512);

  eeprom_read();

  if (G.sernr !=SERNR){
    for( int i = 0; i < 512; i++ ){ EEPROM.write(i,i); }
    EEPROM.commit();

    G.sernr = SERNR;
    strcpy(G.ssid, "WLAN");  
    strcpy(G.passwd, "12345678"); 
    G.prog      = 1;
    G.param1    = 0;
    G.param2    = 0;
    G.prog_init = 1;
    G.conf      = 0;
    for( int i = 0; i < 4; i++ ){ for( int ii = 0; ii < 3; ii++ ){ G.rgb[i][ii] = 0; } }
    G.rgb[0][2] = 100;
    G.rgb[3][1] = 100;
    G.rr        = 0;
    G.gg        = 0;
    G.bb        = 0;
    G.hell      = 2;
    G.geschw    = 10;
    G.client_nr = 0;
    G.zeige_sek = 0;
    G.zeige_min = 1;
    G.ldr       = 0;
    G.ldrCal    = 0;
    strcpy(G.cityid, "");
    strcpy(G.apikey, "");
    strcpy(G.zeitserver, "ptbtime1.ptb.de");
    strcpy(G.hostname, "uhr");     
    strcpy(G.ltext, "HELLO WORLD ");

    G.hh        = 100;
    G.h6        = 100;
    G.h8        = 100;
    G.h12       = 100;
    G.h16       = 100;
    G.h18       = 100;
    G.h20       = 100;
    G.h22       = 100;
    G.h24       = 100;

    for( int i = 0; i < 10; i++ ){ for( int ii = 0; ii < 5; ii++ ){ G.rgb1[i][ii] = 0; } }
    eeprom_write();

  #ifdef DEBUG
    USE_SERIAL.println("eeprom schreiben");
  #endif
  }

  //-------------------------------------
  #ifdef DEBUG
  USE_SERIAL.println("--------------------------------------");
  USE_SERIAL.println("ESP Uhr");
  USE_SERIAL.print("Version         : "), USE_SERIAL.println(VER);
  USE_SERIAL.printf("Chip ID         : %08X\n",     ESP.getChipId());
  USE_SERIAL.printf("Flash ID        : %08X\n\n",   ESP.getFlashChipId());
  USE_SERIAL.printf("CPU Speed       : %u MHz \n\n", ESP.getCpuFreqMHz());

  USE_SERIAL.printf("Flash real Size : %u KByte\n", ESP.getFlashChipRealSize()/1024);
  USE_SERIAL.printf("Flash ide  Size : %u KByte\n", ESP.getFlashChipSize()/1024);
  USE_SERIAL.printf("Flash ide Speed : %u\n\n",     ESP.getFlashChipSpeed());

  USE_SERIAL.printf("Free Heap Size  : %u Byte\n",  ESP.getFreeHeap());
  USE_SERIAL.printf("Sketch Size     : %u Byte \n", ESP.getSketchSize());
  USE_SERIAL.printf("Free Sketch Size: %u Byte \n\n", ESP.getFreeSketchSpace());

  USE_SERIAL.printf("SDK Version     : %s\n", ESP.getSdkVersion());
  USE_SERIAL.print("RESET Info      : "); USE_SERIAL.println(ESP.getResetInfo());
  USE_SERIAL.print("COMPILED        : "); USE_SERIAL.print(__DATE__); USE_SERIAL.print(" "); USE_SERIAL.println(__TIME__);
  USE_SERIAL.println("--------------------------------------");
  #endif

  //-------------------------------------
  // LEDs initialisieren
  //-------------------------------------
  USE_SERIAL.println("WS2812 LED Init");    
  strip.Begin();
  led_single(20);
  led_clear();
  led_show();

  G.conf = 0;
  //-------------------------------------

  //-------------------------------------
  // Start WiFi
  //-------------------------------------
  WlanStart();

  //-------------------------------------


  //-------------------------------------
  // Zeit setzen
  //-------------------------------------
  utc = now();    //current time from the Time Library
  ltime = tzc.toLocal(utc, &tcr);
  _sekunde = second(ltime);
  _minute  = minute(ltime);
  _stunde  = hour(ltime);
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


  //-------------------------------------
  // mDNS--
  //-------------------------------------
  MDNS.begin((const char*)G.hostname);
  MDNS.addService("http", "tcp", 80);
  MDNS.addService("http", "tcp", 81);
  //-------------------------------------


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

  #ifdef DEBUG
   USE_SERIAL.println ( "Websockest started" );
  #endif
  //-------------------------------------

  #ifdef DEBUG
   USE_SERIAL.println("--------------------------------------");
   USE_SERIAL.println ( "Ende Setup" );
   USE_SERIAL.println("--------------------------------------");
   USE_SERIAL.println("");
  #endif

}
//------------------------------------------------------------------------------
// Ende setup()
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Start loop()
//------------------------------------------------------------------------------

void loop()
{

  unsigned long currentMillis = millis();
  count_millis += currentMillis - previousMillis;
  count_delay  += currentMillis - previousMillis;
  #ifdef UHR_169
    count_millis48 += currentMillis - previousMillis;
  #endif
  previousMillis = currentMillis;
  if (count_millis >= interval) {
    count_millis = 0;
    utc = now();    //current time from the Time Library
    ltime = tzc.toLocal(utc, &tcr);
    _sekunde = second(ltime);
    _minute  = minute(ltime);
    _stunde  = hour(ltime);
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
  if (last_sekunde != _sekunde) {

  //--- LDR Regelung
  //
  if (G.ldr == 1 )
  {
    doLDRLogic();
  }
  //--- LDR Regelung

    if (G.prog == 0 && G.conf == 0) {
      show_zeit(0); // Anzeige Uhrzeit ohne Config
    }
    last_sekunde = _sekunde;
  
  #ifdef UHR_242
  
if ((_sekunde == 30) | (_sekunde == 0))  {
    wetterswitch ++;
    if (wetterswitch > 4) {
    wetterswitch = 1;
    }
    #ifdef DEBUG     
      USE_SERIAL.print("Wetterswitch: ");
      USE_SERIAL.println(wetterswitch);
      USE_SERIAL.print("WStunde: ");
      USE_SERIAL.println(wstunde);
    #endif  
  }

   
  #endif
  }

  //------------------------------------------------
  // Minute
  //------------------------------------------------
  if (last_minute != _minute) {
    #ifdef DEBUG
      USE_SERIAL.println(">>>> Begin Minute <<<<");
      TelnetMsg(">>>> Begin Minute <<<<");
    #endif
    if (G.prog == 0 && G.conf == 0) {
      show_zeit(1); // Anzeige Uhrzeit mit Config
    }

    // WLAN reconnect
    wlan_status = WiFi.status();
    #ifdef DEBUG
      USE_SERIAL.printf("WLAN-Status: %s\n", wstatus[wlan_status]);
      TelnetMsg("WLAN-Status: "); TelnetMsg(wstatus[wlan_status]);
    #endif
//    if (wlan_status == 6 && wlan_client == true){
//      WiFiReconnect();
//    }
    _sekunde48 = 0;
    last_minute = _minute;

    USE_SERIAL.printf("%u.%u.%u %u:%u:%u \n", day(ltime), month(ltime), year(ltime), hour(ltime), minute(ltime),  second(ltime));
    #ifdef DEBUG
      USE_SERIAL.println(">>>> Ende  Minute <<<<");
      TelnetMsg(">>>> Ende  Minute <<<<");
    #endif
  }

  //------------------------------------------------
  // Stunde
  //------------------------------------------------
  if (last_stunde != _stunde) {

    #ifdef DEBUG
      USE_SERIAL.println(">>>> Begin Stunde <<<<");
      TelnetMsg(">>>> Begin Stunde <<<<");
    #endif
    // WLAN testen
    wlan_status = WiFi.status();
    #ifdef DEBUG
      USE_SERIAL.printf("WLAN-Status: %s\n", wstatus[wlan_status]);
      TelnetMsg("WLAN-Status: "); TelnetMsg(wstatus[wlan_status]);
    #endif
    if (wlan_client == false && wlan_ssid == true){
      WlanStart();
    }

    last_stunde = _stunde;
    #ifdef DEBUG
      USE_SERIAL.println(">>>> Ende  Stunde <<<<");
      TelnetMsg(">>>> Ende  Stunde <<<<");
    #endif
  }

  //------------------------------------------------
  // Tag
  //------------------------------------------------
  if (count_tag >= 86400) {
    count_tag = 0;
    ntp_flag = true;
  }

  //------------------------------------------------
  // Wetterdaten abrufen
  //------------------------------------------------    
  #ifdef UHR_242
  if (weather_tag >= 600) { 
    weather_tag = 0;
    if (wlan_client == true) {  
      getweather();
    }
  }      
  #endif

  //------------------------------------------------
  // NTP Zeit neu holen
  //------------------------------------------------
  if (ntp_flag == true) {
    #ifdef DEBUG
      USE_SERIAL.println("npt: Neue Zeit holen");
      TelnetMsg("npt: Neue Zeit holen");
    #endif
    ntp_flag = false;
    wlan_status = WiFi.status();
    if (wlan_status == 3) {
      set_ntp_zeit();
      if (unix_time > 0) {
        setTime(unix_time);
        #ifdef DEBUG
         USE_SERIAL.println(unix_time);
         utc = now();    //current time from the Time Library
         ltime = tzc.toLocal(utc, &tcr);
         USE_SERIAL.print(hour(ltime));
         USE_SERIAL.print(":");
         USE_SERIAL.print(minute(ltime));
         USE_SERIAL.print(":");
         USE_SERIAL.print(second(ltime));
         USE_SERIAL.print(" - ");
         USE_SERIAL.print(day(ltime));
         USE_SERIAL.print(".");
         USE_SERIAL.print(month(ltime));
         USE_SERIAL.print(".");
         USE_SERIAL.println(year(ltime));
        #endif
      }
    }
  }
  //------------------------------------------------


  //------------------------------------------------
  // Farbe Uhr / Hintergrund / Rahmen einstellen
  //------------------------------------------------
  if (G.prog == 1) {
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


  //------------------------------------------------
  // Uhrzeit setzen
  //------------------------------------------------
  if (G.conf == 30) {
    utc = now();    //current time from the Time Library
    ltime = tzc.toLocal(utc, &tcr);
    _sekunde = second(ltime);
    _minute  = minute(ltime);
    _stunde  = hour(ltime);
    #ifdef UHR_169
      _sekunde48 = _sekunde * 48 / 60;
    #endif
    show_zeit(1); // Anzeige Uhrzeit mit Config
    eeprom_write();
    delay(100);
    G.conf = 0;
  }
  //------------------------------------------------


  //------------------------------------------------
  // Startwerte speichern
  //------------------------------------------------
  if (G.conf == 20) {
    #ifdef DEBUG
     USE_SERIAL.println("Startwerte gespeichert");
     USE_SERIAL.println(G.rgb[0][0]);
     USE_SERIAL.println(G.rgb[0][1]);
     USE_SERIAL.println(G.rgb[0][2]);
    #endif
    eeprom_write();
    delay(100);
    G.conf = 0;
  }
  //------------------------------------------------


  //------------------------------------------------
  // Helligkeit speichern
  //------------------------------------------------
  if (G.conf == 95) {
    show_zeit(1); // Anzeige Uhrzeit mit Config
    eeprom_write();
    delay(100);
    G.conf = 0;
  }
  //------------------------------------------------


  //------------------------------------------------
  // Anzeige Minuten speichern
  //------------------------------------------------
  if (G.conf == 94) {
    show_zeit(1); // Anzeige Uhrzeit mit Config
    eeprom_write();
    delay(100);
    G.conf = 0;
  }
  //------------------------------------------------

  //------------------------------------------------
  // LDR Einstellung speichern
  //------------------------------------------------
  if (G.conf == 91) {
    eeprom_write();
    delay(100);
    #ifdef DEBUG
      USE_SERIAL.printf("LDR : %u\n\n", G.ldr);
      USE_SERIAL.printf("LDR Kalibrierung: %u\n\n", G.ldrCal);
    #endif
    G.conf = 0;
  }
  //------------------------------------------------

  //------------------------------------------------
  // OpenWeathermap Einstellung speichern
  //------------------------------------------------  
  if (G.conf == 90) {
    #ifdef DEBUG  
      USE_SERIAL.println ("write EEPROM!");    
      USE_SERIAL.print("CityID : ");
      USE_SERIAL.println(G.cityid);
      USE_SERIAL.print("APIkey : ");
      USE_SERIAL.println(G.apikey);
    #endif  
    eeprom_write(); 
    delay(100);  
    G.conf = 0;     
  }
  //------------------------------------------------

  //------------------------------------------------
  // Hostname speichern
  //------------------------------------------------
  if (G.conf == 92) {
    eeprom_write();
    delay(100);
    G.conf = 0;
  }
  //------------------------------------------------

  //------------------------------------------------
  // Anzeige Sekunde speichern
  //------------------------------------------------
  if (G.conf == 93) {
    show_zeit(1); // Anzeige Uhrzeit mit Config
    eeprom_write();
    delay(100);
    G.conf = 0;
  }
  //------------------------------------------------


  //------------------------------------------------
  // Lauftext speichern
  //------------------------------------------------
  if (G.conf == 96) {
    eeprom_write();
    delay(100);
    G.conf = 0;
  }
  //------------------------------------------------

  //------------------------------------------------
  // Zeitserver speichern
  //------------------------------------------------
  if (G.conf == 97) {
    eeprom_write();
    delay(100);
    G.conf = 0;
  }
  //------------------------------------------------



  //------------------------------------------------
  // WLAN-Daten speichern und neu starten
  //------------------------------------------------
  if (G.conf == 99) {
    eeprom_write();
    delay(1000);
    #ifdef DEBUG
      USE_SERIAL.println("Conf: WLAN neu konfiguriert");
    #endif
    WlanStart();
    G.conf = 0;
  }
  //------------------------------------------------



  //------------------------------------------------
  // Sekunden
  //------------------------------------------------
  if (G.prog == 200) {
    if (G.prog_init == 1){
      led_clear();
      G.prog_init = 0;
    }
    char d1[5];
    char d2[5];
    sprintf(d1,"%d",(int)(_sekunde / 10));
    sprintf(d2,"%d",(int)(_sekunde % 10));
    zahlen(d1[0], d2[0]);
  }
  //------------------------------------------------


  //------------------------------------------------
  // Laufschrift
  //------------------------------------------------
  if (G.prog == 201) {
    if (G.prog_init == 1){
      G.prog_init = 0;
      led_clear();
      count_delay = (G.geschw + 1) * 20;
    }
    if (count_delay >= (G.geschw + 1) * 20){
      laufschrift();
      count_delay = 0;
    }
  }
  //------------------------------------------------


  //------------------------------------------------
  // Regenbogen
  //------------------------------------------------
  if (G.prog == 202) {
    if (G.prog_init == 1){
      G.prog_init = 0;
      uhr_clear();
      count_delay = G.geschw * 7 + 1;
    }
    if (count_delay >= G.geschw * 7 + 1){
      rainbowCycle();
      count_delay = 0;
    }
  }
  //------------------------------------------------


  //------------------------------------------------
  // Farbwechsel
  //------------------------------------------------
  if (G.prog == 203) {
    if (G.prog_init == 1){
      G.prog_init = 0;
      led_clear();
      count_delay = G.geschw * 7 + 1;
    }
    if (count_delay >= G.geschw * 7 + 1){
      rainbow();
      count_delay = 0;
    }

  }
  //------------------------------------------------


  //------------------------------------------------
  // Farbe Rahmen
  //------------------------------------------------

  if (G.prog == 204) {
    if (G.prog_init == 1){
      G.prog_init = 0;
      set_farbe();
      led_show();
    }

  }
  /*
  if (G.prog == 204) {
    if (G.prog_init == 1){
      G.last_prog = G.prog;
      G.prog_init = 0;
      led_clear();
      count_delay = 100;
    }

    if (count_delay >= 100){
      int s = _sekunde48;
      int b = 255 * count_millis / 1000;
      rahmen_clear();
      led_set_pixel(0, 0, b, rmatrix[s]);
      led_show();
      count_delay = 0;
    }
  }
  */
  //------------------------------------------------


  //------------------------------------------------
  // Reset
  //------------------------------------------------
  if (G.conf == 100) {
    delay(1000);
    ESP.reset();
    ESP.restart();
    while (1){delay(1);};
  }
  //------------------------------------------------

  //------------------------------------------------
  // Config Senden
  //------------------------------------------------
  if (G.conf == 300) {
    strcpy(str, "{\"command\":\"config\"");
    strcat(str, ",\"ssid\":");
    strcat(str, "\"");
    strcat(str, G.ssid);
    strcat(str, "\",\"zeitserver\":\"");
    strcat(str, G.zeitserver);
    strcat(str, "\",\"hostname\":\"");
    strcat(str, G.hostname);
    strcat(str, "\",\"ltext\":\"");
    strcat(str, G.ltext);
    strcat(str, "\",\"h6\":\"");
    sprintf(s, "%d", G.h6);
    strcat(str, s);
    strcat(str, "\",\"h8\":\"");
    sprintf(s, "%d", G.h8);
    strcat(str, s);
    strcat(str, "\",\"h12\":\"");
    sprintf(s, "%d", G.h12);
    strcat(str, s);
    strcat(str, "\",\"h16\":\"");
    sprintf(s, "%d", G.h16);
    strcat(str, s);
    strcat(str, "\",\"h18\":\"");
    sprintf(s, "%d", G.h18);
    strcat(str, s);
    strcat(str, "\",\"h20\":\"");
    sprintf(s, "%d", G.h20);
    strcat(str, s);
    strcat(str, "\",\"h22\":\"");
    sprintf(s, "%d", G.h22);
    strcat(str, s);
    strcat(str, "\",\"h24\":\"");
    sprintf(s, "%d", G.h24);
    strcat(str, s);
    strcat(str, "\",\"hell\":\"");
    sprintf(s, "%d", G.hell);
    strcat(str, s);
    strcat(str, "\",\"zeige_sek\":\"");
    sprintf(s, "%d", G.zeige_sek);
    strcat(str, s);
    strcat(str, "\",\"zeige_min\":\"");
    sprintf(s, "%d", G.zeige_min);
    strcat(str, s);
    strcat(str, "\",\"ldr\":\"");
    sprintf(s, "%d", G.ldr);
    strcat(str, s);
    strcat(str, "\",\"ldrCal\":\"");
    sprintf(s, "%d", G.ldrCal);
    strcat(str, s);
    strcat(str, "\",\"cityid\":\"");    
    strcat(str, G.cityid);
    strcat(str, "\",\"apikey\":\"");  
    strcat(str, G.apikey);     
    strcat(str, "\"}");
    webSocket.sendTXT(G.client_nr, str, strlen(str));
    G.conf = 0;
  }
  //------------------------------------------------

  //------------------------------------------------
  // conf Farbwerte senden
  //------------------------------------------------
  if (G.conf == 301) {
    strcpy(str, "{\"command\":\"set\"");
    for( int i = 0; i < 4; i++ ){
      for( int ii = 0; ii < 3; ii++ ){
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
    strcat(str, ",\"hell\":\"");
    sprintf(s, "%d", G.hell);
    strcat(str, s);
    strcat(str, "\",\"geschw\":\"");
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
  if (G.conf == 302) {
    String strs = "{\"command\":\"wlan\"";
    strs += ",\"list\":\"";
    strs += WiFiScan(true);
    strs += "\"}";
    webSocket.sendTXT(G.client_nr, strs);
    G.conf = 0;
  }
  //------------------------------------------------


  if (count_delay > 10000){ count_delay = 0; }

}
//------------------------------------------------------------------------------
// Ende loop()
//------------------------------------------------------------------------------


int split(int i, int j) {
   char dest[3];
   int m;
   m=0;
   for (int k=i;k<(i+j);k++){ dest[m] = str[k]; m++; }
   return atoi(dest);
}

//------------------------------------------------------------------------------

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght) {
    int cc = 0;
    int ii;
    int jj;
    char tmp[30];
    payload = (payload == NULL) ? (uint8_t *)"" : payload;
    #ifdef DEBUG
     USE_SERIAL.printf("Client-Nr.: [%u]  WStype: %u payload: %s\n", num, type, payload);
    #endif

    switch(type) {
      case WStype_DISCONNECTED:
      {
        #ifdef DEBUG
         USE_SERIAL.printf("[%u] Disconnected!\n", num);
        #endif
        break;
      }
      case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(num);
        #ifdef DEBUG
         USE_SERIAL.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        #endif

        break;
      }
      case WStype_TEXT:
      {
        #ifdef DEBUG
         USE_SERIAL.printf("[%u] get Text: %s\n", lenght, payload);
        #endif
        for (int k=0;k<lenght;k++){
          str[k] = payload[k];  //does this "copy" is buggy code?
        }

        cc = split(0,3);
        G.param1 = split(3,3);
        G.param2 = split(6,3);
        if (cc == 1) {        // Uhrzeit Vordergrund Farbe einstellen
          G.prog = 1;
          G.rgb[0][0] = split(9,3);
          G.rgb[0][1] = split(12,3);
          G.rgb[0][2] = split(15,3);

          G.rgb[1][0] = split(18,3);
          G.rgb[1][1] = split(21,3);
          G.rgb[1][2] = split(24,3);

          G.rgb[2][0] = split(27,3);
          G.rgb[2][1] = split(30,3);
          G.rgb[2][2] = split(33,3);

          G.rgb[3][0] = split(36,3);
          G.rgb[3][1] = split(39,3);
          G.rgb[3][2] = split(42,3);
          break;
        }

        if (cc == 200) {      // Sekunden
          G.prog = 200;
          if (G.param1 == 0) { G.prog_init = 1; }
          G.rgb[3][0] = split(36,3);
          G.rgb[3][1] = split(39,3);
          G.rgb[3][2] = split(42,3);
          G.hell=split(45,3);
          G.geschw=split(48,3);
          break;
        }

        if (cc == 201) {      // Laufschrift
          G.prog = 201;
          if (G.param1 == 0) { G.prog_init = 1; }
          G.rgb[3][0] = split(36,3);
          G.rgb[3][1] = split(39,3);
          G.rgb[3][2] = split(42,3);
          G.hell=split(45,3);
          G.geschw=split(48,3);
          break;
        }
        if (cc == 202) {      // Regenbogen
          G.prog = 202;
          G.prog_init = 1;
          G.hell=split(45,3);
          G.geschw=split(48,3);
          break;
        }
        if (cc == 203) {      // Farbwechsel
          G.prog = 203;
          G.prog_init = 1;
          G.hell=split(45,3);
          G.geschw=split(48,3);
          break;
        }
        if (cc == 204) {      // Farbe
          G.prog = 204;
          G.prog_init = 1;
          G.rgb[3][0] = split(36,3);
          G.rgb[3][1] = split(39,3);
          G.rgb[3][2] = split(42,3);
          break;
        }

        if (cc == 251) {      // Helligkeit
          G.hell =split(45,3);
          break;
        }
       if (cc == 252) {       // Geschwindigkeit
          G.geschw=split(48,3);
          break;
        }
        if (cc == 20) {       // Startwerte speichern
          G.conf = 20;
          break;
        }

        if (cc == 30) {       // Uhrzeit setzen
          G.conf = 30;
          ii = 0;
          tmp[0] = '\0';
          for (int k=12;k<28;k++){ tmp[ii] = str[k]; ii++; }
          uint32_t tt=atoi(tmp);
          #ifdef DEBUG
           USE_SERIAL.printf("Conf: Time: %d\n", tt );
          #endif
          setTime(tt);
          break;
        }
        if (cc == 92) {         // Hostname speichern
          G.conf = 92;
          ii = 0;
          for (int k=9;k<25;k++){
            if (str[k]!=' '){
              G.hostname[ii] = str[k];
              ii++;
            }
          }
          G.hostname[ii] = '\0';
          break;
        }
        if (cc == 93) {       // Anzeige Sekunden speichern
          G.conf = 93;
          G.zeige_sek   = split(9,3);
          break;
        }
        if (cc == 94) {       // Anzeige Minuten speichern
          G.conf = 94;
          G.zeige_min   = split(9,3);
          break;
        }
        if (cc == 91) {       // LDR speichern
          G.conf = 91;
          G.ldr   = split(9,3);
          G.ldrCal = split(12,3);
          break;
        }
        if (cc == 90) {       // Openweathermap speichern 
          G.conf = 90;
          ii = 0;
          for (int k=9;k<16;k++){
            if (str[k]!=' '){ 
              G.cityid[ii] = str[k];
              ii++;
              }
          }
          G.cityid[ii] = '\0';
          //
          jj = 0;
          for (int l=17;l<49;l++){
            if (str[l]!=' '){ 
              G.apikey[jj] = str[l];
              jj++;
              }
          }
          G.apikey[jj] = '\0'; 
          break; 
        }
        if (cc == 95) {       // Helligkeit speichern
          G.conf = 95;
          G.conf = 95;
          G.h6   = split(9,3);
          G.h8   = split(12,3);
          G.h12  = split(15,3);
          G.h16  = split(18,3);
          G.h18  = split(21,3);
          G.h20  = split(24,3);
          G.h22  = split(27,3);
          G.h24  = split(30,3);
          break;
/*
          int k = 6;
          for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
              k=k+3;
              G.rgb1[i][j] = split(k,3);
              if (G.rgb1[i][j] == -99){ G.rgb1[i][j] = 0; }
            }
          }
          break;
*/
        }
        if (cc == 96) {       // Lauftext speichern
          G.conf = 96;
          ii = 0;
          for (int k=9;k<39;k++){
            if (str[k]!=' '){ G.ltext[ii] = str[k];  ii++; }
          }
          G.ltext[ii] = '\0';
          break;
        }
        if (cc == 97) {       // Zeitserver speichern
          G.conf = 97;
          ii = 0;
          for (int k=9;k<24;k++){  G.zeitserver[ii] = str[k];  ii++; }
          G.zeitserver[ii] = '\0';
          break;
        }
         if (cc == 99) {       // WLAN-Daten speichern und neu starten
          G.conf = 99;
          ii = 0;
          for (int k=9;k<34;k++){
            if (str[k]!=' '){ G.ssid[ii] = str[k]; ii++; }
          }
          G.ssid[ii]='\0';
          ii = 0;
          for (int k=34;k<59;k++){ 
            if (str[k]!=' '){ G.passwd[ii] = str[k]; ii++; }
          }
          G.passwd[ii]='\0';         
              break;
              }
        if (cc == 100) {      // Reset
          G.conf = 100;
          break;
        }

        if (cc == 300) {      // Config anfordern
          G.conf = 300;
          G.client_nr = num;
          break;
        }
        if (cc == 301) {      // Farbwerte anfordern
          G.conf = 301;
          G.client_nr = num;
          break;
        }
        if (cc == 302) {      // Wlan Liste anfordern
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
        #ifdef DEBUG
         USE_SERIAL.printf("[%u] get binary lenght: %u\n", num, lenght);
        #endif
        hexdump(payload, lenght);
        //--echo data back to browser
        //webSocket.sendBIN(num, payload, lenght);
        break;
      }
    }
}

//------------------------------------------------------------------------------

String WiFiScan(bool html)
{
  //-------------------------------------
  // Scan Network
  //-------------------------------------

  int n = WiFi.scanNetworks();
  String htmlwlan, linewlan, cssid;
  int indices[n], swap, quality;

  if (n == 0){
    #ifdef DEBUG
     USE_SERIAL.println("no networks found");
    #endif
    if (html == true){
      htmlwlan += F("<div>Es wurden keine WLAN Netzwerke gefunden</div>");
    }else{
      delay(10);
    }
  }else{
    #ifdef DEBUG
     USE_SERIAL.print(n);
     USE_SERIAL.println(" networks found");
    #endif
    if (html == true){
      //sort networks
      for (int i = 0; i < n; i++) {
        indices[i] = i;
      }
      for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
          if (WiFi.RSSI(indices[j]) > WiFi.RSSI(indices[i])) {
            swap = indices[i]; indices[i] = indices[j]; indices[j] = swap; //hab den Swap Befehl nicht gefunden
          }
        }
      }
      // remove duplicates ( must be RSSI sorted )
      for (int i = 0; i < n; i++) {
        if (indices[i] == -1) continue;
        cssid = WiFi.SSID(indices[i]);
        for (int j = i + 1; j < n; j++) {
          if (cssid == WiFi.SSID(indices[j])) {
            indices[j] = -1; // set dup aps to index -1
          }
        }
      }
    }
    for (int i = 0; i < n; ++i){
      // Print SSID and RSSI for each network found
      #ifdef DEBUG
       USE_SERIAL.print(i + 1);
       USE_SERIAL.print(": ");
       USE_SERIAL.print(WiFi.SSID(i));
       USE_SERIAL.print(" (");
       USE_SERIAL.print(WiFi.RSSI(i));
       USE_SERIAL.print(")");
       USE_SERIAL.println((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*");
      #endif
      if (html == true){
        if (indices[i] == -1) continue; // skip dups
        linewlan = F("<div><a href='#' onclick='wlanlists(this)'>{s}</a>&nbsp;<span class='wlanlistr {l}'>{r}%</span></div>");
        linewlan.replace("{s}", WiFi.SSID(indices[i]));
        if (WiFi.encryptionType(indices[i]) != ENC_TYPE_NONE){
          linewlan.replace("{l}", "wlanlistl");
        }else{
          linewlan.replace("{l}", "");
        }
        quality = WiFi.RSSI(indices[i]);
        if (quality <= -100) {
          quality = 0;
        } else if (quality >= -50) {
          quality = 100;
        } else {
          quality = 2 * (quality + 100);
        }
        linewlan.replace("{r}", String(quality));
        htmlwlan += linewlan;
      }else{

        delay(100);
        if (wlan_ssid == false){
          wlan_ssid = true;
          for (int k = 0 ; WiFi.SSID(i)[k] != '\0'; k++){
            if (WiFi.SSID(i)[k] != G.ssid[k]){ wlan_ssid = false; }
          }
        }
      }
    }
  }
  #ifdef DEBUG
   wlan_status = WiFi.status();
   USE_SERIAL.printf("WLAN-Status: %s\n", wstatus[wlan_status]);
   USE_SERIAL.printf("WLAN-SSID vorhanden: %d\n", wlan_ssid);
  #endif
  return htmlwlan;
  //-------------------------------------
}

//------------------------------------------------------------------------------

void WiFiStart_Client()
{
  unsigned int count = 0;
  // Connect to WiFi network
  #ifdef DEBUG
   USE_SERIAL.println();
   USE_SERIAL.print("Connecting to ");
   USE_SERIAL.println(G.ssid);
   USE_SERIAL.println(G.passwd);
  #endif
  WiFi.disconnect();
  // WLAN-Mode Clinet stetzen
  WiFi.mode(WIFI_STA);
  WiFi.begin((const char*)G.ssid, (const char*)G.passwd);
  while (count < 30) {
    if (WiFi.status() != WL_CONNECTED) {
      count++;
      delay(500);
      #ifdef DEBUG
       USE_SERIAL.print(".");
      #endif
    } else {
      break;
    }
  }
  if (WiFi.status() == WL_CONNECTED) {  // Als Clinet connected
    #ifdef DEBUG
     USE_SERIAL.println("");
     USE_SERIAL.println("WiFi-Client connected");

    // Print the IP address
     USE_SERIAL.println(WiFi.localIP());
    #endif

    // IP-Adresse als Laufschrift anzeigen
  if (show_ip == true) {
    char buf[20];
    sprintf(buf, "IP:%d.%d.%d.%d", WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], WiFi.localIP()[3] );
    uint8_t StringLength = strlen(buf);
    // USE_SERIAL.print("StringLenght: "); USE_SERIAL.print(StringLength);
    StringLength = StringLength * 6;
    for (int i=0; i <= StringLength; i++) {
      zeigeip();
      delay(200);
    }
  }
    // ---- ENDE Print the IP address
    wlan_client = true;
  }

}

//------------------------------------------------------------------------------

void WiFiStart_AP()
{
  // WLAN-Mode AccessPoint stetzen
  #ifdef DEBUG
   USE_SERIAL.println("");
   USE_SERIAL.println("Setup WiFi Access-Point");
  #endif
  WiFi.disconnect();
  delay(300);
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid_ap, password_ap);
  IPAddress myIP = WiFi.softAPIP();
  #ifdef DEBUG
   USE_SERIAL.print("AP IP address: ");
     USE_SERIAL.println(myIP);
  #endif
      // IP-Adresse als Laufschrift anzeigen
    char buf[20];
    sprintf(buf, "IP:%d.%d.%d.%d", WiFi.softAPIP()[0], WiFi.softAPIP()[1], WiFi.softAPIP()[2], WiFi.softAPIP()[3] );
    uint16_t StringLength = sizeof(buf)/ sizeof(buf[0]);
    StringLength = StringLength * 6;
    for (int i=0; i <= StringLength; i++) {
      zeigeipap();
      delay(200);
    }
    // ---- ENDE Print the IP address
  wlan_client = false;

}

//------------------------------------------------------------------------------

void WiFiReconnect()
{
  // WLAN reconnect
  wlan_status = WiFi.status();
  #ifdef DEBUG
    USE_SERIAL.printf("WLAN-Status: %s\n", wstatus[wlan_status]);
  #endif
  //WiFi.reconnect();
  WiFi.disconnect();
  delay(300);
  WiFi.mode(WIFI_STA);
  WiFi.begin((const char*)G.ssid, (const char*)G.passwd);
  delay(300);
  #ifdef DEBUG
   USE_SERIAL.printf("WLAN-Restart-Status: %s\n", wstatus[wlan_status]);
  #endif
}

//------------------------------------------------------------------------------

void WlanStart()
{
  #ifdef DEBUG
    USE_SERIAL.printf("\n-- Begin WlanStart -- \n");
  #endif

  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  delay(300);
  WiFiScan(false);
  if (wlan_ssid == true){
    WiFiStart_Client();
  }
  if (wlan_client == false){
    WiFiStart_AP();
  }

  //-------------------------------------
  // Start udp
  //-------------------------------------
  if (wlan_client == true) {
    #ifdef DEBUG
     USE_SERIAL.println("Start UDP");
    #endif
    udp.begin(localPort);
    #ifdef DEBUG
     USE_SERIAL.print("Local port: ");
     USE_SERIAL.println(udp.localPort());
    #endif
  }

  //-------------------------------------
  // Zeitanfrage beim NTP-Server
  //-------------------------------------
  if (wlan_client == true) {
    set_ntp_zeit();
    // Zeit setzen
    setTime(unix_time);
  }
  #ifdef DEBUG
    USE_SERIAL.printf("-- Ende  WlanStart -- \n\n");
  #endif
}

//------------------------------------------------------------------------------

void eeprom_write()
{
  EEPROM_writeAnything( 0, G );
  EEPROM.commit();
}

//------------------------------------------------------------------------------

void eeprom_read()
{
  EEPROM_readAnything( 0, G );
  #ifdef DEBUG
   USE_SERIAL.print("Version   : ");
   USE_SERIAL.println(VER);
   USE_SERIAL.printf("Sernr     : %u\n",  G.sernr);
   USE_SERIAL.printf("SSID      : %s\n",  G.ssid);
   USE_SERIAL.printf("Passwd    : %s\n",  G.passwd);
   USE_SERIAL.printf("Programm  : %u\n",  G.prog);
   USE_SERIAL.printf("Conf      : %u\n",  G.conf);
   USE_SERIAL.printf("rgb.0.0   : %u\n",  G.rgb[0][0]);
   USE_SERIAL.printf("rgb.0.1   : %u\n",  G.rgb[0][1]);
   USE_SERIAL.printf("rgb.0.2   : %u\n",  G.rgb[0][2]);
   USE_SERIAL.printf("rgb.0.0   : %u\n",  G.rgb[1][0]);
   USE_SERIAL.printf("rgb.0.1   : %u\n",  G.rgb[1][1]);
   USE_SERIAL.printf("rgb.0.2   : %u\n",  G.rgb[1][2]);
   USE_SERIAL.printf("rgb.0.0   : %u\n",  G.rgb[2][0]);
   USE_SERIAL.printf("rgb.0.1   : %u\n",  G.rgb[2][1]);
   USE_SERIAL.printf("rgb.0.2   : %u\n",  G.rgb[2][2]);
   USE_SERIAL.printf("rgb.0.0   : %u\n",  G.rgb[3][0]);
   USE_SERIAL.printf("rgb.0.1   : %u\n",  G.rgb[3][1]);
   USE_SERIAL.printf("rgb.0.2   : %u\n",  G.rgb[3][2]);
   USE_SERIAL.printf("Zeitserver: %s\n",  G.zeitserver);
   USE_SERIAL.printf("Text      : %s\n",  G.ltext);
   USE_SERIAL.printf("H6        : %u\n",  G.h6);
   USE_SERIAL.printf("H8        : %u\n",  G.h8);
   USE_SERIAL.printf("H12       : %u\n",  G.h12);
   USE_SERIAL.printf("H16       : %u\n",  G.h16);
   USE_SERIAL.printf("H18       : %u\n",  G.h18);
   USE_SERIAL.printf("H22       : %u\n",  G.h22);
   USE_SERIAL.printf("H24       : %u\n",  G.h24);
   USE_SERIAL.printf("LDR       : %u\n",  G.ldr);
   USE_SERIAL.printf("LDRCal    : %u\n",  G.ldrCal);
   USE_SERIAL.print("OWM_apikey: ");
   USE_SERIAL.println(G.apikey);
   USE_SERIAL.print("OWM_city  : ");
   USE_SERIAL.println(G.cityid);

  #endif
  delay(100);
}

//------------------------------------------------------------------------------

void WiFiEvent(WiFiEvent_t event) {
  USE_SERIAL.printf("[WiFi-event] event: %d\n", event);

  switch(event) {
    case WIFI_EVENT_STAMODE_GOT_IP:
      USE_SERIAL.println("WiFi connected");
      USE_SERIAL.println("IP address: ");
      USE_SERIAL.println(WiFi.localIP());
      break;
    case WIFI_EVENT_STAMODE_DISCONNECTED:
      USE_SERIAL.println("WiFi lost connection");
      break;
  }
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------



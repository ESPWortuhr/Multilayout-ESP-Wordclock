/*--------------------------------------------------
 * Hier Anpassungen der Hardware vornehmen.
 */

#define VER "2.0.9"  // Software Version

/*--------------------------------------------------------------------------
 * Hier wird definiert, welche Anzahl von LED´s bzw. Reihen verwendet werden
 */
#define UHR_114                       // Uhr mit 10 Reihen, jeweils 11 LED´s pro Reihe + 4 LED´s für Minuten
//#define UHR_125                       // Uhr mit 11 Reihen, jeweils 11 LED´s pro Reihe + 4 LED´s für Minuten
//#define UHR_169                     // Uhr mit zusätzlichen LED´s um den Rahmen seitlich zu beleuchten

#define SERNR 118             //um das eeprom zu löschen, bzw. zu initialisieren, hier eine andere Seriennummer eintragen!
#define DEBUG                 //DEBUG ON|OFF wenn auskommentiert
uint8_t show_ip = true;      // Zeige IP Adresse beim Start 

/*--------------------------------------------------------------------------
 * ENDE Hardware Konfiguration. Ab hier nichts mehr Ändern!!!
/*--------------------------------------------------------------------------
 */

#ifndef _UHR_H
#define _UHR_H

#define USE_SERIAL Serial

#ifdef UHR_114 
#define NUM_PIXELS   114                
#define NUM_SMATRIX  114                
#define ROWS_MATRIX   11                
#endif  

#ifdef UHR_125 
#define NUM_PIXELS   125                
#define NUM_SMATRIX  125                
#define ROWS_MATRIX   12                
#endif  

#ifdef UHR_169 
#define NUM_PIXELS  169
#define NUM_SMATRIX 121
#define ROWS_MATRIX  11
#define NUM_RMATRIX  48
#endif 

#define ESIST 0
#define VOR   1
#define NACH  2
#define UHR   3

#define FUENF   4
#define ZEHN    5 
#define VIERTEL 6
#define ZWANZIG 7
#define HALB    8
#define EINS    9

#define H_EIN    10
#define H_ZWEI   11
#define H_DREI   12
#define H_VIER   13      
#define H_FUENF  14
#define H_SECHS  15
#define H_SIEBEN 16
#define H_ACHT   17
#define H_NEUN   18
#define H_ZEHN   19
#define H_ELF    20
#define H_ZWOELF 21


struct GLOBAL {
  int sernr;
  char ssid[25];    
  char passwd[25];
  int prog;  
  int param1;  
  int param2;        
  int prog_init;  
  int conf;
  int rgb[4][3];
  int rr;
  int gg;
  int bb;   
  int hell;   
  int ldr;
  int ldrCal;
  int geschw;   
  int client_nr;
  int zeige_sek;    
  int zeige_min;
  char zeitserver[30];   
  char hostname[30];  
  char ltext[30];   
  int hh;  
  int h6;
  int h8;
  int h12;
  int h16;      
  int h18;     
  int h20;
  int h22;
  int h24;
  int rgb1[10][5];
  int rgb2[10][5];
  int rgb3[10][5];  
};
GLOBAL G = { };

// LDR 
long waitUntilLDR = 0;
int autoBrightnessEnabled = 1;
int ldrVal = 50;
int oneseconddelay = 1000;

// Telnet vars
bool ConnectionEstablished; // Flag for successfully handled connection
#define MAX_TELNET_CLIENTS 2
WiFiServer TelnetServer(23);
WiFiClient TelnetClient[MAX_TELNET_CLIENTS];

const char *ssid_ap = "Uhr";
const char *password_ap = "12345678";
//--OTA--
const char* host = "webupdate";
//--OTA--

unsigned char wlan_client = false;
unsigned char wlan_status = 99;
unsigned char wlan_ssid = false;

unsigned int _sekunde = 0;
unsigned int _minute = 0;
unsigned int _stunde = 0;
unsigned int last_sekunde = 100;
unsigned int last_minute = 100;
unsigned int last_stunde = 100;

unsigned int count_millis48 = 0;
unsigned long previous48    = 0;
const long interval48       = 1250;   
unsigned int _sekunde48     = 0;
unsigned int last_sekunde48 = 100;

unsigned long unix_time = 0;

unsigned int count_millis    = 0;
unsigned int count_tag       = 0;
unsigned int count_delay     = 0;
unsigned char ntp_flag = false;

unsigned long previousMillis = 0;
const long interval = 1000;   // 1 Sekunde

uint32_t uhrzeit;

char str[300];
char s[3]; 

int dim[20] = {30,50,70,90,110,130,140,160,200,255,255,200,160,100,80,60,40,20,10,0};
int diff[20]= {-30,-20,-20,-20,-20,-20,-10,-20,-40,-55,0,55,40,60,20,20,20,20,10,10};

const uint8_t PixelPin = 2;  // WS2812 Data Port

typedef RowMajorAlternatingLayout MyPanelLayout;
const uint8_t PanelWidth = 13;  // 8 pixel x 8 pixel matrix of leds
const uint8_t PanelHeight = 13;
const uint16_t PixelCount = PanelWidth * PanelHeight;  
NeoTopology<MyPanelLayout> topo(PanelWidth, PanelHeight);
  
NeoPixelBus<NeoGrbFeature, NeoEsp8266Dma800KbpsMethod> strip(PixelCount, PixelPin);

//RgbColor red(128, 0, 0);
//RgbColor green(0, 128, 0);
//RgbColor blue(0, 0, 128);
//RgbColor white(128);
//RgbColor black(0);

const uint16_t left = 0;
const uint16_t right = PanelWidth - 1;
const uint16_t top = 0;
const uint16_t bottom = PanelHeight - 1;

//-- WebSocketserver
WebSocketsServer webSocket = WebSocketsServer(80);

//--OTA--
ESP8266WebServer httpServer(81);
ESP8266HTTPUpdateServer httpUpdater;
//--OTA--

// A UDP instance to let us send and receive packets over UDP
//WiFiUDP udp;

void WiFiScan();
void WiFiStart_Client();
void WiFiStart_AP();
void WiFiReconnect();
void WlanStart();
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght);
void eeprom_write();
void eeprom_read();
int split(int i, int j);
void WiFiEvent(WiFiEvent_t event);

#endif

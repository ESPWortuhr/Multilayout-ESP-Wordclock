#include <Arduino.h>

//------------------------------------------------------------------------------
// Telnet Server für Konsolen Ausgaben
//------------------------------------------------------------------------------

static void Telnet()
{
  // Cleanup disconnected session
  for(uint8_t i = 0; i < MAX_TELNET_CLIENTS; i++)
  {
    if (TelnetClient[i] && !TelnetClient[i].connected())
    {
      Serial.print("Client disconnected ... terminate session "); Serial.println(i+1); 
      TelnetClient[i].stop();
    }
  }
  
  // Check new client connections
  if (TelnetServer.hasClient())
  {
    ConnectionEstablished = false; // Set to false
    
    for(uint8_t i = 0; i < MAX_TELNET_CLIENTS; i++)
    {
      // Serial.print("Checking telnet session "); Serial.println(i+1);
      
      // find free socket
      if (!TelnetClient[i])
      {
        TelnetClient[i] = TelnetServer.available(); 
        
        Serial.print("New Telnet client connected to session "); Serial.println(i+1);
        
        TelnetClient[i].flush();  // clear input buffer, else you get strange characters
        TelnetClient[i].println("Welcome!");
        
        TelnetClient[i].print("Millis since start: ");
        TelnetClient[i].println(millis());
        
        TelnetClient[i].print("Free Heap RAM: ");
        TelnetClient[i].println(ESP.getFreeHeap());
  
        TelnetClient[i].println("----------------------------------------------------------------");
        
        ConnectionEstablished = true; 
        
        break;
      }
      else
      {
        // Serial.println("Session is in use");
      }
    }
 
    if (ConnectionEstablished == false)
    {
      Serial.println("No free sessions ... drop connection");
      TelnetServer.available().stop();
      // TelnetMsg("An other user cannot connect ... MAX_TELNET_CLIENTS limit is reached!");
    }
  }
 
  for(uint8_t i = 0; i < MAX_TELNET_CLIENTS; i++)
  {
    if (TelnetClient[i] && TelnetClient[i].connected())
    {
      if(TelnetClient[i].available())
      { 
        //get data from the telnet client
        while(TelnetClient[i].available())
        {
          Serial.write(TelnetClient[i].read());
        }
      }
    }
  }
}

static void TelnetMsg(String text)
{
  for(uint8_t i = 0; i < MAX_TELNET_CLIENTS; i++)
  {
    if (TelnetClient[i] || TelnetClient[i].connected())
    {
      TelnetClient[i].println(text);
    }
  }
  delay(10);  // to astatic void strange characters left in buffer
}

//------------------------------------------------------------------------------

static inline void led_set_pixel(uint8_t rr, uint8_t gg, uint8_t bb, uint8_t ww, uint16_t i) {
#ifdef Grbw
	strip.SetPixelColor(i, RgbwColor(rr, gg, bb, ww));
#else
	strip.SetPixelColor(i, RgbColor(rr, gg, bb));
#endif
}


//------------------------------------------------------------------------------
// Helligkeitsregelung nach Uhrzeiten oder per LDR
//------------------------------------------------------------------------------


static void led_set(uint16_t i) {
	uint8_t rr, gg, bb, ww;
    if (G.ldr == 1) {
    rr = (int)G.rgb[0][0] / ldrVal;
    gg = (int)G.rgb[0][1] / ldrVal;
    bb = (int)G.rgb[0][2] / ldrVal;
    ww = (int)G.rgb[0][3] / ldrVal;
    }
    else
    {
    rr = (int)G.rgb[0][0] * G.hh / 100;
    gg = (int)G.rgb[0][1] * G.hh / 100;
    bb = (int)G.rgb[0][2] * G.hh / 100;
    ww = (int)G.rgb[0][3] * G.hh / 100;
    }
	led_set_pixel(rr,gg,bb,ww,i);
}

//------------------------------------------------------------------------------

#ifdef UHR_114_Fraenkisch
#include "uhr_func_114_Fraenkisch.h"
#endif

#ifdef UHR_114
#include "uhr_func_114.h"
#endif

#ifdef UHR_125
#include "uhr_func_125.h"
#endif

#ifdef UHR_169
#include "uhr_func_169.h"
#endif

#ifdef UHR_242 
#include "uhr_func_242.h"
#endif 

//------------------------------------------------------------------------------

static inline void led_show() {
  strip.Show();
}

//------------------------------------------------------------------------------

static inline void led_clear_pixel(uint16_t i) {
  strip.SetPixelColor(i, 0);      
}

//------------------------------------------------------------------------------

static inline void led_clear() {
  for(uint16_t i=0; i<NUM_PIXELS; i++)
  {
	  led_clear_pixel(i);
  }
}

//------------------------------------------------------------------------------

static inline void uhr_clear() {
  for(uint16_t i=0; i<NUM_SMATRIX; i++)
  {
    strip.SetPixelColor(smatrix[i], 0);
  }
}

//------------------------------------------------------------------------------

#ifdef UHR_169 
static inline void rahmen_clear() {
  for(uint16_t i=0; i<NUM_RMATRIX; i++)
  {
    strip.SetPixelColor(rmatrix[i], 0);
  }
}
#endif   

//------------------------------------------------------------------------------
//HSV to RGB 8Bit
//Farbkreis h = 0 bis 360 (Farbwert)
//          s = 0 bis 100 (Dunkelstufe)
//          v = 0 bis 100 (Farbsättigung)
//Rückgabewert r,g,b als Pointer
//------------------------------------------------------------------------------

byte *  hsv_to_rgb (unsigned int h,unsigned char s,unsigned char v)
{   
    unsigned char diff;
    unsigned int r = 0, g = 0 ,b = 0, w = 0;
    static byte c[4];

    //Winkel im Farbkeis 0 - 360 in 1 Grad Schritten
    //h = (englisch hue) Farbwert
    //1 Grad Schrittweite, 4.25 Steigung pro Schritt bei 60 Grad

    if(h<61){
      r = 255;
      b = 0;
      g = (425 * h) / 100;
    }else if(h < 121){
      g = 255;
      b = 0;
      r = 255 - ((425 * (h-60))/100);
    }else if(h < 181){
      r = 0;
      g = 255;
      b = (425 * (h-120))/100;
    }else if(h < 241){
      r = 0;
      b = 255;
      g = 255 - ((425 * (h-180))/100);
    }else if(h < 301){
      g = 0;
      b = 255;
      r = (425 * (h-240))/100;
    }else if(h< 361){
      r = 255;
      g = 0;
      b = 255 - ((425 * (h-300))/100);
    }   
    
  //Berechnung der Farbsättigung
  //s = (englisch saturation) Farbsättigung
  s = 255 - s; //Kehrwert berechnen
  diff = ((255 - r) * s)/255;
  r = r + diff;
  diff = ((255 - g) * s)/255;
  g = g + diff;
  diff = ((255 - b) * s)/255;
  b = b + diff;
    
  //Berechnung der Dunkelstufe
  //v = (englisch value) Wert Dunkelstufe einfacher Dreisatz 0..100%
  r = (r * v)/255;   
  g = (g * v)/255;     
  b = (b * v)/255;
  w = (r + g + b)/3;   

  c[0] = r;
  c[1] = g;
  c[2] = b;
  c[3] = w;    
  return c;  
}

//------------------------------------------------------------------------------

static void led_single(uint8_t wait) {

  unsigned int h, a;
  byte *c;
    
  for(uint16_t i=0; i<NUM_PIXELS; i++){

    h = 360*i/(NUM_PIXELS-1);
    h += ((360/(NUM_PIXELS)));
    if(h>360) h-=360;    
    
    led_clear();
    c = hsv_to_rgb(h,255,255);
    led_set_pixel(c[0], c[1], c[2], c[3], i);
    led_show();
    delay(wait);
  }
}

//------------------------------------------------------------------------------

static void set_farbe() {

  unsigned int rr, gg, bb, ww, zz;  
  rr = G.rgb[3][0];
  gg = G.rgb[3][1];
  bb = G.rgb[3][2];
  ww = G.rgb[3][3];
  zz = rr + gg + bb;
  if (zz > 150) {
    zz = zz * 10 / 150;
    rr = (int)rr * 10 / zz;
    gg = (int)gg * 10 / zz;
    bb = (int)bb * 10 / zz;
	ww = (int)ww * 10 / zz;
  }
  for(uint16_t i = 0; i < NUM_PIXELS;i++){
    led_set_pixel(rr, gg, bb, ww, i);
  }
}

//------------------------------------------------------------------------------
// Routine Helligkeitsregelung
//------------------------------------------------------------------------------

static void doLDRLogic() {   
  if(millis() >= waitUntilLDR) {
     waitUntilLDR = millis();
     int temp = analogRead(A0);
     temp = temp - G.ldrCal;
     if (temp >= 900 ) temp = 900;
     if (temp <= 1 ) temp = 1;
      ldrVal = map(temp, 1, 900, 1, 100);
      waitUntilLDR += oneseconddelay;
  }
}

//------------------------------------------------------------------------------

#ifdef UHR_169 
static void set_farbe_rahmen() {

  unsigned int rr, gg, bb, ww, zz;  
  rr = G.rgb[2][0];
  gg = G.rgb[2][1];
  bb = G.rgb[2][2];
  ww = G.rgb[2][3];
  zz = rr + gg + bb;
  if (zz > 150) {
    zz = zz * 10 / 150;
    rr = (int)rr * 10 / zz;
    gg = (int)gg * 10 / zz;
    bb = (int)bb * 10 / zz;
	ww = (int)ww * 10 / zz;
  } 
  for(uint16_t i = 0; i < NUM_RMATRIX;i++){
    led_set_pixel(rr, gg, bb, ww, rmatrix[i]);
  }
}
#endif   

//------------------------------------------------------------------------------

static void rainbow() {

  static int h=0;
  byte *c;
  c = hsv_to_rgb(h, 255, G.hell*10);
    
  for(uint16_t i = 0; i < NUM_PIXELS;i++){
    led_set_pixel(c[0], c[1], c[2], c[3], i);
  }
  led_show();
  h++;
  if (h > 359){ h = 0; }  
}

//-----------------------------------------------------------------------------

static void rainbowCycle() {
  static int h=0;  
  uint16_t hh;
  byte *c;

  hh = h;
  for(uint16_t i=0; i< NUM_SMATRIX; i++) {
    c = hsv_to_rgb(hh, 255, G.hell*10);
    led_set_pixel(c[0], c[1], c[2], c[3], smatrix[i]);
    hh = hh + (360 / NUM_SMATRIX);
    if (hh > 360){ hh = 0; }      
  }
  led_show();
  h++;  
  if (h > 360){ h = 0; }    
}

//------------------------------------------------------------------------------

#ifdef UHR_169 
static void schweif_up(){
  
  int l, c, j;
  static int t=0, x=0;

  for (uint16_t i = 0;i<10;i++){
    l=diff[i]*x/(G.geschw+1);
    c=dim[i]+l;
    if (c > 255){ c = 255; }
    if (c < 0)  { c = 0;   }
    
    G.rr = (G.rgb[3][0] * c)/255;   
    G.gg = (G.rgb[3][1] * c)/255;     
    G.bb = (G.rgb[3][2] * c)/255;
	G.ww = (G.rgb[3][3] * c)/255;  
    j = t + i;
    if (j >= 48){ j = j -48; }
    led_set_pixel(G.rr, G.gg, G.bb, G.ww, rmatrix[i]);
  }  
  led_show();
  x++;
  if (x >(G.geschw+1)){ x=0; t++; }
  if (t >= 48){ t = 0;  }  

}
#endif   

//------------------------------------------------------------------------------

static void zeigeipap() {
    static uint16_t i = 0, ii = 0;
        
    char buf[20];
    sprintf(buf, "IP:%d.%d.%d.%d", WiFi.softAPIP()[0], WiFi.softAPIP()[1], WiFi.softAPIP()[2], WiFi.softAPIP()[3] );

  // Alle Pixes eins nach rechts schieben
  for (uint8_t b = 0;b<10;b++) {
    for (uint8_t a = 0; a < ROWS_MATRIX; a++) {
      strip.SetPixelColor(matrix[a][b], strip.GetPixelColor(matrix[a][b+1]));              
    }
  }  
    
  if (i < 5) {
    for(uint8_t h=0;h<8;h++){
      if (font_7x5[buf[ii]][i] & (1 << h)) {
        led_set_pixel(G.rgb[3][0], G.rgb[3][1], G.rgb[3][2], G.rgb[3][3], matrix[h+1][10]);
      }else{            
        led_clear_pixel(matrix[h+1][10]);  
      }        
    }
  }else{ 
    for(uint8_t h=0;h<8;h++){
      led_clear_pixel(matrix[h+1][10]);          
    }
  }  
  led_show();

  i++;
  if (i > 5) { 
    i = 0; 
    ii++;
    if (ii > strlen(buf)){ ii = 0; }   
  }   
}
//------------------------------------------------------------------------------

static void zeigeip() {
    static int i = 0, ii = 0;
        
    char buf[20];
    sprintf(buf, "IP:%d.%d.%d.%d", WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], WiFi.localIP()[3] );

  // Alle Pixes eins nach recht schieben 
  for (uint8_t b = 0;b<10;b++) {
    for (uint8_t a = 0; a < ROWS_MATRIX; a++) {
      strip.SetPixelColor(matrix[a][b], strip.GetPixelColor(matrix[a][b+1]));              
    }
  }  
    
  if (i < 5) {
    for(uint8_t h=0;h<8;h++){
      if (font_7x5[buf[ii]][i] & (1 << h)) {
		 led_set_pixel(G.rgb[3][0], G.rgb[3][1], G.rgb[3][2], G.rgb[3][3], matrix[h+1][10]);
      }else{            
        led_clear_pixel(matrix[h+1][10]);  
      }        
    }
  }else{ 
    for(uint8_t h=0;h<8;h++){
      led_clear_pixel(matrix[h+1][10]);          
    }
  }  
  led_show();

  i++;
  if (i > 5) { 
    i = 0; 
    ii++;
    if (ii > strlen(buf)){ ii = 0; }   
  }   
}
//------------------------------------------------------------------------------

static void laufschrift() {

  static int i = 0, ii = 0;

  // Alle Pixes eins nach recht schieben 
  for (uint8_t b = 0;b<10;b++) {
    for (uint8_t a = 0; a < ROWS_MATRIX; a++) {
      strip.SetPixelColor(matrix[a][b], strip.GetPixelColor(matrix[a][b+1]));              
    }
  }  
    
  if (i < 5) {
    for(uint8_t h=0;h<8;h++){
      if (font_7x5[G.ltext[ii]][i] & (1 << h)) {
 		 led_set_pixel(G.rgb[3][0], G.rgb[3][1], G.rgb[3][2], G.rgb[3][3], matrix[h+1][10]);
      }else{            
        led_clear_pixel(matrix[h+1][10]);  
      }        
    }
  }else{ 
    for(uint8_t h=0;h<8;h++){
      led_clear_pixel(matrix[h+1][10]);          
    }
  }  
  led_show();

  i++;
  if (i > 5) { 
    i = 0; 
    ii++;
   if (ii > strlen(G.ltext)){ ii = 0; }    
  }  
}

//------------------------------------------------------------------------------

static void zahlen(char d1, char d2) {
   #ifdef DEBUG 
//    USE_SERIAL.printf("d1: %u %c \n", d1, d1);
//    USE_SERIAL.printf("d2: %u %c \n", d2, d2);
   #endif 
   uhr_clear();
   for(uint8_t i=0;i<5;i++){
    for(uint8_t h=0;h<8;h++){
      // 1. Zahl
      if (font_7x5[d1][i] & (1 << h)) {
 		 led_set_pixel(G.rgb[3][0], G.rgb[3][1], G.rgb[3][2], G.rgb[3][3], matrix[h+1][i]);
      }
      // 2. Zahl
      if (font_7x5[d2][i] & (1 << h)) {
 		 led_set_pixel(G.rgb[3][0], G.rgb[3][1], G.rgb[3][2], G.rgb[3][3], matrix[h+1][i+6]);
      }        
    }
  }
  led_show();
}

//------------------------------------------------------------------------------

static void laufen(unsigned int d, unsigned char aktion){
  
  if (aktion == 0){
    for(uint8_t t = 0;t < NUM_SMATRIX;t++) {
      for (uint8_t a = NUM_SMATRIX;a>1;a--)
      {
        strip.SetPixelColor(smatrix[a-1], strip.GetPixelColor(smatrix[a-2]));           
      }
      led_set_pixel(G.rr, G.gg, G.bb, G.ww, smatrix[0]);
      led_show();
      delay(d);
    }
  }
  if (aktion == 1){ 
    for(uint8_t t = 0;t < NUM_SMATRIX;t++) {
      for (uint8_t a = NUM_SMATRIX;a>1;a--)
      {
        //led1[a-1].r= led1[a-2].r;
      }
      //led1[0].r = led[G.anz_leds - t - 1].r;
      led_show();
      delay(d);
    }
  }
}

//------------------------------------------------------------------------------

static void wischen(unsigned char r, unsigned char g, unsigned char b, unsigned int d) {
	uint8_t t;

  for (t = 0; t < NUM_SMATRIX; t++)
  {
    for (uint8_t u = 0; u < 11; u++)
    {
//      led_set_pixel(r, g, b, matrix[t][u]);   
    }
    if (t > 0) {
      for (uint8_t v = 0; v < 11; v++)
      {
        led_set_pixel(G.rr, G.gg, G.bb, G.ww, matrix[t][v]);
      }
    }
    led_show();
    delay(d);
  }
  for (uint8_t u = 0; u < 11; u++)
  {
	  led_set_pixel(G.rr, G.gg, G.bb, G.ww, matrix[t-1][u]);
  }
  led_show();
}

//------------------------------------------------------------------------------

static void schieben(int d, unsigned char aktion){
	uint8_t a;

  if (aktion == 0){ 
    for(uint8_t t = 0;t < NUM_SMATRIX;t++)
    {
      for (a = NUM_SMATRIX-1;a>0;a--)
      {
        for (uint8_t b = 0;b<11;b++)
        {
          strip.SetPixelColor(matrix[a][b], strip.GetPixelColor(matrix[a-1][b]));  
        } 
      }
      for (uint8_t b = 0;b<11;b++)
      {
    	led_set_pixel(G.rr, G.gg, G.bb, G.ww, matrix[a][b]);
      }
      led_show();
      delay(d);
    }
  }
}


//------------------------------------------------------------------------------

static void set_stunde(unsigned char std, unsigned char voll) {

  switch (std) {
    case 0:  uhrzeit |= ((uint32_t)1 << H_ZWOELF);  break;
    case 1:
      if (voll == 1){ uhrzeit |= ((uint32_t)1 << H_EIN); }
      else          { uhrzeit |= ((uint32_t)1 << EINS);  }
      break;
    case 2:  uhrzeit |= ((uint32_t)1 << H_ZWEI);  break;
    case 3:  uhrzeit |= ((uint32_t)1 << H_DREI);  break;
    case 4:  uhrzeit |= ((uint32_t)1 << H_VIER);  break;
    case 5:  uhrzeit |= ((uint32_t)1 << H_FUENF); break;
    case 6:  uhrzeit |= ((uint32_t)1 << H_SECHS); break;
    case 7:  uhrzeit |= ((uint32_t)1 << H_SIEBEN);  break;
    case 8:  uhrzeit |= ((uint32_t)1 << H_ACHT);  break;
    case 9:  uhrzeit |= ((uint32_t)1 << H_NEUN);  break;
    case 10: uhrzeit |= ((uint32_t)1 << H_ZEHN);  break;
    case 11: uhrzeit |= ((uint32_t)1 << H_ELF);   break;
    case 12: uhrzeit |= ((uint32_t)1 << H_ZWOELF);  break;
    case 13:
      if (voll == 1){ uhrzeit |= ((uint32_t)1 << H_EIN); }
      else          { uhrzeit |= ((uint32_t)1 << EINS);  }
      break;
    case 14: uhrzeit |= ((uint32_t)1 << H_ZWEI);  break;
    case 15: uhrzeit |= ((uint32_t)1 << H_DREI);  break;
    case 16: uhrzeit |= ((uint32_t)1 << H_VIER);  break;
    case 17: uhrzeit |= ((uint32_t)1 << H_FUENF); break;
    case 18: uhrzeit |= ((uint32_t)1 << H_SECHS); break;
    case 19: uhrzeit |= ((uint32_t)1 << H_SIEBEN);  break;
    case 20: uhrzeit |= ((uint32_t)1 << H_ACHT);  break;
    case 21: uhrzeit |= ((uint32_t)1 << H_NEUN);  break;
    case 22: uhrzeit |= ((uint32_t)1 << H_ZEHN);  break;
    case 23: uhrzeit |= ((uint32_t)1 << H_ELF);   break;
    case 24: uhrzeit |= ((uint32_t)1 << H_ZWOELF);  break;
  }
}

//------------------------------------------------------------------------------

static void set_uhrzeit() {

  unsigned int m = 0;
  uhrzeit = 0;
  uhrzeit |= ((uint32_t)1 << ESIST);

  m = _minute / 5;
  switch (m) {
    case 0: // volle Stunde
      uhrzeit |= ((uint32_t)1 << UHR);
      set_stunde(_stunde, 1);
      break;
    case 1: // 5 nach
      uhrzeit |= ((uint32_t)1 << FUENF);
      uhrzeit |= ((uint32_t)1 << NACH);
      set_stunde(_stunde, 0);
      break;
    case 2: // 10 nach
      uhrzeit |= ((uint32_t)1 << ZEHN);
      uhrzeit |= ((uint32_t)1 << NACH);
      set_stunde(_stunde, 0);
      break;
    case 3: // viertel nach
      uhrzeit |= ((uint32_t)1 << VIERTEL);
      uhrzeit |= ((uint32_t)1 << NACH);
      set_stunde(_stunde, 0);
      break;
    case 4: // 20 nach
      uhrzeit |= ((uint32_t)1 << ZWANZIG);
      uhrzeit |= ((uint32_t)1 << NACH);
      set_stunde(_stunde, 0);
      break;
    case 5: // 5 vor halb
      uhrzeit |= ((uint32_t)1 << FUENF);
      uhrzeit |= ((uint32_t)1 << VOR);
      uhrzeit |= ((uint32_t)1 << HALB);
      set_stunde(_stunde + 1, 0);
      break;
    case 6: // halb
      uhrzeit |= ((uint32_t)1 << HALB);
      set_stunde(_stunde + 1, 0);
      break;
    case 7: // 5 nach halb
      uhrzeit |= ((uint32_t)1 << FUENF);
      uhrzeit |= ((uint32_t)1 << NACH);
      uhrzeit |= ((uint32_t)1 << HALB);
      set_stunde(_stunde + 1, 0);
      break;
    case 8: // 20 vor
      uhrzeit |= ((uint32_t)1 << ZWANZIG);
      uhrzeit |= ((uint32_t)1 << VOR);
      set_stunde(_stunde + 1, 0);
      break;
    case 9: // viertel vor
      uhrzeit |= ((uint32_t)1 << VIERTEL);
      uhrzeit |= ((uint32_t)1 << VOR);
      set_stunde(_stunde + 1, 0);
      break;
    case 10: // 10 vor
      uhrzeit |= ((uint32_t)1 << ZEHN);
      uhrzeit |= ((uint32_t)1 << VOR);
      set_stunde(_stunde + 1, 0);
      break;
    case 11: // 5 vor
      uhrzeit |= ((uint32_t)1 << FUENF);
      uhrzeit |= ((uint32_t)1 << VOR);
      set_stunde(_stunde + 1, 0);
      break;
  }
}

//------------------------------------------------------------------------------

#ifdef UHR_169 
static void show_sekunde() {
  uint8_t rr, gg, bb, ww, zz;

  rr = G.rgb[2][0];
  gg = G.rgb[2][1];
  bb = G.rgb[2][2];
  ww = G.rgb[2][3];
  zz = rr + gg + bb;
  if (zz > 150) {
    zz = zz * 10 / 150;
    rr = (int)rr * 10 / zz;
    gg = (int)gg * 10 / zz;
    bb = (int)bb * 10 / zz;
    ww = (int)ww * 10 / zz;
  }  
  led_set_pixel(rr, gg, bb, ww, rmatrix[_sekunde48]);
  
}
#endif 


//------------------------------------------------------------------------------

#ifdef UHR_114_Fraenkisch
static void show_minuten() {
  uint8_t m;

  if (G.zeige_min > 0){  
    // Minuten / Sekunden-Animation
    // Minute (1-4)  ermitteln
    m = _minute;
    while (m > 4) { m -= 5; }

    if (m > 0){ led_set(min_arr[G.zeige_min-1][0]); }
    if (m > 1){ led_set(min_arr[G.zeige_min-1][1]); }
    if (m > 2){ led_set(min_arr[G.zeige_min-1][2]); }
    if (m > 3){ led_set(min_arr[G.zeige_min-1][3]); }                                             
  }   
}
#endif  

//------------------------------------------------------------------------------

#ifdef UHR_114
static void show_minuten() {
  uint8_t m;

  if (G.zeige_min > 0){  
    // Minuten / Sekunden-Animation
    // Minute (1-4)  ermitteln
    m = _minute;
    while (m > 4) { m -= 5; }

    if (m > 0){ led_set(min_arr[G.zeige_min-1][0]); }
    if (m > 1){ led_set(min_arr[G.zeige_min-1][1]); }
    if (m > 2){ led_set(min_arr[G.zeige_min-1][2]); }
    if (m > 3){ led_set(min_arr[G.zeige_min-1][3]); }                                             
  }   
}
#endif   

//------------------------------------------------------------------------------

#ifdef UHR_125                                                                                        //vorher 169  markus 
static void show_minuten() {
  uint8_t m;

  if (G.zeige_min > 0){  
    // Minuten / Sekunden-Animation
    // Minute (1-4)  ermitteln
    m = _minute;
    while (m > 4) { m -= 5; }

    if (m > 0){ led_set(min_arr[G.zeige_min-1][0]); }                                               
    if (m > 1){ led_set(min_arr[G.zeige_min-1][1]); }                                             
    if (m > 2){ led_set(min_arr[G.zeige_min-1][2]); }                                             
    if (m > 3){ led_set(min_arr[G.zeige_min-1][3]); }                                             
  }   
}
#endif   

//------------------------------------------------------------------------------

#ifdef UHR_242                                                                                        //vorher 169  markus 
static void show_minuten() {
  uint8_t m;

  if (G.zeige_min > 0){  
    // Minuten / Sekunden-Animation
    // Minute (1-4)  ermitteln
    m = _minute;
    while (m > 4) { m -= 5; }

    if (m > 0){ led_set(min_arr[G.zeige_min-1][0]); }                                               
    if (m > 1){ led_set(min_arr[G.zeige_min-1][1]); }                                             
    if (m > 2){ led_set(min_arr[G.zeige_min-1][2]); }                                             
    if (m > 3){ led_set(min_arr[G.zeige_min-1][3]); }                                             
  }   
}

// Wetterdaten anzeigen
static void show_wetter() {

   switch (wetterswitch) {
   // +6h
   case 1: {
        switch (wstunde) {
          case 1:   w_mittag(); break;
          case 2:   w_abend(); break;
          case 3:   w_nacht(); break;
          case 4:   { w_morgen(); w_frueh(); } break;   
          }
        switch (wtemp_6) {
          case 30:  { w_ueber(); w_dreissig(); w_grad(); } break;
          case 25:  { w_ueber(); w_fuenf(); w_und(); w_zwanzig(); w_grad(); } break;
          case 20:  { w_ueber(); w_zwanzig(); w_grad(); } break;
          case 15:  { w_ueber(); w_fuenf(); w_zehn(); w_grad(); } break;
          case 10:  { w_ueber(); w_zehn(); w_grad(); } break;
          case 5:  { w_ueber(); w_fuenf(); w_grad(); } break;
          case 1:  { w_ueber(); w_null(); w_grad(); } break;
          case -1:  { w_unter(); w_minus(); w_null(); w_grad(); } break;
          case -5:  { w_unter(); w_minus(); w_fuenf(); w_grad(); } break;
          case -10:  { w_unter(); w_minus(); w_zehn(); w_grad(); } break;
          case -15:  { w_unter(); w_minus(); w_fuenf(); w_zehn(); w_grad(); } break;
          case -20:  { w_unter(); w_minus(); w_zwanzig(); w_grad(); } break;
          case -25:  { w_unter(); w_minus(); w_fuenf(); w_und(); w_zwanzig(); w_grad(); } break;
          }
        switch (wwetter_6) {
          case 200: w_gewitter(); break;
          case 300: w_regen(); break;
          case 500: w_regen(); break;
          case 600: w_schnee(); break;
          case 700: w_warnung(); break;
          case 800: w_klar(); break;
          case 801: w_wolken(); break;
          }
      }
      break;
   // +12h
   case 2: {
        switch (wstunde) {
          case 1:   w_abend(); break;
          case 2:   w_nacht(); break;
          case 3:   { w_morgen(); w_frueh(); } break; 
          case 4:   { w_morgen(); w_mittag(); } break;     
          }
        switch (wtemp_12) {
          case 30:  { w_ueber(); w_dreissig(); w_grad(); } break;
          case 25:  { w_ueber(); w_fuenf(); w_und(); w_zwanzig(); w_grad(); } break;
          case 20:  { w_ueber(); w_zwanzig(); w_grad(); } break;
          case 15:  { w_ueber(); w_fuenf(); w_zehn(); w_grad(); } break;
          case 10:  { w_ueber(); w_zehn(); w_grad(); } break;
          case 5:  { w_ueber(); w_fuenf(); w_grad(); } break;
          case 1:  { w_ueber(); w_null(); w_grad(); } break;
          case -1:  { w_unter(); w_minus(); w_null(); w_grad(); } break;
          case -5:  { w_unter(); w_minus(); w_fuenf(); w_grad(); } break;
          case -10:  { w_unter(); w_minus(); w_zehn(); w_grad(); } break;
          case -15:  { w_unter(); w_minus(); w_fuenf(); w_zehn(); w_grad(); } break;
          case -20:  { w_unter(); w_minus(); w_zwanzig(); w_grad(); } break;
          case -25:  { w_unter(); w_minus(); w_fuenf(); w_und(); w_zwanzig(); w_grad(); } break;
          }
        switch (wwetter_12) {
          case 200: w_gewitter(); break;
          case 300: w_regen(); break;
          case 500: w_regen(); break;
          case 600: w_schnee(); break;
          case 700: w_warnung(); break;
          case 800: w_klar(); break;
          case 801: w_wolken(); break;
          }
 
      }        
      break;
    // +18h    
    case 3: {
        switch (wstunde) {
          case 1:   w_nacht(); break;
          case 2:   { w_morgen(); w_frueh(); } break; 
          case 3:   { w_morgen(); w_mittag(); } break; 
          case 4:   { w_morgen(); w_abend(); } break;    
          }
        switch (wtemp_18) {
          case 30:  { w_ueber(); w_dreissig(); w_grad(); } break;
          case 25:  { w_ueber(); w_fuenf(); w_und(); w_zwanzig(); w_grad(); } break;
          case 20:  { w_ueber(); w_zwanzig(); w_grad(); } break;
          case 15:  { w_ueber(); w_fuenf(); w_zehn(); w_grad(); } break;
          case 10:  { w_ueber(); w_zehn(); w_grad(); } break;
          case 5:  { w_ueber(); w_fuenf(); w_grad(); } break;
          case 1:  { w_ueber(); w_null(); w_grad(); } break;
          case -1:  { w_unter(); w_minus(); w_null(); w_grad(); } break;
          case -5:  { w_unter(); w_minus(); w_fuenf(); w_grad(); } break;
          case -10:  { w_unter(); w_minus(); w_zehn(); w_grad(); } break;
          case -15:  { w_unter(); w_minus(); w_fuenf(); w_zehn(); w_grad(); } break;
          case -20:  { w_unter(); w_minus(); w_zwanzig(); w_grad(); } break;
          case -25:  { w_unter(); w_minus(); w_fuenf(); w_und(); w_zwanzig(); w_grad(); } break;
          }
        switch (wwetter_18) {
          case 200: w_gewitter(); break;
          case 300: w_regen(); break;
          case 500: w_regen(); break;
          case 600: w_schnee(); break;
          case 700: w_warnung(); break;
          case 800: w_klar(); break;
          case 801: w_wolken(); break;
          }
 
      }        
      break;
    // +24h
    case 4: {
        switch (wstunde) {
          case 1:   { w_morgen(); w_frueh(); } break;
          case 2:   { w_morgen(); w_mittag(); } break;
          case 3:   { w_morgen(); w_abend(); } break;
          case 4:   { w_morgen(); w_nacht(); } break;    
          }
        switch (wtemp_24) {
          case 30:  { w_ueber(); w_dreissig(); w_grad(); } break;
          case 25:  { w_ueber(); w_fuenf(); w_und(); w_zwanzig(); w_grad(); } break;
          case 20:  { w_ueber(); w_zwanzig(); w_grad(); } break;
          case 15:  { w_ueber(); w_fuenf(); w_zehn(); w_grad(); } break;
          case 10:  { w_ueber(); w_zehn(); w_grad(); } break;
          case 5:  { w_ueber(); w_fuenf(); w_grad(); } break;
          case 1:  { w_ueber(); w_null(); w_grad(); } break;
          case -1:  { w_unter(); w_minus(); w_null(); w_grad(); } break;
          case -5:  { w_unter(); w_minus(); w_fuenf(); w_grad(); } break;
          case -10:  { w_unter(); w_minus(); w_zehn(); w_grad(); } break;
          case -15:  { w_unter(); w_minus(); w_fuenf(); w_zehn(); w_grad(); } break;
          case -20:  { w_unter(); w_minus(); w_zwanzig(); w_grad(); } break;
          case -25:  { w_unter(); w_minus(); w_fuenf(); w_und(); w_zwanzig(); w_grad(); } break;
          }
        switch (wwetter_24) {
          case 200: w_gewitter(); break;
          case 300: w_regen(); break;
          case 500: w_regen(); break;
          case 600: w_schnee(); break;
          case 700: w_warnung(); break;
          case 800: w_klar(); break;
          case 801: w_wolken(); break;
          }
 
      }        
      break;
                
   }      
}
#endif   


//------------------------------------------------------------------------------

static void show_zeit(int flag) {

	uint8_t m, s;
	uint8_t r, g, b, rr, gg, ww, bb, zz;
  if (flag == 1) {
    set_uhrzeit();
  }

  //Helligkeitswert ermitteln
  if      (_stunde < 6) { G.hh = G.h24; }
  else if (_stunde < 8) { G.hh = G.h6;  }
  else if (_stunde < 12){ G.hh = G.h8;  }
  else if (_stunde < 16){ G.hh = G.h12; }
  else if (_stunde < 18){ G.hh = G.h16; }
  else if (_stunde < 20){ G.hh = G.h18; }      
  else if (_stunde < 22){ G.hh = G.h20; }      
  else if (_stunde < 24){ G.hh = G.h22; }  

  //Hintergrund Helligkeit ermitteln
  rr = G.rgb[1][0];
  gg = G.rgb[1][1];
  bb = G.rgb[1][2];
  ww = G.rgb[1][3];
  zz = rr + gg + bb;
  
    //Helligkeit Hintergrund einstellen / LDR
  if (G.ldr == 1) {
  //Helligkeit LDR
  rr = (int)rr / ldrVal;
  gg = (int)gg / ldrVal;
  bb = (int)bb / ldrVal;
  ww = (int)ww / ldrVal;   
  } else {
  rr = (int)rr * G.hh / 100;
  gg = (int)gg * G.hh / 100;
  bb = (int)bb * G.hh / 100;
  ww = (int)ww * G.hh / 100;      
  }
  
  //Hintergrund setzen
  for (uint8_t t = 0; t < ROWS_MATRIX; t++){
    for (uint8_t b = 0; b < 11; b++){
      led_set_pixel(rr, gg, bb, ww, matrix[t][b]);
    }
  }
  
  if (uhrzeit & ((uint32_t)1 << ESIST))   { es_ist();  }
  if (uhrzeit & ((uint32_t)1 << FUENF))   { fuenf();   }
  if (uhrzeit & ((uint32_t)1 << ZEHN))    { zehn();    }
  if (uhrzeit & ((uint32_t)1 << VIERTEL)) { viertel(); }
  if (uhrzeit & ((uint32_t)1 << ZWANZIG)) { zwanzig(); }
  if (uhrzeit & ((uint32_t)1 << HALB))    { halb();    }
  if (uhrzeit & ((uint32_t)1 << EINS))    { eins();    }
  if (uhrzeit & ((uint32_t)1 << VOR))     { vor();     }
  if (uhrzeit & ((uint32_t)1 << NACH))    { nach();    }
  if (uhrzeit & ((uint32_t)1 << H_EIN ))  { h_ein();   }
  if (uhrzeit & ((uint32_t)1 << H_ZWEI))  { h_zwei();  }
  if (uhrzeit & ((uint32_t)1 << H_DREI )) { h_drei();  }
  if (uhrzeit & ((uint32_t)1 << H_VIER )) { h_vier();  }
  if (uhrzeit & ((uint32_t)1 << H_FUENF)) { h_fuenf(); }
  if (uhrzeit & ((uint32_t)1 << H_SECHS)) { h_sechs(); }
  if (uhrzeit & ((uint32_t)1 << H_SIEBEN)){ h_sieben();}
  if (uhrzeit & ((uint32_t)1 << H_ACHT))  { h_acht();  }
  if (uhrzeit & ((uint32_t)1 << H_NEUN))  { h_neun();  }
  if (uhrzeit & ((uint32_t)1 << H_ZEHN))  { h_zehn();  }
  if (uhrzeit & ((uint32_t)1 << H_ELF))   { h_elf();   }
  if (uhrzeit & ((uint32_t)1 << H_ZWOELF)){ h_zwoelf();}
  if (uhrzeit & ((uint32_t)1 << UHR))     { uhr();     }
 
    
//  if (m > 0) {
  uint8_t animation = 50;
//  }

#ifdef UHR_114_Fraenkisch                                                                        
  show_minuten();
#endif

#ifdef UHR_114                                                                        
  show_minuten();
#endif

#ifdef UHR_125
  show_minuten();
#endif

#ifdef UHR_242
  show_minuten();
  show_wetter();
#endif

  led_show();  

}

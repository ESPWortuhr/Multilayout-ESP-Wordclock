#include <Arduino.h>

// NTP
unsigned int localPort = 2390;      // local port to listen for UDP packets
IPAddress timeServerIP; // time.nist.gov NTP server address
//--const char* ntpServerName = "time.nist.gov";
//const char* ntpServerName = "ptbtime1.ptb.de";
const char* ntpServerName = G.zeitserver;
const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message
byte packetBuffer[ NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets

// A UDP instance to let us send and receive packets over UDP
WiFiUDP udp;

// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress& address)
{
  #ifdef DEBUG     
   Serial.println("sending NTP packet...");
  #endif   
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  udp.beginPacket(address, 123); //NTP requests are to port 123
  udp.write(packetBuffer, NTP_PACKET_SIZE);
  udp.endPacket();
}

//------------------------------------------------------------------------------ 

void set_ntp_zeit()
{
  WiFi.hostByName(ntpServerName, timeServerIP);
  Serial.print("NTP Server Name: "), Serial.println(ntpServerName);
  Serial.print("NTP Server IP  : "), Serial.println(timeServerIP);
  sendNTPpacket(timeServerIP); // send an NTP packet to a time server
  // wait to see if a reply is available
  delay(1000);
  int cb = udp.parsePacket();
  if (!cb) {
    #ifdef DEBUG       
     Serial.println("no packet yet");
    #endif     
    unix_time = 0;
  }
  else {
    #ifdef DEBUG       
     Serial.print("packet received, length=");
     Serial.println(cb);
    #endif     
    // We've received a packet, read the data from it
    udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer

    //the timestamp starts at byte 40 of the received packet and is four bytes,
    // or two words, long. First, esxtract the two words:

    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    // combine the four bytes (two words) into a long integer
    // this is NTP time (seconds since Jan 1 1900):
    unsigned long secsSince1900 = highWord << 16 | lowWord;
//--    Serial.print("Seconds since Jan 1 1900 = " );
//--    Serial.println(secsSince1900);

    // now convert NTP time into everyday time:
//--    Serial.print("Unix time = ");
    // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
    const unsigned long seventyYears = 2208988800UL;
    // subtract seventy years:
    unsigned long epoch = secsSince1900 - seventyYears;
    // print Unix time:
//--    Serial.println(epoch);

    unix_time = epoch;
    // print the hour, minute and second:
    #ifdef DEBUG       
     Serial.print("Aktuelle Zeit ");       // UTC is the time at Greenwich Meridian (GMT)
     Serial.print((epoch  % 86400L) / 3600); // print the hour (86400 equals secs per day)
    #endif     
//--    _stunde = (epoch  % 86400L) / 3600;
    #ifdef DEBUG   
     Serial.print(':');
    #endif     
    if ( ((epoch % 3600) / 60) < 10 ) {
      // In the first 10 minutes of each hour, we'll want a leading '0'
      ;
      #ifdef DEBUG         
       Serial.print('0');
      #endif       
    }
    #ifdef DEBUG       
     Serial.print((epoch  % 3600) / 60); // print the minute (3600 equals secs per minute)
    #endif     
//--    _minute = (epoch  % 3600) / 60;
    #ifdef DEBUG   
     Serial.print(':');
    #endif     
    if ( (epoch % 60) < 10 ) {
      ;
      // In the first 10 seconds of each minute, we'll want a leading '0'
      #ifdef DEBUG         
       Serial.print('0');
      #endif       
    }
    #ifdef DEBUG       
     Serial.println(epoch % 60); // print the second
    #endif     
//--    _sekunde = epoch % 60;
  }
  // wait ten seconds before asking for the time again
}  

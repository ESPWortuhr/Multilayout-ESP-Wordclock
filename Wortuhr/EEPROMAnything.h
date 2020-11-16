#include <EEPROM.h>
#include <Arduino.h>  // for type definitions

template <class T> int EEPROM_writeAnything(int ee, const T& value)
{
    const byte* p = (const byte*)(const void*)&value;
    unsigned int i;
    for (i = 0; i < sizeof(value); i++)
          EEPROM.write(ee++, *p++);
    return i;
}

template <class T> int EEPROM_readAnything(int ee, T& value)
{
    byte* p = (byte*)(void*)&value;
    unsigned int i;
    for (i = 0; i < sizeof(value); i++)
          *p++ = EEPROM.read(ee++);
    return i;
}

//------------------------------------------------------------------------------

void eeprom_write() {
    EEPROM_writeAnything(0, G);
    EEPROM.commit();
}

//------------------------------------------------------------------------------

void eeprom_read() {
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
    Serial.printf("rgb.0.3   : %u\n", G.rgb[Foreground][3]);
    Serial.printf("rgb.1.0   : %u\n", G.rgb[Background][0]);
    Serial.printf("rgb.1.1   : %u\n", G.rgb[Background][1]);
    Serial.printf("rgb.1.2   : %u\n", G.rgb[Background][2]);
    Serial.printf("rgb.1.3   : %u\n", G.rgb[Background][3]);
    Serial.printf("rgb.2.0   : %u\n", G.rgb[Frame][0]);
    Serial.printf("rgb.2.1   : %u\n", G.rgb[Frame][1]);
    Serial.printf("rgb.2.2   : %u\n", G.rgb[Frame][2]);
    Serial.printf("rgb.2.3   : %u\n", G.rgb[Frame][3]);
    Serial.printf("rgb.3.0   : %u\n", G.rgb[Effect][0]);
    Serial.printf("rgb.3.1   : %u\n", G.rgb[Effect][1]);
    Serial.printf("rgb.3.2   : %u\n", G.rgb[Effect][2]);
    Serial.printf("rgb.3.3   : %u\n", G.rgb[Effect][3]);
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
    Serial.printf("Uhrtype    : %u\n", G.UhrtypeDef);
	Serial.printf("Colortype    : %u\n", G.Colortype);
    Serial.print("OWM_apikey: ");
    Serial.println(G.apikey);
    Serial.print("OWM_city  : ");
    Serial.println(G.cityid);

    delay(100);
}
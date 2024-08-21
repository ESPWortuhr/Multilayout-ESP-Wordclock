#include <Arduino.h>
#include <EEPROM.h>

namespace eeprom {

//------------------------------------------------------------------------------

template <class T> int writeAnything(int ee, const T &value) {
    const byte *p = (const byte *)(const void *)&value;
    uint32_t i;
    for (i = 0; i < sizeof(value); i++)
        EEPROM.write(ee++, *p++);
    return i;
}

//------------------------------------------------------------------------------

template <class T> int readAnything(int ee, T &value) {
    byte *p = (byte *)(void *)&value;
    unsigned int i;
    for (i = 0; i < sizeof(value); i++)
        *p++ = EEPROM.read(ee++);
    return i;
}

//------------------------------------------------------------------------------

void write() {
    writeAnything(0, G);
    EEPROM.commit();
}

//------------------------------------------------------------------------------

void read() {
    readAnything(0, G);

    Serial.printf("Version   : %s\n", VERSION);
    Serial.printf("Sernr     : %u\n", G.sernr);
    Serial.printf("Programm  : %u\n", G.prog);
    Serial.printf("Conf      : %u\n", G.conf);
    Serial.printf("FgCol.H   : %f\n", G.color[Foreground].H);
    Serial.printf("FgCol.S   : %f\n", G.color[Foreground].S);
    Serial.printf("FgCol.V   : %f\n", G.color[Foreground].B);
    Serial.printf("BgCol.H   : %f\n", G.color[Background].H);
    Serial.printf("BgCol.S   : %f\n", G.color[Background].S);
    Serial.printf("BgCol.V   : %f\n", G.color[Background].B);
    Serial.printf("FrCol.H   : %f\n", G.color[Frame].H);
    Serial.printf("FrCol.S   : %f\n", G.color[Frame].S);
    Serial.printf("FrCol.V   : %f\n", G.color[Frame].B);
    Serial.printf("Zeitserver: %s\n", G.timeserver);
    Serial.printf("Lauftext  : %s\n", G.scrollingText);
    Serial.printf("H6        : %u\n", G.h6);
    Serial.printf("H8        : %u\n", G.h8);
    Serial.printf("H12       : %u\n", G.h12);
    Serial.printf("H16       : %u\n", G.h16);
    Serial.printf("H18       : %u\n", G.h18);
    Serial.printf("H22       : %u\n", G.h22);
    Serial.printf("H24       : %u\n", G.h24);
    Serial.printf("Uhrtype    : %u\n", G.UhrtypeDef);

    Serial.printf("MQTT_State    : %u\n", G.mqtt.state);
    Serial.printf("MQTT_Server    : %s\n", G.mqtt.serverAdress);
    Serial.printf("MQTT_User    : %s\n", G.mqtt.user);
    Serial.printf("MQTT_Pass    : %s\n", G.mqtt.password);
    Serial.printf("MQTT_ClientId    : %s\n", G.mqtt.clientId);
    Serial.printf("MQTT_Topic    : %s\n", G.mqtt.topic);
    Serial.printf("MQTT_Port    : %u\n", G.mqtt.port);

    Serial.printf("autoBrightEnabled    : %u\n", G.autoBrightEnabled);
    Serial.printf("autoBrightOffset    : %u\n", G.autoBrightOffset);
    Serial.printf("autoBrightSlope    : %u\n", G.autoBrightSlope);
    Serial.printf("Uhrtype    : %u\n", G.transitionDuration);
    Serial.printf("transitionType    : %u\n", G.transitionType);
    Serial.printf("transitionDuration    : %u\n", G.transitionSpeed);
    Serial.printf("transitionColorize    : %u\n", G.transitionColorize);
    Serial.printf("transitionDemo    : %u\n", G.transitionDemo);

    Serial.printf("bootLedBlink    : %u\n", G.bootLedBlink);
    Serial.printf("bootLedSweep    : %u\n", G.bootLedSweep);
    Serial.printf("bootShowWifi    : %u\n", G.bootShowWifi);
    Serial.printf("bootShowIP    : %u\n", G.bootShowIP);
    Serial.printf("Colortype    : %u\n", G.Colortype);
    Serial.print("OWM_apikey: ");
    Serial.println(G.openWeatherMap.apikey);
    Serial.print("OWM_city  : ");
    Serial.println(G.openWeatherMap.cityid);

    for (uint8_t i = 0; i < MAX_BIRTHDAY_COUNT; i++) {
        Serial.printf("Birthday%1u: %02u.%02u.%04u\n", i, G.birthday[i].day,
                      G.birthday[i].month, G.birthday[i].year);
    }

    delay(100);
}
} // namespace eeprom

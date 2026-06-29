#include "SensitiveData.h"
#include <Arduino.h>
#include <EEPROM.h>
#include <string.h>

namespace eeprom {

/*
 * Legacy raw-config storage.
 *
 * The public read()/write() API intentionally remains unchanged for now: the
 * complete global config struct G is persisted byte-for-byte. Future storage
 * work should migrate this to a versioned config format or a packed config
 * struct with explicit migration logic.
 */

namespace detail {

template <size_t destSize, size_t sourceSize>
void copyBoundedString(char (&dest)[destSize],
                       const char (&source)[sourceSize]) {
    static_assert(destSize > 0, "destination buffer must not be empty");
    size_t copyLen = min(destSize - 1, sourceSize);
    memcpy(dest, source, copyLen);
    dest[copyLen] = '\0';
}

template <size_t sourceSize>
void printSafeString(const char *label, const char (&source)[sourceSize]) {
    char buffer[sourceSize + 1] = {0};
    copyBoundedString(buffer, source);
    Serial.printf("%s%s\n", label, buffer);
}

void printMaskedPassword() {
    char passMasked[PAYLOAD_LENGTH + 1] = {0};
    copyBoundedString(passMasked, G.mqtt.password);

    size_t passLen = strlen(passMasked);
    if (passLen > 3) {
        memset(passMasked, '*', passLen - 3);
    }
    Serial.printf("MQTT_Pass (masked): %s\n", passMasked);
}

void printMaskedOpenWeatherMapApiKey() {
    char apiKeyMasked[sizeof(G.openWeatherMap.apikey) + 1] = {0};
    sensitive::maskPreservingSuffix(apiKeyMasked, G.openWeatherMap.apikey);
    Serial.printf("OWM_apikey (masked): %s\n", apiKeyMasked);
}

void printConfig() {
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
    printSafeString("Zeitserver: ", G.timeserver);
    printSafeString("Zeitzone  : ", G.timezone);
    printSafeString("Lauftext  : ", G.scrollingText);
    Serial.printf("H6        : %u\n", G.h6);
    Serial.printf("H8        : %u\n", G.h8);
    Serial.printf("H12       : %u\n", G.h12);
    Serial.printf("H16       : %u\n", G.h16);
    Serial.printf("H18       : %u\n", G.h18);
    Serial.printf("H22       : %u\n", G.h22);
    Serial.printf("H24       : %u\n", G.h24);
    Serial.printf("ClockType    : %u\n", G.clockTypeDef);

    Serial.printf("MQTT_State    : %u\n", G.mqtt.state);
    printSafeString("MQTT_Server    : ", G.mqtt.serverAdress);
    printSafeString("MQTT_User    : ", G.mqtt.user);
    printMaskedPassword();
    printSafeString("MQTT_ClientId    : ", G.mqtt.clientId);
    printSafeString("MQTT_Topic    : ", G.mqtt.topic);
    Serial.printf("MQTT_Port    : %u\n", G.mqtt.port);

    Serial.printf("autoBrightEnabled    : %u\n", G.autoBrightEnabled);
    Serial.printf("autoBrightMin    : %u\n", G.autoBrightMin);
    Serial.printf("autoBrightMax    : %u\n", G.autoBrightMax);
    Serial.printf("autoBrightPeak    : %u\n", G.autoBrightPeak);
    Serial.printf("transitionDuration    : %u\n", G.transitionDuration);
    Serial.printf("transitionType    : %u\n", G.transitionType);
    Serial.printf("transitionSpeed    : %u\n", G.transitionSpeed);
    Serial.printf("transitionColorize    : %u\n", G.transitionColorize);
    Serial.printf("transitionDemo    : %u\n", G.transitionDemo);

    Serial.printf("bootLedBlink    : %u\n", G.bootLedBlink);
    Serial.printf("bootLedSweep    : %u\n", G.bootLedSweep);
    Serial.printf("bootShowWifi    : %u\n", G.bootShowWifi);
    Serial.printf("bootShowIP    : %u\n", G.bootShowIP);
    Serial.printf("ledPin    : %u\n", G.hardwarePins.led);
    Serial.printf("powerButtonPin    : %u\n", G.hardwarePins.powerButton);
    Serial.printf("modeButtonPin    : %u\n", G.hardwarePins.modeButton);
    Serial.printf("speedButtonPin    : %u\n", G.hardwarePins.speedButton);
    Serial.printf("i2cSdaPin    : %u\n", G.i2cSdaPin);
    Serial.printf("i2cSclPin    : %u\n", G.i2cSclPin);
    Serial.printf("Colortype    : %u\n", G.Colortype);
    printMaskedOpenWeatherMapApiKey();
    printSafeString("OWM_city  : ", G.openWeatherMap.cityid);

    for (uint8_t i = 0; i < MAX_BIRTHDAY_COUNT; i++) {
        Serial.printf("Birthday%1u: %02u.%02u\n", i, G.birthday[i].day,
                      G.birthday[i].month);
    }
}

} // namespace detail

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

#if GENERAL_VERBOSE
    detail::printConfig();
#endif

    delay(100);
}
} // namespace eeprom

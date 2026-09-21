#include "SensitiveData.h"
#include <Arduino.h>
#include <EEPROM.h>
#include <LittleFS.h>
#include <string.h>

namespace eeprom {

namespace detail {

constexpr char CONFIG_FILE[] = "/wordclock-config.bin";
constexpr char CONFIG_TEMP_FILE[] = "/wordclock-config.tmp";
constexpr char CONFIG_BACKUP_FILE[] = "/wordclock-config.bak";
constexpr uint32_t CONFIG_MAGIC = 0x57434C4B; // WCLK
constexpr uint16_t CONFIG_FORMAT_VERSION = 1;

struct ConfigHeader {
    uint32_t magic;
    uint16_t formatVersion;
    uint16_t schemaVersion;
    uint16_t payloadSize;
    uint16_t reserved;
    uint32_t crc32;
};

uint32_t calculateCrc32(const uint8_t *data, size_t size) {
    uint32_t crc = 0xFFFFFFFF;
    for (size_t i = 0; i < size; ++i) {
        crc ^= data[i];
        for (uint8_t bit = 0; bit < 8; ++bit)
            crc = (crc >> 1) ^ (0xEDB88320 & (0U - (crc & 1U)));
    }
    return ~crc;
}

bool loadConfigFile(const char *path, GLOBAL &config) {
    File file = LittleFS.open(path, "r");
    if (!file)
        return false;

    ConfigHeader header = {};
    if (file.read(reinterpret_cast<uint8_t *>(&header), sizeof(header)) !=
            sizeof(header) ||
        header.magic != CONFIG_MAGIC ||
        header.formatVersion != CONFIG_FORMAT_VERSION ||
        header.schemaVersion != SERNR || header.payloadSize != sizeof(config) ||
        file.read(reinterpret_cast<uint8_t *>(&config), sizeof(config)) !=
            sizeof(config) ||
        calculateCrc32(reinterpret_cast<const uint8_t *>(&config),
                       sizeof(config)) != header.crc32) {
        file.close();
        return false;
    }

    file.close();
    return true;
}

bool storeConfigFile(const GLOBAL &config) {
    ConfigHeader header = {
        CONFIG_MAGIC,
        CONFIG_FORMAT_VERSION,
        SERNR,
        static_cast<uint16_t>(sizeof(config)),
        0,
        calculateCrc32(reinterpret_cast<const uint8_t *>(&config),
                       sizeof(config)),
    };

    File file = LittleFS.open(CONFIG_TEMP_FILE, "w");
    if (!file)
        return false;

    const bool written =
        file.write(reinterpret_cast<const uint8_t *>(&header), sizeof(header)) ==
            sizeof(header) &&
        file.write(reinterpret_cast<const uint8_t *>(&config), sizeof(config)) ==
            sizeof(config);
    file.flush();
    file.close();

    GLOBAL verification = {};
    if (!written || !loadConfigFile(CONFIG_TEMP_FILE, verification)) {
        LittleFS.remove(CONFIG_TEMP_FILE);
        return false;
    }

    LittleFS.remove(CONFIG_BACKUP_FILE);
    if (LittleFS.exists(CONFIG_FILE) &&
        !LittleFS.rename(CONFIG_FILE, CONFIG_BACKUP_FILE)) {
        LittleFS.remove(CONFIG_TEMP_FILE);
        return false;
    }
    if (!LittleFS.rename(CONFIG_TEMP_FILE, CONFIG_FILE)) {
        if (LittleFS.exists(CONFIG_BACKUP_FILE))
            LittleFS.rename(CONFIG_BACKUP_FILE, CONFIG_FILE);
        LittleFS.remove(CONFIG_TEMP_FILE);
        return false;
    }
    return true;
}

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
    char passMasked[sizeof(G.mqtt.password) + 1] = {0};
    sensitive::maskPreservingSuffix(passMasked, G.mqtt.password);
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
    if (LittleFS.begin() && detail::storeConfigFile(G)) {
        Serial.println("Configuration saved");
        return;
    }

    Serial.println("Configuration file unavailable, using legacy EEPROM");
    writeAnything(0, G);
    EEPROM.commit();
}

//------------------------------------------------------------------------------

void read() {
    if (LittleFS.begin()) {
        if (detail::loadConfigFile(detail::CONFIG_FILE, G)) {
            Serial.println("Configuration loaded");
        } else if (detail::loadConfigFile(detail::CONFIG_BACKUP_FILE, G)) {
            Serial.println("Configuration backup loaded");
            LittleFS.remove(detail::CONFIG_FILE);
            LittleFS.rename(detail::CONFIG_BACKUP_FILE, detail::CONFIG_FILE);
        } else if (LittleFS.exists(detail::CONFIG_FILE) ||
                   LittleFS.exists(detail::CONFIG_BACKUP_FILE)) {
            Serial.println("Configuration files invalid, restoring defaults");
            G = GLOBAL{};
        } else {
            readAnything(0, G);
            if (G.sernr == SERNR && detail::storeConfigFile(G))
                Serial.println("Legacy EEPROM configuration migrated");
        }
    } else {
        Serial.println("LittleFS unavailable, loading legacy EEPROM");
        readAnything(0, G);
    }

#if GENERAL_VERBOSE
    detail::printConfig();
#endif

    delay(100);
}
} // namespace eeprom

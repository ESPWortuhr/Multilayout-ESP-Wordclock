#pragma once

#include "I2CBus.h"
#include "Config.h"
#include <Wire.h>

namespace i2cBus {

namespace {

#ifdef ESP8266
constexpr uint8_t MAX_I2C_PIN = 16;
#else
constexpr uint8_t MAX_I2C_PIN = 39;
#endif

bool pinIsInRange(uint8_t pin) { return pin <= MAX_I2C_PIN; }

} // namespace

Pins defaultPins() {
#if defined(ESP8266)
    return {SDA_PIN_ESP8266, SCL_PIN_ESP8266};
#elif defined(ESP32)
#if defined(CONFIG_IDF_TARGET_ESP32C3)
    return {SDA_PIN_ESP32C3, SCL_PIN_ESP32C3};
#else
    return {SDA_PIN_ESP32, SCL_PIN_ESP32};
#endif
#else
    return {HARDWARE_PIN_DISABLED, HARDWARE_PIN_DISABLED};
#endif
}

void setDefaultPins(uint8_t &sdaPin, uint8_t &sclPin) {
    Pins pins = defaultPins();
    sdaPin = pins.sda;
    sclPin = pins.scl;
}

bool pinsAreEnabled(uint8_t sdaPin, uint8_t sclPin) {
    return sdaPin != HARDWARE_PIN_DISABLED && sclPin != HARDWARE_PIN_DISABLED;
}

bool pinsAreValid(uint8_t sdaPin, uint8_t sclPin) {
    const bool sdaDisabled = sdaPin == HARDWARE_PIN_DISABLED;
    const bool sclDisabled = sclPin == HARDWARE_PIN_DISABLED;

    if (sdaDisabled || sclDisabled) {
        return sdaDisabled && sclDisabled;
    }

    return pinIsInRange(sdaPin) && pinIsInRange(sclPin) && sdaPin != sclPin;
}

void begin(uint8_t sdaPin, uint8_t sclPin) {
    Wire.end();

    if (!pinsAreEnabled(sdaPin, sclPin)) {
        Serial.println("I2C disabled");
        return;
    }

    Wire.begin(sdaPin, sclPin);
    Serial.printf("I2C initialized on SDA GPIO%u, SCL GPIO%u\n", sdaPin,
                  sclPin);
}

ScanResult scan(uint8_t sdaPin, uint8_t sclPin, uint8_t restoreSdaPin,
                uint8_t restoreSclPin) {
    ScanResult result;

    if (!pinsAreValid(sdaPin, sclPin)) {
        result.status = ScanStatus::Invalid;
        return result;
    }

    if (!pinsAreEnabled(sdaPin, sclPin)) {
        result.status = ScanStatus::Disabled;
        return result;
    }

    result.status = ScanStatus::Ok;
    begin(sdaPin, sclPin);
    delay(10);

    for (uint8_t address = 1; address < 127; address++) {
        Wire.beginTransmission(address);
        if (Wire.endTransmission() == 0) {
            result.addresses[result.addressCount++] = address;
            if (result.addressCount >= MAX_SCAN_ADDRESS_COUNT) {
                break;
            }
        }
    }

    begin(restoreSdaPin, restoreSclPin);
    return result;
}

byte findBH1750Address(uint8_t sdaPin, uint8_t sclPin) {
    if (!pinsAreEnabled(sdaPin, sclPin)) {
        return 0;
    }

    Wire.beginTransmission(0x23);
    if (Wire.endTransmission() == 0) {
        return 0x23;
    }
    Wire.beginTransmission(0x5C);
    if (Wire.endTransmission() == 0) {
        return 0x5C;
    }
    return 0;
}

} // namespace i2cBus

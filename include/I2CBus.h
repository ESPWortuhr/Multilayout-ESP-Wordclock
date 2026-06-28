#pragma once

#include "Arduino.h"

namespace i2cBus {

constexpr uint8_t MAX_SCAN_ADDRESS_COUNT = 3;

struct Pins {
    uint8_t sda;
    uint8_t scl;
};

enum class ScanStatus : uint8_t {
    Ok,
    Disabled,
    Invalid,
};

struct ScanResult {
    ScanStatus status = ScanStatus::Invalid;
    uint8_t addresses[MAX_SCAN_ADDRESS_COUNT] = {0};
    uint8_t addressCount = 0;
};

Pins defaultPins();
void setDefaultPins(uint8_t &sdaPin, uint8_t &sclPin);
bool pinsAreEnabled(uint8_t sdaPin, uint8_t sclPin);
bool pinsAreValid(uint8_t sdaPin, uint8_t sclPin);
void begin(uint8_t sdaPin, uint8_t sclPin);
ScanResult scan(uint8_t sdaPin, uint8_t sclPin, uint8_t restoreSdaPin,
                uint8_t restoreSclPin);
byte findBH1750Address(uint8_t sdaPin, uint8_t sclPin);

} // namespace i2cBus

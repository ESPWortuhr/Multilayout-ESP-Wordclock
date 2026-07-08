#include "WordClockState.h"

#include "Config.h"

// Definitions of the global word clock state. Declared as `extern` in
// WordClockState.h; defined here exactly once so every translation unit that
// includes the header links against the same objects.

GLOBAL G = {};

// Brightness related variables
float ledGain = DEFAULT_BRIGHTNESS;
bool bh1750Initialized = false;
bool autoBrightUsingBH1750 = false;

uint8_t _second = 0;
uint8_t _secondFrame = 0;
uint8_t _minute = 0;
uint8_t _hour = 0;
uint8_t lastSecond = 0;
uint8_t lastMinute = 0;

uint32_t frontMatrix[MAX_ROW_SIZE] = {0};
uint32_t lastFrontMatrix[MAX_ROW_SIZE] = {0};
uint8_t minuteArray = 0; /* Using a byte as a per bit array */
uint8_t lastMinuteArray = 0;
uint16_t minutePixelArray[4] = {0};
bool frameArray[200] = {false};
bool parametersChanged = false;
bool layoutChanged = false;
bool colorChangedByWebsite = false;
uint8_t statusAccessPoint = 0;
uint8_t i2cScanSdaPin = HARDWARE_PIN_DISABLED;
uint8_t i2cScanSclPin = HARDWARE_PIN_DISABLED;

bool externalRTC = false;

#pragma once

// Note: named "WifiHelper" (not "Wifi") on purpose: a header called Wifi.h
// would collide with the framework's system header <WiFi.h> on case-insensitive
// file systems, shadowing the real WiFi library.

#ifdef ESP8266
#include <ESP8266WiFi.h>
#elif defined(ESP32)
#include <WiFi.h>
#endif

void wifiStart();

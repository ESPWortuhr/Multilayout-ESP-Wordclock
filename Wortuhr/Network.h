#pragma once

#include <Arduino.h>

void Network_info();
void Network_disable();
void Network_reboot();
void Network_resetSettings();
String Network_getSSID();
void Network_changeHostname(const char * hostname);

void Network_setup(const char * hostname);
void Network_loop();

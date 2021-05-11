#pragma once

#include <Arduino.h>
#include <IPAddress.h>

void Network_info();
int Network_getQuality();
void Network_disable();
void Network_rtcMode();
void Network_setStaticIp(const IPAddress &ip, const IPAddress &gw,
                         const IPAddress &sn);
void Network_reboot();
void Network_resetSettings();
String Network_getSSID();
void Network_changeHostname(const char *hostname);

void Network_setup(const char *hostname);
void Network_loop();

#pragma once

#include <WiFiManager.h>

void enableAPAfterDelay() {
    static bool apEnabled = false;
    static unsigned long startTime = 0;
    const unsigned long delayDuration = 5 * 60 * 1000; // 5 Minuten in Millisekunden

    if (!apEnabled) {
        if (startTime == 0) {
            startTime = millis(); // Speichere den Startzeitpunkt, wenn noch nicht geschehen
        }

        if (millis() - startTime >= delayDuration) {
            // Wenn die Zeit abgelaufen ist, schalte den Access Point ein
            WiFi.enableAP(true);
            Serial.println("Enabled Access Point (AP) mode due to no WiFi connection within 5 minutes.");
            apEnabled = true; 
        }
    }
}

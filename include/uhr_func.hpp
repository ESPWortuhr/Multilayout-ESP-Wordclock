#include "leds.h"

#include <Arduino.h>

#include "Uhr.h"

//------------------------------------------------------------------------------
// Telnet Server für Konsolen Ausgaben
//------------------------------------------------------------------------------

static void Telnet() {
    // Cleanup disconnected session
    for (uint8_t i = 0; i < MAX_TELNET_CLIENTS; i++) {
        if (TelnetClient[i] && !TelnetClient[i].connected()) {
            Serial.print("Client disconnected ... terminate session ");
            Serial.println(i + 1);
            TelnetClient[i].stop();
        }
    }

    // Check new client connections
    if (TelnetServer.hasClient()) {
        ConnectionEstablished = false; // Set to false

        for (uint8_t i = 0; i < MAX_TELNET_CLIENTS; i++) {
            // Serial.print("Checking telnet session "); Serial.println(i+1);

            // find free socket
            if (!TelnetClient[i]) {
                TelnetClient[i] = TelnetServer.available();

                Serial.print("New Telnet client connected to session ");
                Serial.println(i + 1);

                TelnetClient[i].flush(); // clear input buffer, else you get
                                         // strange characters
                TelnetClient[i].println("Welcome!");

                TelnetClient[i].print("Millis since start: ");
                TelnetClient[i].println(millis());

                TelnetClient[i].print("Free Heap RAM: ");
                TelnetClient[i].println(ESP.getFreeHeap());

                TelnetClient[i].println("--------------------------------------"
                                        "--------------------------");

                ConnectionEstablished = true;

                break;
            } else {
                // Serial.println("Session is in use");
            }
        }

        if (ConnectionEstablished == false) {
            Serial.println("No free sessions ... drop connection");
            TelnetServer.available().stop();
            // TelnetMsg("An other user cannot connect ... MAX_TELNET_CLIENTS
            // limit is reached!");
        }
    }

    for (uint8_t i = 0; i < MAX_TELNET_CLIENTS; i++) {
        if (TelnetClient[i] && TelnetClient[i].connected()) {
            if (TelnetClient[i].available()) {
                // get data from the telnet client
                while (TelnetClient[i].available()) {
                    Serial.write(TelnetClient[i].read());
                }
            }
        }
    }
}

static void TelnetMsg(const String &text) {
    for (uint8_t i = 0; i < MAX_TELNET_CLIENTS; i++) {
        if (TelnetClient[i] || TelnetClient[i].connected()) {
            TelnetClient[i].println(text);
        }
    }
    delay(10); // to astatic void strange characters left in buffer
}

//------------------------------------------------------------------------------

void led_set_pixel(uint8_t rr, uint8_t gg, uint8_t bb, uint8_t ww, uint16_t i) {

    switch (G.Colortype) {
    case Brg: {
        strip_RGB->SetPixelColor(i, RgbColor(bb, rr, gg));
        break;
    }
    case Grb: {
        strip_RGB->SetPixelColor(i, RgbColor(gg, rr, bb));
        break;
    }
    case Rgb: {
        strip_RGB->SetPixelColor(i, RgbColor(rr, gg, bb));
        break;
    }
    case Rbg: {
        strip_RGB->SetPixelColor(i, RgbColor(rr, bb, gg));
        break;
    }
    case Grbw: {
        strip_RGBW->SetPixelColor(i, RgbwColor(rr, gg, bb, ww));
        break;
    }
    default:
        break;
    }
}

//------------------------------------------------------------------------------

void led_set_pixel_Color_Object(uint16_t i, RgbColor color) {

    switch (G.Colortype) {
    case Brg: {
        strip_RGB->SetPixelColor(i, color);
        break;
    }
    case Grb: {
        strip_RGB->SetPixelColor(i, color);
        break;
    }
    case Rgb: {
        strip_RGB->SetPixelColor(i, color);
        break;
    }
    case Rbg: {
        strip_RGB->SetPixelColor(i, color);
        break;
    }
    case Grbw: {
        strip_RGBW->SetPixelColor(i, RgbwColor(color));
        break;
    }
    default:
        break;
    }
}

//------------------------------------------------------------------------------

void led_set_pixel_Color_Object_rgbw(uint16_t i, RgbwColor color) {
    strip_RGBW->SetPixelColor(i, RgbwColor(color));
}

//------------------------------------------------------------------------------

RgbColor led_get_pixel(uint16_t i) {

    switch (G.Colortype) {
    case Brg: {
        return strip_RGB->GetPixelColor(i);
        break;
    }
    case Grb: {
        return strip_RGB->GetPixelColor(i);
        break;
    }
    case Rgb: {
        return strip_RGB->GetPixelColor(i);
        break;
    }
    case Rbg: {
        return strip_RGB->GetPixelColor(i);
        break;
    }
    default:
        break;
    }
}

//------------------------------------------------------------------------------

RgbwColor led_get_pixel_rgbw(uint16_t i) {
    return strip_RGBW->GetPixelColor(i);
}

//------------------------------------------------------------------------------
// Helligkeitsregelung nach Uhrzeiten oder per LDR
//------------------------------------------------------------------------------
static void set_helligkeit_ldr(uint8_t &rr, uint8_t &gg, uint8_t &bb,
                               uint8_t &ww, uint8_t position) {
    if (G.ldr == 1) {
        rr = G.rgb[position][0] * ldrVal / 100;
        gg = G.rgb[position][1] * ldrVal / 100;
        bb = G.rgb[position][2] * ldrVal / 100;
        ww = G.rgb[position][3] * ldrVal / 100;
    } else {
        rr = G.rgb[position][0] * G.hh / 100;
        gg = G.rgb[position][1] * G.hh / 100;
        bb = G.rgb[position][2] * G.hh / 100;
        ww = G.rgb[position][3] * G.hh / 100;
    }
}

//------------------------------------------------------------------------------

static void set_helligkeit(uint8_t &rr, uint8_t &gg, uint8_t &bb, uint8_t &ww,
                           uint8_t position) {
    rr = G.rgb[position][0];
    gg = G.rgb[position][1];
    bb = G.rgb[position][2];
    ww = G.rgb[position][3];
    uint16_t zz = rr + gg + bb;
    if (zz > 150) {
        zz = zz * 10 / 150;
        rr = rr * 10 / zz;
        gg = gg * 10 / zz;
        bb = bb * 10 / zz;
        ww = ww * 10 / zz;
    }
}

//------------------------------------------------------------------------------

static void led_set(const uint8_t array[]) {
    uint8_t rr, gg, bb, ww;
    set_helligkeit_ldr(rr, gg, bb, ww, Foreground);
    uint8_t i = 0;
    while (i < usedUhrType->NUM_PIXELS()) {

        if (array[i] != 255) {
            led_set_pixel(rr, gg, bb, ww, array[i]);
        }
        i++;
    }
}

//------------------------------------------------------------------------------

void led_show() {
    if (G.Colortype == Grbw) {
        strip_RGBW->Show();
    } else {
        strip_RGB->Show();
    }
}

//------------------------------------------------------------------------------

static inline void led_clear_pixel(uint16_t i) {
    if (G.Colortype == Grbw) {
        strip_RGBW->SetPixelColor(i, 0);
    } else {
        strip_RGB->SetPixelColor(i, 0);
    }
}

//------------------------------------------------------------------------------

void led_clear() {
    for (uint16_t i = 0; i < usedUhrType->NUM_PIXELS(); i++) {
        Word_array[i] = 255;
        Word_array_transition_old2zero[i] = 255;
        Word_array_transition_zero2new[i] = 255;
    }
    for (uint16_t i = 0; i < usedUhrType->NUM_PIXELS(); i++) {
        led_clear_pixel(i);
    }
}

//------------------------------------------------------------------------------

static inline void uhr_clear() {
    for (uint16_t i = 0; i < usedUhrType->NUM_SMATRIX(); i++) {
        led_clear_pixel(usedUhrType->getSMatrix(i));
    }
}

//------------------------------------------------------------------------------

static inline void rahmen_clear() {
    for (uint16_t i = 0; i < usedUhrType->NUM_RMATRIX(); i++) {
        led_clear_pixel(usedUhrType->getRMatrix(i));
    }
}

//------------------------------------------------------------------------------

static void transition_helligkeit(uint8_t &rr, uint8_t &gg, uint8_t &bb,
                                  uint8_t &ww, uint8 percentage) {
    rr = G.rgb[Foreground][0] * percentage / 100;
    gg = G.rgb[Foreground][1] * percentage / 100;
    bb = G.rgb[Foreground][2] * percentage / 100;
    ww = G.rgb[Foreground][3] * percentage / 100;
}

//------------------------------------------------------------------------------

static void led_set() {
    uint8_t rr, gg, bb, ww;
    bool use_new_array = false;
    set_helligkeit(rr, gg, bb, ww, Foreground);
    for (uint8_t i = 0; i < usedUhrType->NUM_PIXELS(); i++) {
        led_set_pixel(rr, gg, bb, ww, Word_array_old[i]);
    }
    for (uint8_t ii = 0; ii < 20; ii++) {
        transition_helligkeit(rr, gg, bb, ww, transition_array[ii]);
        for (uint8_t i = 0; i < usedUhrType->NUM_PIXELS(); i++) {
            if (Word_array_transition_old2zero[i] != 255 &&
                use_new_array == false) {
                led_set_pixel(rr, gg, bb, ww,
                              Word_array_transition_old2zero[i]);
            } else if (Word_array_transition_zero2new[i] != 255 &&
                       use_new_array == true) {
                led_set_pixel(rr, gg, bb, ww,
                              Word_array_transition_zero2new[i]);
            }
        }
        if (transition_array[ii] == 0) {
            use_new_array = true;
        }
    }
    led_show();
    delay(tansition_time);
}

//------------------------------------------------------------------------------

static void copy_array(const uint8_t source[], uint8_t destination[]) {
    for (uint8_t i = 0; i < usedUhrType->NUM_PIXELS(); i++) {
        destination[i] = source[i];
    }
}

//------------------------------------------------------------------------------

static bool changes_in_array() {
    bool return_value = false;
    for (uint16_t i = 0; i < usedUhrType->NUM_PIXELS(); i++) {
        if (Word_array[i] != Word_array_old[i]) {
            if (Word_array_old[i] == 255) {
                Word_array_transition_zero2new[i] = i;
            } else if (Word_array[i] == 255) {
                Word_array_transition_old2zero[i] = i;
            }
            return_value = true;
        }
    }
    return return_value;
}

//------------------------------------------------------------------------------
// HSV to RGB 8Bit
// Farbkreis hue = 0 bis 360 (Farbwert)
//          bri = 0 bis 255 (Dunkelstufe)
//          sat = 0 bis 255 (Farbsättigung)
//------------------------------------------------------------------------------

void hsv_to_rgb(double hue, float sat, float bri, uint8_t *c) {
    hue = 3.14159F * hue / 180.0F;           // convert to radians.
    sat /= 255.0F;                           // from percentage to ratio
    bri /= 255.0F;                           // from percentage to ratio
    sat = sat > 0 ? (sat < 1 ? sat : 1) : 0; // clamp s and i to interval [0,1]
    bri = bri > 0 ? (bri < 1 ? bri : 1) : 0; // clamp s and i to interval [0,1]
    bri = bri *
          std::sqrt(bri); // shape intensity to have finer granularity near 0

    if (G.Colortype == Grbw) {
        if (hue < 2.09439) {
            c[0] = sat * 255.0 * bri / 3.0 *
                   (1 + std::cos(hue) / std::cos(1.047196667 - hue));
            c[1] = sat * 255.0 * bri / 3.0 *
                   (1 + (1 - std::cos(hue) / std::cos(1.047196667 - hue)));
            c[2] = 0;
            c[3] = 255.0 * (1.0 - sat) * bri;
        } else if (hue < 4.188787) {
            hue = hue - 2.09439;
            c[1] = sat * 255.0 * bri / 3.0 *
                   (1 + std::cos(hue) / std::cos(1.047196667 - hue));
            c[2] = sat * 255.0 * bri / 3.0 *
                   (1 + (1 - std::cos(hue) / std::cos(1.047196667 - hue)));
            c[0] = 0;
            c[3] = 255.0 * (1.0 - sat) * bri;
        } else {
            hue = hue - 4.188787;
            c[2] = sat * 255.0 * bri / 3.0 *
                   (1 + std::cos(hue) / std::cos(1.047196667 - hue));
            c[0] = sat * 255.0 * bri / 3.0 *
                   (1 + (1 - std::cos(hue) / std::cos(1.047196667 - hue)));
            c[1] = 0;
            c[3] = 255.0 * (1 - sat) * bri;
        }
    } else {
        while (hue < 0) {
            hue += 360.0F;
        } // cycle h around to 0-360 degrees
        while (hue >= 360) {
            hue -= 360.0F;
        }

        if (hue < 2.09439) {
            c[0] = 255 * bri / 3 *
                   (1 + sat * std::cos(hue) / std::cos(1.047196667 - hue));
            c[1] =
                255 * bri / 3 *
                (1 + sat * (1 - std::cos(hue) / std::cos(1.047196667 - hue)));
            c[2] = 255 * bri / 3 * (1 - sat);
        } else if (hue < 4.188787) {
            hue = hue - 2.09439;
            c[1] = 255 * bri / 3 *
                   (1 + sat * std::cos(hue) / std::cos(1.047196667 - hue));
            c[2] =
                255 * bri / 3 *
                (1 + sat * (1 - std::cos(hue) / std::cos(1.047196667 - hue)));
            c[0] = 255 * bri / 3 * (1 - sat);
        } else {
            hue = hue - 4.188787;
            c[2] =
                255 * bri / 3 * (1 + sat * cos(hue) / cos(1.047196667 - hue));
            c[0] = 255 * bri / 3 *
                   (1 + sat * (1 - cos(hue) / cos(1.047196667 - hue)));
            c[1] = 255 * bri / 3 * (1 - sat);
        }
        c[3] = 0;
    }
}

//------------------------------------------------------------------------------

static void led_single(uint8_t wait) {

    float h;
    uint8_t c[4];

    for (uint16_t i = 0; i < usedUhrType->NUM_PIXELS(); i++) {

        h = 360.0 * i / (usedUhrType->NUM_PIXELS() - 1);
        h = h + 360.0 / usedUhrType->NUM_PIXELS();
        if (h > 360) {
            h = 0;
        }

        led_clear();
        hsv_to_rgb(h, 255, 255, c);
        led_set_pixel(c[0], c[1], c[2], c[3], i);
        led_show();
        delay(wait);
    }
}

//------------------------------------------------------------------------------

static void led_set_all(uint8_t rr, uint8_t gg, uint8_t bb, uint8_t ww) {
    for (int i = 0; i < usedUhrType->NUM_PIXELS(); i++) {
        led_set_pixel(rr, gg, bb, ww, i);
    }
}

static void set_farbe() {
    uint8_t rr, gg, bb, ww;
    set_helligkeit(rr, gg, bb, ww, Effect);
    led_set_all(rr, gg, bb, ww);
}

//------------------------------------------------------------------------------
// Routine Helligkeitsregelung
//------------------------------------------------------------------------------

static void doLDRLogic() {
    int16_t lux = analogRead(A0); // Range 0-1023
    lux = lux - (G.ldrCal * 20);
    if (lux >= 900) {
        lux = 900;
    } // Maximale Helligkeit
    if (lux <= 1) {
        lux = 1;
    } // Minimale Helligkeit
    ldrVal = map(lux, 1, 900, 1, 100);
}

//------------------------------------------------------------------------------

static void set_farbe_rahmen() {
    uint8_t rr, gg, bb, ww;
    set_helligkeit(rr, gg, bb, ww, Frame);

    for (uint16_t i = 0; i < usedUhrType->NUM_RMATRIX(); i++) {
        led_set_pixel(rr, gg, bb, ww, usedUhrType->getRMatrix(i));
    }
}

//------------------------------------------------------------------------------

static void rainbow() {

    static float h = 0.0;
    uint8_t c[4];
    hsv_to_rgb(h, 255, G.hell * 10, c);

    for (uint16_t i = 0; i < usedUhrType->NUM_PIXELS(); i++) {
        led_set_pixel(c[0], c[1], c[2], c[3], i);
    }
    led_show();
    h++;
    if (h > 359) {
        h = 0.0;
    }
}

//-----------------------------------------------------------------------------

static void rainbowCycle() {
    static float h = 0;
    float hh;
    uint8_t c[4];

    hh = h;
    for (uint16_t i = 0; i < usedUhrType->NUM_SMATRIX(); i++) {
        hsv_to_rgb(hh, 255, G.hell * 10, c);
        led_set_pixel(c[0], c[1], c[2], c[3], usedUhrType->getSMatrix(i));
        hh = hh + 360.0 / usedUhrType->NUM_SMATRIX();
        if (hh > 360) {
            hh = 0;
        }
    }
    led_show();
    h++;
    if (h > 360) {
        h = 0.0;
    }
}

//------------------------------------------------------------------------------

void shift_all_pixels_to_right() {
    for (uint8_t b = 0; b < 10; b++) {
        for (uint8_t a = 0; a < usedUhrType->ROWS_MATRIX(); a++) {
            if (G.Colortype == Grbw) {
                led_set_pixel_Color_Object_rgbw(
                    usedUhrType->getMatrix(a, b),
                    led_get_pixel_rgbw(usedUhrType->getMatrix(a, b + 1)));
            } else {
                led_set_pixel_Color_Object(
                    usedUhrType->getMatrix(a, b),
                    led_get_pixel(usedUhrType->getMatrix(a, b + 1)));
            }
        }
    }
}

//------------------------------------------------------------------------------

static void laufschrift(const char *buf) {
    static uint8_t i = 0, ii = 0;

    // Alle Pixes eins nach rechts schieben
    shift_all_pixels_to_right();

    if (i < 5) {
        for (uint8_t h = 0; h < 8; h++) {
            if (pgm_read_byte(&(font_7x5[buf[ii]][i])) & (1u << h)) {
                led_set_pixel(G.rgb[Effect][0], G.rgb[Effect][1],
                              G.rgb[Effect][2], G.rgb[Effect][3],
                              usedUhrType->getMatrix(h + 1, 10));
            } else {
                led_clear_pixel(usedUhrType->getMatrix(h + 1, 10));
            }
        }
    } else {
        for (uint8_t h = 0; h < 8; h++) {
            led_clear_pixel(usedUhrType->getMatrix(h + 1, 10));
        }
    }
    led_show();

    i++;
    if (i > 5) {
        i = 0;
        ii++;
        if (ii > strlen(buf)) {
            ii = 0;
        }
    }
}

//------------------------------------------------------------------------------

static void zeigeip(const char *buf) {
    uint8_t StringLength = strlen(buf);
    StringLength = StringLength * 6; // Times 6, because thats the length of a
                                     // char in the 7x5 font plus spacing
    for (uint8_t i = 0; i <= StringLength; i++) {
        laufschrift(buf);
        delay(200);
    }
}

//------------------------------------------------------------------------------

void set_pixel_for_char(uint8_t i, uint8_t h, uint8_t offset,
                        unsigned char unsigned_d1) {
    if (pgm_read_byte(&(font_7x5[unsigned_d1][i])) & (1u << h)) {
        led_set_pixel(G.rgb[Effect][0], G.rgb[Effect][1], G.rgb[Effect][2],
                      G.rgb[Effect][3],
                      usedUhrType->getMatrix(h + 1, i + offset));
    }
}

static void zahlen(const char d1, const char d2) {
    uhr_clear();
    for (uint8_t i = 0; i < 5; i++) {
        for (uint8_t h = 0; h < 8; h++) {
            // 1. Zahl ohne Offset
            set_pixel_for_char(i, h, 0, static_cast<unsigned char>(d1));
            // 2. Zahl mit Offset
            set_pixel_for_char(i, h, 6, static_cast<unsigned char>(d2));
        }
    }
    led_show();
}

//------------------------------------------------------------------------------

static void set_stunde(uint8_t std, uint8_t voll) {
    switch (std) {
    case 0:
        uhrzeit |= ((uint32_t)1 << H_ZWOELF);
        break;
    case 1:
        if (voll == 1) {
            uhrzeit |= ((uint32_t)1 << H_EIN);
        } else {
            uhrzeit |= ((uint32_t)1 << EINS);
        }
        break;
    case 2:
        uhrzeit |= ((uint32_t)1 << H_ZWEI);
        break;
    case 3:
        uhrzeit |= ((uint32_t)1 << H_DREI);
        break;
    case 4:
        uhrzeit |= ((uint32_t)1 << H_VIER);
        break;
    case 5:
        uhrzeit |= ((uint32_t)1 << H_FUENF);
        break;
    case 6:
        uhrzeit |= ((uint32_t)1 << H_SECHS);
        break;
    case 7:
        uhrzeit |= ((uint32_t)1 << H_SIEBEN);
        break;
    case 8:
        uhrzeit |= ((uint32_t)1 << H_ACHT);
        break;
    case 9:
        uhrzeit |= ((uint32_t)1 << H_NEUN);
        break;
    case 10:
        uhrzeit |= ((uint32_t)1 << H_ZEHN);
        break;
    case 11:
        uhrzeit |= ((uint32_t)1 << H_ELF);
        break;
    case 12:
        uhrzeit |= ((uint32_t)1 << H_ZWOELF);
        break;
    case 13:
        if (voll == 1) {
            uhrzeit |= ((uint32_t)1 << H_EIN);
        } else {
            uhrzeit |= ((uint32_t)1 << EINS);
        }
        break;
    case 14:
        uhrzeit |= ((uint32_t)1 << H_ZWEI);
        break;
    case 15:
        uhrzeit |= ((uint32_t)1 << H_DREI);
        break;
    case 16:
        uhrzeit |= ((uint32_t)1 << H_VIER);
        break;
    case 17:
        uhrzeit |= ((uint32_t)1 << H_FUENF);
        break;
    case 18:
        uhrzeit |= ((uint32_t)1 << H_SECHS);
        break;
    case 19:
        uhrzeit |= ((uint32_t)1 << H_SIEBEN);
        break;
    case 20:
        uhrzeit |= ((uint32_t)1 << H_ACHT);
        break;
    case 21:
        uhrzeit |= ((uint32_t)1 << H_NEUN);
        break;
    case 22:
        uhrzeit |= ((uint32_t)1 << H_ZEHN);
        break;
    case 23:
        uhrzeit |= ((uint32_t)1 << H_ELF);
        break;
    case 24:
        uhrzeit |= ((uint32_t)1 << H_ZWOELF);
        break;
    default:
        break;
    }
}

//------------------------------------------------------------------------------

static void set_uhrzeit() {
    uhrzeit = 0;

    if (_stunde == 23 && _minute == 59 && _sekunde >= 50) {
        Serial.printf("Count down: %d\n", 60 - _sekunde);
        switch (_sekunde) {
        case 50:
            uhrzeit |= (1u << ZEHN);
            break;
        case 51:
            uhrzeit |= (1u << H_NEUN);
            break;
        case 52:
            uhrzeit |= (1u << H_ACHT);
            break;
        case 53:
            uhrzeit |= (1u << H_SIEBEN);
            break;
        case 54:
            uhrzeit |= (1u << H_SECHS);
            break;
        case 55:
            uhrzeit |= (1u << FUENF);
            break;
        case 56:
            uhrzeit |= (1u << H_VIER);
            break;
        case 57:
            uhrzeit |= (1u << H_DREI);
            break;
        case 58:
            uhrzeit |= (1u << H_ZWEI);
            break;
        case 59:
            uhrzeit |= (1u << EINS);
            break;
        }
        return;
    }

    uhrzeit |= ((uint32_t)1 << ESIST);

    uint8_t m = _minute / 5;
    switch (m) {
    case 0: // volle Stunde
        uhrzeit |= ((uint32_t)1 << UHR);
        set_stunde(_stunde, 1);
        break;
    case 1: // 5 nach
        uhrzeit |= ((uint32_t)1 << FUENF);
        uhrzeit |= ((uint32_t)1 << NACH);
        set_stunde(_stunde, 0);
        break;
    case 2: // 10 nach
        uhrzeit |= ((uint32_t)1 << ZEHN);
        uhrzeit |= ((uint32_t)1 << NACH);
        set_stunde(_stunde, 0);
        break;
    case 3: // viertel nach
        if (G.Sprachvariation[ItIs15] == 1) {
            uhrzeit |= ((uint32_t)1 << VIERTEL);
            set_stunde(_stunde + 1, 0);
        } else {
            uhrzeit |= ((uint32_t)1 << VIERTEL);
            uhrzeit |= ((uint32_t)1 << NACH);
            set_stunde(_stunde, 0);
        }
        break;
    case 4: // 20 nach
        if (G.Sprachvariation[ItIs20] == 1) {
            uhrzeit |= ((uint32_t)1 << ZEHN);
            uhrzeit |= ((uint32_t)1 << VOR);
            uhrzeit |= ((uint32_t)1 << HALB);
            set_stunde(_stunde + 1, 0);
        } else {
            uhrzeit |= ((uint32_t)1 << ZWANZIG);
            uhrzeit |= ((uint32_t)1 << NACH);
            set_stunde(_stunde, 0);
        }
        break;
    case 5: // 5 vor halb
        uhrzeit |= ((uint32_t)1 << FUENF);
        uhrzeit |= ((uint32_t)1 << VOR);
        uhrzeit |= ((uint32_t)1 << HALB);
        set_stunde(_stunde + 1, 0);
        break;
    case 6: // halb
        uhrzeit |= ((uint32_t)1 << HALB);
        set_stunde(_stunde + 1, 0);
        break;
    case 7: // 5 nach halb
        uhrzeit |= ((uint32_t)1 << FUENF);
        uhrzeit |= ((uint32_t)1 << NACH);
        uhrzeit |= ((uint32_t)1 << HALB);
        set_stunde(_stunde + 1, 0);
        break;
    case 8: // 20 vor
        if (G.Sprachvariation[ItIs40] == 1) {
            uhrzeit |= ((uint32_t)1 << ZEHN);
            uhrzeit |= ((uint32_t)1 << NACH);
            uhrzeit |= ((uint32_t)1 << HALB);
        } else {
            uhrzeit |= ((uint32_t)1 << ZWANZIG);
            uhrzeit |= ((uint32_t)1 << VOR);
        }
        set_stunde(_stunde + 1, 0);
        break;
    case 9: // viertel vor
        if (G.Sprachvariation[ItIs45] == 1 &&
            G.UhrtypeDef == Uhr_114_Alternative) {
            uhrzeit |= ((uint32_t)1 << DREIVIERTEL);
        } else {
            uhrzeit |= ((uint32_t)1 << VIERTEL);
            uhrzeit |= ((uint32_t)1 << VOR);
        }
        set_stunde(_stunde + 1, 0);
        break;
    case 10: // 10 vor
        uhrzeit |= ((uint32_t)1 << ZEHN);
        uhrzeit |= ((uint32_t)1 << VOR);
        set_stunde(_stunde + 1, 0);
        break;
    case 11: // 5 vor
        uhrzeit |= ((uint32_t)1 << FUENF);
        uhrzeit |= ((uint32_t)1 << VOR);
        set_stunde(_stunde + 1, 0);
        break;
    default:
        break;
    }
}

//------------------------------------------------------------------------------

static void show_sekunde() {
    uint8_t rr, gg, bb, ww;
    set_helligkeit(rr, gg, bb, ww, Effect);

    led_set_pixel(rr, gg, bb, ww, usedUhrType->getRMatrix(_sekunde48));
}

//------------------------------------------------------------------------------

static void show_minuten() {
    if (G.zeige_min > 0) {
        // Minuten / Sekunden-Animation
        // Minute (1-4)  ermitteln
        uint8_t m = _minute;
        while (m > 4) {
            m -= 5;
        }

        if (m > 0) {
            Word_array[usedUhrType->getMinArr(G.zeige_min - 1, 0)] =
                usedUhrType->getMinArr(G.zeige_min - 1, 0);
        }
        if (m > 1) {
            Word_array[usedUhrType->getMinArr(G.zeige_min - 1, 1)] =
                usedUhrType->getMinArr(G.zeige_min - 1, 1);
        }
        if (m > 2) {
            Word_array[usedUhrType->getMinArr(G.zeige_min - 1, 2)] =
                usedUhrType->getMinArr(G.zeige_min - 1, 2);
        }
        if (m > 3) {
            Word_array[usedUhrType->getMinArr(G.zeige_min - 1, 3)] =
                usedUhrType->getMinArr(G.zeige_min - 1, 3);
        }
    }
}

//------------------------------------------------------------------------------
// Wetterdaten anzeigen
//------------------------------------------------------------------------------
static void show_wetter() {

    switch (wetterswitch) {
    // +6h
    case 1: {
        switch (wstunde) {
        case 1:
            usedUhrType->show(w_mittag);
            break;
        case 2:
            usedUhrType->show(w_abend);
            break;
        case 3:
            usedUhrType->show(w_nacht);
            break;
        case 4: {
            usedUhrType->show(w_morgen);
            usedUhrType->show(w_frueh);
        } break;
        }
        switch (wtemp_6) {
        case 30: {
            usedUhrType->show(w_ueber);
            usedUhrType->show(w_dreissig);
            usedUhrType->show(w_grad);
        } break;
        case 25: {
            usedUhrType->show(w_ueber);
            usedUhrType->show(w_fuenf);
            usedUhrType->show(w_und);
            usedUhrType->show(w_zwanzig);
            usedUhrType->show(w_grad);
        } break;
        case 20: {
            usedUhrType->show(w_ueber);
            usedUhrType->show(w_zwanzig);
            usedUhrType->show(w_grad);
        } break;
        case 15: {
            usedUhrType->show(w_ueber);
            usedUhrType->show(w_fuenf);
            usedUhrType->show(w_zehn);
            usedUhrType->show(w_grad);
        } break;
        case 10: {
            usedUhrType->show(w_ueber);
            usedUhrType->show(w_zehn);
            usedUhrType->show(w_grad);
        } break;
        case 5: {
            usedUhrType->show(w_ueber);
            usedUhrType->show(w_fuenf);
            usedUhrType->show(w_grad);
        } break;
        case 1: {
            usedUhrType->show(w_ueber);
            usedUhrType->show(w_null);
            usedUhrType->show(w_grad);
        } break;
        case -1: {
            usedUhrType->show(w_unter);
            usedUhrType->show(w_minus);
            usedUhrType->show(w_null);
            usedUhrType->show(w_grad);
        } break;
        case -5: {
            usedUhrType->show(w_unter);
            usedUhrType->show(w_minus);
            usedUhrType->show(w_fuenf);
            usedUhrType->show(w_grad);
        } break;
        case -10: {
            usedUhrType->show(w_unter);
            usedUhrType->show(w_minus);
            usedUhrType->show(w_zehn);
            usedUhrType->show(w_grad);
        } break;
        case -15: {
            usedUhrType->show(w_unter);
            usedUhrType->show(w_minus);
            usedUhrType->show(w_fuenf);
            usedUhrType->show(w_zehn);
            usedUhrType->show(w_grad);
        } break;
        case -20: {
            usedUhrType->show(w_unter);
            usedUhrType->show(w_minus);
            usedUhrType->show(w_zwanzig);
            usedUhrType->show(w_grad);
        } break;
        case -25: {
            usedUhrType->show(w_unter);
            usedUhrType->show(w_minus);
            usedUhrType->show(w_fuenf);
            usedUhrType->show(w_und);
            usedUhrType->show(w_zwanzig);
            usedUhrType->show(w_grad);
        } break;
        }
        switch (wwetter_6) {
        case 200:
            usedUhrType->show(w_gewitter);
            break;
        case 300:
            usedUhrType->show(w_regen);
            break;
        case 500:
            usedUhrType->show(w_regen);
            break;
        case 600:
            usedUhrType->show(w_schnee);
            break;
        case 700:
            usedUhrType->show(w_warnung);
            break;
        case 800:
            usedUhrType->show(w_klar);
            break;
        case 801:
            usedUhrType->show(w_wolken);
            break;
        }
    } break;
    // +12h
    case 2: {
        switch (wstunde) {
        case 1:
            usedUhrType->show(w_abend);
            break;
        case 2:
            usedUhrType->show(w_nacht);
            break;
        case 3: {
            usedUhrType->show(w_morgen);
            usedUhrType->show(w_frueh);
        } break;
        case 4: {
            usedUhrType->show(w_morgen);
            usedUhrType->show(w_mittag);
        } break;
        }
        switch (wtemp_12) {
        case 30: {
            usedUhrType->show(w_ueber);
            usedUhrType->show(w_dreissig);
            usedUhrType->show(w_grad);
        } break;
        case 25: {
            usedUhrType->show(w_ueber);
            usedUhrType->show(w_fuenf);
            usedUhrType->show(w_und);
            usedUhrType->show(w_zwanzig);
            usedUhrType->show(w_grad);
        } break;
        case 20: {
            usedUhrType->show(w_ueber);
            usedUhrType->show(w_zwanzig);
            usedUhrType->show(w_grad);
        } break;
        case 15: {
            usedUhrType->show(w_ueber);
            usedUhrType->show(w_fuenf);
            usedUhrType->show(w_zehn);
            usedUhrType->show(w_grad);
        } break;
        case 10: {
            usedUhrType->show(w_ueber);
            usedUhrType->show(w_zehn);
            usedUhrType->show(w_grad);
        } break;
        case 5: {
            usedUhrType->show(w_ueber);
            usedUhrType->show(w_fuenf);
            usedUhrType->show(w_grad);
        } break;
        case 1: {
            usedUhrType->show(w_ueber);
            usedUhrType->show(w_null);
            usedUhrType->show(w_grad);
        } break;
        case -1: {
            usedUhrType->show(w_unter);
            usedUhrType->show(w_minus);
            usedUhrType->show(w_null);
            usedUhrType->show(w_grad);
        } break;
        case -5: {
            usedUhrType->show(w_unter);
            usedUhrType->show(w_minus);
            usedUhrType->show(w_fuenf);
            usedUhrType->show(w_grad);
        } break;
        case -10: {
            usedUhrType->show(w_unter);
            usedUhrType->show(w_minus);
            usedUhrType->show(w_zehn);
            usedUhrType->show(w_grad);
        } break;
        case -15: {
            usedUhrType->show(w_unter);
            usedUhrType->show(w_minus);
            usedUhrType->show(w_fuenf);
            usedUhrType->show(w_zehn);
            usedUhrType->show(w_grad);
        } break;
        case -20: {
            usedUhrType->show(w_unter);
            usedUhrType->show(w_minus);
            usedUhrType->show(w_zwanzig);
            usedUhrType->show(w_grad);
        } break;
        case -25: {
            usedUhrType->show(w_unter);
            usedUhrType->show(w_minus);
            usedUhrType->show(w_fuenf);
            usedUhrType->show(w_und);
            usedUhrType->show(w_zwanzig);
            usedUhrType->show(w_grad);
        } break;
        }
        switch (wwetter_12) {
        case 200:
            usedUhrType->show(w_gewitter);
            break;
        case 300:
            usedUhrType->show(w_regen);
            break;
        case 500:
            usedUhrType->show(w_regen);
            break;
        case 600:
            usedUhrType->show(w_schnee);
            break;
        case 700:
            usedUhrType->show(w_warnung);
            break;
        case 800:
            usedUhrType->show(w_klar);
            break;
        case 801:
            usedUhrType->show(w_wolken);
            break;
        }

    } break;
    // +18h
    case 3: {
        switch (wstunde) {
        case 1:
            usedUhrType->show(w_nacht);
            break;
        case 2: {
            usedUhrType->show(w_morgen);
            usedUhrType->show(w_frueh);
        } break;
        case 3: {
            usedUhrType->show(w_morgen);
            usedUhrType->show(w_mittag);
        } break;
        case 4: {
            usedUhrType->show(w_morgen);
            usedUhrType->show(w_abend);
        } break;
        }
        switch (wtemp_18) {
        case 30: {
            usedUhrType->show(w_ueber);
            usedUhrType->show(w_dreissig);
            usedUhrType->show(w_grad);
        } break;
        case 25: {
            usedUhrType->show(w_ueber);
            usedUhrType->show(w_fuenf);
            usedUhrType->show(w_und);
            usedUhrType->show(w_zwanzig);
            usedUhrType->show(w_grad);
        } break;
        case 20: {
            usedUhrType->show(w_ueber);
            usedUhrType->show(w_zwanzig);
            usedUhrType->show(w_grad);
        } break;
        case 15: {
            usedUhrType->show(w_ueber);
            usedUhrType->show(w_fuenf);
            usedUhrType->show(w_zehn);
            usedUhrType->show(w_grad);
        } break;
        case 10: {
            usedUhrType->show(w_ueber);
            usedUhrType->show(w_zehn);
            usedUhrType->show(w_grad);
        } break;
        case 5: {
            usedUhrType->show(w_ueber);
            usedUhrType->show(w_fuenf);
            usedUhrType->show(w_grad);
        } break;
        case 1: {
            usedUhrType->show(w_ueber);
            usedUhrType->show(w_null);
            usedUhrType->show(w_grad);
        } break;
        case -1: {
            usedUhrType->show(w_unter);
            usedUhrType->show(w_minus);
            usedUhrType->show(w_null);
            usedUhrType->show(w_grad);
        } break;
        case -5: {
            usedUhrType->show(w_unter);
            usedUhrType->show(w_minus);
            usedUhrType->show(w_fuenf);
            usedUhrType->show(w_grad);
        } break;
        case -10: {
            usedUhrType->show(w_unter);
            usedUhrType->show(w_minus);
            usedUhrType->show(w_zehn);
            usedUhrType->show(w_grad);
        } break;
        case -15: {
            usedUhrType->show(w_unter);
            usedUhrType->show(w_minus);
            usedUhrType->show(w_fuenf);
            usedUhrType->show(w_zehn);
            usedUhrType->show(w_grad);
        } break;
        case -20: {
            usedUhrType->show(w_unter);
            usedUhrType->show(w_minus);
            usedUhrType->show(w_zwanzig);
            usedUhrType->show(w_grad);
        } break;
        case -25: {
            usedUhrType->show(w_unter);
            usedUhrType->show(w_minus);
            usedUhrType->show(w_fuenf);
            usedUhrType->show(w_und);
            usedUhrType->show(w_zwanzig);
            usedUhrType->show(w_grad);
        } break;
        }
        switch (wwetter_18) {
        case 200:
            usedUhrType->show(w_gewitter);
            break;
        case 300:
            usedUhrType->show(w_regen);
            break;
        case 500:
            usedUhrType->show(w_regen);
            break;
        case 600:
            usedUhrType->show(w_schnee);
            break;
        case 700:
            usedUhrType->show(w_warnung);
            break;
        case 800:
            usedUhrType->show(w_klar);
            break;
        case 801:
            usedUhrType->show(w_wolken);
            break;
        }

    } break;
    // +24h
    case 4: {
        switch (wstunde) {
        case 1: {
            usedUhrType->show(w_morgen);
            usedUhrType->show(w_frueh);
        } break;
        case 2: {
            usedUhrType->show(w_morgen);
            usedUhrType->show(w_mittag);
        } break;
        case 3: {
            usedUhrType->show(w_morgen);
            usedUhrType->show(w_abend);
        } break;
        case 4: {
            usedUhrType->show(w_morgen);
            usedUhrType->show(w_nacht);
        } break;
        }
        switch (wtemp_24) {
        case 30: {
            usedUhrType->show(w_ueber);
            usedUhrType->show(w_dreissig);
            usedUhrType->show(w_grad);
        } break;
        case 25: {
            usedUhrType->show(w_ueber);
            usedUhrType->show(w_fuenf);
            usedUhrType->show(w_und);
            usedUhrType->show(w_zwanzig);
            usedUhrType->show(w_grad);
        } break;
        case 20: {
            usedUhrType->show(w_ueber);
            usedUhrType->show(w_zwanzig);
            usedUhrType->show(w_grad);
        } break;
        case 15: {
            usedUhrType->show(w_ueber);
            usedUhrType->show(w_fuenf);
            usedUhrType->show(w_zehn);
            usedUhrType->show(w_grad);
        } break;
        case 10: {
            usedUhrType->show(w_ueber);
            usedUhrType->show(w_zehn);
            usedUhrType->show(w_grad);
        } break;
        case 5: {
            usedUhrType->show(w_ueber);
            usedUhrType->show(w_fuenf);
            usedUhrType->show(w_grad);
        } break;
        case 1: {
            usedUhrType->show(w_ueber);
            usedUhrType->show(w_null);
            usedUhrType->show(w_grad);
        } break;
        case -1: {
            usedUhrType->show(w_unter);
            usedUhrType->show(w_minus);
            usedUhrType->show(w_null);
            usedUhrType->show(w_grad);
        } break;
        case -5: {
            usedUhrType->show(w_unter);
            usedUhrType->show(w_minus);
            usedUhrType->show(w_fuenf);
            usedUhrType->show(w_grad);
        } break;
        case -10: {
            usedUhrType->show(w_unter);
            usedUhrType->show(w_minus);
            usedUhrType->show(w_zehn);
            usedUhrType->show(w_grad);
        } break;
        case -15: {
            usedUhrType->show(w_unter);
            usedUhrType->show(w_minus);
            usedUhrType->show(w_fuenf);
            usedUhrType->show(w_zehn);
            usedUhrType->show(w_grad);
        } break;
        case -20: {
            usedUhrType->show(w_unter);
            usedUhrType->show(w_minus);
            usedUhrType->show(w_zwanzig);
            usedUhrType->show(w_grad);
        } break;
        case -25: {
            usedUhrType->show(w_unter);
            usedUhrType->show(w_minus);
            usedUhrType->show(w_fuenf);
            usedUhrType->show(w_und);
            usedUhrType->show(w_zwanzig);
            usedUhrType->show(w_grad);
        } break;
        }
        switch (wwetter_24) {
        case 200:
            usedUhrType->show(w_gewitter);
            break;
        case 300:
            usedUhrType->show(w_regen);
            break;
        case 500:
            usedUhrType->show(w_regen);
            break;
        case 600:
            usedUhrType->show(w_schnee);
            break;
        case 700:
            usedUhrType->show(w_warnung);
            break;
        case 800:
            usedUhrType->show(w_klar);
            break;
        case 801:
            usedUhrType->show(w_wolken);
            break;
        }

    } break;
    }
}

//------------------------------------------------------------------------------

static void show_zeit() {
    uint8_t rr, gg, bb, ww;

    set_uhrzeit();

    // Helligkeitswert ermitteln
    if (_stunde < 6) {
        G.hh = G.h24;
    } else if (_stunde < 8) {
        G.hh = G.h6;
    } else if (_stunde < 12) {
        G.hh = G.h8;
    } else if (_stunde < 16) {
        G.hh = G.h12;
    } else if (_stunde < 18) {
        G.hh = G.h16;
    } else if (_stunde < 20) {
        G.hh = G.h18;
    } else if (_stunde < 22) {
        G.hh = G.h20;
    } else if (_stunde < 24) {
        G.hh = G.h22;
    }

    set_helligkeit_ldr(rr, gg, bb, ww, Background);

    // Hintergrund setzen
    for (uint8_t t = 0; t < usedUhrType->ROWS_MATRIX(); t++) {
        for (uint8_t b = 0; b < 11; b++) {
            led_set_pixel(rr, gg, bb, ww, usedUhrType->getMatrix(t, b));
        }
    }

    if (uhrzeit & ((uint32_t)1 << ESIST)) {
        usedUhrType->show(es_ist);
    }
    if (uhrzeit & ((uint32_t)1 << FUENF)) {
        usedUhrType->show(fuenf);
    }
    if (uhrzeit & ((uint32_t)1 << ZEHN)) {
        usedUhrType->show(zehn);
    }
    if (uhrzeit & ((uint32_t)1 << VIERTEL)) {
        usedUhrType->show(viertel);
    }
    if (uhrzeit & ((uint32_t)1 << DREIVIERTEL)) {
        usedUhrType->show(dreiviertel);
    }
    if (uhrzeit & ((uint32_t)1 << ZWANZIG)) {
        usedUhrType->show(zwanzig);
    }
    if (uhrzeit & ((uint32_t)1 << HALB)) {
        usedUhrType->show(halb);
    }
    if (uhrzeit & ((uint32_t)1 << EINS)) {
        usedUhrType->show(eins);
    }
    if (uhrzeit & ((uint32_t)1 << VOR)) {
        usedUhrType->show(vor);
    }
    if (uhrzeit & ((uint32_t)1 << NACH)) {
        usedUhrType->show(nach);
    }
    if (uhrzeit & ((uint32_t)1 << H_EIN)) {
        usedUhrType->show(h_ein);
    }
    if (uhrzeit & ((uint32_t)1 << H_ZWEI)) {
        usedUhrType->show(h_zwei);
    }
    if (uhrzeit & ((uint32_t)1 << H_DREI)) {
        usedUhrType->show(h_drei);
    }
    if (uhrzeit & ((uint32_t)1 << H_VIER)) {
        usedUhrType->show(h_vier);
    }
    if (uhrzeit & ((uint32_t)1 << H_FUENF)) {
        usedUhrType->show(h_fuenf);
    }
    if (uhrzeit & ((uint32_t)1 << H_SECHS)) {
        usedUhrType->show(h_sechs);
    }
    if (uhrzeit & ((uint32_t)1 << H_SIEBEN)) {
        usedUhrType->show(h_sieben);
    }
    if (uhrzeit & ((uint32_t)1 << H_ACHT)) {
        usedUhrType->show(h_acht);
    }
    if (uhrzeit & ((uint32_t)1 << H_NEUN)) {
        usedUhrType->show(h_neun);
    }
    if (uhrzeit & ((uint32_t)1 << H_ZEHN)) {
        usedUhrType->show(h_zehn);
    }
    if (uhrzeit & ((uint32_t)1 << H_ELF)) {
        usedUhrType->show(h_elf);
    }
    if (uhrzeit & ((uint32_t)1 << H_ZWOELF)) {
        usedUhrType->show(h_zwoelf);
    }
    if (uhrzeit & ((uint32_t)1 << UHR)) {
        usedUhrType->show(uhr);
    }

    show_minuten();

    if (G.UhrtypeDef == Uhr_242) {
        show_wetter();
    }

    if (changes_in_array() == true) {
        led_set();
        copy_array(Word_array, Word_array_old);
    } else if (G.prog == COMMAND_MODE_WORD_CLOCK) {
        led_set();
    }
}

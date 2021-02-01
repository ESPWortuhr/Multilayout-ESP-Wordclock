#include "Animation.h"
#include "Uhr.h"
#include <Arduino.h>

Animation::Animation_t Animation::isSilvester(struct tm &tm) {
    static int lastMinute = 99;
    static int minutesActive;
    int _minute = tm.tm_min;
    if (lastMinute != _minute) {
        lastMinute = _minute;
        if ((tm.tm_mon == 12) && (tm.tm_mday == 31) && (tm.tm_hour == 23) &&
                (_minute == 59)) {
            //			if ((month(ltime) == 12) && (day(ltime) == 12)
            //&& (hour(ltime) == 14) && (_minute == 35)) {
            // if (_minute ==
            // G.autoLdrBright) {					// for
            // testing
            minutesActive = 0;
            previousAnimType = (Animation_t)G.animType;
            return COUNTDOWN;
        } else {
            minutesActive++;
        }
        if (silvester && (minutesActive >= 11)) {
            silvester = false;
            G.animType = previousAnimType;
        }
    }
    return (Animation_t)G.animType;
}

//------------------------------------------------------------------------------

bool Animation::isColorization() {
    return ((G.animType != KEINE) &&
            ((G.animColorize == WORDS) || (G.animColorize == CHARACTERS)));
}

//------------------------------------------------------------------------------

bool Animation::changeBrightness() {
    RgbaColor newForeground, newBackground;
    analyzeColors(
            NULL, STRIPE, newForeground,
            newBackground); // nur Vorder- Hintergrund aus LED stripe ermitteln
    bool adjustFg = newForeground != foreground,
            adjustBg = newBackground != background;

    if (adjustFg || adjustBg) {
        HsbColor hsbColor;
        float brightness = HsbColor(newForeground).B;
        hsbColor = HsbColor(foregroundMinute);
        hsbColor.B = brightness;
        foregroundMinute = RgbColor(hsbColor);
        RgbaColor (*matrix[3])[MAX_COL] = {neu, alt, work};
        for (int m = 0; m < 3; m++) {
            for (int r = 0; r < MAX_ROWS; r++) {
                for (int c = 0; c < MAX_COL; c++) {
                    if (adjustBg) {
                        if (!matrix[m][r][c].Foreground) { // set colorBackground
                            matrix[m][r][c] = newBackground;
                        }
                    }
                    if (adjustFg) {
                        if (matrix[m][r][c].Foreground) { // set colorForeground
                            if (isColorization()) {
                                hsbColor = HsbColor(matrix[m][r][c]);
                                hsbColor.B = brightness;
                                matrix[m][r][c] = RgbaColor(hsbColor, true);
                            } else {
                                matrix[m][r][c] = newForeground;
                            }
                        }
                    }
                }
            }
        }
        background = newBackground;
        foreground = newForeground;
    }
    return (adjustFg || adjustBg);
}

//------------------------------------------------------------------------------

// returns hue values with a difference of at least 0.1 (360 * 0,1 = 36 degree)
// avoiding neighbors with very similar colors
float Animation::pseudoRandomHue() {
    static bool first = true;
    if (first) {
        first = false;
        return pseudoRandomHue(true);
    }
    return pseudoRandomHue(false);
}

//------------------------------------------------------------------------------

float Animation::pseudoRandomHue(bool init) {
    static uint16_t lastColors[MAX_RANDOM];
    static uint16_t lastColor = 1000;
    static uint8_t inUse = 1;
    uint16_t hue;
    int i, tries;

    if (init || (inUse == (MAX_RANDOM - 1))) {
        inUse = 1;
        lastColors[0] = lastColor;
        for (i = 1; i < MAX_RANDOM; i++) {
            lastColors[i] = 9999;
        }
    }
    hue = 1000 + random(1000); // 1000 ... 2000
    tries = i = 0;
    do {
        if (lastColors[i] == 9999) { // empty array element
            inUse = i;
            lastColor = lastColors[i] = hue;
            return ((float)(hue - 1000)) / 1000.0; // 0.0 ... 1.0
        }
        if (((hue > (lastColors[i] - 100)) && (hue < (lastColors[i] + 100)))) {
            // hue matches an existing color, try new hue
            hue = 1000 + random(1000);
            tries++;
            i = 0;
        } else {
            // check next array element
            i++;
        }
    } while ((i < MAX_RANDOM) && (tries < 20));
    return pseudoRandomHue(true);
}

//------------------------------------------------------------------------------

// colorize foreground
void Animation::colorize(RgbaColor (*dest)[MAX_COL]) {
    bool changeColor = true;
    HsbColor hsbColor = HsbColor(foreground);
    hsbColor.H = pseudoRandomHue();
    foregroundMinute = isColorization() ? RgbaColor(hsbColor, true) : foreground;
    hsbColor.H = pseudoRandomHue();
    for (int r = 0; r < MAX_ROWS; r++) {
        for (int c = 0; c < MAX_COL; c++) {
            if (dest[r][c].Foreground) {
                if ((G.animColorize == CHARACTERS) || changeColor) {
                    changeColor = false;
                    hsbColor.H = pseudoRandomHue();
                }
                dest[r][c] = isColorization() ?
                        RgbaColor(hsbColor, true) : foreground;
            } else {
                changeColor = true;
            }
        }
    }
}

//------------------------------------------------------------------------------

void Animation::saveMatrix() {
    //		Serial.println("Save Matrix");
    memcpy(alt, neu, sizeof(alt));
    analyzeColors(neu, STRIPE, foreground,
            background); // copy from stripe to neu[][]
    foregroundMinute = foreground;
    if (isColorization()) {
        colorize(neu);
    }
    return;
}

//------------------------------------------------------------------------------

// kopiere (interne Matrix oder vom LED Stripe) und ermittle Vorder- und
// Hintergrundfarbe
void Animation::analyzeColors(RgbaColor (*dest)[MAX_COL],
        RgbaColor (*source)[MAX_COL],
        RgbaColor &foreground, RgbaColor &background) {
    RgbaColor color, color1(0), color2(0);
    int colorCounter1 = 0, colorCounter2 = 0;
    for (int r = 0; r < MAX_ROWS; r++) {
        for (int c = 0; c < MAX_COL; c++) {
            if (source == STRIPE) {
                color = led_get_pixel(
                        usedUhrType->getFrontMatrix(r + ROW_START, c + COL_START));
            } else {
                color = source[r][c];
            }
            if (dest != NULL) {
                dest[r][c] = color;
            }
            if (color == color1) {
                colorCounter1++;
            } else {
                if (color == color2) {
                    colorCounter2++;
                } else {
                    // noch keine Farbe zugewiesen
                    if (colorCounter1 > 0) {
                        color2 = color;
                        colorCounter2 = 1;
                    } else {
                        color1 = color;
                        colorCounter1 = 1;
                    }
                }
            }
        }
    }
    if (colorCounter1 > colorCounter2) { // Mehrheitsentscheid ?!
        background = color1;
        foreground = color2;
    } else {
        background = color2;
        foreground = color1;
    }
    foreground.Foreground = true;
    background.Foreground = false;
    if (dest != NULL) {
        for (int r = 0; r < MAX_ROWS; r++) {
            for (int c = 0; c < MAX_COL; c++) {
                dest[r][c].Foreground = dest[r][c] == foreground;
            }
        }
    }
}

//------------------------------------------------------------------------------

void Animation::set_minutes() {
    if (G.zeige_min > 0) {
        uint8_t m = lastMinute % 5;
        led_set_pixel_Color_Object(
                usedUhrType->getMinArr(G.zeige_min - 1, 0),
                m > 0 ? foregroundMinute : background);
        led_set_pixel_Color_Object(
                usedUhrType->getMinArr(G.zeige_min - 1, 1),
                m > 1 ? foregroundMinute : background);
        led_set_pixel_Color_Object(
                usedUhrType->getMinArr(G.zeige_min - 1, 2),
                m > 2 ? foregroundMinute : background);
        led_set_pixel_Color_Object(
                usedUhrType->getMinArr(G.zeige_min - 1, 3),
                m > 3 ? foregroundMinute : background);
    }
}
// Ueberschreibe die LEDs mit interner Matrix
void Animation::copy2Stripe(RgbaColor (*source)[MAX_COL]) {
    for (uint8_t row = 0; row < MAX_ROWS; row++) {
        for (uint8_t col = 0; col < MAX_COL; col++) {
            led_set_pixel_Color_Object(
                    usedUhrType->getFrontMatrix(row + ROW_START, col + COL_START),
                    source[row][col]);
        }
    }
    set_minutes();
}

//------------------------------------------------------------------------------

void Animation::copy2Work(RgbaColor (*source)[MAX_COL]) {
    memcpy(work, source, sizeof(work));
}

//------------------------------------------------------------------------------

void Animation::fillMatrix(RgbaColor (*matrix)[MAX_COL], RgbaColor color,
        bool fg) {
    for (uint8_t row = 0; row < MAX_ROWS; row++) {
        for (uint8_t col = 0; col < MAX_COL; col++) {
            matrix[row][col].changeRgb(color);
            matrix[row][col].Foreground = fg;
        }
    }
}

//------------------------------------------------------------------------------

// changed: 0   Aenderungen, z.B. Farbe, keine aenderung des Inhalts
// changed: 1   Inhalt hat sich geaendert
bool Animation::led_show_notify(bool changed, uint8_t minute) {
    bool led_show = true;

    if (G.animType == KEINE) {
        if ((changed == 1) && (lastMinute != minute)) {
            lastMinute = minute;
            matrixChanged =
                    true; // fuer den Fall, dass die Animation eingeschaltet wird
        }
    } else {
        bool brightnessChanged = changeBrightness(); // Helligkeit anpassen
        if (changed) {
            // matrix hat sich geaendert
            if (lastMinute != minute) {
                lastMinute = minute;
                matrixChanged = true;
                nextActionTime = 0; // ###################
                phase = 1;          // -> starte Animation
                led_show = false;   // ###################
            }
        } else {
            if (brightnessChanged) {
                copy2Stripe(work);
            }
            led_show = isIdle();
        }
    }
    return led_show;
}

//------------------------------------------------------------------------------

// muss staendig aufgerufen werden!!
void Animation::loop(struct tm &tm) {
    static Animation_t lastAnimType = KEINE;
    static uint8_t lastAnimDemo = 99;
    static uint8_t lastAnimSpeed = 99;
    static uint8_t lastAnimColor = 99;

    if (matrixChanged) {
        matrixChanged = false;
        saveMatrix();
    }

    if ((G.prog == COMMAND_IDLE) || (G.prog == COMMAND_MODE_WORD_CLOCK)) {
        G.animType = isSilvester(tm);

        if (G.animType == KEINE) {
            if (lastAnimType != KEINE) {
                lastAnimType = KEINE;
                memcpy(work, neu, sizeof(work));
                colorize(work);
                copy2Stripe(work);
                led_show();
            }
        } else {
            if (isIdle()) {
                if (G.animType != lastAnimType) {
                    lastAnimType = (Animation_t)G.animType;
                    phase = 1;
                }
                if (G.animColorize != lastAnimColor) {
                    lastAnimColor = G.animColorize;
                    colorize(work);
                }
            }

            uint32_t now = millis();
            if ((!isIdle()) && (now >= nextActionTime)) {
                nextActionTime = now + animationDelay;
                if ((G.animType != lastAnimType) ||
                        (G.animDuration != lastAnimSpeed) ||
                        (G.animColorize != lastAnimColor) ||
                        (G.animDemo != lastAnimDemo)) {
                    lastAnimType = (Animation_t)G.animType;
                    lastAnimDemo = G.animDemo;
                    lastAnimSpeed = G.animDuration;
                    lastAnimColor = G.animColorize;
                    phase = 1;
                    colorize(neu);
                }
                //					Serial.printf("Anim:
                // type %d phase %d\n", G.animType, phase);
                switch (G.animType) {
                case HOCH_ROLLEN:
                    phase = animScrollDown(false);
                    break;
                case RUNTER_ROLLEN:
                    phase = animScrollDown(true);
                    break;
                case LINKS_SCHIEBEN:
                    phase = animScrollRight(false); // links schieben
                    break;
                case RECHTS_SCHIEBEN:
                    phase = animScrollRight(true);
                    break;
                case UEBERBLENDEN:
                    phase = animFade();
                    break;
                case LASER:
                    phase = animLaser();
                    break;
                case MATRIX:
                    phase = animMatrixRain();
                    break;
                case BAELLE:
                    phase = animBalls();
                    break;
                case FIRE:
                    phase = animFire();
                    break;
                case COUNTDOWN:
                    phase = animCountdown(tm);
                    break;
                }
            }
            animColorChange();
            copy2Stripe(work);
            led_show();
        }
    }
}

//------------------------------------------------------------------------------
/*
void Animation::blockgrafik(uint8_t rot, uint8_t gruen, uint8_t blau,
                            BLOCKS num) {
    for (uint8_t row = 0; row < MAX_ROWS; row++) {
        for (uint8_t col = 0; col < MAX_COL; col++) {
            if (grafik_11x10[num][row] & (1 << (10 - col))) {
                led_set_pixel(rot, gruen, blau, 0,
                              usedUhrType->getFrontMatrix(row, col));
            } else {
                led_set_pixel(0, 0, 0, 0,
                              usedUhrType->getFrontMatrix(row, col));
            }
        }
    }
    led_show();
}
 */

//------------------------------------------------------------------------------

uint16_t Animation::reverse(uint16_t num, bool mirrored) {
    // reverse left 11 bits
    if (mirrored) {
        uint16_t res = 0;
        for (int i = 0; i < 11; i++) {
            if (num & (1 << i)) {
                res |= 0b10000000000 >> i;
            }
        }
        num = res;
    }
    return num;
}

//------------------------------------------------------------------------------

void Animation::copyBlock(RgbaColor color, uint block, bool fgbg, bool mirrored,
        bool init) {
    if (!init) {
        memcpy(alt, tmp, sizeof(alt));
    }
    uint16_t column;
    for (int r = 0; r < MAX_ROWS; r++) {
        for (int c = 0; c < MAX_COL; c++) {
            column =
                    reverse(pgm_read_word(&(grafik_11x10[block][r])), mirrored);
            if (column & (1 << (10 - c))) {
                tmp[r][c] = color;
                tmp[r][c].Foreground = true;
            } else {
                if (fgbg) {
                    tmp[r][c].Foreground = false;
                }
            }
        }
    }
    if (init) {
        memcpy(alt, tmp, sizeof(alt));
    }
}

//------------------------------------------------------------------------------

void Animation::demoMode(uint8_t &_minute, uint8_t _sekunde) {
    static uint8_t test_second = 0;
    static uint8_t test_minute = 45;
    if (G.animDemo) {
        if (isIdle() && ((_sekunde % 10) == 0) && (test_second != _sekunde)) {
            test_minute += 5;
            if (test_minute >= 60) {
                test_minute = _minute % 5;
            }
        }
        _minute = test_minute;
        test_second = _sekunde;
    }
}

//------------------------------------------------------------------------------

// langsam == 1 -> 5s
// mittel  == 2 -> 3.5s
// schnell == 3 -> 2s
uint16_t Animation::calcDelay(uint16_t frames) {
    uint pause;
    if (frames == 0) { // avoid div 0
        frames = 10;
    }
    switch (G.animDuration) {
    case 1:
        pause = 2000 / frames;
        break; // 2 seconds
    case 3:
        pause = 5000 / frames;
        break; // 5 seconds
    default:
        pause = 3500 / frames;
        break; // 3.5 seconds
    }
    if (pause == 0) {
        pause = 1;
    }
    return pause;
}

//------------------------------------------------------------------------------

// die neue matrix rutscht von oben rein     | unten rein
//                      row                  |               row
// phase | 9876543210   alt  neu   wechsel   | 9876543210   alt  neu   wechsel
//       | unten oben                        | unten oben
//   1   | aaaaaaaaan   0-8  9     0         | naaaaaaaaa   1-9  0     9
//   2   | aaaaaaaann   0-7  8-9   1         | nnaaaaaaaa   2-9  0-1   8
//   3   | aaaaaaannn   0-6  7-9   2         | nnnaaaaaaa   3-9  0-2   7
//   4   | aaaaaannnn   0-5  6-9   3         | nnnnaaaaaa   4-9  0-3   6
//   5   | aaaaannnnn   0-4  5-9   4         | nnnnnaaaaa   5-9  0-4   5
//   6   | aaaannnnnn   0-3  4-9   5         | nnnnnnaaaa   6-9  0-5   4
//   7   | aaannnnnnn   0-4  3-9   6         | nnnnnnnaaa   7-9  0-6   3
//   8   | aannnnnnnn   0-1  2-9   7         | nnnnnnnnaa   8-9  0-7   2
//   9   | annnnnnnnn   0    1-9   8         | nnnnnnnnna   9    0-8   1
//  10   | nnnnnnnnnn        0-9   9         | nnnnnnnnnn        0-9   0

uint16_t Animation::animScrollDown(bool dirDown) {
    int wechsel, rowAlt, rowNeu;
    bool copyFromNeu;

    if (phase == 1) {
        animationDelay = calcDelay(MAX_ROWS);
    }
    if (dirDown) {
        wechsel = phase;
        rowAlt = 0;
        rowNeu = MAX_ROWS - phase;
    } else {
        wechsel = MAX_ROWS - phase;
        rowAlt = phase;
        rowNeu = 0;
    }
    for (uint8_t row = 0; row < MAX_ROWS; row++) {
        copyFromNeu = (row >= wechsel) ^ dirDown;
        for (uint8_t col = 0; col < MAX_COL; col++) {
            if (copyFromNeu) {
                work[row][col] = neu[rowNeu][col];
            } else {
                work[row][col] = alt[rowAlt][col];
            }
        }
        if (copyFromNeu) {
            rowNeu++;
        } else {
            rowAlt++;
        }
    }
    if (phase >= MAX_ROWS) {
        return 0;
    }
    return phase + 1;
}

//------------------------------------------------------------------------------

// die neue matrix rutscht von rechts rein / links rein
//                       col
// phase | 01234567890   alt  neu   Wechsel  01234567890   alt  neu   Wechsel
//   1   | aaaaaaaaaan   1-10 0      0       naaaaaaaaaa   0-9  10    10
//   2   | aaaaaaaaann   2-10 0-1    1       nnaaaaaaaaa   0-8  9-10   9
//   3   | aaaaaaaannn   3-10 0-2    2       nnnaaaaaaaa   0-7  8-10   8
//
//  10   | annnnnnnnnn   10   0-9    9       nnnnnnnnnna   0    1-10   1
//  11   | nnnnnnnnnnn        0-10  10       nnnnnnnnnnn        0-10   0
uint16_t Animation::animScrollRight(bool dirRight) {
    int wechsel, colAlt, colNeu;
    bool copyFromNeu;

    if (phase == 1) {
        animationDelay = calcDelay(MAX_COL);
    }
    if (dirRight) {
        wechsel = phase;
        colAlt = 0;
        colNeu = MAX_COL - phase;
    } else {
        wechsel = MAX_COL - phase;
        colAlt = phase;
        colNeu = 0;
    }
    for (uint8_t col = 0; col < MAX_COL; col++) {
        copyFromNeu = (col >= wechsel) ^ dirRight;
        for (uint8_t row = 0; row < MAX_ROWS; row++) {
            if (copyFromNeu) {
                work[row][col] = neu[row][colNeu];
            } else {
                work[row][col] = alt[row][colAlt];
            }
        }
        if (copyFromNeu) {
            colNeu++;
        } else {
            colAlt++;
        }
    }
    if (phase >= MAX_COL) {
        return 0;
    }
    return phase + 1;
}

//------------------------------------------------------------------------------

// In jeder Spalte fällt ein Ball jeweils vom hoechsten Buchstaben.
uint16_t Animation::animBalls() {
    static uint32_t starttime;
    static int numBalls;
    int oldR, r, c, ballsDown;
    uint32_t timeDelta, now;

    if (phase == 1) {
        animationDelay = 50; // 20 Frames per second
        numBalls = 0;
        for (c = 0; (c < MAX_COL) && (numBalls < MAX_BAELLE); c++) {
            for (r = 0; (r < MAX_ROWS) && (numBalls < MAX_BAELLE); r++) {
                if (work[r][c].Foreground) {
                    baelle[numBalls].begin(r, c, work[r][c], background,
                            100 * numBalls);
                    numBalls++;
                    break;
                }
            }
        }
        starttime = millis();
    }

    now = millis();
    timeDelta = now - starttime;
    starttime = now;
    ballsDown = 0;
    for (int b = 0; b < numBalls; b++) {
        oldR = baelle[b].r;
        ballsDown += baelle[b].move(timeDelta);
        r = baelle[b].r; // r, c neue Koordinaten
        c = baelle[b].c;
        if (r > oldR) { // abwaerts
            for (; r > oldR; oldR++) {
                work[oldR][c] = background;
            }
        } else { // aufwaerts
            for (; r < oldR; oldR--) {
                work[oldR][c] = background;
            }
        }
        work[r][c] = baelle[b].color;
    }
    if (ballsDown >= numBalls) {
        copy2Work(neu); // TODO schiess Baelle hoch
        return 0;
    }
    return phase + 1;
}

//------------------------------------------------------------------------------

uint16_t Animation::animFire() {
    static const bool blockInit = true;
    static const bool blockFgOnly = false;
    static const bool blockFgBg = true;
    static const uint blendingFrames = 20;
    static bool sparkle, mirrored = false;
    static uint subPhase;

    if (phase == 1) {
        animationDelay = calcDelay(blendingFrames * 10) / 2; // FIRE_1 .. 6 + 4
        phase = FIRE_1;
        sparkle = false;
        subPhase = 1;
        memcpy(tmp, neu, sizeof(tmp));
        copyBlock(_white, phase, blockFgBg, mirrored, blockInit);
    }

    if (subPhase > blendingFrames) {
        subPhase = 1;
        phase++;
        switch (phase) {
        case FIRE_4:
            copyBlock(_red, FIRE_4, blockFgBg, mirrored);
            break;
        case FIRE_5:
            copyBlock(_red, FIRE_5, blockFgBg, mirrored);
            break;
        case FIRE_6:
            copyBlock(_red, FIRE_6, blockFgBg, mirrored);
            break;
        case (FIRE_6 + 1):
            		        copyBlock(_blue, FIRE_6, blockFgBg, mirrored);
        copyBlock(_green, FIRE_4, blockFgOnly, mirrored);
        copyBlock(_yellow, FIRE_5, blockFgOnly, mirrored);
        break;
        case (FIRE_6 + 2):
            		        sparkle = true;
        break;
        case (FIRE_6 + 3):
            		        break;
        case (FIRE_6 + 4):
            		        copy2Work(neu);
        animationDelay = 1000;
        mirrored = !mirrored;
        return silvester ? 1 : 0; // end of animation
        break;
        default:
            copyBlock(_white, phase, blockFgBg, mirrored);
        }
    }

    RgbColor color;
    float progress = (float)subPhase++ * (1.0 / blendingFrames);
    if (progress > 1.0) {
        progress = 1.0;
    }
    for (uint8_t col = 0; col < MAX_COL; col++) {
        for (uint8_t row = 0; row < MAX_ROWS; row++) {
            if (sparkle) {
                if (tmp[row][col].Foreground) {
                    if (random(8) == 0) {
                        tmp[row][col].Foreground = false;
                        ;
                        alt[row][col].Foreground = true;
                        work[row][col].changeRgb(_white); // _white on
                    }
                } else {
                    if (alt[row][col].Foreground) {
                        tmp[row][col].Foreground = true;
                        alt[row][col].Foreground = false;
                        work[row][col].changeRgb(background); // _white off
                    }
                }
            } else {
                if (tmp[row][col].Foreground) {
                    color = color.LinearBlend(background, tmp[row][col],
                            progress); // blend in
                    work[row][col].changeRgb(color);
                } else {
                    if (alt[row][col].Foreground) {
                        color = color.LinearBlend(alt[row][col], background,
                                progress); // blend out
                        work[row][col].changeRgb(color);
                        tmp[row][col] = work[row][col];
                    }
                }
            }
        }
    }
    return phase;
}

//------------------------------------------------------------------------------

void Animation::set_pixel_for_char(uint8_t col, uint8_t row, uint8_t offsetCol,
        uint8_t offsetRow, unsigned char unsigned_d1,
        HsbColor color) {
    if (pgm_read_byte(&(font_7x5[unsigned_d1][col])) & (1u << row)) {
        work[row + 1][col + offsetCol] = RgbaColor(color, true);
    }
}

//------------------------------------------------------------------------------

uint16_t Animation::animCountdown(struct tm &tm) {
    static int lastSecond = 99, countDown = 60;
    int _second = tm.tm_sec; // 0 - 59
    if (_second != lastSecond) {
        if (phase == 1) {
            animationDelay = 50;
        }
        if (countDown < 0) { // 60 - 0
            countDown = 60;
            lastSecond = 99;
            silvester = true;
            G.animType = FIRE;
            return 1; // continue FIRE in phase 1
        }
        lastSecond = _second;
        fillMatrix(work, background, false);
        HsbColor hsbColor_1 = HsbColor(foreground);
        HsbColor hsbColor_2 = HsbColor(foreground);
        hsbColor_1.H = pseudoRandomHue();
        hsbColor_2.H = pseudoRandomHue();
        char seconds[8];
        sprintf(seconds, "%d", countDown); // start 23:59:00		60 - 0
        // for (uint8_t i = 0; i < 5; i++) {
        for (uint8_t col = 0; col < 5; col++) {     // column
            for (uint8_t row = 0; row < 8; row++) { // row
                if (countDown >= 10) {
                    // 1. Zahl ohne Offset
                    set_pixel_for_char(col, row, 0, 0,
                            static_cast<unsigned char>(seconds[0]),
                            hsbColor_1);
                    // 2. Zahl mit Offset
                    set_pixel_for_char(col, row, 6, 0,
                            static_cast<unsigned char>(seconds[1]),
                            hsbColor_2);
                } else {
                    set_pixel_for_char(col, row, 3, 0,
                            static_cast<unsigned char>(seconds[0]),
                            hsbColor_1);
                }
            }
        }
        countDown--;
        phase++;
    }
    return phase;
}

//------------------------------------------------------------------------------

void Animation::animColorChange() {
    static uint32_t lastTimeColor = 0;
    static uint32_t pauseZeitColor =
            50; // 20 Wechsel / Sekunde -> 1200 W. / Minute

    if (isColorization() && (G.animSpeed > 0)) {
        uint32_t now = millis();
        if (now >= (lastTimeColor + pauseZeitColor)) {
            lastTimeColor = now;
            // Farbwinkel 0.0 - 1.0
            float deltaHue =
                    fmod(1.0 / (G.animSpeed * 20.0),
                            1.0); // Alle Farben in 1 .. 60 Sekunden / 0 == aus
            HsbColor hsbColor;
            for (uint8_t row = 0; row < MAX_ROWS; row++) {
                for (uint8_t col = 0; col < MAX_COL; col++) {
                    if (work[row][col].Foreground) {
                        hsbColor = HsbColor(work[row][col]);
                        hsbColor.H = fmod(hsbColor.H + deltaHue, 1.0);
                        work[row][col] = RgbaColor(hsbColor, true);
                    }
                }
            }
            hsbColor = HsbColor(foregroundMinute);
            hsbColor.H = fmod(hsbColor.H + deltaHue, 1.0);
            foregroundMinute = RgbColor(hsbColor);
        }
    }
}

//------------------------------------------------------------------------------

uint16_t Animation::animLaser() {
    static uint8_t row = 0, col = 0;
    static bool loeschPhase = true;
    static RgbaColor strahl(255, false);

    if (phase == 1) {
        // erster Durchgang loeschen
        animationDelay = calcDelay(MAX_ROWS * MAX_COL * 2);
        row = 0;
        col = 0;
        copy2Work(alt);
        loeschPhase = true;
    }

    if (loeschPhase) {
        work[row][col] = background;
    } else {
        work[row][col] = neu[row][col];
    }

    if (++col >= MAX_COL) {
        col = 0;
        row++;
    }
    if (row < MAX_ROWS) {
        work[row][col] = strahl;
    } else {
        row = 0;
        if (loeschPhase == false) {
            return 0;
        }
        // starte zweiten Durchgang
        loeschPhase = false;
    }
    return phase + 1;
}

//------------------------------------------------------------------------------

uint16_t Animation::animFade() {
    uint frames = 100;
    RgbColor color;

    if (phase == 1) {
        animationDelay = calcDelay(frames);
        memcpy(alt, work, sizeof(alt));
    }
    float progress = (float)phase / (float)frames;

    for (uint8_t col = 0; col < MAX_COL; col++) {
        for (uint8_t row = 0; row < MAX_ROWS; row++) {
            color = color.LinearBlend(alt[row][col], neu[row][col], progress);
            work[row][col].changeRgb(color);
        }
    }
    if (phase >= frames) {
        copy2Work(neu); // take all color.Foreground's from neu
        return 0;
    }
    return phase + 1;
}

//------------------------------------------------------------------------------

uint16_t Animation::animMatrixRain() {
    uint frames = 100;
    int row, col;
    RgbaColor fadeColor, rainColor;

    if (phase == 1) {
        animationDelay = calcDelay(frames);
        memcpy(alt, work,
                sizeof(alt)); // work is still the previous animated array
        int stop;
        uint8_t brightness = foreground.CalculateBrightness();
        for (col = 0; col < MAX_COL; col++) {
            stop = MAX_ROWS - 1;
            for (row = MAX_ROWS - 1; row >= 0; row--) {
                if (work[row][col].Foreground) {
                    stop = row;
                    break;
                }
            }
            rain[col].begin(frames, stop, brightness);
        }
    }
    float progress = ((float)phase) / frames;
    for (col = 0; col < MAX_COL; col++) {
        for (row = 0; row < MAX_ROWS; row++) {
            fadeColor =
                    fadeColor.LinearBlend(alt[row][col], neu[row][col], progress);
            rainColor = rain[col].get(row);
            rainColor =
                    rainColor.LinearBlend(fadeColor, rainColor, rainColor.Alpha);
            work[row][col].changeRgb(rainColor);
        }
    }
    if (phase >= frames) {
        copy2Work(neu); // take all color.Foreground's from neu
        return 0;
    }
    return phase + 1;
}

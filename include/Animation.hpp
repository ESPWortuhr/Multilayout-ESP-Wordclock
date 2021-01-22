/*
 * Animation.cpp
 *
 *  Created on: 15.01.2019
 *      Author: Andreas
 */

#define MAX_BAELLE ANIMATION_COLUMNS
#define MAX_ANIM_PERIOD 5
#define MAX_RANDOM 10
#define STRIPE NULL

// ###############################################################################
class Rain {
public:
    Rain(){};
    virtual ~Rain(){};

    void begin(int frames, int stop, uint8_t helligkeit) {
        white = RgbaColor(helligkeit, 1.0f);
        // white.Lighten(helligkeit);
        green = RgbaColor(0, helligkeit, 0, 0.5);
        // green.Lighten(helligkeit);
        offset = -(ANIMATION_ROWS - 1 - 3); // 3 = min deadtime
        speed = 1;
        stopping = false;
        speedlimit = random(1, 4);
        lifetime = random(4, ANIMATION_ROWS);
        deadtime = random(3, ANIMATION_ROWS); // min = 3, max deadtime
        //    speedlimit = 2;
        //    lifetime = 4;
        //    deadtime = 3;
        phase = 1;
        stopLine = ANIMATION_ROWS - 1 - stop;
        stopPhase = frames - speedlimit * ANIMATION_ROWS;
    }

    RgbaColor get(int r) {
        int row = (ANIMATION_ROWS - 1 - r);
        // pro Bild laeuft row von (ANIMATION_ROWS - 1) runter auf 0

        int pos = (row + offset) % (deadtime + lifetime);

        if (row == 0) { // letzte row faer dieses Bild
            if (phase++ == stopPhase) {
                stopTop = ANIMATION_ROWS;
                stopBottom = stopLine;
                stopping = true;
            }
            if (speed++ >= speedlimit) {
                speed = 1;
                offset++;
            }
        }

        if (stopping) {
            if ((row >= stopTop) || ((row < stopLine) && (row >= stopBottom))) {
                if ((row == 0) && (speed == 1)) {
                    stopTop--;
                    stopBottom--;
                }
                return transparent;
            }
            if ((row == 0) && (speed == 1)) {
                stopTop--;
                stopBottom--;
            }
        }
        if ((pos) < deadtime) {
            return transparent;
        }
        if ((pos) == deadtime) {
            return white;
        }
        if ((pos) < (deadtime + lifetime)) {
            return green;
        }
        offset = 0;
        return transparent;
    }

protected:
    int speed, speedlimit, offset, lifetime, deadtime;
    int phase, frames, stopPhase, stopLine, stopTop, stopBottom;
    bool stopping;
    RgbaColor white = RgbaColor(255, 255, 255, 0.9);
    RgbaColor green = RgbaColor(0, 255, 0, 0.75);
    RgbaColor transparent = RgbaColor(0, 0, 0, 0.0);
};
Rain rain[ANIMATION_COLUMNS] = {
    Rain(),
};
// ###############################################################################
class Ball {
public:
    Ball(){};
    virtual ~Ball(){};

    void begin(int row, int column, RgbaColor foreground, RgbaColor background,
               int delay) {
        this->delay = delay;
        y = row << 8; // increase precision
        r = row;
        c = column;
        vy = 0;
        colorForeground = foreground;
        colorBackground = background;
        g = 9810 / G.animDuration;
        end = (y == unten) ? 1 : 0;
        lastDown = false;
    }

    // ###############################################################################
    // x =  0, y = 0 -> links oben
    // x = 10, y = 9 -> rechts unten
    // v positiv    -> nach unten
    int move(int timedelta) {
        if (!end) {
            delay -= timedelta;
            if (delay <= 0) {
                int _vy = vy;
                y += (((g * timedelta) / 1000) * timedelta) / 2000 +
                     (vy * timedelta) / 1000;
                vy += (g * timedelta) / 1000;
                if ((vy > 0) && (y >= unten)) { // unterer Wendepunkt
                    y = unten;                  // - (y - unten);
                    vy = -(vy * 6) / 10;        // Energie vernichten
                    if (lastDown) {
                        end = 1;
                    }
                }
                lastDown |= (_vy < 0) && (vy >= 0) &&
                            (y > lastPos); // oberer Wendepunkt
                r = y >> 8;
            }
            color = end ? colorBackground : colorForeground;
        }
        return end;
    }

public:
    int r, c; // after calling move() r and c contain new actual values
    RgbaColor color;

protected:
    static const int unten = ((ANIMATION_ROWS - 1) << 8);
    static const int lastPos = ((ANIMATION_ROWS - 3) << 8);
    ;
    int g, vy, y, end, delay;
    bool lastDown;
    RgbaColor colorForeground, colorBackground;
};
Ball baelle[MAX_BAELLE] = {
    Ball(),
};
// ###############################################################################

Animation::Animation(){};
Animation::~Animation(){};

void Animation::begin() {
    uint8_t row, column;
    for (row = 0; row < ANIMATION_ROWS; row++) {
        for (column = 0; column < ANIMATION_COLUMNS; column++) {
            getIndex[row][column] = usedUhrType->getFrontMatrix(row, column);
        }
    }
    animation.blockgrafik(0, 0, 255, WLAN); // blau
}

//------------------------------------------------------------------------------
bool Animation::isIdle() { return phase == 0; }
//------------------------------------------------------------------------------
Animation::Ani Animation::isSilvester(struct tm &tm) {
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
            previousAnimType = (Ani)G.animType;
            return COUNTDOWN;
        } else {
            minutesActive++;
        }
        if (silvester && (minutesActive >= 11)) {
            silvester = false;
            G.animType = previousAnimType;
        }
    }
    return (Ani)G.animType;
}
//------------------------------------------------------------------------------
bool Animation::isColorization() {
    return (G.animColorize == WORDS) || (G.animColorize == CHARACTERS);
}
//------------------------------------------------------------------------------
void Animation::changeBrightness() {
    int fgChanged = 0, bgChanged = 0;
    RgbaColor newForeground, newBackground;
    analyzeColors(
        NULL, STRIPE, newForeground,
        newBackground); // nur Vorder- Hintergrund aus LED stripe ermitteln
    bool adjustFg = newForeground != foreground,
         adjustBg = newBackground != background;

    if (adjustFg || adjustBg) {
        HsbColor hsbColor;
        float brightness = HsbColor(newForeground).B;

        for (int r = 0; r < ANIMATION_ROWS; r++) {
            for (int c = 0; c < ANIMATION_COLUMNS; c++) {
                if (adjustBg) {
                    if (!work[r][c].Foreground) { // set colorBackground
                        bgChanged++;
                        work[r][c] = newBackground;
                    }
                }
                if (adjustFg) {
                    if (work[r][c].Foreground) { // set colorForeground
                        fgChanged++;
                        if (isColorization()) {
                            hsbColor = HsbColor(work[r][c]);
                            hsbColor.B = brightness;
                            work[r][c] = RgbaColor(hsbColor, true);
                        } else {
                            work[r][c] = newForeground;
                        }
                    }
                }
            }
        }
        background = newBackground;
        foreground = newForeground;
    }
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
void Animation::saveMatrix() {
    bool changeColor = true;
    //		Serial.println("Save Matrix");
    memcpy(alt, neu, sizeof(alt));
    analyzeColors(neu, STRIPE, foreground,
                  background); // copy from stripe to neu[][]
    if (isColorization()) {
        HsbColor hsbColor = HsbColor(foreground);
        for (int r = 0; r < ANIMATION_ROWS; r++) {
            for (int c = 0; c < ANIMATION_COLUMNS; c++) {
                if (neu[r][c].Foreground) {
                    if ((G.animColorize == CHARACTERS) || changeColor) {
                        changeColor = false;
                        hsbColor.H = pseudoRandomHue();
                    }
                    neu[r][c] = RgbaColor(hsbColor, true);
                } else {
                    changeColor = true;
                }
            }
        }
    }
    return;
}
//------------------------------------------------------------------------------
// kopiere (interne Matrix oder vom LED Stripe) und ermittle Vorder- und
// Hintergrundfarbe
void Animation::analyzeColors(RgbaColor (*dest)[ANIMATION_COLUMNS],
                              RgbaColor (*source)[ANIMATION_COLUMNS],
                              RgbaColor &foreground, RgbaColor &background) {
    RgbaColor color, color1(0), color2(0);
    int colorCounter1 = 0, colorCounter2 = 0;
    for (int r = 0; r < ANIMATION_ROWS; r++) {
        for (int c = 0; c < ANIMATION_COLUMNS; c++) {
            if (source == STRIPE) {
                color = led_get_pixel(getIndex[r + ANIMATION_ROW_START]
                                              [c + ANIMATION_COLUMNS_START]);
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
        for (int r = 0; r < ANIMATION_ROWS; r++) {
            for (int c = 0; c < ANIMATION_COLUMNS; c++) {
                dest[r][c].Foreground = dest[r][c] == foreground;
            }
        }
    }
}
//------------------------------------------------------------------------------
// Ueberschreibe die LEDs mit interner Matrix
void Animation::copy2Stripe(RgbaColor (*source)[ANIMATION_COLUMNS]) {
    for (int r = 0; r < ANIMATION_ROWS; r++) {
        for (int c = 0; c < ANIMATION_COLUMNS; c++) {
            RgbColor color = led_get_pixel(0);
            led_set_pixel_Color_Object(
                (uint16_t)getIndex[r + ANIMATION_ROW_START]
                                  [c + ANIMATION_COLUMNS_START],
                source[r][c]);
        }
    }
}
//------------------------------------------------------------------------------
void Animation::copy2Work(RgbaColor (*source)[ANIMATION_COLUMNS]) {
    memcpy(work, source, sizeof(work));
}
//------------------------------------------------------------------------------
void Animation::fillMatrix(RgbaColor (*matrix)[ANIMATION_COLUMNS],
                           RgbaColor color, bool fg) {
    for (int r = 0; r < ANIMATION_ROWS; r++) {
        for (int c = 0; c < ANIMATION_COLUMNS; c++) {
            matrix[r][c].changeRgb(color);
            matrix[r][c].Foreground = fg;
        }
    }
}
//------------------------------------------------------------------------------
// changed: 0   Aenderungen, z.B. Farbe, keine aenderung des Inhalts
// changed: 1   Inhalt hat sich geaendert
bool Animation::led_show_notify(bool changed, uint8_t minute) {
    static uint8_t lastMinute = 99;
    bool led_show = true;

    if (G.animType == KEINE) {
        if ((changed == 1) && (lastMinute != minute)) {
            lastMinute = minute;
            matrixChanged =
                true; // fuer den Fall, dass die Animation eingeschaltet wird
        }
    } else {
        switch (changed) {
        case 0:
            if (isIdle()) {
                changeBrightness(); // Helligkeit anpassen
                copy2Stripe(work);
            } else {
                led_show = false;
            }
            break;
        case 1: // matrix hat sich geaendert
            if (lastMinute != minute) {
                lastMinute = minute;
                matrixChanged = true;
                nextActionTime = 0; // ###################
                phase = 1;          // -> starte Animation
                led_show = false;   // ###################
            }
            break;
        }
    }
    return led_show;
}

//------------------------------------------------------------------------------
// muss staendig aufgerufen werden!!
void Animation::loop(struct tm &tm) {
    static Ani lastAnimType = KEINE;
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
            lastAnimType = KEINE;
        } else {
            if (isIdle() && (G.animType != lastAnimType)) {
                lastAnimType = (Ani)G.animType;
                phase = 1;
            }

            uint32_t now = millis();
            if ((!isIdle()) && (now >= nextActionTime)) {
                nextActionTime = now + animationDelay;
                if ((G.animType != lastAnimType) ||
                    (G.animDuration != lastAnimSpeed) ||
                    (G.animColorize != lastAnimColor) ||
                    (G.animDemo != lastAnimDemo)) {
                    lastAnimType = (Ani)G.animType;
                    lastAnimDemo = G.animDemo;
                    lastAnimSpeed = G.animDuration;
                    lastAnimColor = G.animColorize;
                    phase = 1;
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

void Animation::blockgrafik(uint8_t rot, uint8_t gruen, uint8_t blau,
                            BLOCKS num) {
    for (int r = 0; r < ANIMATION_ROWS; r++) {
        for (int c = 0; c < ANIMATION_COLUMNS; c++) {
            if (grafik_11x10[num][r] & (1 << (10 - c))) {
                led_set_pixel(rot, gruen, blau, 0, getIndex[r][c]);
            } else {
                led_set_pixel(0, 0, 0, 0, getIndex[r][c]);
            }
        }
    }
    led_show();
}

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

void Animation::copyBlock(RgbaColor color, uint block, bool fgbg, bool mirrored,
                          bool init) {
    if (!init) {
        memcpy(alt, tmp, sizeof(alt));
    }
    uint16_t column;
    for (int r = 0; r < ANIMATION_ROWS; r++) {
        for (int c = 0; c < ANIMATION_COLUMNS; c++) {
            column = reverse(grafik_11x10[block][r], mirrored);
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
uint Animation::calcDelay(uint frames) {
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

uint Animation::animScrollDown(bool dirDown) {
    int wechsel, rowAlt, rowNeu;
    bool copyFromNeu;

    if (phase == 1) {
        animationDelay = calcDelay(ANIMATION_ROWS);
    }
    if (dirDown) {
        wechsel = phase;
        rowAlt = 0;
        rowNeu = ANIMATION_ROWS - phase;
    } else {
        wechsel = ANIMATION_ROWS - phase;
        rowAlt = phase;
        rowNeu = 0;
    }
    for (int r = 0; r < ANIMATION_ROWS; r++) {
        copyFromNeu = (r >= wechsel) ^ dirDown;
        for (int c = 0; c < ANIMATION_COLUMNS; c++) {
            if (copyFromNeu) {
                work[r][c] = neu[rowNeu][c];
            } else {
                work[r][c] = alt[rowAlt][c];
            }
        }
        if (copyFromNeu) {
            rowNeu++;
        } else {
            rowAlt++;
        }
    }
    if (phase >= ANIMATION_ROWS) {
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
uint Animation::animScrollRight(bool dirRight) {
    int wechsel, colAlt, colNeu;
    bool copyFromNeu;

    if (phase == 1) {
        animationDelay = calcDelay(ANIMATION_COLUMNS);
    }
    if (dirRight) {
        wechsel = phase;
        colAlt = 0;
        colNeu = ANIMATION_COLUMNS - phase;
    } else {
        wechsel = ANIMATION_COLUMNS - phase;
        colAlt = phase;
        colNeu = 0;
    }
    for (int c = 0; c < ANIMATION_COLUMNS; c++) {
        copyFromNeu = (c >= wechsel) ^ dirRight;
        for (int r = 0; r < ANIMATION_ROWS; r++) {
            if (copyFromNeu) {
                work[r][c] = neu[r][colNeu];
            } else {
                work[r][c] = alt[r][colAlt];
            }
        }
        if (copyFromNeu) {
            colNeu++;
        } else {
            colAlt++;
        }
    }
    if (phase >= ANIMATION_COLUMNS) {
        return 0;
    }
    return phase + 1;
}
//------------------------------------------------------------------------------
// In jeder Spalte fällt ein Ball jeweils vom hoechsten Buchstaben.
uint Animation::animBalls() {
    static uint32_t starttime;
    static int numBalls;
    int oldR, r, c, ballsDown;
    uint32_t timeDelta, now;

    if (phase == 1) {
        //		copy2Work(alt);
        animationDelay = 50; // 20 Frames per second
        numBalls = 0;
        for (c = 0; (c < ANIMATION_COLUMNS) && (numBalls < MAX_BAELLE); c++) {
            for (r = 0; (r < ANIMATION_ROWS) && (numBalls < MAX_BAELLE); r++) {
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
uint Animation::animFire() {
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
    for (int c = 0; c < ANIMATION_COLUMNS; c++) {
        for (int r = 0; r < ANIMATION_ROWS; r++) {
            if (sparkle) {
                if (tmp[r][c].Foreground) {
                    if (random(8) == 0) {
                        tmp[r][c].Foreground = false;
                        ;
                        alt[r][c].Foreground = true;
                        work[r][c].changeRgb(_white); // _white on
                    }
                } else {
                    if (alt[r][c].Foreground) {
                        tmp[r][c].Foreground = true;
                        alt[r][c].Foreground = false;
                        work[r][c].changeRgb(background); // _white off
                    }
                }
            } else {
                if (tmp[r][c].Foreground) {
                    color = color.LinearBlend(background, tmp[r][c],
                                              progress); // blend in
                    work[r][c].changeRgb(color);
                } else {
                    if (alt[r][c].Foreground) {
                        color = color.LinearBlend(alt[r][c], background,
                                                  progress); // blend out
                        work[r][c].changeRgb(color);
                        tmp[r][c] = work[r][c];
                    }
                }
            }
        }
    }
    return phase;
}
void Animation::set_pixel_for_char(uint8_t i, uint8_t h, uint8_t offset,
                                   unsigned char unsigned_d1, HsbColor color) {
    if (font_7x5[unsigned_d1][i] & (1u << h)) {
        work[h + 1][i + offset] = RgbaColor(color, true);
    }
}
//------------------------------------------------------------------------------
uint Animation::animCountdown(struct tm &tm) {
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
        for (uint8_t i = 0; i < 5; i++) {  // column
            for (uint8_t h = 0; h < 8; h++) { // row
                if (countDown >= 10) {
                    // 1. Zahl ohne Offset
                    set_pixel_for_char(i, h, 0,
                                       static_cast<unsigned char>(seconds[0]),
                                       hsbColor_1);
                    // 2. Zahl mit Offset
                    set_pixel_for_char(i, h, 6,
                                       static_cast<unsigned char>(seconds[1]),
                                       hsbColor_2);
                } else {
                    set_pixel_for_char(i, h, 3,
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
            for (int r = 0; r < ANIMATION_ROWS; r++) {
                for (int c = 0; c < ANIMATION_COLUMNS; c++) {
                    if (work[r][c].Foreground) {
                        hsbColor = HsbColor(work[r][c]);
                        hsbColor.H = fmod(hsbColor.H + deltaHue, 1.0);
                        work[r][c] = RgbaColor(hsbColor, true);
                    }
                }
            }
        }
    }
}
//------------------------------------------------------------------------------
uint Animation::animLaser() {
    static uint32_t r = 0, c = 0;
    static bool loeschPhase = true;
    static RgbaColor strahl(255, false);

    if (phase == 1) {
        // erster Durchgang loeschen
        animationDelay = calcDelay(ANIMATION_ROWS * ANIMATION_COLUMNS * 2);
        r = 0;
        c = 0;
        copy2Work(alt);
        loeschPhase = true;
    }

    if (loeschPhase) {
        work[r][c] = background;
    } else {
        work[r][c] = neu[r][c];
    }

    if (++c >= ANIMATION_COLUMNS) {
        c = 0;
        r++;
    }
    if (r < ANIMATION_ROWS) {
        work[r][c] = strahl;
    } else {
        r = 0;
        if (loeschPhase == false) {
            return 0;
        }
        // starte zweiten Durchgang
        loeschPhase = false;
    }
    return phase + 1;
}
//------------------------------------------------------------------------------
uint Animation::animFade() {
    uint frames = 100;
    RgbColor color;

    if (phase == 1) {
        animationDelay = calcDelay(frames);
        memcpy(alt, work, sizeof(alt));
    }
    float progress = (float)phase / (float)frames;

    for (int c = 0; c < ANIMATION_COLUMNS; c++) {
        for (int r = 0; r < ANIMATION_ROWS; r++) {
            color = color.LinearBlend(alt[r][c], neu[r][c], progress);
            work[r][c].changeRgb(color);
        }
    }
    if (phase >= frames) {
        copy2Work(neu); // take all color.Foreground's from neu
        return 0;
    }
    return phase + 1;
}
//------------------------------------------------------------------------------
uint32_t Animation::animMatrixRain() {
    uint frames = 100;
    int r, c;
    RgbaColor fadeColor, rainColor;

    if (phase == 1) {
        animationDelay = calcDelay(frames);
        memcpy(alt, work,
               sizeof(alt)); // work is still the previous animated array
        int stop;
        uint8_t brightness = foreground.CalculateBrightness();
        for (c = 0; c < ANIMATION_COLUMNS; c++) {
            stop = ANIMATION_ROWS - 1;
            for (r = ANIMATION_ROWS - 1; r >= 0; r--) {
                if (work[r][c].Foreground) {
                    stop = r;
                    break;
                }
            }
            rain[c].begin(frames, stop, brightness);
        }
    }
    float progress = ((float)phase) / frames;
    for (c = 0; c < ANIMATION_COLUMNS; c++) {
        for (r = 0; r < ANIMATION_ROWS; r++) {
            fadeColor = fadeColor.LinearBlend(alt[r][c], neu[r][c], progress);
            rainColor = rain[c].get(r);
            rainColor =
                rainColor.LinearBlend(fadeColor, rainColor, rainColor.Alpha);
            work[r][c].changeRgb(rainColor);
        }
    }
    if (phase >= frames) {
        copy2Work(neu); // take all color.Foreground's from neu
        return 0;
    }
    return phase + 1;
}
//------------------------------------------------------------------------------

// ###############################################################################

Animation animation;

#include "Animation.h"
#include "Uhr.h"
#include <Arduino.h>

#define STRIPE NULL
#define MAX_RANDOM 10

Animation::Animation(uint16 frameH, uint16 frameV, uint16 rows, uint16 cols) {
    row_start = frameH;
    col_start = frameV;
    max_rows = rows - 2 * frameH;
    max_cols = cols - 2 * frameV;
    sizeofColumn = max_cols * sizeof(RgbfColor);
    old = new RgbfColor *[max_rows];
    act = new RgbfColor *[max_rows];
    work = new RgbfColor *[max_rows];
    for (uint8_t row = 0; row < max_rows; row++) {
        old[row] = new RgbfColor[max_cols];
        act[row] = new RgbfColor[max_cols];
        work[row] = new RgbfColor[max_cols];
    }
    rain = new Rain[max_cols];
    balls = new Ball[max_cols];
    for (uint8_t col = 0; col < max_cols; col++) {
        rain[col] = Rain(max_rows, max_cols);
        balls[col] = Ball(max_rows);
    }
    snake = new Snake(max_rows, max_cols);
    firework = new Firework(max_rows, max_cols);
    animType = (Animation_t)G.animType;
    lastAnimType = (Animation_t)G.animType;
    lastAnimDemo = G.animDemo;
    lastAnimDuration = G.animDuration;
    lastAnimColorize = G.animColorize;
}

Animation::~Animation() {
#if 0
    // matrix type changes during runtime forces a ESP reset
    // probably to avoid reinitializing the Neopixelbus
    for (uint8_t row = 0; row < max_rows; row++) {
        delete[] old[row];
        delete[] act[row];
        delete[] work[row];
    }
    delete[] old;
    delete[] act;
    delete[] work;
    delete[] tmp;
    delete[] rain;
#endif
}

//------------------------------------------------------------------------------

bool Animation::isSilvester(Animation_t &type, struct tm &tm, bool trigger) {
    static uint8_t minutesAfterMidnight;

    if (trigger) {
#if 1
        if ((tm.tm_mon == 12) && (tm.tm_mday == 31) && (tm.tm_hour == 23) &&
            (tm.tm_min == 59))
#else
        if (tm.tm_min == G.autoLdrBright) // for testing
#endif
        {
            minutesAfterMidnight = 0;
            type = COUNTDOWN;
        } else {
            minutesAfterMidnight++;
        }
        if ((type == SILVESTER) && (minutesAfterMidnight >= 11)) {
            type = getAnimationType(true);
        }
    }
    return (type == COUNTDOWN) || (type == SILVESTER);
}

//------------------------------------------------------------------------------

Animation::Animation_t Animation::getAnimationType(bool trigger) {
    if (G.animType == RANDOM) {
        if (trigger) {
            return (Animation_t)random(animTypeFirst, animTypeLast + 1);
        } else {
            return animType;
        }
    }
    return (Animation_t)G.animType;
}

//------------------------------------------------------------------------------
bool Animation::isColorization() {
    return ((animType != KEINE) &&
            ((G.animColorize == WORDS) || (G.animColorize == CHARACTERS)));
}

//------------------------------------------------------------------------------

bool Animation::changeBrightness() {
    RgbfColor newForeground, newBackground;
    // determine only foreground and background from LED stripe
    analyzeColors(NULL, STRIPE, newForeground, newBackground);
    bool adjustFg = newForeground != foreground,
         adjustBg = newBackground != background;

    if (adjustFg || adjustBg) {
        HsbColor hsbColor;
        float brightness = HsbColor(newForeground).B;
        hsbColor = HsbColor(foregroundMinute);
        hsbColor.B = brightness;
        foregroundMinute = RgbColor(hsbColor);
        RgbfColor **matrix[3] = {act, old, work};
        for (uint8_t m = 0; m < 3; m++) {
            for (uint8_t r = 0; r < max_rows; r++) {
                for (uint8_t c = 0; c < max_cols; c++) {
                    if (adjustBg) {
                        if (!matrix[m][r][c].isForeground()) {
                            matrix[m][r][c] = newBackground;
                        }
                    }
                    if (adjustFg) {
                        if (matrix[m][r][c].isForeground()) {
                            if (isColorization()) {
                                hsbColor = HsbColor(matrix[m][r][c]);
                                hsbColor.B = brightness;
                                matrix[m][r][c].changeRgb(hsbColor);
                            } else {
                                matrix[m][r][c].changeRgb(newForeground);
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
    uint32_t i, tries;

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
            return (static_cast<float>(hue - 1000)) / 1000.0; // 0.0 ... 1.0
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
void Animation::colorize(RgbfColor **dest) {
    bool changeColor = true;
    HsbColor hsbColor = HsbColor(foreground);
    hsbColor.H = pseudoRandomHue();
    foregroundMinute = isColorization() ? RgbColor(hsbColor) : foreground;
    hsbColor.H = pseudoRandomHue();
    for (uint8_t r = 0; r < max_rows; r++) {
        for (uint8_t c = 0; c < max_cols; c++) {
            if (dest[r][c].isForeground()) {
                if ((G.animColorize == CHARACTERS) || changeColor) {
                    changeColor = false;
                    hsbColor.H = pseudoRandomHue();
                }
                dest[r][c].changeRgb(isColorization() ? hsbColor : foreground);
            } else {
                changeColor = true;
            }
        }
    }
}

//------------------------------------------------------------------------------

void Animation::saveMatrix() {
    static bool firstRun = true;
    copyMatrix(old, act);
    analyzeColors(act, STRIPE, foreground, background);
    foregroundMinute = foreground;
    if (isColorization()) {
        colorize(act);
    }
    if (firstRun) {
        firstRun = false;
        copyMatrix(work, act);
    }
    return;
}

//------------------------------------------------------------------------------

// copy (internal matrix or from LED stripe) and determine foreground and
// background color
void Animation::analyzeColors(RgbfColor **dest, RgbfColor **source,
                              RgbfColor &foreground, RgbfColor &background) {
    RgbfColor color, color1(0), color2(0);
    uint32_t colorCounter1 = 0, colorCounter2 = 0;
    for (uint8_t r = 0; r < max_rows; r++) {
        for (uint8_t c = 0; c < max_cols; c++) {
            if (source == STRIPE) {
                color = led.getPixel(
                    usedUhrType->getFrontMatrix(r + row_start, c + col_start));
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
                    // no color assigned yet
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
    if (colorCounter1 > colorCounter2) { // Majority vote ?!
        background = color1;
        foreground = color2;
    } else {
        background = color2;
        foreground = color1;
    }
    foreground.setForeground(true);
    foreground.setOverlay(false);
    background.setForeground(false);
    background.setOverlay(false);
    if (dest != NULL) {
        for (uint8_t r = 0; r < max_rows; r++) {
            for (uint8_t c = 0; c < max_cols; c++) {
                dest[r][c].setForeground(dest[r][c] == foreground);
            }
        }
    }
}

//------------------------------------------------------------------------------

void Animation::set_minutes() {
    if (G.zeige_min > 0) {
        uint8_t m = lastMinute % 5;
        led.setPixelColorObject(usedUhrType->getMinArr(G.zeige_min - 1, 0),
                                m > 0 ? foregroundMinute : background);
        led.setPixelColorObject(usedUhrType->getMinArr(G.zeige_min - 1, 1),
                                m > 1 ? foregroundMinute : background);
        led.setPixelColorObject(usedUhrType->getMinArr(G.zeige_min - 1, 2),
                                m > 2 ? foregroundMinute : background);
        led.setPixelColorObject(usedUhrType->getMinArr(G.zeige_min - 1, 3),
                                m > 3 ? foregroundMinute : background);
    }
}
// Overwrite the LEDs with internal matrix
void Animation::copy2Stripe(RgbfColor **source) {
    for (uint8_t row = 0; row < max_rows; row++) {
        for (uint8_t col = 0; col < max_cols; col++) {
            led.setPixelColorObject(
                usedUhrType->getFrontMatrix(row + row_start, col + col_start),
                source[row][col]);
        }
    }
    set_minutes();
}

//------------------------------------------------------------------------------

void Animation::copyMatrix(RgbfColor **dest, RgbfColor **source) {
    for (uint8_t r = 0; r < max_rows; r++) {
        memcpy(dest[r], source[r], sizeofColumn);
    }
}

//------------------------------------------------------------------------------

void Animation::copyMatrixFlags(RgbfColor **dest, RgbfColor **source) {
    for (uint8_t r = 0; r < max_rows; r++) {
        for (uint8_t c = 0; c < max_cols; c++) {
            dest[r][c].setFlags(source[r][c].getFlags());
        }
    }
}

//------------------------------------------------------------------------------

void Animation::fillMatrix(RgbfColor **matrix, RgbfColor color) {
    for (uint8_t row = 0; row < max_rows; row++) {
        for (uint8_t col = 0; col < max_cols; col++) {
            matrix[row][col] = color;
        }
    }
}

//------------------------------------------------------------------------------

// changed: 0 changes, e.g. color, no change of content
// changed: 1 content has changed
bool Animation::led_show_notify(bool changed, uint8_t minute) {
    bool led_show = true;

    if (animType == KEINE) {
        if (changed && (lastMinute != minute)) {
            lastMinute = minute;
            // in case the animation is switched on
            matrixChanged = true;
        }
    } else {
        bool brightnessChanged = changeBrightness(); // Adapt brightness
        if (changed) {
            if (lastMinute != minute) {
                lastMinute = minute;
                matrixChanged = true;
                nextActionTime = 0; // ###################
                phase = 1;          // -> start Animation
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

void Animation::loop(struct tm &tm) {
    static uint8_t lastMinute = 99;
    bool minuteChange = false;

    if (matrixChanged) {
        matrixChanged = false;
        saveMatrix();
    }

    if ((G.prog == COMMAND_IDLE) || (G.prog == COMMAND_MODE_WORD_CLOCK)) {
        if (lastMinute != _minute) {
            lastMinute = _minute;
            minuteChange = true;
        }
        if (!isSilvester(animType, tm, minuteChange)) {
            animType = getAnimationType(minuteChange);
        }

        if (animType == KEINE) {
            if (lastAnimType != KEINE) {
                lastAnimType = KEINE;
                copyMatrix(work, act);
                colorize(work);
                copy2Stripe(work);
                led.show();
            }
        } else {
            if ((animType != lastAnimType) ||
                (G.animDuration != lastAnimDuration) ||
                (G.animDemo != lastAnimDemo)) {
                lastAnimType = animType;
                lastAnimDemo = G.animDemo;
                lastAnimDuration = G.animDuration;
                phase = 1;
            }
            if (G.animColorize != lastAnimColorize) {
                lastAnimColorize = G.animColorize;
                colorize(isIdle() ? work : act);
            }

            uint32_t now = millis();
            if ((!isIdle()) && (now >= nextActionTime)) {
                nextActionTime = now + animationDelay;
                // Serial.printf("Anim: type %d phase %d\n", animType, phase);
                switch (animType) {
                case HOCH_ROLLEN:
                    phase = animScrollDown(false);
                    break;
                case RUNTER_ROLLEN:
                    phase = animScrollDown(true);
                    break;
                case LINKS_SCHIEBEN:
                    phase = animScrollRight(false); // shift left
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
                case SILVESTER:
                case FIRE:
                    phase = animFire();
                    break;
                case SCHLANGE:
                    phase = animSnake();
                    break;
                case COUNTDOWN:
                    phase = animCountdown(tm);
                    break;
                case RANDOM:
                case KEINE:
                    break;
                }
            }
            animColorChange();
            copy2Stripe(work);
            led.show();
        }
    }
}

//------------------------------------------------------------------------------

uint16_t Animation::reverse(uint16_t num, bool mirrored) {
    // reverse left 11 bits
    if (mirrored) {
        uint16_t res = 0;
        for (uint8_t i = 0; i < 11; i++) {
            if (num & (1 << i)) {
                res |= 0b10000000000 >> i;
            }
        }
        num = res;
    }
    return num;
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

// slow == 1 -> 5s
// mid  == 2 -> 3.5s
// fast == 3 -> 2s
uint16_t Animation::calcDelay(uint16_t frames) {
    uint32_t pause;
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

// the new matrix slides in from above       | below
//                      row                  |               row
// phase | 9876543210   old  act   wechsel   | 9876543210   old  act   wechsel
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
    uint32_t wechsel, rowAlt, rowNeu;
    bool copyFromNeu;

    if (phase == 1) {
        animationDelay = calcDelay(max_rows) / 4;
    }
    if (dirDown) {
        wechsel = phase;
        rowAlt = 0;
        rowNeu = max_rows - phase;
    } else {
        wechsel = max_rows - phase;
        rowAlt = phase;
        rowNeu = 0;
    }
    for (uint8_t row = 0; row < max_rows; row++) {
        copyFromNeu = (row >= wechsel) ^ dirDown;
        for (uint8_t col = 0; col < max_cols; col++) {
            if (copyFromNeu) {
                work[row][col] = act[rowNeu][col];
            } else {
                work[row][col] = old[rowAlt][col];
            }
        }
        if (copyFromNeu) {
            rowNeu++;
        } else {
            rowAlt++;
        }
    }
    if (phase >= max_rows) {
        return 0;
    }
    return phase + 1;
}

//------------------------------------------------------------------------------

// the new matrix slides in from the right / in from the left
//                       col
// phase | 01234567890   old  act   Wechsel  01234567890   old  act   Wechsel
//   1   | aaaaaaaaaan   1-10 0      0       naaaaaaaaaa   0-9  10    10
//   2   | aaaaaaaaann   2-10 0-1    1       nnaaaaaaaaa   0-8  9-10   9
//   3   | aaaaaaaannn   3-10 0-2    2       nnnaaaaaaaa   0-7  8-10   8
//
//  10   | annnnnnnnnn   10   0-9    9       nnnnnnnnnna   0    1-10   1
//  11   | nnnnnnnnnnn        0-10  10       nnnnnnnnnnn        0-10   0
uint16_t Animation::animScrollRight(bool dirRight) {
    uint32_t wechsel, colAlt, colNeu;
    bool copyFromNeu;

    if (phase == 1) {
        animationDelay = calcDelay(max_cols) / 4;
    }
    if (dirRight) {
        wechsel = phase;
        colAlt = 0;
        colNeu = max_cols - phase;
    } else {
        wechsel = max_cols - phase;
        colAlt = phase;
        colNeu = 0;
    }
    for (uint8_t col = 0; col < max_cols; col++) {
        copyFromNeu = (col >= wechsel) ^ dirRight;
        for (uint8_t row = 0; row < max_rows; row++) {
            if (copyFromNeu) {
                work[row][col] = act[row][colNeu];
            } else {
                work[row][col] = old[row][colAlt];
            }
        }
        if (copyFromNeu) {
            colNeu++;
        } else {
            colAlt++;
        }
    }
    if (phase >= max_cols) {
        return 0;
    }
    return phase + 1;
}

//------------------------------------------------------------------------------

// In each column, one ball falls from the highest letter.
uint16_t Animation::animBalls() {
    static uint32_t starttime;
    static uint32_t numBalls;
    uint32_t oldR, r, c, ballsDown;
    uint32_t timeDelta, now;

    if (phase == 1) {
        animationDelay = 50; // 20 Frames per second
        numBalls = 0;
        for (c = 0; (c < max_cols) && (numBalls < max_cols); c++) {
            for (r = 0; (r < max_rows) && (numBalls < max_cols); r++) {
                if (work[r][c].isForeground()) {
                    balls[numBalls].begin(r, c, work[r][c], background,
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
    for (uint8_t b = 0; b < numBalls; b++) {
        oldR = balls[b].r;
        ballsDown += balls[b].move(timeDelta);
        r = balls[b].r; // r, c new coordinates
        c = balls[b].c;
        if (r > oldR) { // down
            for (; r > oldR; oldR++) {
                work[oldR][c] = background;
            }
        } else { // up
            for (; r < oldR; oldR--) {
                work[oldR][c] = background;
            }
        }
        work[r][c] = balls[b].color;
    }
    if (ballsDown >= numBalls) {
        copyMatrix(work, act); // TODO(ATho95): shot Balls up
        return 0;
    }
    return phase + 1;
}

//------------------------------------------------------------------------------

uint16_t Animation::animFire() {
    static const uint8_t blendingFrames = 20;
    static bool sparkle, mirrored = false;
    static uint32_t subPhase;

    if (phase == 1) {
        // FIRE_1 .. 6 + 4 = 10
        animationDelay = calcDelay(blendingFrames * 10) / 2;
        phase = FIRE_1;
        sparkle = false;
        subPhase = 1;
        firework->prepare(0, _white, FIRE_1, mirrored);
        // use current colors to be blended to act
        copyMatrix(old, work);
        copyMatrixFlags(work, act);
    }

    bool lastSubPhase = subPhase == blendingFrames;
    if (subPhase > blendingFrames) {
        subPhase = 1;
        phase++;
        switch (phase) {
        case FIRE_4:
            firework->prepare(0, _red, FIRE_4, mirrored);
            break;
        case FIRE_5:
            firework->prepare(0, _red, FIRE_5, mirrored);
            break;
        case FIRE_6:
            firework->prepare(0, _red, FIRE_6, mirrored);
            break;
        case (FIRE_6 + 1):
            firework->prepare(0, _blue, FIRE_6, mirrored);
            firework->prepare(1, _green, FIRE_4, mirrored);
            firework->prepare(2, _yellow, FIRE_5, mirrored);
            break;
        case (FIRE_6 + 2):
            animationDelay *= 2;
            sparkle = true;
            break;
        case (FIRE_6 + 3):
            break;
        case (FIRE_6 + 4):
            mirrored = !mirrored;
            copyMatrix(old, act); // old contains artefacts
            if ((animType == SILVESTER)) {
                animationDelay = 500;
                return 1; // next animation in 500ms
            }
            return 0; // end of animation
            break;
        default:
            firework->prepare(0, _white, static_cast<Icons>(phase), mirrored);
        }
    }

    RgbColor overlayColor;
    float progress = static_cast<float>(subPhase) / blendingFrames;
    if (progress > 1.0) {
        progress = 1.0;
    }
    float fading =
        static_cast<float>((phase - FIRE_1) * blendingFrames + subPhase) /
        (blendingFrames * (FIRE_6 - FIRE_1 + 2));
    if (fading > 1.0) {
        fading = 1.0;
    }
    bool overlay;
    for (uint8_t col = 0; col < max_cols; col++) {
        for (uint8_t row = 0; row < max_rows; row++) {
            overlay = firework->getPixel(row, col, overlayColor);
            if (sparkle) {
                if (work[row][col].isOverlay()) {
                    // overlayed in previous run
                    work[row][col] = old[row][col]; // restore color
                    work[row][col].setOverlay(false);
                } else {
                    if (overlay && (random(10) == 0) && (!lastSubPhase)) {
                        old[row][col] = work[row][col]; // save color
                        work[row][col].changeRgb(_white);
                        work[row][col].setOverlay();
                    }
                }
            } else {
                // blend old -> act
                RgbColor color =
                    color.LinearBlend(old[row][col], act[row][col], fading);
                if (overlay && !lastSubPhase) {
                    // blend in overlay
                    overlayColor =
                        overlayColor.LinearBlend(color, overlayColor, progress);
                    work[row][col].changeRgb(overlayColor);
                    work[row][col].setOverlay();
                } else {
                    if (work[row][col].isOverlay()) {
                        // blend out overlay
                        color =
                            color.LinearBlend(overlayColor, color, progress);
                        work[row][col].changeRgb(color);
                        if (lastSubPhase) {
                            work[row][col].setOverlay(false);
                        }
                    } else {
                        // set mix of old and act
                        work[row][col].changeRgb(color);
                    }
                }
            }
        }
    }
    subPhase++;
    return phase;
}

//------------------------------------------------------------------------------

void Animation::setPixelForChar(uint8_t col, uint8_t row, uint8_t offsetCol,
                                   unsigned char unsigned_d1, HsbColor color) {
    if (pgm_read_byte(&(font_7x5[unsigned_d1][col])) & (1u << row)) {
        work[row + 1][col + offsetCol].changeRgb(color);
    }
}

//------------------------------------------------------------------------------

uint16_t Animation::animCountdown(struct tm &tm) {
    static uint8_t lastSecond = 99, countDown = 60;
    uint8_t _second = tm.tm_sec; // 0 - 59
    if (_second != lastSecond) {
        if (phase == 1) {
            animationDelay = 50;
        }
        if (countDown < 0) { // 60 - 0
            countDown = 60;
            lastSecond = 99;
            animType = SILVESTER;
            return 1; // continue FIRE in phase 1
        }
        lastSecond = _second;
        fillMatrix(work, background);
        HsbColor hsbColor_1 = HsbColor(foreground);
        HsbColor hsbColor_2 = HsbColor(foreground);
        hsbColor_1.H = pseudoRandomHue();
        hsbColor_2.H = pseudoRandomHue();
        char seconds[8];
        // start 23:59:00     60 - 0
        snprintf(seconds, sizeof(seconds), "%d", countDown);
        // for (uint8_t i = 0; i < 5; i++) {
        for (uint8_t row = 0; row < 8; row++) {     // row
            for (uint8_t col = 0; col < 5; col++) { // column
                if (countDown >= 10) {
                    // 1. Number without Offset
                    setPixelForChar(col, row, 0,
                                       static_cast<unsigned char>(seconds[0]),
                                       hsbColor_1);
                    // 2. Number with Offset
                    setPixelForChar(col, row, 6,
                                       static_cast<unsigned char>(seconds[1]),
                                       hsbColor_2);
                } else {
                    setPixelForChar(col, row, 3,
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
    static uint32_t pauseZeitColor = 50;

    if (isColorization() && (G.animSpeed > 0)) {
        uint32_t now = millis();
        if (now >= (lastTimeColor + pauseZeitColor)) {
            lastTimeColor = now;
            float deltaHue = fmod(1.0 / (G.animSpeed * 20.0), 1.0);
            HsbColor hsbColor;
            for (uint8_t row = 0; row < max_rows; row++) {
                for (uint8_t col = 0; col < max_cols; col++) {
                    if (work[row][col].isForeground()) {
                        hsbColor = HsbColor(work[row][col]);
                        hsbColor.H = fmod(hsbColor.H + deltaHue, 1.0);
                        work[row][col].changeRgb(hsbColor);
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
    static RgbfColor strahl(255);

    if (phase == 1) {
        animationDelay = calcDelay(max_rows * max_cols * 2);
        row = 0;
        col = 0;
        copyMatrix(work, old);
        loeschPhase = true;
    }

    if (loeschPhase) {
        work[row][col] = background;
    } else {
        work[row][col] = act[row][col];
    }

    if (++col >= max_cols) {
        col = 0;
        row++;
    }
    if (row < max_rows) {
        work[row][col] = strahl;
    } else {
        row = 0;
        if (loeschPhase == false) {
            return 0;
        }
        loeschPhase = false;
    }
    return phase + 1;
}

//------------------------------------------------------------------------------

uint16_t Animation::animFade() {
    uint16_t frames = 100;
    RgbColor color;

    if (phase == 1) {
        animationDelay = calcDelay(frames);
        copyMatrix(old, work);
    }
    float progress = static_cast<float>(phase) / static_cast<float>(frames);

    for (uint8_t col = 0; col < max_cols; col++) {
        for (uint8_t row = 0; row < max_rows; row++) {
            color = color.LinearBlend(old[row][col], act[row][col], progress);
            work[row][col].changeRgb(color);
        }
    }
    if (phase >= frames) {
        copyMatrix(work, act); // take all color.Foreground's from act
        return 0;
    }
    return phase + 1;
}

//------------------------------------------------------------------------------

uint16_t Animation::animMatrixRain() {
    uint16_t frames = 100;
    uint8_t row, col;
    RgbfColor fadeColor;
    RgbaColor rainColor;

    if (phase == 1) {
        animationDelay = calcDelay(frames);
        copyMatrix(old, work); // work is still the previous animated array
        uint8_t stop;
        uint8_t brightness = foreground.CalculateBrightness();
        for (col = 0; col < max_cols; col++) {
            stop = max_rows - 1;
            for (row = max_rows - 1; row >= 0; row--) {
                if (work[row][col].isForeground()) {
                    stop = row;
                    break;
                }
            }
            rain[col].begin(frames, stop, brightness);
        }
    }
    float progress = static_cast<float>(phase) / static_cast<float>(frames);
    for (col = 0; col < max_cols; col++) {
        for (row = 0; row < max_rows; row++) {
            fadeColor =
                fadeColor.LinearBlend(old[row][col], act[row][col], progress);
            rainColor = rain[col].get(row);
            rainColor = rainColor.LinearBlend(fadeColor, rainColor,
                                              rainColor.getAlpha());
            work[row][col].changeRgb(rainColor);
        }
    }
    if (phase >= frames) {
        copyMatrix(work, act); // take all color.Foreground's from act
        return 0;
    }
    return phase + 1;
}

//------------------------------------------------------------------------------

uint16_t Animation::animSnake() {
    uint16_t food = 40;

    if (phase == 1) {
        animationDelay = calcDelay(food);
        snake->begin(this);
    }

    if (!snake->drawSnake()) {
        return 0;
    }
    return phase + 1;
}

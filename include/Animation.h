/*
 * Animation.h
 *
 *  Created on: 15.01.2019
 *      Author: Andreas
 */

#ifndef ANIMATION_H_
#define ANIMATION_H_

#include "Animation_Blocks.h"
#include <NeoPixelBus.h>

// ********** getestet nur mit UHR_114 und LED_LAYOUT_REVERSE ***************
#define ANIMATION_ROW_START 0     /* Startindex der Reihe */
#define ANIMATION_ROWS 10         /* Anzahl der Reihen (Y-Richtung) */
#define ANIMATION_COLUMNS_START 0 /* Startindex der Spalte */
#define ANIMATION_COLUMNS 11      /* Anzahl der Spalten (X-Richtung) */

#if (defined(UHR_125) || defined(UHR_242))
#undef ANIMATION_ROWS
#define ANIMATION_ROWS 11 /* Anzahl der Reihen (Y-Richtung) */
#endif

// **************************************************************************
// #define USE_SERIAL Serial

// ###############################################################################
// compare operators are derived from RgbColor which compare R, G, B only
struct RgbaColor : RgbColor {
    RgbaColor() : RgbColor() {
        Alpha = 1.0;
        Foreground = false;
    };
    RgbaColor(RgbColor rgb) : RgbColor(rgb) {
        Alpha = 1.0;
        Foreground = false;
    }
    RgbaColor(uint8_t h) : RgbColor(h) {
        Alpha = 1.0;
        Foreground = false;
    }
    RgbaColor(RgbColor rgb, bool f) : RgbColor(rgb) {
        Alpha = 1.0;
        Foreground = f;
    }
    RgbaColor(HsbColor hsb, bool f) : RgbColor(hsb) {
        Alpha = 1.0;
        Foreground = f;
    }
    RgbaColor(uint8_t h, bool f) : RgbColor(h) {
        Alpha = 1.0;
        Foreground = f;
    }
    // ---------------------
    RgbaColor(RgbColor rgb, float a) : RgbColor(rgb) {
        Alpha = a;
        Foreground = false;
    }
    RgbaColor(uint8_t h, float a) : RgbColor(h) {
        Alpha = a;
        Foreground = false;
    }
    RgbaColor(uint8_t r, uint8_t g, uint8_t b, float a) : RgbColor(r, g, b) {
        Alpha = a;
        Foreground = false;
    }

    void changeRgb(RgbColor color) {
        R = color.R;
        G = color.G;
        B = color.B;
    }
    float Alpha;
    bool Foreground;
};
// ###############################################################################

class Animation {
public:
    Animation();
    virtual ~Animation();

    enum Ani {
        KEINE = 0,
        HOCH_ROLLEN,
        RUNTER_ROLLEN,
        LINKS_SCHIEBEN,
        RECHTS_SCHIEBEN,
        UEBERBLENDEN,
        LASER,
        MATRIX,
        BAELLE,
        FIRE,
        // only internaly used
        COUNTDOWN
    };

    enum Colorize { OFF = 1, WORDS, CHARACTERS };

    void begin();
    void loop(struct tm &tm);
    bool led_show_notify(bool flag, uint8_t minute);
    void blockgrafik(uint8_t rot, uint8_t gruen, uint8_t blau, BLOCKS num);
    void demoMode(uint8_t &_minute, uint8_t _sekunde);
    //------------------------------------------------------------------------------
protected:
    class Rain;
    class Ball;
    uint16_t phase = 0;
    bool silvester = false;
    bool matrixChanged = false;
    uint animationDelay = 100;
    Ani previousAnimType = KEINE;
    uint32_t nextActionTime = 0;
    uint8_t getIndex[ANIMATION_ROWS][ANIMATION_COLUMNS];

    RgbaColor _black = RgbaColor(0, 0, 0, 1.0);
    RgbaColor _white = RgbaColor(255, 255, 255, 1.0);
    RgbaColor _green = RgbaColor(0, 255, 0, 1.0);
    RgbaColor _red = RgbaColor(255, 0, 0, 1.0);
    RgbaColor _blue = RgbaColor(0, 0, 255, 1.0);
    RgbaColor _yellow = RgbaColor(255, 255, 0, 1.0);

    RgbaColor foreground = RgbaColor(0, true);
    RgbaColor background = RgbaColor(100, true);
    RgbaColor alt[ANIMATION_ROWS][ANIMATION_COLUMNS];
    RgbaColor neu[ANIMATION_ROWS][ANIMATION_COLUMNS];
    RgbaColor work[ANIMATION_ROWS][ANIMATION_COLUMNS];
    RgbaColor tmp[ANIMATION_ROWS][ANIMATION_COLUMNS];

    uint16_t reverse(uint16_t num, bool mirror);
    void set_pixel_for_char(uint8_t i, uint8_t h, uint8_t offset,
                            unsigned char unsigned_d1, HsbColor color);
    bool isIdle();
    Ani isSilvester(struct tm &tm);
    bool isColorization();
    void changeBrightness();
    float pseudoRandomHue();
    float pseudoRandomHue(bool init);
    void saveMatrix();
    void analyzeColors(RgbaColor (*dest)[ANIMATION_COLUMNS],
                       RgbaColor (*source)[ANIMATION_COLUMNS],
                       RgbaColor &foreground, RgbaColor &background);
    void copy2Stripe(RgbaColor (*source)[ANIMATION_COLUMNS]);
    void copy2Work(RgbaColor (*source)[ANIMATION_COLUMNS]);
    void copyBlock(RgbaColor color, uint block, bool fgbg, bool mirrored,
                   bool init = false);
    void fillMatrix(RgbaColor (*matrix)[ANIMATION_COLUMNS], RgbaColor color,
                    bool fg);
    uint calcDelay(uint phasen);
    uint animScrollDown(bool dirDown);
    uint animScrollRight(bool dirRight);
    uint animBalls();
    uint animFire();
    uint animFade();
    uint animLaser();
    uint animCountdown(struct tm &tm);
    uint32_t animMatrixRain();

    void animColorChange();
};

extern Animation animation;

#endif /* ANIMATION_H_ */

#pragma once

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
    Animation() = default;
    virtual ~Animation() = default;

    enum Animation_t {
        KEINE = 0,
        HOCH_ROLLEN = 1,
        RUNTER_ROLLEN = 2,
        LINKS_SCHIEBEN = 3,
        RECHTS_SCHIEBEN = 4,
        UEBERBLENDEN = 5,
        LASER = 6,
        MATRIX = 7,
        BAELLE = 8,
        FIRE = 9,
        // only internaly used
        COUNTDOWN = 10
    };

    enum Colorize { OFF = 1, WORDS = 2, CHARACTERS = 3 };

    void begin();
    void loop(struct tm &tm);
    bool led_show_notify(bool flag, uint8_t minute);
    void demoMode(uint8_t &_minute, uint8_t _sekunde);
    //------------------------------------------------------------------------------
protected:
    class Rain;
    class Ball;
    uint16_t phase = 0;
    bool silvester = false;
    bool matrixChanged = false;
    uint animationDelay = 100;
    Animation_t previousAnimType = KEINE;
    uint32_t nextActionTime = 0;

    RgbaColor _black = RgbaColor(0, 0, 0, 1.0);
    RgbaColor _white = RgbaColor(255, 255, 255, 1.0);
    RgbaColor _green = RgbaColor(0, 255, 0, 1.0);
    RgbaColor _red = RgbaColor(255, 0, 0, 1.0);
    RgbaColor _blue = RgbaColor(0, 0, 255, 1.0);
    RgbaColor _yellow = RgbaColor(255, 255, 0, 1.0);

    RgbaColor foreground = RgbaColor(0, true);
    RgbaColor background = RgbaColor(100, true);
    RgbaColor alt[MAX_ROWS][MAX_COL];
    RgbaColor neu[MAX_ROWS][MAX_COL];
    RgbaColor work[MAX_ROWS][MAX_COL];
    RgbaColor tmp[MAX_ROWS][MAX_COL];

    uint16_t reverse(uint16_t num, bool mirror);
    void set_pixel_for_char(uint8_t col, uint8_t row, uint8_t offsetCol,
                            uint8_t offsetRow, unsigned char unsigned_d1,
                            HsbColor color);
    inline bool isIdle() { return phase == 0; }
    Animation_t isSilvester(struct tm &tm);
    bool isColorization();
    void changeBrightness();
    float pseudoRandomHue();
    float pseudoRandomHue(bool init);
    void saveMatrix();
    void analyzeColors(RgbaColor (*dest)[MAX_COL], RgbaColor (*source)[MAX_COL],
                       RgbaColor &foreground, RgbaColor &background);
    void copy2Stripe(RgbaColor (*source)[MAX_COL]);
    void copy2Work(RgbaColor (*source)[MAX_COL]);
    void copyBlock(RgbaColor color, uint block, bool fgbg, bool mirrored,
                   bool init = false);
    void fillMatrix(RgbaColor (*matrix)[MAX_COL], RgbaColor color, bool fg);
    uint16_t calcDelay(uint16_t phasen);
    uint16_t animScrollDown(bool dirDown);
    uint16_t animScrollRight(bool dirRight);
    uint16_t animBalls();
    uint16_t animFire();
    uint16_t animFade();
    uint16_t animLaser();
    uint16_t animCountdown(struct tm &tm);
    uint16_t animMatrixRain();

    void animColorChange();
};

extern Animation animation;

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
        offset = -(MAX_ROWS - 1 - 3); // 3 = min deadtime
        speed = 1;
        stopping = false;
        speedlimit = random(1, 4);
        lifetime = random(4, MAX_ROWS);
        deadtime = random(3, MAX_ROWS); // min = 3, max deadtime
        //    speedlimit = 2;
        //    lifetime = 4;
        //    deadtime = 3;
        phase = 1;
        stopLine = MAX_ROWS - 1 - stop;
        stopPhase = frames - speedlimit * MAX_ROWS;
    }

    RgbaColor get(int r) {
        int row = (MAX_ROWS - 1 - r);
        // pro Bild laeuft row von (MAX_ROWS - 1) runter auf 0

        int pos = (row + offset) % (deadtime + lifetime);

        if (row == 0) { // letzte row faer dieses Bild
            if (phase++ == stopPhase) {
                stopTop = MAX_ROWS;
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
Rain rain[MAX_COL] = {
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
    static const int unten = ((MAX_ROWS - 1) << 8);
    static const int lastPos = ((MAX_ROWS - 3) << 8);
    ;
    int g, vy, y, end, delay;
    bool lastDown;
    RgbaColor colorForeground, colorBackground;
};
Ball baelle[MAX_BAELLE] = {
    Ball(),
};

// ###############################################################################

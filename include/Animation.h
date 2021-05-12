#pragma once

#include "icons.h"
#include <queue>

// ###############################################################################
// compare operators are derived from RgbColor which compare R, G, B only
// one hot coding for F_xxx
#define F_NULL 0
#define F_FOREGROUND 1
#define F_OVERLAY 2
struct RgbfColor : RgbColor {
    RgbfColor() : RgbColor() {
        R = 0;
        G = 0;
        B = 0;
        Flags = F_NULL;
    };
    RgbfColor(RgbColor rgb) : RgbColor(rgb) { Flags = F_NULL; }
    RgbfColor(uint8_t h) : RgbColor(h) { Flags = F_NULL; }
    RgbfColor(uint8_t r, uint8_t g, uint8_t b) : RgbColor(r, g, b) {
        Flags = F_NULL;
    }
    RgbfColor(RgbColor rgb, uint f) : RgbColor(rgb) { Flags = f; }
    RgbfColor(HsbColor hsb, uint f) : RgbColor(hsb) { Flags = f; }
    RgbfColor(uint8_t h, uint f) : RgbColor(h) { Flags = f; }

    void changeRgb(RgbColor color) {
        R = color.R;
        G = color.G;
        B = color.B;
    }
    void changeRgb(HsbColor hsb) {
        RgbColor color = RgbColor(hsb);
        R = color.R;
        G = color.G;
        B = color.B;
    }
    void setForeground(bool flag = true) {
        if (flag) {
            Flags |= F_FOREGROUND;
        } else {
            Flags &= ~F_FOREGROUND;
        }
    }
    bool isForeground() { return (Flags & F_FOREGROUND) > 0; }
    void setOverlay(bool flag = true) {
        if (flag) {
            Flags |= F_OVERLAY;
        } else {
            Flags &= ~F_OVERLAY;
        }
    }
    bool isOverlay() { return (Flags & F_OVERLAY) > 0; }
    void setFlags(uint8_t flags) { Flags = flags; }
    uint8_t getFlags() { return Flags; }

protected:
    uint Flags;
};
struct RgbaColor : RgbfColor {
    RgbaColor() : RgbfColor() { Alpha = 255; };
    RgbaColor(RgbColor rgb) : RgbfColor(rgb) { Alpha = 255; }
    RgbaColor(uint8_t h) : RgbfColor(h) { Alpha = 255; }
    RgbaColor(RgbColor rgb, uint f) : RgbfColor(rgb, f) { Alpha = 255; }
    RgbaColor(HsbColor hsb, uint f) : RgbfColor(hsb, f) { Alpha = 255; }
    RgbaColor(uint8_t h, uint f) : RgbfColor(h, f) { Alpha = 255; }
    // ---------------------
    RgbaColor(RgbColor rgb, float a) : RgbfColor(rgb) {
        Alpha = (uint8_t)(a * 255);
    }
    RgbaColor(uint8_t h, float a) : RgbfColor(h) { Alpha = (uint8_t)(a * 255); }
    RgbaColor(uint8_t r, uint8_t g, uint8_t b, float a) : RgbfColor(r, g, b) {
        Alpha = (uint8_t)(a * 255);
    }
    float getAlpha() { return Alpha / 255.0; }

protected:
    uint8_t Alpha;
};
// ###############################################################################
class Snake;
class Firework;
class Rain;
class Ball;
class Animation {
    friend class Snake;
    friend class Firework;

public:
    Animation(uint16 topLeftRow, uint16 topLeftCol, uint16 bottomRightRow,
              uint16 bottomRightCol);
    ~Animation();

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
        SCHLANGE = 10,
        // only internaly used
        RANDOM = 11,
        COUNTDOWN = 98,
        SILVESTER = 99
    };
    static const Animation_t animTypeFirst = HOCH_ROLLEN;
    static const Animation_t animTypeLast = SCHLANGE;

    enum Colorize { OFF = 1, WORDS = 2, CHARACTERS = 3 };

    void begin();
    void loop(struct tm &tm);
    bool led_show_notify(bool flag, uint8_t minute);
    void demoMode(uint8_t &_minute, uint8_t _sekunde);
    //------------------------------------------------------------------------------
protected:
    uint16_t phase = 0;
    bool matrixChanged = false;
    uint animationDelay = 100;
    Animation_t animType = KEINE;
    uint32_t nextActionTime = 0;
    uint8_t lastMinute = 100;

    RgbColor _white = RgbColor(255, 255, 255);
    RgbColor _green = RgbColor(0, 255, 0);
    RgbColor _red = RgbColor(255, 0, 0);
    RgbColor _blue = RgbColor(0, 0, 255);
    RgbColor _yellow = RgbColor(255, 255, 0);

    RgbColor foregroundMinute = RgbColor(0);
    RgbfColor foreground = RgbfColor(0, F_FOREGROUND);
    RgbfColor background = RgbfColor(100, F_NULL);

    Animation_t lastAnimType;
    uint8_t lastAnimDemo;
    uint8_t lastAnimDuration;
    uint8_t lastAnimColorize;

    uint8_t max_rows, max_cols, row_start, col_start;
    uint16 sizeofColumn;
    RgbfColor **old;
    RgbfColor **act;
    RgbfColor **work;
    Rain *rain;
    Snake *snake;
    Ball *balls;
    Firework *firework;

    uint16_t reverse(uint16_t num, bool mirror);
    void set_pixel_for_char(uint8_t col, uint8_t row, uint8_t offsetCol,
                            unsigned char unsigned_d1, HsbColor color);
    inline bool isIdle() { return phase == 0; }
    bool isSilvester(Animation_t &type, struct tm &tm, bool trigger);
    Animation_t getAnimationType(bool trigger);
    bool isColorization();
    bool changeBrightness();
    float pseudoRandomHue();
    float pseudoRandomHue(bool init);
    void colorize(RgbfColor **dest);
    void saveMatrix();
    void analyzeColors(RgbfColor **dest, RgbfColor **source,
                       RgbfColor &foreground, RgbfColor &background);
    void set_minutes(void);
    void copy2Stripe(RgbfColor **source);
    void copyMatrix(RgbfColor **dest, RgbfColor **source);
    void copyMatrixFlags(RgbfColor **dest, RgbfColor **source);
    void fillMatrix(RgbfColor **matrix, RgbfColor color);
    uint16_t calcDelay(uint16_t phasen);
    uint16_t animScrollDown(bool dirDown);
    uint16_t animScrollRight(bool dirRight);
    uint16_t animBalls();
    uint16_t animFire();
    uint16_t animFade();
    uint16_t animLaser();
    uint16_t animCountdown(struct tm &tm);
    uint16_t animMatrixRain();
    uint16_t animSnake();

    void animColorChange();
};

extern Animation *animation;

// ###############################################################################

class Rain {
public:
    Rain(){};
    Rain(uint8_t rows, uint8_t cols) {
        max_rows = rows;
        max_cols = cols;
    }
    virtual ~Rain(){};

    void begin(int frames, int stop, uint8_t helligkeit) {
        white = RgbaColor(helligkeit, 1.0f);
        // white.Lighten(helligkeit);
        green = RgbaColor(0, helligkeit, 0, 0.5);
        // green.Lighten(helligkeit);
        offset = -(max_rows - 1 - 3); // 3 = min deadtime
        speed = 1;
        stopping = false;
        speedlimit = random(1, 4);
        lifetime = random(4, max_rows);
        deadtime = random(3, max_rows); // min = 3, max deadtime
        //    speedlimit = 2;
        //    lifetime = 4;
        //    deadtime = 3;
        phase = 1;
        stopLine = max_rows - 1 - stop;
        stopPhase = frames - speedlimit * max_rows;
    }

    RgbaColor get(int r) {
        int row = (max_rows - 1 - r);
        // pro Bild laeuft row von (max_rows - 1) runter auf 0

        int pos = (row + offset) % (deadtime + lifetime);

        if (row == 0) { // letzte row faer dieses Bild
            if (phase++ == stopPhase) {
                stopTop = max_rows;
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
    uint8_t max_rows, max_cols;
    int speed, speedlimit, offset, lifetime, deadtime;
    int phase, frames, stopPhase, stopLine, stopTop, stopBottom;
    bool stopping;
    RgbaColor white = RgbaColor(255, 255, 255, 0.9);
    RgbaColor green = RgbaColor(0, 255, 0, 0.75);
    RgbaColor transparent = RgbaColor(0, 0, 0, 0.0);
};

// ###############################################################################

class Ball {
public:
    Ball(){};
    Ball(uint8_t max_rows) {
        unten = ((max_rows - 1) << 8);
        lastPos = ((max_rows - 3) << 8);
    }
    virtual ~Ball(){};

    void begin(int row, int column, RgbfColor foreground, RgbfColor background,
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
    RgbfColor color;

protected:
    int unten;
    int lastPos;
    int g, vy, y, end, delay;
    bool lastDown;
    RgbfColor colorForeground, colorBackground;
};

// ###############################################################################

#define SNAKE_LENGTH 8

class Snake {
public:
    Snake(uint8_t rows, uint8_t cols) {
        max_rows = rows;
        max_cols = cols;
        motions = new GoToPos[rows * 2 + 2];
    };
    ~Snake() { delete[] motions; };

protected:
    struct Coord {
        bool useAct;
        int8 row;
        int8 col;
    };
    struct GoToPos {
        bool useAct;
        int8 row;
        int8 min;
        int8 max;
    };
    uint8_t max_rows, max_cols;
    GoToPos *motions;
    Coord head;
    bool goRight;
    int index;
    std::queue<Coord> snake;
    RgbfColor **work;
    RgbfColor **old;
    RgbfColor **act;
    RgbfColor snakeColor;
    Animation *animation;

public:
    void begin(Animation *animation) {
        this->old = animation->old;
        this->act = animation->act;
        this->work = animation->work;
        this->animation = animation;
        HsbColor hsbColor = HsbColor(animation->foreground);
        hsbColor.H = fmodf(hsbColor.H + 0.5, 1.0);
        snakeColor = RgbfColor(hsbColor, true);
        goRight = true;
        head = {false, 0, -1};
        snake.empty();
        index = 0;
        getMotions(old, false, 0, 1);
        getMotions(act, true, max_rows - 1, -1);
        motions[index] = {false, -1, 0, 0}; // move out on left side
        motions[index + 1] = {false, -2, 0,
                              (int8)(max_cols - 1)}; // last on right side
        index = 0;
    }

    bool drawSnake() {
        Coord tail;
        bool moving = getNextMove(head);

        if ((moving && (snake.size() >= SNAKE_LENGTH)) ||
            (!moving && snake.size())) {
            tail = snake.front();
            snake.pop();
            work[tail.row][tail.col] =
                tail.useAct ? act[tail.row][tail.col] : animation->background;
        }
        if (moving) {
            snake.push(head);
            work[head.row][head.col] = snakeColor;
        }
        return snake.size();
    }

protected:
    void getMotions(RgbfColor **matrix, bool useAct, int8_t row, int8_t delta) {
        // order in motions: old -> down, act -> up
        int8_t left, right, rowCounter = max_rows;
        while (rowCounter-- > 0) {
            left = max_cols;
            right = 0;
            for (int col = 0; col < max_cols; col++) {
                if (matrix[row][col].isForeground()) {
                    // search for right most foreground
                    right = col;
                }
                if (matrix[row][max_cols - 1 - col].isForeground()) {
                    // search for left most foreground
                    left = max_cols - 1 - col;
                }
            }
            if (left <= right) {
                motions[index] = {useAct, row, left, right};
                if (useAct && (index > 0) && (row == motions[index - 1].row)) {
                    // dont't increment index when
                    // old and act have the same last row
                    motions[index - 1].useAct = true;
                    motions[index - 1].min = min(left, motions[index - 1].min);
                    motions[index - 1].max = max(right, motions[index - 1].max);
                } else {
                    index++;
                }
            }
            row += delta;
        }
    }

    bool getNextMove(Coord &head) {
        bool verticalMove = true;
        int8_t left = motions[index].min;
        int8_t right = motions[index].max;
        int8_t leftNext = motions[index + 1].min;
        int8_t rightNext = motions[index + 1].max;
        int8_t rowNext = motions[index + 1].row;
        if (head.row == motions[index].row) {
            if (goRight) {
                if (head.col < max(right, rightNext)) {
                    head.col++;
                    verticalMove = false;
                }
            } else {
                if (head.col > min(left, leftNext)) {
                    head.col--;
                    verticalMove = false;
                }
            }
        }
        if (verticalMove) {
            if (head.row != rowNext) {
                head.row = head.row < rowNext ? head.row + 1 : head.row - 1;
            }
            if (head.row == rowNext) {
                index++;
            }
            goRight = index % 2 ? false : true;
        }
        head.useAct = motions[index].useAct;
        return (head.row >= 0) && (head.col >= 0);
    }
};

// ###############################################################################

class Firework {
protected:
    RgbColor colors[3];
    Icons icons[3];
    bool mirrored;
    int maxLayer;
    uint8_t max_rows, max_cols;

public:
    Firework(uint8_t rows, uint8_t cols) {
        max_rows = rows;
        max_cols = cols;
    }

    // layers must be prepared in ascending order !!!
    void prepare(int layer, RgbColor &color, Icons icon, bool mirrored) {
        if (layer == 0) {
            icons[1] = static_cast<Icons>(0);
            icons[2] = static_cast<Icons>(0);
            this->mirrored = mirrored;
        }
        colors[layer] = color;
        icons[layer] = icon;
        maxLayer = layer;
    }

    bool getPixel(uint8_t r, uint8_t c, RgbColor &color) {
        // void Animation::copyBlock(RgbfColor color, uint block, bool fgbg,
        // bool mirrored,
        //                          bool init) {

        if ((r < 10) && (r < max_rows) && (c < 11) && (c < max_cols)) {
            uint16_t pixels = 0;
            for (int layer = 0; layer <= maxLayer; layer++) {
                if (icons[layer] != static_cast<Icons>(0)) {
                    pixels = animation->reverse(
                        pgm_read_word(&(grafik_11x10[icons[layer]][r])),
                        mirrored);
                    if (pixels & (1 << c)) {
                        color = colors[layer];
                        return true;
                    }
                }
            }
        }
        return false;
    }
};

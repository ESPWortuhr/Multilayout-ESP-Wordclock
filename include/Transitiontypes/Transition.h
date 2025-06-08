#pragma once

#include "font.h"
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
    RgbfColor(RgbColor rgb, uint32_t f) : RgbColor(rgb) { Flags = f; }
    RgbfColor(HsbColor hsb, uint32_t f) : RgbColor(hsb) { Flags = f; }
    RgbfColor(uint8_t h, uint32_t f) : RgbColor(h) { Flags = f; }

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
    uint32_t Flags;
};
struct RgbaColor : RgbfColor {
    RgbaColor() : RgbfColor() { Alpha = 255; };
    RgbaColor(RgbColor rgb) : RgbfColor(rgb) { Alpha = 255; }
    RgbaColor(uint8_t h) : RgbfColor(h) { Alpha = 255; }
    RgbaColor(RgbColor rgb, uint32_t f) : RgbfColor(rgb, f) { Alpha = 255; }
    RgbaColor(HsbColor hsb, uint32_t f) : RgbfColor(hsb, f) { Alpha = 255; }
    RgbaColor(uint8_t h, uint32_t f) : RgbfColor(h, f) { Alpha = 255; }
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

enum Transition_t {
    NO_TRANSITION = 0,
    ROLL_UP = 1,
    ROLL_DOWN = 2,
    SHIFT_LEFT = 3,
    SHIFT_RIGHT = 4,
    FADE = 5,
    LASER = 6,
    MATRIX_RAIN = 7,
    BALLS = 8,
    FIRE = 9,
    SNAKE = 10,
    COLORED = 12,
    // only internaly used
    RANDOM = 11,

    BIRTHDAY = 97,
    NEWYEAR_COUNTDOWN = 98,
    NEWYEAR_FIRE = 99
};

enum Colorize { OFF = 0, WORDS = 1, CHARACTERS = 2 };

class Snake;
class Firework;
class Rain;
class Ball;
class Transition {
    friend class Snake;
    friend class Firework;

public:
    static const Transition_t transitionTypeFirst = ROLL_UP;
    static const Transition_t transitionTypeLast = SNAKE;

protected:
    uint16_t phase = 0;
    bool matrixChanged = false;
    uint32_t transitionDelay = 100;
    Transition_t transitionType = NO_TRANSITION;
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

    Transition_t lastTransitionType;
    uint8_t lastTransitionDemo;
    uint8_t lastTransitionDuration;
    uint8_t lastTransitionColorize;

    uint8_t maxRows, maxCols;
    uint16_t sizeofColumn;
    RgbfColor **old;
    RgbfColor **act;
    RgbfColor **work;
    Rain *rain;
    Snake *snake;
    Ball *balls;
    Firework *firework;

protected:
    //------------------------------------------------------------------------------
    // Helper Functions
    //------------------------------------------------------------------------------
    uint16_t reverse(uint16_t num, bool mirror, uint8_t grafic_cols);
    void setPixelForChar(uint8_t col, uint8_t row, uint8_t offsetCol,
                         unsigned char unsigned_d1, HsbColor color,
                         fontSize font);
    inline bool isIdle() { return phase == 0; }
    bool isSpecialEvent(Transition_t &type, struct tm &tm, bool trigger);
    Transition_t getTransitionType(bool trigger);
    bool isColorization();
    bool changeBrightness();
    float pseudoRandomHue();
    float pseudoRandomHue(bool init);
    void colorize(RgbfColor **dest);
    void saveMatrix();
    void analyzeColors(RgbfColor **dest, RgbfColor **source,
                       RgbfColor &foreground, RgbfColor &background);
    void copy2Stripe(RgbfColor **source);
    void copyMatrix(RgbfColor **dest, RgbfColor **source);
    void copyMatrixFlags(RgbfColor **dest, RgbfColor **source);
    void fillMatrix(RgbfColor **matrix, RgbfColor color);
    uint16_t calcDelay(uint16_t phasen);
    bool changesInTransitionTypeDurationOrDemo();

    //------------------------------------------------------------------------------
    // Transitions
    //------------------------------------------------------------------------------
    uint16_t transitionScrollDown(bool dirDown);
    uint16_t transitionScrollRight(bool dirRight);
    uint16_t transitionBalls();
    uint16_t transitionFire();
    uint16_t transitionFade();
    void transitionColorChange();
    uint16_t transitionLaser();
    uint16_t transitionCountdown(struct tm &tm);
    uint16_t transitionMatrixRain();
    uint16_t transitionSnake();

public:
    Transition(uint8_t bottomRightRow, uint8_t bottomRightCol);
    ~Transition();

    //------------------------------------------------------------------------------
    // Loop Helper Functions
    //------------------------------------------------------------------------------
    void demoMode(uint8_t &_hour, uint8_t &_minute, uint8_t _second);
    void initTransitionStart();
    bool hasMinuteChanged();
    bool isOverwrittenByTransition(WordclockChanges flag, uint8_t minute);

    //------------------------------------------------------------------------------
    // Loop Functions
    //------------------------------------------------------------------------------
    void init();

    //------------------------------------------------------------------------------
    // Loop Functions
    //------------------------------------------------------------------------------
    void loop(struct tm &tm);
};

extern Transition *transition;

// ###############################################################################

class Rain {
public:
    Rain(){};
    Rain(uint8_t rows, uint8_t cols) {
        maxRows = rows;
        maxCols = cols;
    }
    virtual ~Rain(){};

    void begin(int32_t frames, int32_t stop, uint8_t helligkeit) {
        white = RgbaColor(helligkeit, 1.0f);
        // white.Lighten(helligkeit);
        green = RgbaColor(0, helligkeit, 0, 0.5);
        // green.Lighten(helligkeit);
        offset = -(maxRows - 1 - 3); // 3 = min deadtime
        speed = 1;
        stopping = false;
        speedlimit = random(1, 4);
        lifetime = random(4, maxRows);
        deadtime = random(3, maxRows); // min = 3, max deadtime
        //    speedlimit = 2;
        //    lifetime = 4;
        //    deadtime = 3;
        phase = 1;
        stopLine = maxRows - 1 - stop;
        stopPhase = frames - speedlimit * maxRows;
    }

    RgbaColor get(int32_t _row) {
        int32_t row = (maxRows - 1 - _row);
        // per image row runs from (maxRows - 1) down to 0

        int32_t pos = (row + offset) % (deadtime + lifetime);

        if (row == 0) { // last row for this image
            if (phase++ == stopPhase) {
                stopTop = maxRows;
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
    uint8_t maxRows, maxCols;
    int32_t speed, speedlimit, offset, lifetime, deadtime;
    int32_t phase, frames, stopPhase, stopLine, stopTop, stopBottom;
    bool stopping;
    RgbaColor white = RgbaColor(255, 255, 255, 0.9);
    RgbaColor green = RgbaColor(0, 255, 0, 0.75);
    RgbaColor transparent = RgbaColor(0, 0, 0, 0.0);
};

// ###############################################################################

class Ball {
public:
    Ball(){};
    Ball(uint8_t maxRows) {
        unten = ((maxRows - 1) << 8);
        lastPos = ((maxRows - 3) << 8);
    }
    virtual ~Ball(){};

    void begin(int32_t _row, int32_t _col, RgbfColor foreground,
               RgbfColor background, int32_t delay) {
        this->delay = delay;
        y = _row << 8; // increase precision
        row = _row;
        col = _col;
        vy = 0;
        colorForeground = foreground;
        colorBackground = background;
        g = 9810 / G.transitionDuration;
        end = (y == unten) ? 1 : 0;
        lastDown = false;
    }

    // ###############################################################################
    // x =  0, y = 0 -> left down
    // x = 10, y = 9 -> right down
    // v positiv    -> down
    int32_t move(int32_t timedelta) {
        if (!end) {
            delay -= timedelta;
            if (delay <= 0) {
                int32_t _vy = vy;
                y += (((g * timedelta) / 1000) * timedelta) / 2000 +
                     (vy * timedelta) / 1000;
                vy += (g * timedelta) / 1000;
                if ((vy > 0) && (y >= unten)) { // lower turning point
                    y = unten;                  // - (y - unten);
                    vy = -(vy * 6) / 10;        // Destroy energy
                    if (lastDown) {
                        end = 1;
                    }
                }
                lastDown |= (_vy < 0) && (vy >= 0) &&
                            (y > lastPos); // upper turning point
                row = y >> 8;
            }
            color = end ? colorBackground : colorForeground;
        }
        return end;
    }

public:
    int32_t row, col; // after calling move() row & col contain new actual val
    RgbfColor color;

protected:
    int32_t unten;
    int32_t lastPos;
    int32_t g, vy, y, end, delay;
    bool lastDown;
    RgbfColor colorForeground, colorBackground;
};

// ###############################################################################

#define SNAKE_LENGTH 8

class Snake {
public:
    Snake(uint8_t rows, uint8_t cols) {
        maxRows = rows;
        maxCols = cols;
        motions = new GoToPos[rows * 2 + 2];
    };
    ~Snake() { delete[] motions; };

protected:
    struct Coord {
        bool useAct;
        int8_t row;
        int8_t col;
    };
    struct GoToPos {
        bool useAct;
        int8_t row;
        int8_t min;
        int8_t max;
    };
    uint8_t maxRows, maxCols;
    GoToPos *motions;
    Coord head;
    bool goRight;
    int32_t index;
    std::queue<Coord> snake;
    RgbfColor **work;
    RgbfColor **old;
    RgbfColor **act;
    RgbfColor snakeColor;
    Transition *transition;

public:
    void begin(Transition *transition) {
        this->old = transition->old;
        this->act = transition->act;
        this->work = transition->work;
        this->transition = transition;
        HsbColor hsbColor = HsbColor(transition->foreground);
        hsbColor.H = fmodf(hsbColor.H + 0.5, 1.0);
        snakeColor = RgbfColor(hsbColor, true);
        goRight = true;
        head = {false, 0, -1};
        index = 0;
        getMotions(old, false, 0, 1);
        getMotions(act, true, maxRows - 1, -1);
        motions[index] = {false, -1, 0, 0}; // move out on left side
        motions[index + 1] = {false, -2, 0,
                              (int8_t)(maxCols - 1)}; // last on right side
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
                tail.useAct ? act[tail.row][tail.col] : transition->background;
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
        int8_t left, right, rowCounter = maxRows;
        while (rowCounter-- > 0) {
            left = maxCols;
            right = 0;
            for (int32_t col = 0; col < maxCols; col++) {
                if (matrix[row][col].isForeground()) {
                    // search for right most foreground
                    right = col;
                }
                if (matrix[row][maxCols - 1 - col].isForeground()) {
                    // search for left most foreground
                    left = maxCols - 1 - col;
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
    int32_t maxLayer;
    uint8_t maxRows, maxCols;

public:
    Firework(uint8_t rows, uint8_t cols) {
        maxRows = rows;
        maxCols = cols;
    }

    // layers must be prepared in ascending order !!!
    void prepare(int32_t layer, RgbColor &color, Icons icon, bool mirrored) {
        if (layer == 0) {
            icons[1] = static_cast<Icons>(0);
            icons[2] = static_cast<Icons>(0);
            this->mirrored = mirrored;
        }
        colors[layer] = color;
        icons[layer] = icon;
        maxLayer = layer;
    }

    bool getPixel(uint8_t row, uint8_t col, RgbColor &color) {
        // void Transition::copyBlock(RgbfColor color, uint32_t block, bool
        // fgbg, bool mirrored,
        //                          bool init) {

        if ((row < 10) && (row < maxRows) && (col < 11) && (col < maxCols)) {
            uint16_t pixels = 0;
            for (int32_t layer = 0; layer <= maxLayer; layer++) {
                if (icons[layer] != static_cast<Icons>(0)) {
                    if (usedUhrType->colsWordMatrix() < 11 ||
                        usedUhrType->rowsWordMatrix() < 10) {
                        pixels = transition->reverse(
                            pgm_read_word(&(grafik_8x8[icons[layer]][row])),
                            mirrored, GRAFIK_8X8_COLS);
                    } else {
                        pixels = transition->reverse(
                            pgm_read_word(&(grafik_11x10[icons[layer]][row])),
                            mirrored, GRAFIK_11X10_COLS);
                    }
                    if (pixels & (1 << col)) {
                        color = colors[layer];
                        return true;
                    }
                }
            }
        }
        return false;
    }
};

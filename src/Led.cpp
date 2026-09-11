#include "Led.h"

#include "Config.h"
#include "Font.h"
#include "LedStripInterface.h"
#include "NeoMultiFeature.hpp"
#include "Symbols.h"
#include "WordClockState.h"
#include "WordClockTypes/ClockType.hpp"
#include <Arduino.h>

// Led itself addresses the front through the global usedClockType.
extern ClockType *usedClockType;

#include "Render/ColorStage.h"
#include "Render/RenderPipeline.h"

extern Led led;
extern RenderPipeline renderPipeline;
extern LedStripInterface *activeLedStrip;

namespace {
const uint8_t whiteAdjR[3] = {255, 255, 255};
const uint8_t whiteAdjG[3] = {180, 215, 107};
const uint8_t whiteAdjB[3] = {107, 177, 253};

RgbwColor convertRgbToRgbw(RgbColor light, WhiteType wType) {
    RgbwColor returnColor;
    uint8_t wIdx = static_cast<uint8_t>(wType);

    // Use fast 32-bit integer math! (255 * 255 = 65025, easily fits in
    // uint32_t)
    uint32_t whiteValueR = (light.R * 255) / whiteAdjR[wIdx];
    uint32_t whiteValueG = (light.G * 255) / whiteAdjG[wIdx];
    uint32_t whiteValueB = (light.B * 255) / whiteAdjB[wIdx];

    // Find the minimum using integer comparison
    uint32_t minValW = min(whiteValueR, min(whiteValueG, whiteValueB));
    returnColor.W = (minValW <= 255 ? (uint8_t)minValW : 255);

    // Integer math for the return channels
    returnColor.R =
        static_cast<uint8_t>(light.R - (minValW * whiteAdjR[wIdx]) / 255);
    returnColor.G =
        static_cast<uint8_t>(light.G - (minValW * whiteAdjG[wIdx]) / 255);
    returnColor.B =
        static_cast<uint8_t>(light.B - (minValW * whiteAdjB[wIdx]) / 255);

    return returnColor;
}
} // namespace

//------------------------------------------------------------------------------
// Helper Functions
//------------------------------------------------------------------------------

/* Based on https://graphics.stanford.edu/~seander/bithacks.html */

uint8_t Led::reverse8BitOrder(uint8_t x) {
    x = ((x >> 1) & 0x55) | ((x << 1) & 0xAA);
    x = ((x >> 2) & 0x33) | ((x << 2) & 0xCC);
    return (x >> 4) | (x << 4);
}

//------------------------------------------------------------------------------

/* Based on https://graphics.stanford.edu/~seander/bithacks.html */

uint32_t Led::reverse32BitOrder(uint32_t x) {
    x = (((x & 0xaaaaaaaa) >> 1) | ((x & 0x55555555) << 1));
    x = (((x & 0xcccccccc) >> 2) | ((x & 0x33333333) << 2));
    x = (((x & 0xf0f0f0f0) >> 4) | ((x & 0x0f0f0f0f) << 4));
    x = (((x & 0xff00ff00) >> 8) | ((x & 0x00ff00ff) << 8));
    return ((x >> 16) | (x << 16));
}

//------------------------------------------------------------------------------

Led::NumberFont Led::numberFontFor(uint8_t cols, uint8_t rows) {
    NumberFont numberFont;
    numberFont.font = normalSizeASCII;
    numberFont.width = pgm_read_byte(&(fontWidth[numberFont.font]));
    numberFont.height = pgm_read_byte(&(fontHeight[numberFont.font]));
    if (cols < numberFont.width * 2 + 1 || rows < numberFont.height) {
        numberFont.font = smallSizeNumbers;
        numberFont.width = pgm_read_byte(&(fontWidth[numberFont.font]));
        numberFont.height = pgm_read_byte(&(fontHeight[numberFont.font]));
    }
    return numberFont;
}

//------------------------------------------------------------------------------

void Led::checkIfHueIsOutOfBound(uint16_t &hue) {
    if (hue >= 360) {
        hue = 0;
    }
}

//------------------------------------------------------------------------------

bool Led::getCharCol(fontSize font, uint8_t col, uint8_t row,
                     unsigned char uChar) {
    switch (font) {
    case normalSizeASCII:
        return pgm_read_byte(&(font_7x5[uChar][col])) & (1u << row);
        break;

    case smallSizeNumbers:
        return pgm_read_byte(&(font_5x3[uChar][col])) & (1u << row);
        break;

    default:
        return false;
        break;
    }
}

//------------------------------------------------------------------------------

void Led::applyMirroringAndReverseIfDefined() {
    if (G.layoutVariant[ReverseMinDirection]) {
        mirrorMinuteArrayVertical();
    }
    if (G.layoutVariant[MirrorVertical]) {
        mirrorFrontMatrixVertical();
    }
    if (G.layoutVariant[MirrorHorizontal]) {
        mirrorFrontMatrixHorizontal();
    }
}

//------------------------------------------------------------------------------

void Led::resetFrontMatrixBuffer() {
    for (uint8_t i = 0; i < usedClockType->rowsWordMatrix(); i++) {
        frontMatrix[i] = 0;
    }
    ClockType::resetWordIds();
}

//------------------------------------------------------------------------------
// Brightness Functions
//------------------------------------------------------------------------------

float Led::setBrightnessAuto(float val) {
    // G.hh contains time-dependent brightness values in %.
    return (val * ledGain) / 100.f;
}

//------------------------------------------------------------------------------

uint8_t Led::getCurrentManualBrightnessSetting() {
    // Set Brighness hour dependent
    if (_hour < 6) {
        return G.h24;
    } else if (_hour < 8) {
        return G.h6;
    } else if (_hour < 12) {
        return G.h8;
    } else if (_hour < 16) {
        return G.h12;
    } else if (_hour < 18) {
        return G.h16;
    } else if (_hour < 20) {
        return G.h18;
    } else if (_hour < 22) {
        return G.h20;
    } else if (_hour < 24) {
        return G.h22;
    } else {
        return DEFAULT_BRIGHTNESS;
    }
}

//------------------------------------------------------------------------------

HsbColor Led::getColorbyPositionWithAppliedBrightness(ColorPosition position) {
    HsbColor color = G.color[position];

    if (G.autoBrightEnabled == 1) {
        color.B = setBrightnessAuto(color.B);
    } else {
        color.B *= getCurrentManualBrightnessSetting() / 100.f;
    }
    return color;
}

//------------------------------------------------------------------------------
// Manipulate Functions
//------------------------------------------------------------------------------

void Led::mirrorMinuteArrayVertical() {
    minuteArray = reverse8BitOrder(minuteArray);
    minuteArray >>= 4;
}

//------------------------------------------------------------------------------

void Led::mirrorFrontMatrixVertical() {
    const uint8_t cols = usedClockType->colsWordMatrix();
    for (uint8_t row = 0; row < usedClockType->rowsWordMatrix(); row++) {
        frontMatrix[row] = reverse32BitOrder(frontMatrix[row]);
        frontMatrix[row] >>= (32 - cols);

        // Keep the word tags aligned with the cells they describe.
        if (cols <= MAX_COL_SIZE) {
            for (uint8_t col = 0; col < cols / 2; col++) {
                const uint8_t tmp = frontWordId[row][col];
                frontWordId[row][col] = frontWordId[row][cols - 1 - col];
                frontWordId[row][cols - 1 - col] = tmp;
            }
        }
    }
}

//------------------------------------------------------------------------------

void Led::mirrorFrontMatrixHorizontal() {
    const uint8_t rows = usedClockType->rowsWordMatrix();
    uint32_t tempMatrix[MAX_ROW_SIZE] = {0};
    memcpy(&tempMatrix, &frontMatrix, sizeof tempMatrix);
    for (uint8_t row = 0; row < rows; row++) {
        frontMatrix[row] = tempMatrix[rows - row - 1];
    }

    // Keep the word tags aligned with the cells they describe.
    for (uint8_t row = 0; row < rows / 2; row++) {
        for (uint8_t col = 0; col < MAX_COL_SIZE; col++) {
            const uint8_t tmp = frontWordId[row][col];
            frontWordId[row][col] = frontWordId[rows - 1 - row][col];
            frontWordId[rows - 1 - row][col] = tmp;
        }
    }
}

//------------------------------------------------------------------------------

void Led::shiftColumnToRight() {
    for (uint8_t row = 0; row < MAX_ROW_SIZE; row++) {
        frontMatrix[row] <<= 1;
    }
}

//------------------------------------------------------------------------------
// Pixel set Functions
//------------------------------------------------------------------------------

void Led::setState(const bool newState) {
    static float storedBrightness[ColorPositionCount];
    static bool hasStoredBrightness = false;

    if (newState == getState()) {
        return;
    }

    if (newState) {
        for (uint8_t i = 0; i < ColorPositionCount; i++) {
            G.color[i].B = hasStoredBrightness ? storedBrightness[i] : 1.f;
        }
    } else {
        for (uint8_t i = 0; i < ColorPositionCount; i++) {
            storedBrightness[i] = G.color[i].B;
            G.color[i].B = 0.f;
        }
        hasStoredBrightness = true;

        clear();
        show();
    }

    parametersChanged = true;
}

//------------------------------------------------------------------------------

void Led::setPixel(uint16_t ledIndex, HsbColor color) {
    if (G.Colortype == Grbw) {
        activeLedStrip->setRgbwPixel(
            ledIndex, convertRgbToRgbw(RgbColor(color), G.wType));
    } else {
        activeLedStrip->setRgbPixel(ledIndex, color);
    }
}

//------------------------------------------------------------------------------

void Led::setPixel(uint8_t row, uint8_t col, HsbColor color) {
    const uint8_t numLEDsPerLetter = getLedsPerLetter(G.buildTypeDef);
    uint16_t ledIndex = usedClockType->getFrontMatrixIndex(row, col);
    for (int i = 0; i < numLEDsPerLetter; i++) {
        setPixel(ledIndex * numLEDsPerLetter + i, color);
    }
}

//------------------------------------------------------------------------------

/*
 * Every mode except the two rainbows paints its lit pixels here - the seconds,
 * the digital clock, the scrolling text, the plain colour - so this is where
 * they pick up the gradient. The word clock reaches the strip through the
 * render pipeline instead and is coloured there.
 *
 * Only the foreground ramps: the background is one colour by definition, and
 * inverting the ramp behind the letters would be noise, not a gradient.
 */

void Led::setbyFrontMatrix(ColorPosition colorPosition,
                           bool applyMirrorAndReverse) {
    if (applyMirrorAndReverse) {
        applyMirroringAndReverseIfDefined();
    }
    const HsbColor displayedColor =
        getColorbyPositionWithAppliedBrightness(colorPosition);
    const bool ramp =
        (colorPosition == Foreground) && colorStage.foregroundIsGradient();
    const HsbColor rampEnd =
        ramp ? getColorbyPositionWithAppliedBrightness(GradientEnd)
             : displayedColor;

    const uint8_t rows = usedClockType->rowsWordMatrix();
    for (uint8_t row = 0; row < rows; row++) {
        const HsbColor rowColor =
            ramp ? gradientColorAt(displayedColor, rampEnd, row, rows)
                 : displayedColor;

        for (uint8_t col = 0; col < usedClockType->colsWordMatrix(); col++) {
            bool boolSetPixel = usedClockType->getFrontMatrixPixel(row, col);
            if (colorPosition == Background) {
                boolSetPixel = !boolSetPixel;
            }

            if (boolSetPixel) {
                setPixel(row, col, rowColor);
            } else if (colorPosition != Background) {
                clearPixel(row, col);
            }
        }
    }
}

//------------------------------------------------------------------------------

/*
 * Callers that bring their own colours rather than reading them from the
 * configuration - the symbol mode dims to effectBri, the firework picks a hue
 * per rocket. Passing the same colour twice paints it flat.
 */

void Led::setbyFrontMatrixGradient(HsbColor from, HsbColor to,
                                   bool applyMirrorAndReverse) {
    if (applyMirrorAndReverse) {
        applyMirroringAndReverseIfDefined();
    }
    const bool ramp = (from.H != to.H) || (from.S != to.S) || (from.B != to.B);

    const uint8_t rows = usedClockType->rowsWordMatrix();
    for (uint8_t row = 0; row < rows; row++) {
        const HsbColor rowColor =
            ramp ? gradientColorAt(from, to, row, rows) : from;

        for (uint8_t col = 0; col < usedClockType->colsWordMatrix(); col++) {
            if (usedClockType->getFrontMatrixPixel(row, col)) {
                setPixel(row, col, rowColor);
            }
        }
    }
}

//------------------------------------------------------------------------------

void Led::setbyColorMatrix(const ColorMatrix &matrix) {
    for (uint8_t row = 0; row < matrix.rows(); row++) {
        for (uint8_t col = 0; col < matrix.cols(); col++) {
            const RgbfColor &cell = matrix[row][col];
            setPixel(row, col, HsbColor{RgbColor(cell.R, cell.G, cell.B)});
        }
    }
}

//------------------------------------------------------------------------------

void Led::setbyMinuteArray(ColorPosition colorPosition) {
    if (!usedClockType->hasMinuteLeds()) {
        return;
    }

    HsbColor displayedColor =
        getColorbyPositionWithAppliedBrightness(colorPosition);

    /* Set minutes According to minute byte */
    for (uint8_t i = 0; i < 4; i++) {
        /* Bitwise check whether Pixel bit is set */
        if ((minuteArray >> i) & 1U) {
            setPixel(minutePixelArray[i], displayedColor);
        }
    }
}

//------------------------------------------------------------------------------

void Led::setbySecondArray(ColorPosition colorPosition) {

    HsbColor displayedColor =
        getColorbyPositionWithAppliedBrightness(colorPosition);

    const uint8_t offesetSecondsFrame =
        (usedClockType->numPixelsFrameMatrix() / 8);

    for (uint8_t i = 0; i < usedClockType->numPixelsFrameMatrix(); i++) {
        if (frameArray[i]) {
            if (i <
                usedClockType->numPixelsFrameMatrix() - offesetSecondsFrame) {
                setPixel(usedClockType->getFrameMatrixIndex(i) +
                             offesetSecondsFrame,
                         displayedColor);
            } else {
                setPixel(usedClockType->getFrameMatrixIndex(i) -
                             usedClockType->numPixelsFrameMatrix() +
                             offesetSecondsFrame,
                         displayedColor);
            }
        }
    }
}

//------------------------------------------------------------------------------

void Led::drawBitmapSymbol(BitmapSymbol symbolNum) {
    resetFrontMatrixBuffer();
    if (usedClockType->colsWordMatrix() < 11 ||
        usedClockType->rowsWordMatrix() < 10) {
        uint8_t offsetCol =
            (usedClockType->colsWordMatrix() - SYMBOL_8X8_COLS) / 2;

        for (uint8_t row = 0; row < SYMBOL_8X8_ROWS; row++) {

            uint16_t rowData = pgm_read_word(&(symbol_8x8[symbolNum][row]));

            for (uint8_t col = 0; col < SYMBOL_8X8_COLS; col++) {
                usedClockType->setFrontMatrixPixel(
                    row, col + offsetCol,
                    (rowData & (1 << (SYMBOL_8X8_COLS - col - 1))));
            }
        }
    } else {
        uint8_t offsetCol =
            (usedClockType->colsWordMatrix() - SYMBOL_11X10_COLS) / 2;

        for (uint8_t row = 0; row < SYMBOL_11X10_ROWS; row++) {

            uint16_t rowData = pgm_read_word(&(symbol_11x10[symbolNum][row]));

            for (uint8_t col = 0; col < SYMBOL_11X10_COLS; col++) {
                usedClockType->setFrontMatrixPixel(
                    row, col + offsetCol,
                    (rowData & (1 << (SYMBOL_11X10_COLS - col - 1))));
            }
        }
    }
}

//------------------------------------------------------------------------------

void Led::setBitmapSymbol(BitmapSymbol symbolNum, HsbColor color) {
    setBitmapSymbol(symbolNum, color, color);
}

//------------------------------------------------------------------------------

void Led::setBitmapSymbol(BitmapSymbol symbolNum, HsbColor from, HsbColor to) {
    drawBitmapSymbol(symbolNum);
    setbyFrontMatrixGradient(from, to);
    show();
}

//------------------------------------------------------------------------------

void Led::setSingle(uint8_t wait) {
    uint16_t hue;
    uint16_t counterLed = 0;
    uint16_t numPixelsWordMatrix =
        usedClockType->rowsWordMatrix() * usedClockType->colsWordMatrix();

    for (uint8_t row = 0; row < usedClockType->rowsWordMatrix(); row++) {
        for (uint8_t col = 0; col < usedClockType->colsWordMatrix(); col++) {
            hue = 360.f * counterLed / numPixelsWordMatrix;
            checkIfHueIsOutOfBound(hue);

            clear();
            if (row % 2 != 0) {
                setPixel(row, usedClockType->colsWordMatrix() - 1 - col,
                         HsbColor(hue / 360.f, 1.f, G.effectBri / 100.f));
            } else {
                setPixel(row, col,
                         HsbColor(hue / 360.f, 1.f, G.effectBri / 100.f));
            }
            show();
            delay(wait);
            counterLed++;
        }
    }
}

//------------------------------------------------------------------------------

void Led::setPixelForChar(uint8_t col, uint8_t row, uint8_t offsetCol,
                          uint8_t offsetRow, unsigned char unsigned_d1,
                          fontSize font) {

    if (getCharCol(font, col, row, unsigned_d1)) {
        usedClockType->setFrontMatrixPixel(row + offsetRow, col + offsetCol);
    }
}

//------------------------------------------------------------------------------

void Led::set(WordclockChanges changed) {
    setbyFrontMatrix(Foreground);
    setbyFrontMatrix(Background, false);

    const bool owns = renderPipeline.ownsDisplay();
    const bool minuteChanged = owns ? false : renderPipeline.hasMinuteChanged();
    const DisplayAction action = decideDisplayAction(
        changed, owns, renderPipeline.animates(), minuteChanged);

    if (action.drawMinutesAndFrame) {
        if (G.minuteVariant != MinuteVariant::Off) {
            setbyMinuteArray(colorStage.minutePosition());
        }

        if (G.secondVariant != SecondVariant::Off) {
            setbySecondArray(Frame);
        }
    }

    renderPipeline.applyDisplayAction(action, _minute);

    if (action.showFromLed) {
        show();
    }
}

//------------------------------------------------------------------------------
// Pixel get Functions
//------------------------------------------------------------------------------

bool Led::getState() {
    for (uint8_t i = 0; i < ColorPositionCount; i++) {
        if (G.color[i].B > 0) {
            return true;
        }
    }
    return false;
}

//------------------------------------------------------------------------------
// Pixel Clear Functions
//------------------------------------------------------------------------------

void Led::clearPixel(uint8_t row, uint8_t col) {
    const uint8_t numLEDsPerLetter = getLedsPerLetter(G.buildTypeDef);
    uint16_t ledIndex = usedClockType->getFrontMatrixIndex(row, col);
    for (int i = 0; i < numLEDsPerLetter; i++) {
        clearPixel(ledIndex * numLEDsPerLetter + i);
    }
}

//------------------------------------------------------------------------------

void Led::clearPixel(uint16_t i) { activeLedStrip->clearPixel(i); }

//------------------------------------------------------------------------------

void Led::clearClock() {
    for (uint8_t row = 0; row < usedClockType->rowsWordMatrix(); row++) {
        for (uint8_t col = 0; col < usedClockType->colsWordMatrix(); col++) {
            usedClockType->setFrontMatrixPixel(row, col, false);
            clearPixel(row, col);
        }
    }
}

//------------------------------------------------------------------------------

void Led::clearRow(uint8_t row) {
    for (uint8_t col = 0; col < usedClockType->colsWordMatrix(); col++) {
        usedClockType->setFrontMatrixPixel(row, col, false);
        clearPixel(row, col);
    }
}

//------------------------------------------------------------------------------

void Led::clearMinArray() {
    if (usedClockType->hasMinuteLeds()) {
        for (uint16_t i = minutePixelArray[0]; i <= minutePixelArray[3]; i++) {
            clearPixel(i);
        }
    }
    minuteArray = 0;
}

//------------------------------------------------------------------------------

void Led::clearFrontExeptofFontspace(uint8_t offsetRow) {
    for (uint8_t i = 0; i < offsetRow; i++) {
        clearRow(i);
    }

    uint8_t height = pgm_read_byte(&(fontHeight[normalSizeASCII]));

    for (uint8_t i = usedClockType->rowsWordMatrix(); i > offsetRow + height;
         i--) {
        clearRow(i - 1);
    }
}

//------------------------------------------------------------------------------

void Led::clearFrame() {
    for (uint16_t i = 0; i < usedClockType->numPixelsFrameMatrix(); i++) {
        clearPixel(usedClockType->getFrameMatrixIndex(i));
    }
}

//------------------------------------------------------------------------------

void Led::clear() {
    clearClock();
    clearFrame();
    clearMinArray();
}

//------------------------------------------------------------------------------
// Show Functions
//------------------------------------------------------------------------------

void Led::showNumbers(const char d1, const char d2) {
    clearClock();

    const NumberFont numberFont = numberFontFor(
        usedClockType->colsWordMatrix(), usedClockType->rowsWordMatrix());
    const unsigned char unsigned_d1 = numberFont.glyph(d1);
    const unsigned char unsigned_d2 = numberFont.glyph(d2);

    uint8_t offsetRow =
        (usedClockType->rowsWordMatrix() - numberFont.height) / 2;
    bool isSingleDigit = (d1 == ' ' || d1 == '0');
    uint8_t offsetLetter0, offsetLetter1, offsetCenter;

    if (isSingleDigit) {
        offsetCenter = (usedClockType->colsWordMatrix() - numberFont.width) / 2;
    } else {
        offsetLetter0 = usedClockType->colsWordMatrix() / 2 - numberFont.width;
        offsetLetter1 = usedClockType->colsWordMatrix() / 2 + 1;

        if (usedClockType->has24HourLayout()) {
            offsetLetter0 = 3;
            offsetLetter1 = numberFont.width + 4;
        }
    }

    for (uint8_t col = 0; col < numberFont.width; col++) {
        for (uint8_t row = 0; row < numberFont.height; row++) {

            if (isSingleDigit) {
                setPixelForChar(col, row, offsetCenter, offsetRow, unsigned_d2,
                                numberFont.font);
            } else {
                setPixelForChar(col, row, offsetLetter0, offsetRow, unsigned_d1,
                                numberFont.font);
                setPixelForChar(col, row, offsetLetter1, offsetRow, unsigned_d2,
                                numberFont.font);
            }
        }
    }

    mirrorFrontMatrixVertical(); // Needed for correct displaying of Chars
    setbyFrontMatrix(Foreground);
    show();
}

//------------------------------------------------------------------------------

fontSize Led::determineFontSize() {

    if (G.clockTypeDef == Ger16x18) {
        return normalSizeASCII;
    }
    return smallSizeNumbers;
}

//------------------------------------------------------------------------------

void Led::setupDigitalClock(fontSize &usedFontSize, uint8_t &offsetLetterH0,
                            uint8_t &offsetLetterH1, uint8_t &offsetLetterMin0,
                            uint8_t &offsetLetterMin1, uint8_t &offsetRow0,
                            uint8_t &offsetRow1) {

    uint8_t letterSpacing = 1;
    if (usedClockType->rowsWordMatrix() >=
        pgm_read_byte(&(fontHeight[usedFontSize])) * 2) {
        letterSpacing++;
    }

    // 1st Row of letters vertical Offset
    offsetLetterH0 = 0;
    offsetLetterH1 = offsetLetterH0 +
                     pgm_read_byte(&(fontWidth[usedFontSize])) + letterSpacing;

    // 2nd Row of letters vertical Offset
    offsetLetterMin1 = usedClockType->colsWordMatrix() -
                       pgm_read_byte(&(fontWidth[usedFontSize]));
    offsetLetterMin0 = offsetLetterMin1 -
                       pgm_read_byte(&(fontWidth[usedFontSize])) -
                       letterSpacing;

    // 1st Row of letters horizontal Offset
    offsetRow0 = 0;
    // 2nd Row of letters horizontal Offset
    offsetRow1 = usedClockType->rowsWordMatrix() -
                 pgm_read_byte(&(fontHeight[usedFontSize]));
}

//------------------------------------------------------------------------------

void Led::toggleDigitalClockSecond(const fontSize &usedFontSize,
                                   const uint8_t &offsetRow1,
                                   const uint8_t &offsetMin0) {
    if (!(_second % 2)) {
        return;
    }

    // The separator sits in the gap left of the minutes block. On narrow
    // layouts (e.g. 8 columns) there is no such gap, so the column would
    // become negative -- skip the separator instead of drawing out of bounds.
    const int8_t distanceToMinutes = (usedFontSize == normalSizeASCII) ? 3 : 2;
    const int column = static_cast<int>(offsetMin0) - distanceToMinutes;
    if (column < 0) {
        return;
    }

    const int8_t upperRowOffset = (usedFontSize == normalSizeASCII) ? 2 : 1;
    const int8_t lowerRowOffset = (usedFontSize == normalSizeASCII) ? 4 : 3;
    usedClockType->setFrontMatrixPixel(offsetRow1 + upperRowOffset, column);
    usedClockType->setFrontMatrixPixel(offsetRow1 + lowerRowOffset, column);
}

//------------------------------------------------------------------------------

void Led::showDigitalClock(const char min1, const char min0, const char h1,
                           const char h0) {

    uint8_t offsetLetterH0, offsetLetterH1, offsetLetterMin0, offsetLetterMin1,
        offsetRow0, offsetRow1;

    resetFrontMatrixBuffer();

    fontSize usedFontSize = determineFontSize();

    // The offsets only depend on the layout and the font size, so recomputing
    // them on every call is cheap. Caching them in static variables used to
    // leave them at zero (or at values of a previously selected layout)
    // whenever the first call came in without parametersChanged being set.
    setupDigitalClock(usedFontSize, offsetLetterH0, offsetLetterH1,
                      offsetLetterMin0, offsetLetterMin1, offsetRow0,
                      offsetRow1);

    toggleDigitalClockSecond(usedFontSize, offsetRow1, offsetLetterMin0);

    bool showHours = true;
    bool showMinutes = true;
    // toogle hours and minutes if clock is not high enough
    if (usedClockType->rowsWordMatrix() <
        (pgm_read_byte(&(fontHeight[usedFontSize])) * 2)) {
        if (_second % 4 < 2) { // show hours every 2 seconds
            showHours = true;
            showMinutes = false;
        } else { // show minutes
            showHours = false;
            showMinutes = true;
        }
    }

    uint8_t width = pgm_read_byte(&(fontWidth[usedFontSize]));
    uint8_t height = pgm_read_byte(&(fontHeight[usedFontSize]));

    for (uint8_t col = 0; col < width; col++) {
        for (uint8_t row = 0; row < height; row++) {
            // 1st Row: Hours
            if (showHours) {
                setPixelForChar(col, row, offsetLetterH1, offsetRow0,
                                static_cast<unsigned char>(h1), usedFontSize);
                setPixelForChar(col, row, offsetLetterH0, offsetRow0,
                                static_cast<unsigned char>(h0), usedFontSize);
            }
            // 2nd Row: Minutes
            if (showMinutes) {
                setPixelForChar(col, row, offsetLetterMin1, offsetRow1,
                                static_cast<unsigned char>(min1), usedFontSize);
                setPixelForChar(col, row, offsetLetterMin0, offsetRow1,
                                static_cast<unsigned char>(min0), usedFontSize);
            }
        }
    }

    mirrorFrontMatrixVertical(); // Needed for correct displaying of Chars
    setbyFrontMatrix(Foreground);
    show();
}

//------------------------------------------------------------------------------

void Led::show() { activeLedStrip->show(); }

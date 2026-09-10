/*
 * The minute LEDs and the seconds frame share one strip: the frame starts
 * where the minute section ends. When the two disagree the frame lights - and
 * Led::clearMinArray() clears - LEDs that belong to the minutes.
 *
 * How many minute LEDs sit between word matrix and frame is the wiring
 * (G.minuteLedCount). Selecting LED4x or LED7x declares it; Off and InWords
 * keep the last declared wiring, so switching to them must not move
 * the frame or resize the strip.
 *
 * The list of layouts comes from CLOCK_TYPES_LIST, so a newly added layout is
 * covered without touching this file.
 */

#include "WordClockState.h"

#include "WordClockTypes/ClockType.hpp"

// Globals the layouts expect from the unity build.
GLOBAL G = {};
uint32_t frontMatrix[MAX_ROW_SIZE] = {0};
uint8_t frontWordId[MAX_ROW_SIZE][MAX_COL_SIZE] = {{WORD_ID_NONE}};

#include "AllLayouts.gen.h"

#include "support/check.h"

namespace {

ClockType *const LAYOUTS[] = {
#define X(name, id, var, i18n) &var,
    CLOCK_TYPES_LIST
#undef X
};

const char *const LAYOUT_NAMES[] = {
#define X(name, id, var, i18n) #name,
    CLOCK_TYPES_LIST
#undef X
};

const size_t LAYOUT_COUNT = sizeof(LAYOUTS) / sizeof(LAYOUTS[0]);

const MinuteVariant VARIANTS[] = {MinuteVariant::Off, MinuteVariant::LED4x,
                                  MinuteVariant::LED7x, MinuteVariant::InWords};

const uint8_t WIRINGS[] = {MINUTE_LEDS_WIRED_4, MINUTE_LEDS_WIRED_7};

const char *variantName(MinuteVariant variant) {
    switch (variant) {
    case MinuteVariant::Off:
        return "Off";
    case MinuteVariant::LED4x:
        return "LED4x";
    case MinuteVariant::LED7x:
        return "LED7x";
    case MinuteVariant::InWords:
        return "InWords";
    }
    return "?";
}

/* Mirrors ClockWork::determineWhichMinuteVariant(). */
uint8_t minuteArrayColumn(MinuteVariant variant) {
    switch (variant) {
    case MinuteVariant::LED7x:
        return 1;
    default:
        return 0;
    }
}

MinuteVariant ledVariantFor(uint8_t wiring) {
    return wiring == MINUTE_LEDS_WIRED_7 ? MinuteVariant::LED7x
                                         : MinuteVariant::LED4x;
}

char message[200];

void setUp(uint8_t wiring, MinuteVariant variant) {
    G.buildTypeDef = BuildTypeDef::Normal;
    G.secondsFrameLedCount = 60;
    G.minuteLedCount = wiring;
    G.minuteVariant = variant;
}

void ledVariantsDeclareTheWiring() {
    for (uint8_t wiring : WIRINGS) {
        for (uint8_t previous : WIRINGS) {
            snprintf(message, sizeof(message),
                     "%s declares %u wired LEDs (was %u)",
                     variantName(ledVariantFor(wiring)), wiring, previous);
            check(minuteLedCountFor(ledVariantFor(wiring), previous) == wiring,
                  message);
        }
    }
}

void otherVariantsKeepTheLastWiring() {
    for (uint8_t wiring : WIRINGS) {
        const uint8_t declared =
            minuteLedCountFor(ledVariantFor(wiring), MINUTE_LEDS_WIRED_4);

        for (MinuteVariant variant : VARIANTS) {
            if (variant == MinuteVariant::LED4x ||
                variant == MinuteVariant::LED7x) {
                continue;
            }

            snprintf(message, sizeof(message),
                     "%s after %s keeps %u wired LEDs", variantName(variant),
                     variantName(ledVariantFor(wiring)), wiring);
            check(minuteLedCountFor(variant, declared) == wiring, message);
        }
    }
}

void minuteLedsAndFrameDoNotOverlap() {
    for (size_t i = 0; i < LAYOUT_COUNT; i++) {
        ClockType *layout = LAYOUTS[i];

        for (MinuteVariant variant : VARIANTS) {
            for (uint8_t previous : WIRINGS) {
                setUp(minuteLedCountFor(variant, previous), variant);
                if (!layout->supportsMinuteVariant(variant) ||
                    !layout->hasMinuteLeds()) {
                    continue;
                }

                uint16_t minutePixel[4] = {0};
                layout->getMinuteArray(minutePixel, minuteArrayColumn(variant));

                snprintf(message, sizeof(message),
                         "%s/%s/%u wired: minute LEDs run in strip order",
                         LAYOUT_NAMES[i], variantName(variant),
                         G.minuteLedCount);
                check(minutePixel[0] <= minutePixel[1] &&
                          minutePixel[1] <= minutePixel[2] &&
                          minutePixel[2] <= minutePixel[3],
                      message);

                // The span Led::clearMinArray() wipes, not just the four lit
                // LEDs.
                const uint16_t firstMinute = minutePixel[0];
                const uint16_t lastMinute = minutePixel[3];

                for (uint16_t f = 0; f < layout->numPixelsFrameMatrix(); f++) {
                    const uint16_t framePixel = layout->getFrameMatrixIndex(f);

                    snprintf(message, sizeof(message),
                             "%s/%s/%u wired: frame LED %u (pixel %u) is "
                             "clear of the minute LEDs %u..%u",
                             LAYOUT_NAMES[i], variantName(variant),
                             G.minuteLedCount, f, framePixel, firstMinute,
                             lastMinute);
                    check(framePixel < firstMinute || framePixel > lastMinute,
                          message);
                }
            }
        }
    }
}

void stripGeometryIgnoresTheMinuteVariant() {
    for (uint8_t wiring : WIRINGS) {
        for (size_t i = 0; i < LAYOUT_COUNT; i++) {
            ClockType *layout = LAYOUTS[i];

            setUp(wiring, ledVariantFor(wiring));
            const uint16_t frameStart = layout->getFrameMatrixIndex(0);
            const uint16_t stripLength = layout->numPixelsOnStrip();

            for (MinuteVariant variant : VARIANTS) {
                if (variant == MinuteVariant::LED4x ||
                    variant == MinuteVariant::LED7x) {
                    continue;
                }
                setUp(minuteLedCountFor(variant, wiring), variant);

                snprintf(message, sizeof(message),
                         "%s/%s after %s: frame stays at %u (got %u)",
                         LAYOUT_NAMES[i], variantName(variant),
                         variantName(ledVariantFor(wiring)), frameStart,
                         layout->getFrameMatrixIndex(0));
                check(layout->getFrameMatrixIndex(0) == frameStart, message);

                snprintf(message, sizeof(message),
                         "%s/%s after %s: strip length stays %u (got %u)",
                         LAYOUT_NAMES[i], variantName(variant),
                         variantName(ledVariantFor(wiring)), stripLength,
                         layout->numPixelsOnStrip());
                check(layout->numPixelsOnStrip() == stripLength, message);
            }
        }
    }
}

void offKeepsTheFrameBehindSevenWiredLeds() {
    for (size_t i = 0; i < LAYOUT_COUNT; i++) {
        ClockType *layout = LAYOUTS[i];

        setUp(minuteLedCountFor(MinuteVariant::Off, MINUTE_LEDS_WIRED_7),
              MinuteVariant::Off);
        if (!layout->supportsSecondsFrame() ||
            !layout->supportsMinuteVariant(MinuteVariant::LED7x)) {
            continue;
        }

        uint16_t minutePixel[4] = {0};
        layout->getMinuteArray(minutePixel,
                               minuteArrayColumn(MinuteVariant::LED7x));

        snprintf(message, sizeof(message),
                 "%s: minutes off after LED7x - frame (pixel %u) stays "
                 "behind the last minute LED %u",
                 LAYOUT_NAMES[i], layout->getFrameMatrixIndex(0),
                 minutePixel[3]);
        check(layout->getFrameMatrixIndex(0) > minutePixel[3], message);
    }
}

void layoutsWithoutLedVariantsHaveNoMinuteLeds() {
    for (uint8_t wiring : WIRINGS) {
        for (size_t i = 0; i < LAYOUT_COUNT; i++) {
            ClockType *layout = LAYOUTS[i];

            setUp(wiring, MinuteVariant::Off);
            const bool offersLedVariant =
                layout->supportsMinuteVariant(MinuteVariant::LED4x) ||
                layout->supportsMinuteVariant(MinuteVariant::LED7x);

            snprintf(message, sizeof(message),
                     "%s/%u wired: minute LEDs exactly when LED4x or LED7x "
                     "is offered",
                     LAYOUT_NAMES[i], wiring);
            check(layout->hasMinuteLeds() == offersLedVariant, message);

            snprintf(message, sizeof(message),
                     "%s/%u wired: no LEDs reserved without minute LEDs (%u)",
                     LAYOUT_NAMES[i], wiring, layout->numPixelsMinuteMatrix());
            check(offersLedVariant || layout->numPixelsMinuteMatrix() == 0,
                  message);
        }
    }
}

/* A frame that starts inside the word matrix would repaint letters. */
void frameStartsBehindTheWordMatrix() {
    for (uint8_t wiring : WIRINGS) {
        for (size_t i = 0; i < LAYOUT_COUNT; i++) {
            ClockType *layout = LAYOUTS[i];

            setUp(wiring, MinuteVariant::Off);
            if (!layout->supportsSecondsFrame()) {
                continue;
            }

            const uint16_t wordMatrixPixels =
                layout->rowsWordMatrix() * layout->colsWordMatrix();

            snprintf(message, sizeof(message),
                     "%s/%u wired: frame starts behind the word matrix",
                     LAYOUT_NAMES[i], wiring);
            check(layout->getFrameMatrixIndex(0) >= wordMatrixPixels, message);
        }
    }
}

} // namespace

int main() {
    ledVariantsDeclareTheWiring();
    otherVariantsKeepTheLastWiring();
    minuteLedsAndFrameDoNotOverlap();
    stripGeometryIgnoresTheMinuteVariant();
    offKeepsTheFrameBehindSevenWiredLeds();
    layoutsWithoutLedVariantsHaveNoMinuteLeds();
    frameStartsBehindTheWordMatrix();
    return report("minute_frame_layout");
}

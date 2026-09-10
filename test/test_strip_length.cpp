/*
 * Every LED a layout addresses must exist on the strip.
 *
 * The strip used to be allocated with a fixed 300 * ledsPerLetter pixels.
 * NeoPixelBus silently drops SetPixelColor() beyond its pixel count, so any
 * layout that reached further - a large matrix, ExtraLedPerRow, or a seconds
 * frame configured up to MAX_SECONDS_FRAME_LED_COUNT - simply stayed dark.
 *
 * numPixelsOnStrip() now sizes the strip, so this walks every layout through
 * every geometry the web UI can produce and asserts that the letter, minute
 * and frame indices all fall below it.
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

const BuildTypeDef BUILD_TYPES[] = {
    BuildTypeDef::Normal, BuildTypeDef::DoubleResM1, BuildTypeDef::DoubleRes,
    BuildTypeDef::TrippleRes, BuildTypeDef::QuadRes};

const MinuteVariant MINUTE_VARIANTS[] = {
    MinuteVariant::Off, MinuteVariant::LED4x, MinuteVariant::LED7x,
    MinuteVariant::InWords};

const uint8_t FRAME_COUNTS[] = {0, 1, 60, MAX_SECONDS_FRAME_LED_COUNT};

const uint8_t MINUTE_WIRINGS[] = {MINUTE_LEDS_WIRED_4, MINUTE_LEDS_WIRED_7};

char message[160];

uint16_t countOutOfRange(ClockType *layout) {
    const uint16_t pixelCount = layout->numPixelsOnStrip();
    const uint8_t ledsPerLetter = getLedsPerLetter(G.buildTypeDef);
    uint16_t outOfRange = 0;

    for (uint8_t row = 0; row < layout->rowsWordMatrix(); row++) {
        for (uint8_t col = 0; col < layout->colsWordMatrix(); col++) {
            const uint16_t ledIndex = layout->getFrontMatrixIndex(row, col);
            for (uint8_t i = 0; i < ledsPerLetter; i++) {
                if (ledIndex * ledsPerLetter + i >= pixelCount) {
                    outOfRange++;
                }
            }
        }
    }

    for (uint8_t variant = 0; variant < 2; variant++) {
        uint16_t minutePixels[4] = {0};
        layout->getMinuteArray(minutePixels, variant);
        for (uint8_t i = 0; i < 4; i++) {
            if (minutePixels[i] >= pixelCount) {
                outOfRange++;
            }
        }
    }

    for (uint16_t i = 0; i < layout->numPixelsFrameMatrix(); i++) {
        if (layout->getFrameMatrixIndex(i) >= pixelCount) {
            outOfRange++;
        }
    }

    return outOfRange;
}

void checkLayout(ClockType *layout, const char *name) {
    uint16_t outOfRange = 0;
    uint16_t largestStrip = 0;

    for (uint8_t wiring : MINUTE_WIRINGS) {
        G.minuteLedCount = wiring;
        for (BuildTypeDef buildType : BUILD_TYPES) {
            for (uint8_t variantBits = 0; variantBits < 8; variantBits++) {
                for (MinuteVariant minuteVariant : MINUTE_VARIANTS) {
                    for (uint8_t frameCount : FRAME_COUNTS) {
                        G.buildTypeDef = buildType;
                        G.layoutVariant[FlipHorzVert] = variantBits & 1;
                        G.layoutVariant[ExtraLedPerRow] = variantBits & 2;
                        G.layoutVariant[MeanderRows] = variantBits & 4;
                        G.minuteVariant = minuteVariant;
                        G.secondsFrameLedCount = frameCount;

                        outOfRange += countOutOfRange(layout);

                        const uint16_t pixelCount = layout->numPixelsOnStrip();
                        if (pixelCount > largestStrip) {
                            largestStrip = pixelCount;
                        }
                    }
                }
            }
        }
    }

    snprintf(message, sizeof(message),
             "%s: %u LED indices fall outside the allocated strip", name,
             outOfRange);
    check(outOfRange == 0, message);

    snprintf(message, sizeof(message), "%s: strip length is plausible (%u)",
             name, largestStrip);
    check(largestStrip > 0 && largestStrip <= 2048, message);
}

} // namespace

int main() {
    for (size_t i = 0; i < LAYOUT_COUNT; i++) {
        checkLayout(LAYOUTS[i], LAYOUT_NAMES[i]);
    }

    return report("strip length");
}

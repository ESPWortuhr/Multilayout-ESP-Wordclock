#pragma once

#include <Arduino.h>

enum fontSize : uint8_t {
    normalSizeASCII = 0,
    smallSizeNumbers = 1,
};

const PROGMEM uint8_t fontHeight[2] = {7, 5};
const PROGMEM uint8_t fontWidth[2] = {5, 3};

struct NumberFont {
    fontSize font;
    uint8_t width;
    uint8_t height;

    static NumberFont of(fontSize font) {
        NumberFont numberFont;
        numberFont.font = font;
        numberFont.width = pgm_read_byte(&(fontWidth[font]));
        numberFont.height = pgm_read_byte(&(fontHeight[font]));
        return numberFont;
    }

    unsigned char glyph(char digit) const {
        const unsigned char character = static_cast<unsigned char>(digit);
        return font == smallSizeNumbers
                   ? static_cast<unsigned char>(character - '0')
                   : character;
    }
};

inline NumberFont numberFontFor(uint8_t cols, uint8_t rows,
                                uint8_t digitRows = 1) {
    const NumberFont normal = NumberFont::of(normalSizeASCII);
    if (cols >= normal.width * 2 + 1 && rows >= normal.height * digitRows) {
        return normal;
    }
    return NumberFont::of(smallSizeNumbers);
}

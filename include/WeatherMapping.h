#pragma once

#include "WordClockTypes/FrontWord.h"
#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/*--------------------------------------------------
List of Conditions http://openweathermap.org/weather-conditions
Weather condition code (wetterid):

2xx Thunderstorm
3xx Drizzle
5xx Rain
6xx Snow
7xx Clouds (Warning)
800 Clear
80x Clouds
--------------------------------------------------*/

namespace forecast {

const uint8_t slots = 4;
const uint8_t maxWords = 9;

inline int8_t temperatureStep(double celsius) {
    if (celsius >= 30) {
        return 30;
    } else if (celsius >= 0 && celsius <= 5) {
        return 1;
    } else if (celsius < 0 && celsius >= -5) {
        return -1;
    } else if (celsius <= -25) {
        return -25;
    }
    return static_cast<int8_t>(celsius - fmod(celsius, 5));
}

inline uint16_t conditionGroup(uint16_t id) {
    if (id == 800) {
        return 800;
    } else if (id > 800 && id < 900) {
        return 801;
    } else if (id >= 200 && id < 800) {
        return id / 100 * 100;
    }
    return 0;
}

inline uint8_t daytimeFor(uint8_t hour) { return ((hour + 21) % 24) / 6; }

struct Words {
    FrontWord word[maxWords];
    uint8_t count;

    const FrontWord *begin() const { return word; }
    const FrontWord *end() const { return word + count; }
};

namespace detail {

inline void append(Words &words, FrontWord word) {
    if (word != FrontWord{} && words.count < maxWords) {
        words.word[words.count++] = word;
    }
}

template <size_t N> void append(Words &words, const FrontWord (&add)[N]) {
    for (FrontWord word : add) {
        append(words, word);
    }
}

inline void appendDaytime(Words &words, uint8_t index) {
    static const FrontWord daytimeWords[7][2] = {
        {FrontWord::w_mittag},
        {FrontWord::w_abend},
        {FrontWord::w_nacht},
        {FrontWord::w_morgen, FrontWord::w_frueh},
        {FrontWord::w_morgen, FrontWord::w_mittag},
        {FrontWord::w_morgen, FrontWord::w_abend},
        {FrontWord::w_morgen, FrontWord::w_nacht},
    };
    if (index < 7) {
        append(words, daytimeWords[index]);
    }
}

inline void appendTemperature(Words &words, int8_t temperature) {
    struct Degrees {
        uint8_t value;
        FrontWord number[3];
    };
    static const Degrees degrees[] = {
        {1, {FrontWord::w_null}},
        {5, {FrontWord::w_fuenf}},
        {10, {FrontWord::w_zehn}},
        {15, {FrontWord::w_fuenf, FrontWord::w_zehn}},
        {20, {FrontWord::w_zwanzig}},
        {25, {FrontWord::w_fuenf, FrontWord::w_und, FrontWord::w_zwanzig}},
        {30, {FrontWord::w_dreissig}},
    };

    for (const Degrees &entry : degrees) {
        if (entry.value != abs(temperature)) {
            continue;
        }
        if (temperature > 0) {
            append(words, FrontWord::w_ueber);
        } else {
            append(words, FrontWord::w_unter);
            append(words, FrontWord::w_minus);
        }
        append(words, entry.number);
        append(words, FrontWord::w_grad);
        return;
    }
}

inline void appendCondition(Words &words, uint16_t condition) {
    switch (condition) {
    case 200:
        append(words, FrontWord::w_gewitter);
        break;
    case 300:
    case 500:
        append(words, FrontWord::w_regen);
        break;
    case 600:
        append(words, FrontWord::w_schnee);
        break;
    case 700:
        append(words, FrontWord::w_warnung);
        break;
    case 800:
        append(words, FrontWord::w_klar);
        break;
    case 801:
        append(words, FrontWord::w_wolken);
        break;
    }
}

} // namespace detail

inline Words wordsFor(uint8_t slot, uint8_t daytime, int8_t temperature,
                      uint16_t condition) {
    Words words = {};
    detail::appendDaytime(words, slot + daytime);
    detail::appendTemperature(words, temperature);
    detail::appendCondition(words, condition);
    return words;
}

} // namespace forecast

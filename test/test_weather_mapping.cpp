#include "WeatherMapping.h"

#include "support/check.h"

#include <initializer_list>
#include <stdio.h>

namespace {

char message[200];

bool sameWords(const forecast::Words &actual,
               std::initializer_list<FrontWord> expected) {
    if (actual.count != expected.size()) {
        return false;
    }
    uint8_t i = 0;
    for (FrontWord word : expected) {
        if (actual.word[i++] != word) {
            return false;
        }
    }
    return true;
}

void checkTemperatureSteps() {
    struct Case {
        double celsius;
        int8_t step;
    };
    const Case cases[] = {
        {35.0, 30},   {30.0, 30},   {29.9, 25},   {25.0, 25}, {24.9, 20},
        {13.27, 10},  {7.0, 5},     {5.01, 5},    {5.0, 1},   {0.0, 1},
        {-0.1, -1},   {-5.0, -1},   {-5.1, -5},   {-9.9, -5}, {-10.0, -10},
        {-24.9, -20}, {-25.0, -25}, {-40.0, -25},
    };
    for (const Case &c : cases) {
        const int8_t step = forecast::temperatureStep(c.celsius);
        snprintf(message, sizeof(message),
                 "temperatureStep(%.2f) = %d, expected %d", c.celsius, step,
                 c.step);
        check(step == c.step, message);
    }
}

void checkConditionGroups() {
    struct Case {
        uint16_t id;
        uint16_t group;
    };
    const Case cases[] = {
        {200, 200}, {232, 200}, {300, 300}, {321, 300}, {500, 500},
        {531, 500}, {600, 600}, {622, 600}, {701, 700}, {781, 700},
        {800, 800}, {801, 801}, {804, 801}, {899, 801}, {900, 0},
        {962, 0},   {199, 0},   {0, 0},     {65535, 0},
    };
    for (const Case &c : cases) {
        const uint16_t group = forecast::conditionGroup(c.id);
        snprintf(message, sizeof(message),
                 "conditionGroup(%u) = %u, expected %u", c.id, group, c.group);
        check(group == c.group, message);
    }
}

void checkDaytimes() {
    const uint8_t expected[24] = {3, 3, 3, 0, 0, 0, 0, 0, 0, 1, 1, 1,
                                  1, 1, 1, 2, 2, 2, 2, 2, 2, 3, 3, 3};
    for (uint8_t hour = 0; hour < 24; hour++) {
        const uint8_t daytime = forecast::daytimeFor(hour);
        snprintf(message, sizeof(message), "daytimeFor(%u) = %u, expected %u",
                 hour, daytime, expected[hour]);
        check(daytime == expected[hour], message);
    }
}

void checkDaytimeWords() {
    const std::initializer_list<FrontWord> phrases[7] = {
        {FrontWord::w_mittag},
        {FrontWord::w_abend},
        {FrontWord::w_nacht},
        {FrontWord::w_morgen, FrontWord::w_frueh},
        {FrontWord::w_morgen, FrontWord::w_mittag},
        {FrontWord::w_morgen, FrontWord::w_abend},
        {FrontWord::w_morgen, FrontWord::w_nacht},
    };
    for (uint8_t slot = 0; slot < forecast::slots; slot++) {
        for (uint8_t daytime = 0; daytime < 4; daytime++) {
            snprintf(message, sizeof(message),
                     "slot %u at daytime %u shows phrase %u", slot, daytime,
                     slot + daytime);
            check(sameWords(forecast::wordsFor(slot, daytime, 0, 0),
                            phrases[slot + daytime]),
                  message);
        }
    }
}

void checkTemperatureWords() {
    const FrontWord m = FrontWord::w_mittag;
    const FrontWord ueber = FrontWord::w_ueber;
    const FrontWord unter = FrontWord::w_unter;
    const FrontWord minus = FrontWord::w_minus;
    const FrontWord grad = FrontWord::w_grad;
    struct Case {
        int8_t temperature;
        std::initializer_list<FrontWord> words;
    };
    const Case cases[] = {
        {30, {m, ueber, FrontWord::w_dreissig, grad}},
        {25,
         {m, ueber, FrontWord::w_fuenf, FrontWord::w_und, FrontWord::w_zwanzig,
          grad}},
        {20, {m, ueber, FrontWord::w_zwanzig, grad}},
        {15, {m, ueber, FrontWord::w_fuenf, FrontWord::w_zehn, grad}},
        {10, {m, ueber, FrontWord::w_zehn, grad}},
        {5, {m, ueber, FrontWord::w_fuenf, grad}},
        {1, {m, ueber, FrontWord::w_null, grad}},
        {-1, {m, unter, minus, FrontWord::w_null, grad}},
        {-5, {m, unter, minus, FrontWord::w_fuenf, grad}},
        {-10, {m, unter, minus, FrontWord::w_zehn, grad}},
        {-15, {m, unter, minus, FrontWord::w_fuenf, FrontWord::w_zehn, grad}},
        {-20, {m, unter, minus, FrontWord::w_zwanzig, grad}},
        {-25,
         {m, unter, minus, FrontWord::w_fuenf, FrontWord::w_und,
          FrontWord::w_zwanzig, grad}},
        {0, {m}},
        {2, {m}},
        {31, {m}},
    };
    for (const Case &c : cases) {
        snprintf(message, sizeof(message), "temperature %d", c.temperature);
        check(sameWords(forecast::wordsFor(0, 0, c.temperature, 0), c.words),
              message);
    }
}

void checkConditionWords() {
    const FrontWord m = FrontWord::w_mittag;
    struct Case {
        uint16_t condition;
        std::initializer_list<FrontWord> words;
    };
    const Case cases[] = {
        {200, {m, FrontWord::w_gewitter}},
        {300, {m, FrontWord::w_regen}},
        {500, {m, FrontWord::w_regen}},
        {600, {m, FrontWord::w_schnee}},
        {700, {m, FrontWord::w_warnung}},
        {800, {m, FrontWord::w_klar}},
        {801, {m, FrontWord::w_wolken}},
        {0, {m}},
        {100, {m}},
        {400, {m}},
    };
    for (const Case &c : cases) {
        snprintf(message, sizeof(message), "condition %u", c.condition);
        check(sameWords(forecast::wordsFor(0, 0, 0, c.condition), c.words),
              message);
    }
}

void checkLongestForecast() {
    const forecast::Words words = forecast::wordsFor(3, 3, -25, 600);
    check(words.count == forecast::maxWords, "longest forecast fills Words");
    check(sameWords(words,
                    {FrontWord::w_morgen, FrontWord::w_nacht,
                     FrontWord::w_unter, FrontWord::w_minus, FrontWord::w_fuenf,
                     FrontWord::w_und, FrontWord::w_zwanzig, FrontWord::w_grad,
                     FrontWord::w_schnee}),
          "longest forecast keeps daytime, temperature, condition order");
}

void checkForecastFromTheClock() {
    const forecast::Words words = forecast::wordsFor(
        0, forecast::daytimeFor(21), forecast::temperatureStep(13.09),
        forecast::conditionGroup(804));
    check(sameWords(words, {FrontWord::w_morgen, FrontWord::w_frueh,
                            FrontWord::w_ueber, FrontWord::w_zehn,
                            FrontWord::w_grad, FrontWord::w_wolken}),
          "13.09 C overcast at 21:00 reads morgen frueh ueber zehn grad "
          "wolken");
}

} // namespace

int main() {
    checkTemperatureSteps();
    checkConditionGroups();
    checkDaytimes();
    checkDaytimeWords();
    checkTemperatureWords();
    checkConditionWords();
    checkLongestForecast();
    checkForecastFromTheClock();

    return report("weather mapping");
}

#include "WordClockState.h"

#include "support/check.h"

int main() {
    check(secondVariantIsValid(0), "second variant Off is valid");
    check(secondVariantIsValid(3), "second variant FrameSectorToggle is valid");
    check(!secondVariantIsValid(4), "second variant 4 is rejected");
    check(!secondVariantIsValid(255), "second variant 255 is rejected");

    check(minuteVariantIsValid(0), "minute variant Off is valid");
    check(minuteVariantIsValid(3), "minute variant InWords is valid");
    check(!minuteVariantIsValid(4), "minute variant 4 is rejected");

    check(itIsVariantIsValid(0), "it-is variant Permanent is valid");
    check(itIsVariantIsValid(4), "it-is variant Off is valid");
    check(!itIsVariantIsValid(5), "it-is variant 5 is rejected");

    check(colorTypeIsValid(Brg), "color type Brg is valid");
    check(colorTypeIsValid(Grbw), "color type Grbw is valid");
    check(!colorTypeIsValid(6), "color type 6 is rejected");
    check(!colorTypeIsValid(1000), "color type 1000 is rejected");

    check(buildTypeIsValid(0), "build type Normal is valid");
    check(buildTypeIsValid(4), "build type QuadRes is valid");
    check(!buildTypeIsValid(5), "build type 5 is rejected");

    return report("enum_validation");
}

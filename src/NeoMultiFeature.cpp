#include "NeoMultiFeature.hpp"

#include "Config.h"         // DEFAULT_LEDTYPE
#include "WordClockState.h" // LedColorVariants (value of DEFAULT_LEDTYPE)

// Single definition of the static color-type selector shared by all
// NeoMultiFeature users.
uint8_t NeoMultiFeature::_colortype = DEFAULT_LEDTYPE;

#include "PortalSettings.h"

#include "WordClockState.h"

namespace eeprom {
void write();
}

namespace {
struct ClockTypeLabel {
    const char *key;
    const char *label;
};

const ClockTypeLabel CLOCK_TYPE_LABELS[] = {
    {"bn-9-8", "🇧🇩 9 × 8"},
    {"ch-10-11", "🇨🇭 10 x 11"},
    {"ch-10-11-v2", "🇨🇭 10 x 11 Version 2"},
    {"ch-10-11-zh", "🇨🇭 10 x 11 Zurich German"},
    {"en-08-08", "🇬🇧 8 × 8"},
    {"en-10-11", "🇬🇧 10 × 11"},
    {"en-11-12", "🇬🇧 11 × 12"},
    {"es-08-08-cuarto", "🇪🇸 8 × 8 Quarter hours"},
    {"es-10-11", "🇪🇸 10 × 11"},
    {"fr-10-11", "🇫🇷 10 x 11"},
    {"de-08-08", "🇩🇪 8 × 8"},
    {"de-08-08-viertel", "🇩🇪 8 × 8 Quarter hours"},
    {"de-10-11", "🇩🇪 10 × 11"},
    {"de-10-11-alt", "🇩🇪 10 × 11 Alternative"},
    {"de-10-11-bayerisch", "🇩🇪 10 × 11 Bavarian"},
    {"de-10-11-clock", "🇩🇪 10 × 11 Clock"},
    {"de-10-11-mrrioes", "🇩🇪 10 × 11 mrrioes"},
    {"de-10-11-nero", "🇩🇪 10 × 11 Nero"},
    {"de-10-11-schwaebisch", "🇩🇪 10 × 11 Swabian Style"},
    {"de-11-11", "🇩🇪 11 × 11"},
    {"de-11-11-schwaebisch", "🇩🇪 11 × 11 Swabian Style"},
    {"de-11-11-v2", "🇩🇪 11 × 11 Version 2"},
    {"de-11-11-v3", "🇩🇪 11 × 11 (panbachi)"},
    {"de-13-13", "🇩🇪 13 × 13"},
    {"de-16-8", "🇩🇪 16 × 8"},
    {"de-16-18", "🇩🇪 16 × 18"},
    {"de-22-11-weather", "🇩🇪 22 × 11 Weather"},
    {"hu-10-10", "🇭🇺 10 x 10"},
    {"it-10-11", "🇮🇹 10 × 11"},
    {"nl-10-11", "🇳🇱 10 × 11"},
    {"ro-10-11", "🇷🇴 10 x 11"},
    {"ru-10-11", "🇷🇺 10 × 11"},
    {"se-10-11", "🇸🇪 10 × 11"},
    {"tr-10-11", "🇹🇷 10 × 11"},
};

struct ClockTypeOption {
    uint8_t id;
    const char *key;
};

const ClockTypeOption CLOCK_TYPE_OPTIONS[] = {
#define X(name, id, var, i18n) {id, i18n},
    CLOCK_TYPES_LIST
#undef X
};

const char *const LAYOUT_LABELS[] = {
    "Invert Minute Counting Direction",
    "Mirror Word Clock Vertically",
    "Mirror Word Clock Horizontally",
    "Flip Horizontally and Vertically Layout",
    "Add Extra LED per Row",
    "Rows placed in meanders",
};

const char *const BUILD_TYPE_LABELS[] = {
    "Each LED corresponds to one letter",
    "Every second LED corresponds to one letter",
    "Two LEDs per letter",
    "Three LEDs per letter",
    "Four LEDs per letter",
};

constexpr uint8_t LAYOUT_COUNT =
    sizeof(LAYOUT_LABELS) / sizeof(LAYOUT_LABELS[0]);
constexpr uint8_t BUILD_TYPE_COUNT =
    sizeof(BUILD_TYPE_LABELS) / sizeof(BUILD_TYPE_LABELS[0]);

const char *clockTypeLabel(const char *key) {
    for (const ClockTypeLabel &entry : CLOCK_TYPE_LABELS) {
        if (strcmp(entry.key, key) == 0) {
            return entry.label;
        }
    }
    return key;
}

String layoutArgName(uint8_t index) { return "lv" + String(index); }

bool parseUnsigned(const String &value, uint32_t &result) {
    if (value.length() == 0) {
        return false;
    }
    char *end = nullptr;
    result = strtoul(value.c_str(), &end, 10);
    return end != nullptr && *end == '\0';
}

bool parseHexColor(const String &value, uint32_t &rgb) {
    if (value.length() != 7 || value[0] != '#') {
        return false;
    }
    char *end = nullptr;
    rgb = strtoul(value.c_str() + 1, &end, 16);
    return end != nullptr && *end == '\0';
}
} // namespace

namespace portalSettings {

String render() {
    RgbColor rgb(G.color[Foreground]);
    char colorValue[8];
    snprintf(colorValue, sizeof(colorValue), "#%02X%02X%02X", rgb.R, rgb.G,
             rgb.B);

    String html = "<label for='color'>Color</label>"
                  "<input type='color' id='color' name='color' value='";
    html += colorValue;
    html += "' style='height:3em;padding:2px'>";

    html += "<label for='clocktype'>Choice of Clock Variant</label>"
            "<select id='clocktype' name='clocktype'>";
    for (const ClockTypeOption &option : CLOCK_TYPE_OPTIONS) {
        html += "<option value='" + String(option.id) + "'";
        if (option.id == G.clockTypeDef) {
            html += " selected";
        }
        html += ">";
        html += clockTypeLabel(option.key);
        html += "</option>";
    }
    html += "</select>";

    for (uint8_t i = 0; i < LAYOUT_COUNT; i++) {
        const String name = layoutArgName(i);
        html += "<br/><input type='checkbox' id='" + name + "' name='" + name +
                "' value='1'";
        if (G.layoutVariant[i]) {
            html += " checked";
        }
        html += "> <label for='" + name + "'>";
        html += LAYOUT_LABELS[i];
        html += "</label>";
    }

    html += "<br/><label for='buildtype'>Buildtype</label>"
            "<select id='buildtype' name='buildtype'>";
    for (uint8_t i = 0; i < BUILD_TYPE_COUNT; i++) {
        html += "<option value='" + String(i) + "'";
        if (i == static_cast<uint8_t>(G.buildTypeDef)) {
            html += " selected";
        }
        html += ">";
        html += BUILD_TYPE_LABELS[i];
        html += "</option>";
    }
    html += "</select>"
            "<p><small>Changes to the matrix size are only applied when the "
            "clock is restarted.</small></p>";
    return html;
}

void apply(const ArgLookup &arg) {
    String value;
    uint32_t number = 0;

    if (arg("color", value) && parseHexColor(value, number)) {
        G.color[Foreground] = HsbColor(
            RgbColor(number >> 16, (number >> 8) & 0xFF, number & 0xFF));
    }

    bool clockTypeChanged = false;
    if (arg("clocktype", value) && parseUnsigned(value, number) &&
        number <= UINT8_MAX &&
        isValidClockTypeDef(static_cast<uint8_t>(number)) &&
        number != G.clockTypeDef) {
        G.clockTypeDef = static_cast<uint8_t>(number);
        clockTypeChanged = true;
    }

    for (uint8_t i = 0; i < LAYOUT_COUNT; i++) {
        G.layoutVariant[i] = arg(layoutArgName(i).c_str(), value);
    }

    if (arg("buildtype", value) && parseUnsigned(value, number) &&
        buildTypeIsValid(number)) {
        G.buildTypeDef = static_cast<BuildTypeDef>(number);
    }

    Serial.println("Settings saved from captive portal");
    eeprom::write();
    parametersChanged = true;
    G.conf =
        clockTypeChanged ? COMMAND_SET_CLOCK_TYPE : COMMAND_SET_LAYOUT_VARIANT;
}

} // namespace portalSettings

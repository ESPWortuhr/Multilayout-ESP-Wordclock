/*
 * Every cell a word lights up must carry that word's id.
 *
 * This is the invariant that replaced ~150 lines of hard coded per layout
 * patch-ups in colorize(). Those existed because word boundaries used to be
 * guessed from horizontal runs of lit cells, which fails for words that are
 * not laid out horizontally - ZWEI spanning two rows, EINS running down a
 * column, TWENTY over three rows.
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
const uint8_t LAST_WORD = static_cast<uint8_t>(FrontWord::happy_birthday);

char message[160];

void drawOnly(ClockType *layout, FrontWord word) {
    for (uint8_t row = 0; row < MAX_ROW_SIZE; row++) {
        frontMatrix[row] = 0;
    }
    ClockType::resetWordIds();
    layout->show(word);
}

struct Drawn {
    uint16_t cells = 0;
    uint8_t rowsSpanned = 0;
    uint16_t untaggedCells = 0; // lit but carrying no word
    uint16_t strayTags = 0;     // dark but claiming a word
    uint16_t foreignCells = 0;  // tagged with a different word
    uint8_t distinctWords = 0;
};

Drawn inspect(ClockType *layout, FrontWord word) {
    Drawn drawn;
    const uint8_t expected = static_cast<uint8_t>(word);
    uint8_t seen[16] = {0};

    for (uint8_t row = 0; row < layout->rowsWordMatrix(); row++) {
        bool rowUsed = false;
        for (uint8_t col = 0; col < layout->colsWordMatrix(); col++) {
            const bool lit = layout->getFrontMatrixPixel(row, col);
            const uint8_t id = frontWordId[row][col];

            if (!lit) {
                if (id != WORD_ID_NONE) {
                    drawn.strayTags++;
                }
                continue;
            }

            drawn.cells++;
            rowUsed = true;

            if (id == WORD_ID_NONE) {
                drawn.untaggedCells++;
                continue;
            }
            if (id != expected) {
                drawn.foreignCells++;
            }

            bool known = false;
            for (uint8_t i = 0; i < drawn.distinctWords; i++) {
                if (seen[i] == id) {
                    known = true;
                    break;
                }
            }
            if (!known && drawn.distinctWords < 16) {
                seen[drawn.distinctWords++] = id;
            }
        }
        if (rowUsed) {
            drawn.rowsSpanned++;
        }
    }
    return drawn;
}

/*
 * The invariant that actually matters for colouring: no lit cell may be left
 * without a word, and no dark cell may claim one. Composed words legitimately
 * produce several ids - Ger13x13 draws "zwei und" as ZWEI plus UND, which are
 * two visible words and are meant to be coloured separately.
 */
void everyLitCellBelongsToAWord() {
    uint16_t untagged = 0;
    uint16_t stray = 0;

    for (size_t i = 0; i < LAYOUT_COUNT; i++) {
        for (uint8_t w = 1; w <= LAST_WORD; w++) {
            // FrontWord::error is the "no word" id and is never drawn.
            const FrontWord word = static_cast<FrontWord>(w);
            drawOnly(LAYOUTS[i], word);
            const Drawn drawn = inspect(LAYOUTS[i], word);

            if (drawn.untaggedCells == 0 && drawn.strayTags == 0) {
                continue;
            }
            untagged += drawn.untaggedCells;
            stray += drawn.strayTags;
            snprintf(message, sizeof(message),
                     "%s: word %u leaves %u cells untagged and %u stray tags",
                     LAYOUT_NAMES[i], w, drawn.untaggedCells, drawn.strayTags);
            check(false, message);
        }
    }

    snprintf(message, sizeof(message),
             "no lit cell is left without a word (%u layouts)",
             static_cast<unsigned>(LAYOUT_COUNT));
    check(untagged == 0, message);
    check(stray == 0, "no dark cell claims a word");
}

/*
 * The cases the deleted patch-ups existed for: words that do not sit on one
 * row. Each must light cells across several rows and still read as one word.
 */
void nonHorizontalWordsStayOneWord() {
    struct Case {
        ClockType *layout;
        const char *layoutName;
        FrontWord word;
        const char *wordName;
    };

    const Case cases[] = {
        {&_de08x08, "Ger08x08", FrontWord::hour_2, "ZWEI over two rows"},
        {&_de08x08, "Ger08x08", FrontWord::eins, "EINS down a column"},
        {&_en08x08, "Eng08x08", FrontWord::min_20, "TWENTY over three rows"},
        {&_en08x08, "Eng08x08", FrontWord::hour_5, "FIVE diagonally"},
        {&_en08x08, "Eng08x08", FrontWord::hour_9, "NINE down a column"},
    };

    for (const Case &c : cases) {
        drawOnly(c.layout, c.word);
        const Drawn drawn = inspect(c.layout, c.word);

        snprintf(message, sizeof(message), "%s: %s is drawn at all",
                 c.layoutName, c.wordName);
        check(drawn.cells > 0, message);

        snprintf(message, sizeof(message), "%s: %s spans several rows",
                 c.layoutName, c.wordName);
        check(drawn.rowsSpanned > 1, message);

        snprintf(message, sizeof(message), "%s: %s reads as a single word",
                 c.layoutName, c.wordName);
        check(drawn.distinctWords == 1 && drawn.foreignCells == 0, message);
    }
}

/*
 * Ger13x13 composes words by calling show() from inside drawWord() and keeps
 * drawing afterwards. Everything it draws has to end up tagged - if show() had
 * cleared the current word instead of restoring it, the parts drawn after a
 * nested call would be left without one.
 */
void nestedShowKeepsTheOuterWord() {
    drawOnly(&_de13x13, FrontWord::min_22);
    const Drawn drawn = inspect(&_de13x13, FrontWord::min_22);

    check(drawn.cells > 0, "Ger13x13: min_22 is drawn at all");
    check(drawn.untaggedCells == 0,
          "Ger13x13: drawing after a nested show() stays tagged");
    // "zwei und zwanzig": ZWEI and ZWANZIG come from nested show() calls, UND
    // is drawn by min_22 itself - three separately colourable words.
    check(drawn.distinctWords == 3,
          "Ger13x13: a composed minute reads as three separate words");
}

} // namespace

int main() {
    everyLitCellBelongsToAWord();
    nonHorizontalWordsStayOneWord();
    nestedShowKeepsTheOuterWord();
    return report("word identity");
}

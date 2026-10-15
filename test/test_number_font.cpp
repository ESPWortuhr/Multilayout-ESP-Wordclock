#include "NumberFont.h"
#include "support/check.h"

namespace {

struct Layout {
    const char *name;
    uint8_t rows;
    uint8_t cols;
    fontSize oneDigitRow;
    fontSize twoDigitRows;
};

const Layout layouts[] = {
    {"8x8", 8, 8, smallSizeNumbers, smallSizeNumbers},
    {"BN 9x8", 8, 9, smallSizeNumbers, smallSizeNumbers},
    {"HU 10x10", 10, 10, smallSizeNumbers, smallSizeNumbers},
    {"10x11", 10, 11, normalSizeASCII, smallSizeNumbers},
    {"11x11", 11, 11, normalSizeASCII, smallSizeNumbers},
    {"EN 11x12", 11, 12, normalSizeASCII, smallSizeNumbers},
    {"13x13", 13, 13, normalSizeASCII, smallSizeNumbers},
    {"16x8", 8, 16, normalSizeASCII, smallSizeNumbers},
    {"16x18", 16, 18, normalSizeASCII, normalSizeASCII},
    {"22x11 weather", 22, 11, normalSizeASCII, normalSizeASCII},
};

char message[120];

void fontFollowsLayoutSize() {
    for (const Layout &layout : layouts) {
        snprintf(message, sizeof(message), "%s: one digit row", layout.name);
        check(numberFontFor(layout.cols, layout.rows).font ==
                  layout.oneDigitRow,
              message);
        snprintf(message, sizeof(message), "%s: two digit rows", layout.name);
        check(numberFontFor(layout.cols, layout.rows, 2).font ==
                  layout.twoDigitRows,
              message);
    }
}

void chosenFontFitsTheLayout() {
    for (uint8_t cols = 7; cols <= 22; cols++) {
        for (uint8_t rows = 5; rows <= 22; rows++) {
            for (uint8_t digitRows = 1; digitRows <= 2; digitRows++) {
                const NumberFont font = numberFontFor(cols, rows, digitRows);
                if (font.font != normalSizeASCII) {
                    continue;
                }
                snprintf(message, sizeof(message),
                         "%ux%u, %u digit rows: big digits fit", rows, cols,
                         digitRows);
                check(font.width * 2 + 1 <= cols &&
                          font.height * digitRows <= rows,
                      message);
            }
        }
    }
}

void sizesComeFromTheFontTable() {
    const NumberFont normal = NumberFont::of(normalSizeASCII);
    check(normal.width == 5 && normal.height == 7, "normal font is 5x7");
    const NumberFont small = NumberFont::of(smallSizeNumbers);
    check(small.width == 3 && small.height == 5, "small font is 3x5");
}

void glyphMatchesTheFontIndexing() {
    check(NumberFont::of(smallSizeNumbers).glyph('7') == 7,
          "small font indexes digits from 0");
    check(NumberFont::of(normalSizeASCII).glyph('7') == '7',
          "normal font indexes by ASCII");
}

} // namespace

int main() {
    fontFollowsLayoutSize();
    chosenFontFitsTheLayout();
    sizesComeFromTheFontTable();
    glyphMatchesTheFontIndexing();
    return report("number font");
}

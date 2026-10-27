#!/usr/bin/env python3
import glob
import hashlib
import os
import re
import shutil
import subprocess
import sys
import tempfile
from xml.sax.saxutils import escape

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SVG_DIR = os.path.join(ROOT, "svg")
TYPES_DIR = os.path.join(ROOT, "include", "WordClockTypes")
STATE_H = os.path.join(ROOT, "include", "WordClockState.h")

PT_PER_MM = 72 / 25.4

STRIPS = (
    {"pitch_mm": 1000 / 60, "sizes": (300, 400, 500), "border_mm": 25,
     "label": "60 LEDs/m"},
    {"pitch_mm": 1000 / 60, "sizes": (250,), "border_mm": 15,
     "rows": (10, 11), "max_cols": 11, "label": "60 LEDs/m"},
    {"pitch_mm": 2 * 1000 / 74, "sizes": (400,), "border_mm": 25,
     "rows": (10, 11), "max_cols": 11, "label": "74 LEDs/m, every second LED"},
    {"pitch_mm": 1000 / 30, "sizes": (500,), "border_mm": 25,
     "rows": (10, 11), "max_cols": 11, "label": "30 LEDs/m"},
)

REFERENCE_PITCH = 47.25
FONT_SIZE = 30
BASELINE_OFFSET = 10.0
CELL = 42.5
CELL_RADIUS = 8.5
MINUTE_DOT_RADIUS = 6.4
MINUTE_DOT_OFFSET = 0.94

TEXT_FONT = "'Helvetica'"
BENGALI_FONT = "'Kohinoor Bangla', 'Bangla MN', sans-serif"
STENCIL_FONT = "'DIN Next Stencil';font-weight:300"
STENCIL_FONT_SIZE = 34

BACKGROUNDS = ("#F70055", "#000055", "#2FE5B3", "#60BE6D", "#F7A600",
               "#0080F7", "#9B30F7", "#F75A00", "#00A3A3", "#C2185B")


def clock_types():
    entries = re.findall(r'X\((\w+), \d+, (\w+), "[^"]+"\)',
                         open(STATE_H, encoding="utf-8").read())
    headers = {}
    for path in glob.glob(os.path.join(TYPES_DIR, "*.hpp")):
        source = open(path, encoding="utf-8").read()
        for name, instance in entries:
            if re.search(r"\b" + instance + r"\s*;", source):
                headers[name] = path
    return [(name, headers[name]) for name, _ in entries if name in headers]


def read_layout(path):
    source = open(path, encoding="utf-8").read()
    start = source.index("Layout Front")
    block = source[start:source.index("*/", start)]
    rows = [m.group(1) for m in
            (re.match(r"^\s*\*\s*[0-9X]\s*\|(.*)$", line)
             for line in block.splitlines()) if m]

    match = re.search(r"colsWordMatrix\(\) override \{ return (\d+)", source)
    cols = int(match.group(1)) if match else 11

    cells = []
    for row_index, row in enumerate(rows):
        tokens = row.split()
        if len(tokens) == cols or "'" in row:
            col = 0
            for token in tokens:
                parts = token.split("'")
                for part_index, part in enumerate(parts):
                    if part_index > 0:
                        cells.append(("apostrophe", row_index, col - 0.5))
                    cells.append(("dot" if part == "*" else "letter",
                                  row_index, col, part))
                    col += 1
        else:
            text = row[1:]
            for index, char in enumerate(text):
                if char == "X":
                    cells.append(("dot", row_index, index / 2))

    has_led4x = ("hasLed4x() override { return false; }" not in source
                 and not any(cell[0] == "dot" for cell in cells))
    return rows, cols, has_led4x, cells


def front_size(cols, rows, corner_dots, pitch, sizes, border_mm):
    cell = CELL * pitch / REFERENCE_PITCH
    dots = 2 * MINUTE_DOT_OFFSET * pitch if corner_dots else 0
    width = (cols - 1) * pitch + dots + cell
    height = (rows - 1) * pitch + dots + cell

    def pick(extent):
        for size in sizes:
            if size * PT_PER_MM - extent >= 2 * border_mm * PT_PER_MM:
                return size
        return None

    width_mm, height_mm = pick(width), pick(height)
    if width_mm is None or height_mm is None:
        return None
    if max(width, height) / min(width, height) < 1.3:
        width_mm = height_mm = max(width_mm, height_mm)
    return width_mm, height_mm


def background(name):
    digest = int(hashlib.md5(name.encode()).hexdigest(), 16)
    return BACKGROUNDS[digest % len(BACKGROUNDS)]


def build_svg(name, rows, cols, has_led4x, cells, font, font_size, strip):
    pitch = strip["pitch_mm"] * PT_PER_MM
    scale = pitch / REFERENCE_PITCH
    size = front_size(cols, len(rows), has_led4x, pitch, strip["sizes"],
                      strip["border_mm"])
    if size is None:
        return None, None, None
    width_mm, height_mm = size
    width, height = width_mm * PT_PER_MM, height_mm * PT_PER_MM
    origin_x = width / 2 - (cols - 1) * pitch / 2
    origin_y = height / 2 - (len(rows) - 1) * pitch / 2
    cell_size = CELL * scale
    cell_radius = CELL_RADIUS * scale
    baseline = BASELINE_OFFSET * scale
    dot_radius = MINUTE_DOT_RADIUS * scale ** 0.5
    dot_offset = MINUTE_DOT_OFFSET * pitch
    font_size *= scale

    def centre(row, col):
        return origin_x + col * pitch, origin_y + row * pitch

    def guide(x, y):
        return (f'\t\t<rect x="{x - cell_size / 2:.2f}" '
                f'y="{y - cell_size / 2:.2f}" width="{cell_size:.2f}" '
                f'height="{cell_size:.2f}" rx="{cell_radius:.2f}"/>')

    letters, dots, guides = [], [], []
    for cell in cells:
        kind, row, col = cell[0], cell[1], cell[2]
        x, y = centre(row, col)
        if kind == "letter":
            letters.append(
                f'\t<text x="{x:.2f}" y="{y + baseline:.2f}" '
                f'class="letter">{escape(cell[3])}</text>')
        elif kind == "apostrophe":
            letters.append(
                f'\t<text x="{x:.2f}" y="{y + baseline:.2f}" '
                f'class="letter">\'</text>')
        else:
            dots.append(f'\t<circle cx="{x:.2f}" cy="{y:.2f}" '
                        f'r="{dot_radius:.2f}"/>')
        if kind != "apostrophe":
            guides.append(guide(x, y))

    if has_led4x:
        last_row, last_col = len(rows) - 1, cols - 1
        left, top = centre(0, 0)
        right, bottom = centre(last_row, last_col)
        for x, y in ((left - dot_offset, top - dot_offset),
                     (right + dot_offset, top - dot_offset),
                     (right + dot_offset, bottom + dot_offset),
                     (left - dot_offset, bottom + dot_offset)):
            dots.append(f'\t<circle cx="{x:.2f}" cy="{y:.2f}" '
                        f'r="{dot_radius:.2f}"/>')
            guides.append(guide(x, y))

    svg = f'''<?xml version="1.0" encoding="utf-8"?>
<svg version="1.1" xmlns="http://www.w3.org/2000/svg"
	 xmlns:inkscape="http://www.inkscape.org/namespaces/inkscape"
	 width="{width_mm}mm" height="{height_mm}mm" viewBox="0 0 {width:.1f} {height:.1f}">
<title>{name} {width_mm}x{height_mm} mm, {strip['label']}</title>
<style type="text/css">
	.background{{fill:{background(name)};fill-opacity:0.1647;}}
	.letter{{font-family:{font};font-size:{font_size:.1f}px;text-anchor:middle;}}
	.guide{{fill:none;stroke:#FF0000;stroke-width:0.2835;}}
</style>
<rect class="background" width="{width:.1f}" height="{height:.1f}"/>
<g id="minutes" inkscape:label="Minutes" inkscape:groupmode="layer">
{chr(10).join(dots)}
</g>
<g id="letters" inkscape:label="Letters" inkscape:groupmode="layer">
{chr(10).join(letters)}
</g>
<g id="guides" inkscape:label="Guides" inkscape:groupmode="layer" style="display:none">
	<g class="guide">
{chr(10).join(guides)}
	</g>
</g>
</svg>
'''
    return svg, width_mm, height_mm


def write_stencil(svg, target):
    with tempfile.TemporaryDirectory() as tmp:
        source = os.path.join(tmp, "front.svg")
        with open(source, "w", encoding="utf-8") as handle:
            handle.write(svg)
        subprocess.run(["inkscape", source, "--export-text-to-path",
                        f"--export-filename={target}"],
                       check=True, capture_output=True)


def main():
    args = [arg for arg in sys.argv[1:] if not arg.startswith("--")]
    force = "--force" in sys.argv
    have_inkscape = shutil.which("inkscape") is not None
    if not have_inkscape:
        print("Inkscape not found, skipping the stencil variants.")

    for name, header in clock_types():
        if args and name not in args:
            continue
        rows, cols, has_led4x, cells = read_layout(header)
        bengali = any(re.search(r"[\u0980-\u09FF]", row) for row in rows)
        font = BENGALI_FONT if bengali else TEXT_FONT

        for strip in STRIPS:
            if "rows" in strip and (len(rows) not in strip["rows"]
                                  or cols > strip["max_cols"]):
                continue
            svg, width_mm, height_mm = build_svg(name, rows, cols, has_led4x,
                                                 cells, font, FONT_SIZE, strip)
            if svg is None:
                continue
            size = f"{width_mm}x{height_mm}"
            folder = os.path.join(SVG_DIR, f"Front_{size}")

            editable_dir = os.path.join(folder, "SVG_withStandardFontEditable")
            editable = os.path.join(editable_dir, f"{name}_{size}.svg")
            if force or not os.path.exists(editable):
                os.makedirs(editable_dir, exist_ok=True)
                with open(editable, "w", encoding="utf-8") as handle:
                    handle.write(svg)
                print(f"{name}: {os.path.relpath(editable, ROOT)}")

            if bengali or not have_inkscape:
                continue
            stencil_dir = os.path.join(folder, "SVG_withStancilFontNonEditable")
            stencil = os.path.join(stencil_dir, f"{name}Stencil_{size}.svg")
            if force or not os.path.exists(stencil):
                os.makedirs(stencil_dir, exist_ok=True)
                stencil_svg, _, _ = build_svg(name, rows, cols, has_led4x,
                                              cells, STENCIL_FONT,
                                              STENCIL_FONT_SIZE, strip)
                write_stencil(stencil_svg, stencil)
                print(f"{name}: {os.path.relpath(stencil, ROOT)}")

if __name__ == "__main__":
    main()

#!/usr/bin/env python3
import glob
import hashlib
import math
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
     "rows": (10, 11), "max_cols": 11, "skip": True,
     "label": "74 LEDs/m, every second LED"},
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

STRIP_WIDTH_MM = 10
SLOT_WIDTH_MM = 6.5
SLOT_GAP_MM = 1.35
CHANNEL_WIDTH_MM = 12
OUTLINE_RADIUS_MM = 20

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
                    cells.append(("dot", row_index, math.ceil(index / 2)))

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


def mm(value):
    return value * PT_PER_MM


def arrow_head(tip, previous, length):
    dx, dy = tip[0] - previous[0], tip[1] - previous[1]
    norm = (dx * dx + dy * dy) ** 0.5 or 1
    ux, uy = dx / norm, dy / norm
    base = (tip[0] - ux * length, tip[1] - uy * length)
    half = length * 0.4
    left = (base[0] - uy * half, base[1] + ux * half)
    right = (base[0] + uy * half, base[1] - ux * half)
    return (f'\t<polygon class="head" points="{tip[0]:.2f},{tip[1]:.2f} '
            f'{left[0]:.2f},{left[1]:.2f} {right[0]:.2f},{right[1]:.2f}"/>')


def arrow_line(start, end, head):
    return [f'\t<line class="arrow" x1="{start[0]:.2f}" y1="{start[1]:.2f}" '
            f'x2="{end[0]:.2f}" y2="{end[1]:.2f}"/>',
            arrow_head(end, start, head)]


def arrow_curve(start, control1, control2, end, head):
    return [f'\t<path class="arrow" d="M{start[0]:.2f},{start[1]:.2f} '
            f'C{control1[0]:.2f},{control1[1]:.2f} '
            f'{control2[0]:.2f},{control2[1]:.2f} {end[0]:.2f},{end[1]:.2f}"/>',
            arrow_head(end, control2, head)]


def rounded_rect(css, x0, y0, x1, y1, radius=0.0):
    return (f'\t<rect class="{css}" x="{min(x0, x1):.2f}" '
            f'y="{min(y0, y1):.2f}" width="{abs(x1 - x0):.2f}" '
            f'height="{abs(y1 - y0):.2f}" rx="{radius:.2f}"/>')


def montage_key(rows, cols, has_led4x, cells):
    positions = tuple(sorted((cell[1], cell[2]) for cell in cells
                             if cell[0] != "apostrophe"))
    return (len(rows), cols, has_led4x, positions)


def build_montage(title, rows, cols, has_led4x, cells, strip):
    pitch = strip["pitch_mm"] * PT_PER_MM
    scale = pitch / REFERENCE_PITCH
    size = front_size(cols, rows, has_led4x, pitch, strip["sizes"],
                      strip["border_mm"])
    if size is None:
        return None, None, None
    width_mm, height_mm = size
    width, height = mm(width_mm), mm(height_mm)
    origin_x = width / 2 - (cols - 1) * pitch / 2
    origin_y = height / 2 - (rows - 1) * pitch / 2
    led_pitch = pitch / 2 if strip.get("skip") else pitch
    cell_size = CELL * scale
    dot_offset = MINUTE_DOT_OFFSET * pitch
    strip_half = mm(STRIP_WIDTH_MM) / 2
    slot = mm(SLOT_WIDTH_MM)
    gap = mm(SLOT_GAP_MM)
    head = mm(3)

    def back_x(col):
        return width - (origin_x + col * pitch)

    def row_y(row):
        return origin_y + row * pitch

    all_x = [back_x(col) for col in range(cols)]
    positions = {row: all_x for row in range(rows)}
    strips = {row: (min(all_x) - led_pitch / 2, max(all_x) + led_pitch / 2)
              for row in range(rows)}
    left = min(start for start, _ in strips.values())
    right = max(end for _, end in strips.values())
    first_x = min(min(xs) for xs in positions.values())
    last_x = max(max(xs) for xs in positions.values())
    left_slot = (left - gap - slot, left - gap)
    right_slot = (right + gap, right + gap + slot)
    order = sorted(strips)
    last_row = order[-1]
    ends_right = (len(order) - 1) % 2 == 0
    channel_right = not ends_right

    cut, mark, arrows = [], [], []

    cut.append(rounded_rect("cut", 0, 0, width, height, mm(OUTLINE_RADIUS_MM)))

    for row, (start, end) in strips.items():
        y = row_y(row)
        mark.append(rounded_rect("mark", start, y - strip_half, end,
                                 y + strip_half))
        for x in positions[row]:
            mark.append(f'\t<line class="mark" x1="{x:.2f}" '
                        f'y1="{y - strip_half:.2f}" x2="{x:.2f}" '
                        f'y2="{y - strip_half + mm(2):.2f}"/>')
            mark.append(f'\t<line class="mark" x1="{x:.2f}" '
                        f'y1="{y + strip_half:.2f}" x2="{x:.2f}" '
                        f'y2="{y + strip_half - mm(2):.2f}"/>')

    top_y = row_y(order[0]) - dot_offset
    bottom_y = row_y(last_row) + dot_offset
    corner_left = first_x - dot_offset
    corner_right = last_x + dot_offset
    half_cell = cell_size / 2
    if has_led4x:
        channel = mm(CHANNEL_WIDTH_MM)
        if channel_right:
            outer = max(corner_right + slot / 2, right + gap + channel)
            right_slot = (outer - channel, outer)
        else:
            outer = min(corner_left - slot / 2, left - gap - channel)
            left_slot = (outer, outer + channel)

    for index, (row, following) in enumerate(zip(order, order[1:])):
        turn_right = index % 2 == 0
        if has_led4x and turn_right == channel_right:
            continue
        x0, x1 = right_slot if turn_right else left_slot
        cut.append(rounded_rect("cut", x0, row_y(row) - strip_half, x1,
                                row_y(following) + strip_half, slot / 4))

    corners = None
    if has_led4x:
        corners = {}
        for name, x, y in (("tl", corner_left, top_y),
                           ("tr", corner_right, top_y),
                           ("bl", corner_left, bottom_y),
                           ("br", corner_right, bottom_y)):
            corners[name] = (x, y)
            mark.append(rounded_rect("mark", x - half_cell, y - half_cell,
                                     x + half_cell, y + half_cell,
                                     CELL_RADIUS * scale))
        for y in (top_y, bottom_y):
            cut.append(rounded_rect("cut", corner_left + half_cell,
                                    y - slot / 2, corner_right - half_cell,
                                    y + slot / 2, slot / 4))
        x0, x1 = right_slot if channel_right else left_slot
        cut.append(rounded_rect("cut", x0, top_y + half_cell, x1,
                                bottom_y - half_cell, slot / 4))
        x0, x1 = right_slot if ends_right else left_slot
        cut.append(rounded_rect("cut", x0, row_y(last_row) - strip_half, x1,
                                bottom_y - half_cell, slot / 4))

    first_row_y = row_y(order[0])
    if corners and not channel_right:
        x = corner_left - half_cell - mm(8)
        arrows += arrow_line((x, first_row_y),
                             (strips[order[0]][0], first_row_y), head)
        arrows.append(f'\t<text class="label" x="{x:.2f}" '
                      f'y="{first_row_y - mm(2):.2f}">DIN</text>')
    else:
        feed = (corner_left, first_row_y)
        cut.append(rounded_rect("cut", feed[0] - half_cell,
                                feed[1] - half_cell, feed[0] + half_cell,
                                feed[1] + half_cell, CELL_RADIUS * scale))
        arrows += arrow_line((feed[0], feed[1]),
                             (strips[order[0]][0], feed[1]), head)
        arrows.append(f'\t<text class="label" x="{feed[0]:.2f}" '
                      f'y="{feed[1] - mm(2):.2f}">DIN</text>')

    for index, row in enumerate(order):
        start, end = strips[row]
        y = row_y(row)
        middle = (start + end) / 2
        length = min(2 * pitch, (end - start) / 3)
        if index % 2 == 0:
            arrows += arrow_line((middle - length / 2, y),
                                 (middle + length / 2, y), head)
        else:
            arrows += arrow_line((middle + length / 2, y),
                                 (middle - length / 2, y), head)

    for index, (row, following) in enumerate(zip(order, order[1:])):
        if index % 2 == 0:
            x_from, x_to = strips[row][1], strips[following][1]
            bulge = (right_slot[0] + right_slot[1]) / 2
        else:
            x_from, x_to = strips[row][0], strips[following][0]
            bulge = (left_slot[0] + left_slot[1]) / 2
        reach = x_from + (bulge - x_from) / 0.75
        arrows += arrow_curve((x_from, row_y(row)), (reach, row_y(row)),
                              (reach, row_y(following)),
                              (x_to, row_y(following)), head)

    if corners:
        end_x = strips[last_row][1] if ends_right else strips[last_row][0]
        chain = (["br", "bl", "tl", "tr"] if ends_right
                 else ["bl", "br", "tr", "tl"])
        first = corners[chain[0]]
        arrows += arrow_curve((end_x, row_y(last_row)),
                              (first[0], row_y(last_row)),
                              (first[0], row_y(last_row)),
                              (first[0], first[1] - half_cell), head)
        for a, b in zip(chain, chain[1:]):
            (ax, ay), (bx, by) = corners[a], corners[b]
            if ay == by:
                step = half_cell if bx > ax else -half_cell
                arrows += arrow_line((ax + step, ay), (bx - step, by), head)
            else:
                step = half_cell if by > ay else -half_cell
                arrows += arrow_line((ax, ay + step), (bx, by - step), head)
        for number, name in enumerate(chain, start=1):
            x, y = corners[name]
            arrows.append(f'\t<text class="label" x="{x:.2f}" '
                          f'y="{y + mm(1.5):.2f}">{number}</text>')

    arrows.append(f'\t<text class="caption" x="{width / 2:.2f}" '
                  f'y="{height - mm(8):.2f}">{escape(title)} · '
                  f'{width_mm} × {height_mm} mm · {strip["label"]} · '
                  f'Rückseite / rear view</text>')

    svg = f'''<?xml version="1.0" encoding="utf-8"?>
<svg version="1.1" xmlns="http://www.w3.org/2000/svg"
	 xmlns:inkscape="http://www.inkscape.org/namespaces/inkscape"
	 width="{width_mm}mm" height="{height_mm}mm" viewBox="0 0 {width:.1f} {height:.1f}">
<title>Montage {escape(title)} {width_mm}x{height_mm} mm, {strip["label"]}</title>
<style type="text/css">
	.cut{{fill:none;stroke:#FF0000;stroke-width:0.236224;}}
	.mark{{fill:none;stroke:#00FF00;stroke-width:0.236224;}}
	.arrow{{fill:none;stroke:#000000;stroke-width:0.75;}}
	.head{{fill:#000000;}}
	.label{{font-family:'Helvetica';font-size:{mm(4):.1f}px;text-anchor:middle;}}
	.caption{{font-family:'Helvetica';font-size:{mm(3.5):.1f}px;text-anchor:middle;}}
</style>
<g id="montage" inkscape:label="Montage" inkscape:groupmode="layer">
{chr(10).join(cut + mark)}
</g>
<g id="direction" inkscape:label="Richtung" inkscape:groupmode="layer">
{chr(10).join(arrows)}
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

    montages = {}
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
            key = (STRIPS.index(strip),) + montage_key(rows, cols, has_led4x,
                                                       cells)
            full = len(key[4]) == len(rows) * cols
            title = f"{len(rows)}x{cols}" if full else name
            if full and not has_led4x:
                if any(cell[0] == "dot" for cell in cells):
                    title += " Minuten in der Reihe"
                else:
                    title += " ohne Minuten-LEDs"
            montages.setdefault(key, (title, rows, cols, has_led4x, cells,
                                      strip))
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

    for title, rows, cols, has_led4x, cells, strip in montages.values():
        svg, width_mm, height_mm = build_montage(title, len(rows), cols,
                                                 has_led4x, cells, strip)
        size = f"{width_mm}x{height_mm}"
        slug = title.replace(" ", "_")
        target = os.path.join(SVG_DIR, f"Front_{size}", "Montage",
                              f"Montage_{slug}_{size}.svg")
        if force or not os.path.exists(target):
            os.makedirs(os.path.dirname(target), exist_ok=True)
            with open(target, "w", encoding="utf-8") as handle:
                handle.write(svg)
            print(f"{title}: {os.path.relpath(target, ROOT)}")

if __name__ == "__main__":
    main()

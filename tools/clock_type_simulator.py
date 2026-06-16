#!/usr/bin/env python3
"""Render ClockType front matrix definitions from the C++ headers.

The simulator intentionally reads the existing `include/WordClockTypes/*.hpp` files
instead of duplicating the matrix definitions in Python. This makes it useful
for checking coordinate changes while editing an ClockType.
"""

from __future__ import annotations

import argparse
import re
import sys
from dataclasses import dataclass, field
from pathlib import Path
from typing import Iterable


REPO_ROOT = Path(__file__).resolve().parents[1]
CLOCK_TYPES_DIR = REPO_ROOT / "include" / "WordClockTypes"

DEFAULT_CAPABILITIES = {
    "hasAQuarter": False,
    "hasDreiviertel": False,
    "hasQuarterTen": True,
    "hasTwenty": True,
    "hasTwentyfive": False,
    "hasThirtyfive": False,
    "hasForty": False,
    "hasFifty": False,
    "hasFiftyFive": False,
    "hasMitternacht": False,
    "has24HourLayout": False,
    "has60MinuteLayout": False,
    "hasOnlyQuarterLayout": False,
    "hasWeatherLayout": False,
    "hasSecondsFrame": False,
    "hasDaytimeWords": False,
    "hasLed4x": True,
    "hasMinuteInWords": False,
    "hasSpecialWordBeen": False,
    "hasSpecialWordHappyBirthday": False,
}

ROMAN_HALF_AFTER = {
    "EN08X08",
    "EN10X11",
    "EN11X12",
    "IT10X11",
    "RO10X11",
    "ES08X08CUARTO",
    "ES10X11",
}

HALF_WITHOUT_OFFSET = {"FR10X11", "RU10X11"}

MINUTE_VARIANTS = {
    "off": 0,
    "led4x": 1,
    "led7x": 2,
    "corners": 3,
    "in-words": 4,
}


@dataclass(frozen=True)
class Action:
    row: int
    start: int
    end: int
    state: bool = True


@dataclass
class ClockType:
    name: str
    class_name: str
    path: Path
    rows: int
    cols: int
    lang: str
    layout: list[str]
    actions: dict[str, list[Action]] = field(default_factory=dict)
    capabilities: dict[str, bool] = field(default_factory=dict)

    def has(self, capability: str) -> bool:
        if capability == "hasMinuteCorners":
            return self.rows == 11
        if capability == "hasLed7x":
            return self.capabilities.get(capability, self.has("hasLed4x"))
        return self.capabilities.get(capability, DEFAULT_CAPABILITIES[capability])

    def supports_minute_variant(self, variant: str) -> bool:
        if variant == "off":
            return True
        if variant == "led4x":
            return self.has("hasLed4x")
        if variant == "led7x":
            return self.has("hasLed7x")
        if variant == "corners":
            return self.has("hasMinuteCorners")
        if variant == "in-words":
            return self.has("hasMinuteInWords")
        return False

    def supported_minute_variant_mask(self) -> int:
        mask = 0
        for variant, bit in MINUTE_VARIANTS.items():
            if self.supports_minute_variant(variant):
                mask |= 1 << bit
        return mask

    @property
    def normalized_name(self) -> str:
        return normalize_name(self.name)

    def resolve_word(self, word: str) -> str:
        wanted = normalize_word(word)
        for known in self.actions:
            if normalize_word(known) == wanted:
                return known
        raise KeyError(word)

    def show(self, words: Iterable[str]) -> tuple[set[tuple[int, int]], list[str]]:
        lit: set[tuple[int, int]] = set()
        missing: list[str] = []

        for word in words:
            try:
                known_word = self.resolve_word(word)
            except KeyError:
                missing.append(word)
                continue

            for action in self.actions[known_word]:
                for col in range(action.start, action.end + 1):
                    cell = (action.row, col)
                    if action.state:
                        lit.add(cell)
                    else:
                        lit.discard(cell)

        return lit, missing

    def active_letters(self, lit: set[tuple[int, int]]) -> str:
        letters: list[str] = []
        for row_index, row in enumerate(self.layout):
            for display_col, char in enumerate(row):
                code_col = self.cols - display_col - 1
                if (row_index, code_col) in lit and char != " ":
                    letters.append(char)
        return "".join(letters)

    def render(
        self,
        lit: set[tuple[int, int]],
        *,
        show_inactive_letters: bool = False,
        row_numbers: bool = True,
    ) -> str:
        rendered: list[str] = []
        for row_index in range(self.rows):
            row = self.layout[row_index] if row_index < len(self.layout) else ""
            row = row.ljust(self.cols)[: self.cols]
            cells: list[str] = []
            for display_col, char in enumerate(row):
                code_col = self.cols - display_col - 1
                active = (row_index, code_col) in lit
                if active:
                    cells.append(char if char != " " else "#")
                elif show_inactive_letters:
                    cells.append(char.lower() if char != " " else " ")
                else:
                    cells.append("." if char != " " else " ")

            prefix = f"{row_index:02d}  " if row_numbers else ""
            rendered.append(prefix + " ".join(cells))
        return "\n".join(rendered)


def normalize_name(value: str) -> str:
    return re.sub(r"[^A-Z0-9]", "", value.upper())


def normalize_word(value: str) -> str:
    value = value.strip()
    value = value.removeprefix("FrontWord::")
    return value.lower()


def parse_int(value: str) -> int:
    value = value.strip()
    if not re.fullmatch(r"\d+", value):
        raise ValueError(f"Unsupported non-literal coordinate: {value!r}")
    return int(value)


def parse_layout(text: str, rows: int, cols: int) -> list[str]:
    layout: list[str] = []
    in_layout = False

    for line in text.splitlines():
        if "Layout Front" in line:
            in_layout = True
            continue
        if not in_layout:
            continue

        match = re.search(r"\|\s*(.*?)\s*$", line)
        if not match:
            if layout and line.strip().endswith("*/"):
                break
            continue

        raw = match.group(1)
        chars = "".join(raw.split())
        if not chars:
            continue
        if not re.search(r"[A-Za-zÄÖÜäöüẞß°]", chars):
            continue

        layout.append(chars.ljust(cols)[:cols])
        if len(layout) == rows:
            break

    return layout


def parse_actions(text: str) -> dict[str, list[Action]]:
    actions: dict[str, list[Action]] = {}
    active_cases: list[str] = []

    for line in text.splitlines():
        case_match = re.search(r"case\s+FrontWord::([A-Za-z0-9_]+)\s*:", line)
        if case_match:
            active_cases.append(case_match.group(1))
            continue

        if not active_cases:
            continue

        word_match = re.search(
            r"setFrontMatrixWord\(\s*([^,]+)\s*,\s*([^,]+)\s*,\s*([^)]+)\)",
            line,
        )
        pixel_match = re.search(
            r"setFrontMatrixPixel\(\s*([^,]+)\s*,\s*([^,\)]+)(?:,\s*([^)]+))?\)",
            line,
        )

        try:
            if word_match:
                action = Action(
                    row=parse_int(word_match.group(1)),
                    start=parse_int(word_match.group(2)),
                    end=parse_int(word_match.group(3)),
                )
            elif pixel_match:
                row = parse_int(pixel_match.group(1))
                col = parse_int(pixel_match.group(2))
                state_arg = pixel_match.group(3)
                action = Action(
                    row=row,
                    start=col,
                    end=col,
                    state=(state_arg is None or state_arg.strip() != "false"),
                )
            else:
                action = None
        except ValueError:
            action = None

        if action:
            for case_name in active_cases:
                actions.setdefault(case_name, []).append(action)

        if re.search(r"\bbreak\s*;", line):
            active_cases = []
        elif re.search(r"\bdefault\s*:", line):
            active_cases = []

    return actions


def parse_clock_type(path: Path) -> ClockType | None:
    if path.name == "ClockType.hpp":
        return None

    text = path.read_text(encoding="utf-8")
    class_match = re.search(r"class\s+(\w+)_t\s*:\s*public\s+ClockType", text)
    if not class_match:
        return None

    class_stem = class_match.group(1)
    rows_match = re.search(r"rowsWordMatrix\(\)\s*override\s*\{\s*return\s+(\d+)\s*;", text)
    cols_match = re.search(r"colsWordMatrix\(\)\s*override\s*\{\s*return\s+(\d+)\s*;", text)
    lang_match = re.search(r"LanguageAbbreviation::([A-Z]+)", text)

    rows = int(rows_match.group(1)) if rows_match else 10
    cols = int(cols_match.group(1)) if cols_match else 11
    capabilities = DEFAULT_CAPABILITIES.copy()
    for cap, value in re.findall(
        r"const\s+bool\s+(has\w+)\(\)\s*(?:override\s*)?\{\s*return\s+(true|false)\s*;",
        text,
    ):
        capabilities[cap] = value == "true"

    return ClockType(
        name=path.stem.replace(".", ""),
        class_name=f"{class_stem}_t",
        path=path,
        rows=rows,
        cols=cols,
        lang=lang_match.group(1) if lang_match else "?",
        layout=parse_layout(text, rows, cols),
        actions=parse_actions(text),
        capabilities=capabilities,
    )


def load_clock_types() -> list[ClockType]:
    return sorted(
        (u for path in CLOCK_TYPES_DIR.glob("*.hpp") if (u := parse_clock_type(path))),
        key=lambda clock_type: clock_type.name.lower(),
    )


def find_clock_type(clock_types: list[ClockType], requested: str) -> ClockType:
    wanted = normalize_name(requested)
    exact = [u for u in clock_types if normalize_name(u.name) == wanted]
    if exact:
        return exact[0]

    matches = [
        u
        for u in clock_types
        if wanted in normalize_name(u.name) or wanted in normalize_name(u.class_name)
    ]
    if len(matches) == 1:
        return matches[0]
    if not matches:
        raise SystemExit(f"Unknown ClockType {requested!r}. Use --list to see names.")

    names = ", ".join(u.name for u in matches)
    raise SystemExit(f"Ambiguous ClockType {requested!r}: {names}")


def rounded_minute(minute: int) -> int:
    return (minute // 5) * 5


def set_daytime_words(clock_type: ClockType, hour: int, words: list[str]) -> None:
    if not clock_type.has("hasDaytimeWords"):
        return

    hour %= 24
    if hour == 0:
        words.append("mitternachts")
    elif hour < 5:
        words.append("day_night")
    elif hour < 10:
        words.extend(["day_morning", "day_early"])
    elif hour < 12:
        words.append("day_morning")
    elif hour < 14:
        words.append("day_noon")
    elif hour >= 18:
        words.append("day_evening")


def has_twenty_and_check_for_usage(
    clock_type: ClockType, *, use_twenty: bool, use_forty: bool
) -> bool:
    return clock_type.has("hasTwenty") and (use_twenty or use_forty or clock_type.lang == "ES")


def has_dreiviertel_and_check_for_usage(clock_type: ClockType, *, use_45: bool) -> bool:
    if not clock_type.has("hasDreiviertel"):
        return False
    return clock_type.lang != "DE" or use_45


def add_number_word(number: int, words: list[str]) -> None:
    if 1 <= number <= 59:
        words.append(f"min_{number}")


def show_minute(
    clock_type: ClockType,
    minute: int,
    words: list[str],
    *,
    minute_variant: str,
) -> None:
    residual = minute % 5
    if not residual:
        return
    if clock_type.has("hasSpecialWordBeen"):
        words.append("nur" if residual == 0 else "gewesen")
    if clock_type.has("hasMinuteInWords") and minute_variant == "in-words":
        words.extend(["plus", "minute"])
        if residual > 1:
            words.append("minuten")
        words.append(f"m_num{residual}")


def set_minute(
    clock_type: ClockType,
    minute: int,
    words: list[str],
    *,
    minute_variant: str,
    use_15: bool,
    use_20: bool,
    use_40: bool,
    use_45: bool,
    show_a_quarter: bool,
) -> tuple[int, bool]:
    offset_hour = 0
    full_hour = False

    if clock_type.has("has60MinuteLayout"):
        words.append("uhr")
        if minute == 0:
            full_hour = True
        else:
            add_number_word(minute, words)
            words.append("und")
            words.append("minute" if minute == 1 else "minuten")
        return offset_hour, full_hour

    if clock_type.has("hasOnlyQuarterLayout"):
        if 8 <= minute <= 22:
            if use_15:
                words.append("viertel")
                offset_hour = 1
            else:
                if show_a_quarter:
                    words.append("a_quarter")
                words.extend(["viertel", "v_nach"])
        elif 23 <= minute <= 37:
            if clock_type.normalized_name in ROMAN_HALF_AFTER:
                words.extend(["halb", "nach"])
            elif clock_type.normalized_name in HALF_WITHOUT_OFFSET:
                words.append("halb")
            else:
                words.append("halb")
                offset_hour = 1
        elif 38 <= minute <= 52:
            if has_dreiviertel_and_check_for_usage(clock_type, use_45=use_45):
                words.append("dreiviertel")
            else:
                if show_a_quarter:
                    words.append("a_quarter")
                words.extend(["viertel", "v_vor"])
            offset_hour = 1
        elif 53 <= minute <= 59:
            offset_hour = 1
        return offset_hour, full_hour

    show_minute(clock_type, minute, words, minute_variant=minute_variant)
    minute = rounded_minute(minute)
    is_turkish = clock_type.lang == "TR"

    if minute == 0:
        words.append("uhr")
        full_hour = True
    elif 1 <= minute <= 10:
        add_number_word(minute, words)
        if clock_type.normalized_name != "FR10X11":
            words.append("nach")
    elif 11 <= minute <= 14:
        add_number_word(minute, words)
        words.append("nach")
    elif minute == 15:
        if is_turkish:
            words.extend(["viertel", "nach"])
        elif use_15:
            words.append("viertel")
            offset_hour = 1
        else:
            if show_a_quarter:
                words.append("a_quarter")
            words.extend(["viertel", "v_nach"])
    elif 16 <= minute <= 19:
        add_number_word(minute, words)
        words.append("nach")
    elif minute == 20:
        if is_turkish or has_twenty_and_check_for_usage(
            clock_type, use_twenty=use_20, use_forty=use_40
        ):
            words.append("min_20")
            if clock_type.normalized_name != "FR10X11":
                words.append("nach")
        else:
            words.extend(["min_10", "vor", "halb"])
            offset_hour = 1
    elif 21 <= minute <= 25:
        if clock_type.has("hasTwentyfive"):
            words.append("min_25")
            if clock_type.normalized_name != "FR10X11":
                words.append("nach")
        else:
            words.extend(["min_5", "vor", "halb"])
            offset_hour = 1
    elif 26 <= minute <= 28:
        add_number_word(30 - minute, words)
        words.extend(["vor", "halb"])
        offset_hour = 1
    elif minute == 29:
        words.extend(["min_1", "minute", "vor", "halb"])
        offset_hour = 1
    elif minute == 30:
        if is_turkish:
            words.append("halb")
            full_hour = True
        elif clock_type.normalized_name in ROMAN_HALF_AFTER:
            words.extend(["halb", "nach"])
        elif clock_type.normalized_name in HALF_WITHOUT_OFFSET:
            words.append("halb")
        else:
            words.append("halb")
            offset_hour = 1
    elif minute == 31:
        words.extend(["min_1", "minute", "nach", "halb"])
        offset_hour = 1
    elif 32 <= minute <= 35:
        if clock_type.has("hasThirtyfive"):
            words.extend(["min_35", "nach"])
        elif clock_type.has("hasTwentyfive"):
            words.extend(["min_25", "vor"])
            offset_hour = 1
        else:
            words.extend(["min_5", "nach", "halb"])
            offset_hour = 1
    elif 36 <= minute <= 39:
        add_number_word(minute - 30, words)
        words.extend(["nach", "halb"])
        offset_hour = 1
    elif minute == 40:
        if is_turkish:
            words.extend(["min_40", "nach"])
        elif clock_type.has("hasForty"):
            words.append("min_40")
        elif has_twenty_and_check_for_usage(clock_type, use_twenty=use_20, use_forty=use_40):
            words.extend(["min_20", "vor"])
        else:
            words.extend(["min_10", "nach", "halb"])
        if not is_turkish:
            offset_hour = 1
    elif 41 <= minute <= 44:
        add_number_word(60 - minute, words)
        words.append("vor")
        offset_hour = 1
    elif minute == 45:
        if is_turkish:
            words.extend(["min_45", "nach"])
        elif has_dreiviertel_and_check_for_usage(clock_type, use_45=use_45):
            words.append("dreiviertel")
        else:
            if show_a_quarter:
                words.append("a_quarter")
            words.extend(["viertel", "v_vor"])
        if not is_turkish:
            offset_hour = 1
    elif 46 <= minute <= 50:
        if is_turkish:
            words.extend(["min_50", "nach"])
        elif clock_type.has("hasFifty"):
            words.append("min_50")
        else:
            add_number_word(60 - minute, words)
            words.append("vor")
        if not is_turkish:
            offset_hour = 1
    elif 51 <= minute <= 55:
        if is_turkish:
            words.extend(["min_55", "nach"])
        elif clock_type.has("hasFiftyFive"):
            words.append("min_55")
        else:
            add_number_word(60 - minute, words)
            words.append("vor")
        if not is_turkish:
            offset_hour = 1
    elif 56 <= minute <= 58:
        add_number_word(60 - minute, words)
        words.extend(["minuten", "vor"])
        offset_hour = 1
    elif minute == 59:
        words.extend(["min_1", "minute", "vor"])
        offset_hour = 1

    if clock_type.normalized_name == "RU10X11":
        offset_hour = 0
    return offset_hour, full_hour


def set_hour(clock_type: ClockType, hour: int, full_hour: bool, words: list[str]) -> None:
    midnight = clock_type.has("hasMitternacht") and hour % 24 == 0
    if not clock_type.has("has24HourLayout"):
        hour %= 12

    if clock_type.lang == "TR" and full_hour:
        words.append("hour_0" if hour == 0 else f"hour_{hour + 12}")
    elif hour == 0:
        words.append("hour_0" if midnight else "hour_12")
    elif hour == 1:
        if full_hour or clock_type.lang != "DE":
            words.append("hour_1")
        else:
            words.append("eins")
    else:
        words.append(f"hour_{hour}")


def it_is_visible(minute: int, variant: str) -> bool:
    minute = rounded_minute(minute)
    if variant == "permanent":
        return True
    if variant == "hourly":
        return minute == 0
    if variant == "half-hourly":
        return minute % 30 == 0
    if variant == "quarterly":
        return minute % 15 == 0
    return False


def set_it_is(clock_type: ClockType, hour: int, minute: int, words: list[str], variant: str) -> None:
    if not it_is_visible(minute, variant):
        return

    hour %= 12
    if clock_type.normalized_name == "RU10X11":
        if hour == 1:
            words.append("es_ist__singular__")
        elif hour in {2, 3, 4}:
            words.append("es_ist___plural___")
        else:
            words.append("es_ist")
    elif clock_type.normalized_name == "ES10X11" and hour == 1 and rounded_minute(minute) < 35:
        words.append("es_ist___plural___")
    elif clock_type.normalized_name == "ES10X11" and hour == 0 and rounded_minute(minute) > 34:
        words.append("es_ist___plural___")
    else:
        words.append("es_ist")


def words_for_time(
    clock_type: ClockType,
    hour: int,
    minute: int,
    *,
    minute_variant: str,
    it_is_variant: str,
    use_15: bool,
    use_20: bool,
    use_40: bool,
    use_45: bool,
    show_a_quarter: bool,
) -> list[str]:
    words: list[str] = []
    offset_hour, full_hour = set_minute(
        clock_type,
        minute,
        words,
        minute_variant=minute_variant,
        use_15=use_15,
        use_20=use_20,
        use_40=use_40,
        use_45=use_45,
        show_a_quarter=show_a_quarter,
    )
    set_hour(clock_type, hour + offset_hour, full_hour, words)
    set_daytime_words(clock_type, hour + offset_hour, words)
    set_it_is(clock_type, hour + offset_hour, minute, words, it_is_variant)
    return words


def parse_time(value: str) -> tuple[int, int]:
    match = re.fullmatch(r"(\d{1,2}):(\d{2})", value.strip())
    if not match:
        raise argparse.ArgumentTypeError("Use HH:MM, for example 12:00")
    hour = int(match.group(1))
    minute = int(match.group(2))
    if not (0 <= hour <= 23 and 0 <= minute <= 59):
        raise argparse.ArgumentTypeError("Time must be within 00:00..23:59")
    return hour, minute


def print_available_words(clock_type: ClockType) -> None:
    for word in sorted(clock_type.actions):
        print(word)


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Render an ClockType definition from include/WordClockTypes/*.hpp."
    )
    parser.add_argument("clock_type", nargs="?", help="ClockType name, e.g. DE16x18")
    parser.add_argument("--list", action="store_true", help="List parsed WordClockTypes")
    parser.add_argument("--words", nargs="+", help="FrontWord names to render")
    parser.add_argument("--word", action="append", help="FrontWord name to render")
    parser.add_argument("--available-words", action="store_true", help="List words for an ClockType")
    parser.add_argument("--time", type=parse_time, help="Render a clock time as HH:MM")
    parser.add_argument(
        "--it-is",
        choices=["permanent", "quarterly", "half-hourly", "hourly", "off"],
        default="permanent",
        help="When to include the ES IST words for --time",
    )
    parser.add_argument(
        "--minute-variant",
        choices=["off", "led4x", "led7x", "corners", "in-words"],
        default="off",
        help="Minute variant for residual minutes",
    )
    parser.add_argument("--use-15", action="store_true", help="Use 'viertel' to next hour")
    parser.add_argument("--use-20", action="store_true", help="Prefer direct 'twenty past'")
    parser.add_argument("--use-40", action="store_true", help="Prefer direct 'twenty to'")
    parser.add_argument("--use-45", action="store_true", help="Use 'dreiviertel' where supported")
    parser.add_argument("--a-quarter", action="store_true", help="Show 'a quarter' on layouts that have it")
    parser.add_argument("--show-inactive", action="store_true", help="Show inactive letters in lowercase")
    parser.add_argument("--no-row-numbers", action="store_true", help="Hide row numbers")
    parser.add_argument("--active-letters", action="store_true", help="Print active letters as one string")

    args = parser.parse_args()
    clock_types = load_clock_types()

    if args.list:
        for clock_type in clock_types:
            marker = "layout" if clock_type.layout else "no-layout-comment"
            print(f"{clock_type.name:28} {clock_type.rows:2}x{clock_type.cols:<2} {clock_type.lang:2} {marker}")
        return 0

    if not args.clock_type:
        parser.error("clock_type is required unless --list is used")

    clock_type = find_clock_type(clock_types, args.clock_type)
    if not clock_type.layout:
        raise SystemExit(f"{clock_type.name} has no parseable layout comment to render.")

    if args.available_words:
        print_available_words(clock_type)
        return 0

    words: list[str] = []
    if args.words:
        words.extend(args.words)
    if args.word:
        words.extend(args.word)
    if args.time:
        hour, minute = args.time
        words.extend(
            words_for_time(
                clock_type,
                hour,
                minute,
                minute_variant=args.minute_variant,
                it_is_variant=args.it_is,
                use_15=args.use_15,
                use_20=args.use_20,
                use_40=args.use_40,
                use_45=args.use_45,
                show_a_quarter=args.a_quarter,
            )
        )

    if not words:
        parser.error("use --word/--words, --time, or --available-words")

    lit, missing = clock_type.show(words)
    print(f"{clock_type.name} ({clock_type.rows}x{clock_type.cols})")
    print("words:", " ".join(words))
    if missing:
        print("missing mappings:", " ".join(missing), file=sys.stderr)
    if args.active_letters:
        print("active:", clock_type.active_letters(lit))
    print(
        clock_type.render(
            lit,
            show_inactive_letters=args.show_inactive,
            row_numbers=not args.no_row_numbers,
        )
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

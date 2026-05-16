#!/usr/bin/env python3
"""Local web GUI for the Uhrtype simulator."""

from __future__ import annotations

import argparse
import html
import json
import sys
import webbrowser
from http import HTTPStatus
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
from pathlib import Path
from urllib.parse import parse_qs, urlparse

sys.dont_write_bytecode = True
sys.path.insert(0, str(Path(__file__).resolve().parent))

import uhrtype_simulator as sim


HTML = """<!doctype html>
<html lang="de">
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Uhrtype Simulator</title>
  <style>
    :root {
      color-scheme: dark;
      --bg: #17191c;
      --panel: #20242a;
      --panel-2: #272c33;
      --line: #3a424b;
      --text: #f2f5f7;
      --muted: #aeb7c2;
      --lit: #ffd257;
      --lit-text: #1b1608;
      --accent: #5fb3a7;
      --danger: #f08d77;
    }

    * { box-sizing: border-box; }

    body {
      margin: 0;
      min-height: 100vh;
      background: var(--bg);
      color: var(--text);
      font: 15px/1.4 system-ui, -apple-system, BlinkMacSystemFont, "Segoe UI", sans-serif;
    }

    .app {
      min-height: 100vh;
      display: grid;
      grid-template-columns: minmax(280px, 360px) minmax(0, 1fr);
    }

    aside {
      border-right: 1px solid var(--line);
      background: var(--panel);
      padding: 18px;
      display: flex;
      flex-direction: column;
      gap: 18px;
    }

    main {
      display: flex;
      flex-direction: column;
      min-width: 0;
    }

    header {
      min-height: 76px;
      border-bottom: 1px solid var(--line);
      padding: 16px 22px;
      display: flex;
      align-items: center;
      justify-content: space-between;
      gap: 16px;
    }

    h1 {
      margin: 0;
      font-size: 20px;
      font-weight: 700;
      letter-spacing: 0;
    }

    .subline {
      color: var(--muted);
      font-size: 13px;
      margin-top: 3px;
    }

    .clock {
      font-variant-numeric: tabular-nums;
      font-size: clamp(36px, 8vw, 76px);
      font-weight: 800;
      line-height: 1;
      color: var(--lit);
    }

    .field {
      display: grid;
      gap: 7px;
    }

    label, .label {
      color: var(--muted);
      font-size: 12px;
      text-transform: uppercase;
      letter-spacing: .08em;
      font-weight: 700;
    }

    select, input[type="time"], input[type="range"], button {
      width: 100%;
      color: var(--text);
      border: 1px solid var(--line);
      background: var(--panel-2);
      border-radius: 7px;
      font: inherit;
    }

    select, input[type="time"] {
      height: 42px;
      padding: 0 12px;
    }

    input[type="range"] {
      accent-color: var(--accent);
    }

    .steps {
      display: grid;
      grid-template-columns: repeat(4, 1fr);
      gap: 8px;
    }

    button {
      min-height: 38px;
      cursor: pointer;
      font-weight: 700;
    }

    button:hover {
      border-color: var(--accent);
    }

    .toggles {
      display: grid;
      grid-template-columns: repeat(2, minmax(0, 1fr));
      gap: 8px;
    }

    .toggle {
      min-height: 38px;
      display: flex;
      align-items: center;
      gap: 8px;
      padding: 8px 10px;
      border: 1px solid var(--line);
      border-radius: 7px;
      background: var(--panel-2);
      color: var(--text);
      font-weight: 650;
    }

    .toggle input {
      width: 16px;
      height: 16px;
      accent-color: var(--accent);
    }

    .matrix-wrap {
      flex: 1;
      min-height: 0;
      padding: 22px;
      display: grid;
      align-items: center;
      justify-items: center;
      overflow: auto;
    }

    .matrix {
      display: grid;
      gap: 6px;
      padding: 18px;
      border: 1px solid var(--line);
      background: #111315;
      border-radius: 8px;
      width: min(100%, 980px);
    }

    .row {
      display: grid;
      gap: 6px;
    }

    .cell {
      aspect-ratio: 1;
      min-width: 0;
      display: grid;
      place-items: center;
      border: 1px solid #252a30;
      background: #1c2025;
      color: #59636f;
      border-radius: 6px;
      font-weight: 800;
      font-size: clamp(11px, 2.2vw, 24px);
      line-height: 1;
      font-variant-numeric: tabular-nums;
    }

    .cell.active {
      background: var(--lit);
      border-color: #ffe49a;
      color: var(--lit-text);
      box-shadow: 0 0 16px rgba(255, 210, 87, .28);
    }

    .meta {
      border-top: 1px solid var(--line);
      padding: 12px 22px;
      display: flex;
      gap: 16px;
      justify-content: space-between;
      color: var(--muted);
      font-size: 13px;
      min-height: 44px;
    }

    .words {
      color: var(--text);
      overflow-wrap: anywhere;
    }

    .warning {
      color: var(--danger);
    }

    @media (max-width: 760px) {
      .app {
        grid-template-columns: 1fr;
      }

      aside {
        border-right: 0;
        border-bottom: 1px solid var(--line);
      }

      header, .matrix-wrap, .meta {
        padding-left: 14px;
        padding-right: 14px;
      }

      .matrix {
        gap: 4px;
        padding: 10px;
      }

      .row {
        gap: 4px;
      }

      .cell {
        border-radius: 4px;
      }
    }
  </style>
</head>
<body>
  <div class="app">
    <aside>
      <div>
        <h1>Uhrtype Simulator</h1>
        <div class="subline" id="layoutMeta">-</div>
      </div>

      <div class="field">
        <label for="uhrtype">Uhrtype</label>
        <select id="uhrtype"></select>
      </div>

      <div class="field">
        <label for="time">Uhrzeit</label>
        <input id="time" type="time" step="60" value="12:00">
        <input id="timeRange" type="range" min="0" max="1439" step="1" value="720">
      </div>

      <div class="steps">
        <button data-step="-60">-1h</button>
        <button data-step="-5">-5m</button>
        <button data-step="5">+5m</button>
        <button data-step="60">+1h</button>
      </div>

      <div class="field">
        <div class="label">Varianten</div>
        <div class="toggles">
          <label class="toggle"><input id="use15" type="checkbox">Viertel</label>
          <label class="toggle"><input id="use45" type="checkbox">Dreiviertel</label>
          <label class="toggle"><input id="use20" type="checkbox">20 nach</label>
          <label class="toggle"><input id="use40" type="checkbox">20 vor</label>
        </div>
      </div>

      <div class="field">
        <label for="itIs">ES IST</label>
        <select id="itIs">
          <option value="permanent">Immer</option>
          <option value="quarterly">Viertelstuendlich</option>
          <option value="half-hourly">Halbstuendlich</option>
          <option value="hourly">Stuendlich</option>
          <option value="off">Aus</option>
        </select>
      </div>

      <div class="field">
        <label for="minuteVariant">Minuten</label>
        <select id="minuteVariant">
          <option value="off">Aus</option>
          <option value="led4x">LED4x</option>
          <option value="led7x">LED7x</option>
          <option value="corners">Ecken</option>
          <option value="in-words">Worte</option>
        </select>
      </div>
    </aside>

    <main>
      <header>
        <div>
          <h1 id="title">-</h1>
          <div class="subline" id="activeLetters">-</div>
        </div>
        <div class="clock" id="clock">12:00</div>
      </header>

      <div class="matrix-wrap">
        <div class="matrix" id="matrix"></div>
      </div>

      <div class="meta">
        <div class="words" id="words">-</div>
        <div class="warning" id="warning"></div>
      </div>
    </main>
  </div>

  <script>
    const state = {
      uhrtypes: [],
      selected: "",
      requestId: 0
    };

    const $ = (id) => document.getElementById(id);
    const controls = ["uhrtype", "time", "timeRange", "use15", "use20", "use40", "use45", "itIs", "minuteVariant"];

    function minutesToTime(total) {
      total = ((total % 1440) + 1440) % 1440;
      const h = Math.floor(total / 60).toString().padStart(2, "0");
      const m = (total % 60).toString().padStart(2, "0");
      return `${h}:${m}`;
    }

    function timeToMinutes(value) {
      const [h, m] = value.split(":").map(Number);
      return h * 60 + m;
    }

    function selectedMeta() {
      return state.uhrtypes.find((item) => item.name === $("uhrtype").value);
    }

    function setTime(value) {
      $("time").value = value;
      $("timeRange").value = String(timeToMinutes(value));
      $("clock").textContent = value;
    }

    function renderMatrix(payload) {
      const matrix = $("matrix");
      matrix.textContent = "";
      matrix.style.gridTemplateRows = `repeat(${payload.rows.length}, minmax(0, 1fr))`;

      payload.rows.forEach((row) => {
        const rowEl = document.createElement("div");
        rowEl.className = "row";
        rowEl.style.gridTemplateColumns = `repeat(${row.length}, minmax(0, 1fr))`;

        row.forEach((cell) => {
          const cellEl = document.createElement("div");
          cellEl.className = cell.active ? "cell active" : "cell";
          cellEl.textContent = cell.char;
          rowEl.appendChild(cellEl);
        });
        matrix.appendChild(rowEl);
      });
    }

    async function update() {
      const requestId = ++state.requestId;
      const params = new URLSearchParams({
        uhrtype: $("uhrtype").value,
        time: $("time").value,
        it_is: $("itIs").value,
        minute_variant: $("minuteVariant").value,
        use_15: $("use15").checked ? "1" : "0",
        use_20: $("use20").checked ? "1" : "0",
        use_40: $("use40").checked ? "1" : "0",
        use_45: $("use45").checked ? "1" : "0"
      });

      const response = await fetch(`/api/render?${params}`);
      const payload = await response.json();
      if (requestId !== state.requestId) return;

      $("title").textContent = `${payload.name} (${payload.rows_count}x${payload.cols_count})`;
      $("layoutMeta").textContent = `${payload.lang} / ${payload.rows_count}x${payload.cols_count}`;
      $("activeLetters").textContent = payload.active_letters || "-";
      $("words").textContent = payload.words.join(" ");
      $("warning").textContent = payload.missing.length ? `Fehlt: ${payload.missing.join(" ")}` : "";
      renderMatrix(payload);
    }

    async function init() {
      const response = await fetch("/api/uhrtypes");
      state.uhrtypes = await response.json();
      const select = $("uhrtype");
      state.uhrtypes.forEach((item) => {
        const option = document.createElement("option");
        option.value = item.name;
        option.textContent = `${item.name} (${item.rows}x${item.cols})`;
        if (!item.has_layout) option.disabled = true;
        select.appendChild(option);
      });

      const preferred = state.uhrtypes.find((item) => item.name === "DE16x18");
      select.value = preferred ? preferred.name : state.uhrtypes.find((item) => item.has_layout).name;
      setTime("12:00");
      controls.forEach((id) => $(id).addEventListener("input", update));
      $("time").addEventListener("input", () => {
        $("timeRange").value = String(timeToMinutes($("time").value));
        $("clock").textContent = $("time").value;
      });
      $("timeRange").addEventListener("input", () => setTime(minutesToTime(Number($("timeRange").value))));
      document.querySelectorAll("button[data-step]").forEach((button) => {
        button.addEventListener("click", () => {
          setTime(minutesToTime(timeToMinutes($("time").value) + Number(button.dataset.step)));
          update();
        });
      });
      update();
    }

    init();
  </script>
</body>
</html>
"""


def bool_arg(params: dict[str, list[str]], name: str) -> bool:
    return params.get(name, ["0"])[0] in {"1", "true", "yes", "on"}


def cells_for_render(uhrtype: sim.Uhrtype, lit: set[tuple[int, int]]) -> list[list[dict[str, object]]]:
    rows: list[list[dict[str, object]]] = []
    for row_index in range(uhrtype.rows):
        raw = uhrtype.layout[row_index] if row_index < len(uhrtype.layout) else ""
        raw = raw.ljust(uhrtype.cols)[: uhrtype.cols]
        row: list[dict[str, object]] = []
        for display_col, char in enumerate(raw):
            code_col = uhrtype.cols - display_col - 1
            row.append({"char": char, "active": (row_index, code_col) in lit})
        rows.append(row)
    return rows


class Handler(BaseHTTPRequestHandler):
    uhrtypes = sim.load_uhrtypes()

    def log_message(self, format: str, *args: object) -> None:
        return

    def send_json(self, payload: object, status: HTTPStatus = HTTPStatus.OK) -> None:
        body = json.dumps(payload, ensure_ascii=False).encode("utf-8")
        self.send_response(status)
        self.send_header("Content-Type", "application/json; charset=utf-8")
        self.send_header("Content-Length", str(len(body)))
        self.end_headers()
        self.wfile.write(body)

    def send_html(self) -> None:
        body = HTML.encode("utf-8")
        self.send_response(HTTPStatus.OK)
        self.send_header("Content-Type", "text/html; charset=utf-8")
        self.send_header("Content-Length", str(len(body)))
        self.end_headers()
        self.wfile.write(body)

    def do_GET(self) -> None:
        parsed = urlparse(self.path)
        if parsed.path == "/":
            self.send_html()
            return
        if parsed.path == "/api/uhrtypes":
            self.send_json(
                [
                    {
                        "name": uhrtype.name,
                        "rows": uhrtype.rows,
                        "cols": uhrtype.cols,
                        "lang": uhrtype.lang,
                        "has_layout": bool(uhrtype.layout),
                    }
                    for uhrtype in self.uhrtypes
                ]
            )
            return
        if parsed.path == "/api/render":
            self.render_api(parse_qs(parsed.query))
            return

        self.send_error(HTTPStatus.NOT_FOUND, "Not found")

    def render_api(self, params: dict[str, list[str]]) -> None:
        try:
            uhrtype = sim.find_uhrtype(self.uhrtypes, params.get("uhrtype", ["DE16x18"])[0])
            hour, minute = sim.parse_time(params.get("time", ["12:00"])[0])
        except (SystemExit, argparse.ArgumentTypeError) as exc:
            self.send_json({"error": html.escape(str(exc))}, HTTPStatus.BAD_REQUEST)
            return

        words = sim.words_for_time(
            uhrtype,
            hour,
            minute,
            minute_variant=params.get("minute_variant", ["off"])[0],
            it_is_variant=params.get("it_is", ["permanent"])[0],
            use_15=bool_arg(params, "use_15"),
            use_20=bool_arg(params, "use_20"),
            use_40=bool_arg(params, "use_40"),
            use_45=bool_arg(params, "use_45"),
            show_a_quarter=bool_arg(params, "a_quarter"),
        )
        lit, missing = uhrtype.show(words)
        self.send_json(
            {
                "name": uhrtype.name,
                "rows_count": uhrtype.rows,
                "cols_count": uhrtype.cols,
                "lang": uhrtype.lang,
                "words": words,
                "missing": missing,
                "active_letters": uhrtype.active_letters(lit),
                "rows": cells_for_render(uhrtype, lit),
            }
        )


def main() -> int:
    parser = argparse.ArgumentParser(description="Start the local Uhrtype simulator GUI.")
    parser.add_argument("--host", default="127.0.0.1")
    parser.add_argument("--port", type=int, default=8765)
    parser.add_argument("--open", action="store_true", help="Open the GUI in the default browser")
    args = parser.parse_args()

    server = ThreadingHTTPServer((args.host, args.port), Handler)
    url = f"http://{args.host}:{args.port}/"
    print(f"Uhrtype simulator GUI: {url}")
    if args.open:
        webbrowser.open(url)
    try:
        server.serve_forever()
    except KeyboardInterrupt:
        print()
    finally:
        server.server_close()
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

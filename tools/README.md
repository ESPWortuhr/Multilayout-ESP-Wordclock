# Tools

## Uhrtype simulator

`uhrtype_simulator.py` renders the front matrix definitions from
`include/Uhrtypes/*.hpp`. It parses the layout comment and the
`show(FrontWord)` coordinate mappings, so it is useful for checking Uhrtype
edits without flashing hardware.

Examples:

```sh
python3 tools/uhrtype_simulator.py --list
python3 tools/uhrtype_simulator.py DE16x18 --word day_noon --active-letters --show-inactive
python3 tools/uhrtype_simulator.py DE16x18 --time 12:00 --active-letters
python3 tools/uhrtype_simulator.py DE10x11 --time 22:45 --use-45
python3 tools/uhrtype_simulator.py EN10x11 --available-words
```

The simulator warns about words that a layout does not map, matching the
firmware behavior where unknown `FrontWord` values simply do not light pixels.

## Uhrtype GUI

`uhrtype_gui.py` starts a local web GUI around the simulator.

```sh
python3 tools/uhrtype_gui.py
```

Then open `http://127.0.0.1:8765/`.

#!/bin/sh
# Host tests for the render stage. No framework, no PlatformIO: just a compiler.
#
#   ./test/run.sh
#
# The stubs in test/support/ stand in for Arduino.h and NeoPixelBus.h, so the
# headers under test compile without the embedded toolchain.

set -e
cd "$(dirname "$0")/.."

CXX="${CXX:-c++}"
# Address and UB sanitizer: the failure class this project keeps hitting is
# indexing a buffer that belongs to a previous front matrix geometry, and that
# is invisible without them.
SAN="${SAN:--fsanitize=address,undefined -fno-omit-frame-pointer}"
OUT="$(mktemp -d)"
trap 'rm -rf "$OUT"' EXIT

# Layout list, globbed exactly like extra_scripts.py does for the firmware, so a
# newly added layout is picked up by the tests without editing them.
ls include/WordClockTypes/*.hpp |
    sed 's|include/|#include "|; s|$|"|' > "$OUT/AllLayouts.gen.h"

status=0
for src in test/test_*.cpp; do
    name="$(basename "$src" .cpp)"
    # shellcheck disable=SC2086
    "$CXX" -std=c++17 -Wall -Wextra -Wno-unused-parameter -g $SAN \
        -Itest/support -Iinclude -Itest -I"$OUT" -o "$OUT/$name" "$src"
    "$OUT/$name" || status=1
done

[ "$status" -eq 0 ] && echo "all tests passed"
exit "$status"

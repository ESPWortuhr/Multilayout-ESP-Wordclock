#pragma once
// Tiny assertion helper: no framework, no dependency.

#include <stdio.h>

static int g_failures = 0;
static int g_checks = 0;

inline void check(bool condition, const char *what) {
    g_checks++;
    if (!condition) {
        g_failures++;
        printf("  FAIL  %s\n", what);
    }
}

inline int report(const char *suite) {
    printf("%s: %d checks, %d failures\n", suite, g_checks, g_failures);
    return g_failures == 0 ? 0 : 1;
}

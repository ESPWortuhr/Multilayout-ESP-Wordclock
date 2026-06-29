#pragma once

#include <stddef.h>
#include <string.h>

namespace sensitive {

template <size_t destSize, size_t sourceSize>
void copyBoundedString(char (&dest)[destSize],
                       const char (&source)[sourceSize]) {
    static_assert(destSize > 0, "destination buffer must not be empty");
    const size_t copyLen = (destSize - 1 < sourceSize) ? destSize - 1
                                                       : sourceSize;
    memcpy(dest, source, copyLen);
    dest[copyLen] = '\0';
}

template <size_t destSize, size_t sourceSize>
void maskPreservingSuffix(char (&dest)[destSize],
                          const char (&source)[sourceSize],
                          size_t visibleSuffix = 3) {
    copyBoundedString(dest, source);

    const size_t valueLen = strlen(dest);
    if (valueLen > visibleSuffix) {
        memset(dest, '*', valueLen - visibleSuffix);
    }
}

template <size_t sourceSize>
bool matchesMaskedValue(const char *candidate, const char (&source)[sourceSize],
                        size_t visibleSuffix = 3) {
    char masked[sourceSize + 1] = {0};
    maskPreservingSuffix(masked, source, visibleSuffix);
    return strcmp(candidate, masked) == 0;
}

} // namespace sensitive

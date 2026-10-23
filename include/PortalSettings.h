#pragma once

#include <Arduino.h> // String
#include <functional>

namespace portalSettings {
using ArgLookup = std::function<bool(const char *, String &)>;

String render();
void apply(const ArgLookup &arg);
} // namespace portalSettings

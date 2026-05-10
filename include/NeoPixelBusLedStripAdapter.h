#pragma once

#include "LedStripInterface.h"
#include "NeoMultiFeature.hpp"

template <typename T_METHOD>
class RgbLedStripAdapter : public LedStripInterface {
private:
    NeoPixelBus<NeoMultiFeature, T_METHOD> *strip;

public:
    explicit RgbLedStripAdapter(uint16_t pixelCount) {
        strip = new NeoPixelBus<NeoMultiFeature, T_METHOD>(pixelCount);
        strip->Begin();
    }

    RgbLedStripAdapter(uint16_t pixelCount, uint8_t pin) {
        strip = new NeoPixelBus<NeoMultiFeature, T_METHOD>(pixelCount, pin);
        strip->Begin();
    }

    ~RgbLedStripAdapter() override { delete strip; }

    void setRgbPixel(uint16_t index, HsbColor color) override {
        strip->SetPixelColor(index, color);
    }

    void setRgbwPixel(uint16_t index, RgbwColor color) override {
        strip->SetPixelColor(index, RgbColor(color.R, color.G, color.B));
    }

    RgbColor getPixel(uint16_t index) override {
        return strip->GetPixelColor(index);
    }

    void clearPixel(uint16_t index) override { strip->SetPixelColor(index, 0); }

    void show() override { strip->Show(); }
};

template <typename T_METHOD>
class RgbwLedStripAdapter : public LedStripInterface {
private:
    NeoPixelBus<NeoGrbwFeature, T_METHOD> *strip;

public:
    explicit RgbwLedStripAdapter(uint16_t pixelCount) {
        strip = new NeoPixelBus<NeoGrbwFeature, T_METHOD>(pixelCount);
        strip->Begin();
    }

    RgbwLedStripAdapter(uint16_t pixelCount, uint8_t pin) {
        strip = new NeoPixelBus<NeoGrbwFeature, T_METHOD>(pixelCount, pin);
        strip->Begin();
    }

    ~RgbwLedStripAdapter() override { delete strip; }

    void setRgbPixel(uint16_t index, HsbColor color) override {
        RgbColor rgbColor(color);
        strip->SetPixelColor(index,
                             RgbwColor(rgbColor.R, rgbColor.G, rgbColor.B, 0));
    }

    void setRgbwPixel(uint16_t index, RgbwColor color) override {
        strip->SetPixelColor(index, color);
    }

    RgbColor getPixel(uint16_t index) override {
        return RgbColor(strip->GetPixelColor(index));
    }

    void clearPixel(uint16_t index) override { strip->SetPixelColor(index, 0); }

    void show() override { strip->Show(); }
};

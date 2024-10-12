#pragma once

/*
 * NeoMultiFeature.hpp
 *
 *  Created on: 03.02.2021
 *      Author: Andreas
 */

#include <NeoPixelBus.h>

//------------------------------------------------------------------------------

class NeoMultiFeature
    : public Neo3ByteFeature<ColorIndexR, ColorIndexG, ColorIndexB>,
      public NeoElementsNoSettings {
protected:
    static uint8_t _colortype;

public:
    static void setColortype(uint8_t colortype) { _colortype = colortype; }

    static void applyPixelColor(uint8_t *pPixels, uint16_t indexPixel,
                                ColorObject color) {
        uint8_t *p = getPixelAddress(pPixels, indexPixel);

        switch (_colortype) {
        case Bgr:
            *p++ = color.B;
            *p++ = color.G;
            *p = color.R;
            break;
        case Brg:
            *p++ = color.B;
            *p++ = color.R;
            *p = color.G;
            break;
        case Grb:
            *p++ = color.G;
            *p++ = color.R;
            *p = color.B;
            break;
        case Rbg:
            *p++ = color.R;
            *p++ = color.B;
            *p = color.G;
            break;
        case Rgb:
        default:
            *p++ = color.R;
            *p++ = color.G;
            *p = color.B;
            break;
        }
    }

    static ColorObject retrievePixelColor(const uint8_t *pPixels,
                                          uint16_t indexPixel) {
        ColorObject color;
        const uint8_t *p = getPixelAddress(pPixels, indexPixel);

        switch (_colortype) {
        case Bgr:
            color.B = *p++;
            color.G = *p++;
            color.R = *p;
            break;
        case Brg:
            color.B = *p++;
            color.R = *p++;
            color.G = *p;
            break;
        case Grb:
            color.G = *p++;
            color.R = *p++;
            color.B = *p;
            break;
        case Rbg:
            color.R = *p++;
            color.B = *p++;
            color.G = *p;
            break;
        case Rgb:
        default:
            color.R = *p++;
            color.G = *p++;
            color.B = *p;
            break;
        }

        return color;
    }

    static ColorObject retrievePixelColor_P(PGM_VOID_P pPixels,
                                            uint16_t indexPixel) {
        ColorObject color;
        const uint8_t *p =
            getPixelAddress((const uint8_t *)pPixels, indexPixel);

        switch (_colortype) {
        case Bgr:
            color.B = pgm_read_byte(p++);
            color.G = pgm_read_byte(p++);
            color.R = pgm_read_byte(p);
            break;
        case Brg:
            color.B = pgm_read_byte(p++);
            color.R = pgm_read_byte(p++);
            color.G = pgm_read_byte(p);
            break;
        case Grb:
            color.G = pgm_read_byte(p++);
            color.R = pgm_read_byte(p++);
            color.B = pgm_read_byte(p);
            break;
            break;
        case Rbg:
            color.R = pgm_read_byte(p++);
            color.B = pgm_read_byte(p++);
            color.G = pgm_read_byte(p);
            break;
        case Rgb:
        default:
            color.R = pgm_read_byte(p++);
            color.G = pgm_read_byte(p++);
            color.B = pgm_read_byte(p);
        }

        return color;
    }
};

uint8_t NeoMultiFeature::_colortype = DEFAULT_LEDTYPE; // init static member

//------------------------------------------------------------------------------

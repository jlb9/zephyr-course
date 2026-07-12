// #############################################################################
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#include "rgb.hpp"
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

// 6-segment HSV rainbow: full spectrum including yellow, cyan, magenta.
// At peak transitions R+G+B can reach 510, drawing up to 2x nominal LED power.
uint32_t rgb::fromHSV(uint8_t hue) {
    uint16_t h = static_cast<uint16_t>(hue) * 6U;    // 0..1530, each of 6 segments is exactly 255 wide
    uint8_t x = static_cast<uint8_t>(h & 0xFF);      // position within segment (0..255)
    uint8_t r, g, b;
    switch (h >> 8) {
        case 0:  r = 255;     g = x;       b = 0;       break;    // Red -> Yellow
        case 1:  r = 255 - x; g = 255;     b = 0;       break;    // Yellow -> Green
        case 2:  r = 0;       g = 255;     b = x;       break;    // Green -> Cyan
        case 3:  r = 0;       g = 255 - x; b = 255;     break;    // Cyan -> Blue
        case 4:  r = x;       g = 0;       b = 255;     break;    // Blue -> Magenta
        case 5:  r = 255;     g = 0;       b = 255 - x; break;    // Magenta -> Red
        default: r = 0;       g = 0;       b = 0;       break;
    }
    return (static_cast<uint32_t>(r) << 16) | (static_cast<uint32_t>(g) << 8) | b;
}

// 3-segment power-conscious HSV: only two primaries mix at a time, R+G+B = 255 always.
// Skips yellow/cyan/magenta; total LED power is constant across all hues.
uint32_t rgb::fromHSVPowerConscious(uint8_t hue) {
    uint16_t h = static_cast<uint16_t>(hue) * 3U;    // 0..765, each of 3 segments is exactly 255 wide
    uint8_t x = static_cast<uint8_t>(h & 0xFF);
    uint8_t r, g, b;
    switch (h >> 8) {
        case 0:  r = 255 - x; g = x;       b = 0;       break;    // Red -> Green
        case 1:  r = 0;       g = 255 - x; b = x;       break;    // Green -> Blue
        case 2:  r = x;       g = 0;       b = 255 - x; break;    // Blue -> Red
        default: r = 0;       g = 0;       b = 0;       break;
    }
    return (static_cast<uint32_t>(r) << 16) | (static_cast<uint32_t>(g) << 8) | b;
}

// Sine-wave: three 120°-offset sine waves, all channels always partially lit.
// Produces very smooth, organic transitions; no hard segment boundaries.
uint32_t rgb::fromSineWave(uint8_t hue) {
    float angle = static_cast<float>(hue) * (2.0f * static_cast<float>(M_PI) / 256.0f);
    uint8_t r = static_cast<uint8_t>(sinf(angle) * 127.0f + 128.0f);
    uint8_t g = static_cast<uint8_t>(sinf(angle + 2.0f * static_cast<float>(M_PI) / 3.0f) * 127.0f + 128.0f);
    uint8_t b = static_cast<uint8_t>(sinf(angle + 4.0f * static_cast<float>(M_PI) / 3.0f) * 127.0f + 128.0f);
    return (static_cast<uint32_t>(r) << 16) | (static_cast<uint32_t>(g) << 8) | b;
}

uint8_t rgb::red(uint32_t color) { return static_cast<uint8_t>((color >> 16) & 0xFF); }
uint8_t rgb::green(uint32_t color) { return static_cast<uint8_t>((color >> 8) & 0xFF); }
uint8_t rgb::blue(uint32_t color) { return static_cast<uint8_t>(color & 0xFF); }

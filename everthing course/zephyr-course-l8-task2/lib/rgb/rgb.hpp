// #############################################################################
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#pragma once
#include <stdint.h>

class rgb {
  public:
    static uint32_t fromHSV(uint8_t hue);
    static uint32_t fromHSVPowerConscious(uint8_t hue);
    static uint32_t fromSineWave(uint8_t hue);
    static uint8_t red(uint32_t color);
    static uint8_t green(uint32_t color);
    static uint8_t blue(uint32_t color);

#ifndef CONFIG_ZTEST

  private:
#endif
};

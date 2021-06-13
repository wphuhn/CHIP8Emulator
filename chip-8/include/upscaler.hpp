#ifndef CHIP_8_INCLUDE_UPSCALER_HPP_
#define CHIP_8_INCLUDE_UPSCALER_HPP_

#include "chip8machine.hpp"

#define PIXEL_COLOR 0xFFFF
#define X_SCALE 8
#define Y_SCALE 8

class Upscaler {
 public:
    static const int x_scale = X_SCALE;
    static const int y_scale = Y_SCALE;
    static const int pixel_color = PIXEL_COLOR;

    static void upscale(unsigned short*, int, const Chip8Machine&);
};

#endif  // CHIP_8_INCLUDE_UPSCALER_HPP_

#include "upscaler.hpp"

namespace Emulator {

void Upscaler::upscale(unsigned short *frame_buffer, int width,
                       const Chip8Machine &my_machine) {
  for (int y_machine = 0; y_machine < my_machine.display_height; y_machine++) {
    for (int x_machine = 0; x_machine < my_machine.display_width; x_machine++) {
      unsigned short pixel = my_machine.get_pixel(x_machine, y_machine);
      if (pixel > 0) {
        pixel = pixel_color;
      }
      for (int y_sub = 0; y_sub < Upscaler::y_scale; y_sub++) {
        for (int x_sub = 0; x_sub < Upscaler::x_scale; x_sub++) {
          unsigned short x = Upscaler::y_scale * x_machine + x_sub;
          unsigned short y = Upscaler::y_scale * y_machine + y_sub;
          frame_buffer[y * width + x] = pixel;
        }
      }
    }
  }
}

}  //  namespace Emulator

#ifndef CHIP_8_INCLUDE_DISPLAY_HPP_
#define CHIP_8_INCLUDE_DISPLAY_HPP_

#include <iomanip>
#include <string>
#include <vector>

#include "chip8types.hpp"

class Display {
 public:
  Display(int, int, PIXEL_TYPE = 0);

  const int width;
  const int height;
  const PIXEL_TYPE off_pixel;

  const PIXEL_TYPE &get_pixel(int, int) const;
  void set_pixel(int, int, PIXEL_TYPE);
  void clear();
  explicit operator std::string() const;
 private:
  std::vector<std::vector<PIXEL_TYPE> > screen;
};

#endif  // CHIP_8_INCLUDE_DISPLAY_HPP_

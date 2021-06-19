/// \file display.hpp
/// \brief Representation of the display by the machine (no upscaling!)

#ifndef CHIP_8_INCLUDE_DISPLAY_HPP_
#define CHIP_8_INCLUDE_DISPLAY_HPP_

#include <iomanip>
#include <string>
#include <vector>

#include "chip8types.hpp"

namespace Emulator {

/// \class Display
/// \brief Representation of the display by the machine (no upscaling!)
class Display {
 public:
  Display(int, int, PIXEL_TYPE = 0);

  /// \var width
  /// \brief Width of screen (in number of pixels)
  const int width;

  /// \var height
  /// \brief Width of display (in number of pixels)
  const int height;

  /// \var off_pixel
  /// \brief Value for pixel when "off"; all other values assumed "on"
  const PIXEL_TYPE off_pixel;

  const PIXEL_TYPE &get_pixel(int, int) const;
  void set_pixel(int, int, PIXEL_TYPE);
  void clear();
  explicit operator std::string() const;
 private:
  std::vector<std::vector<PIXEL_TYPE> > screen;
};

}  // namespace Emulator

#endif  // CHIP_8_INCLUDE_DISPLAY_HPP_

#include "display.hpp"

namespace Emulator {

/// \brief Create screen with fixed height and width
/// \param height_ Height of screen (in number of pixels)
/// \param width_ Width of screen (in number of pixels)
/// \param off_pixel_ Default value for pixels
Display::Display(int height_, int width_, PIXEL_TYPE off_pixel_)
    : height(height_), width(width_), off_pixel(off_pixel_) {
  screen = std::vector<std::vector<PIXEL_TYPE> >(
      height,
      std::vector<PIXEL_TYPE>(width));
  clear();
}

/// \brief Get the value of the pixel located at (x, y) position
/// \param x Horizontal position of pixel, where 0 corresponds to left edge
/// \param y Vertical position of pixel, where 0 corresponds to upper edge
/// \return Value of the pixel located at (x,y) position
const PIXEL_TYPE &Display::get_pixel(const int x, const int y) const {
  return screen[y][x];
}

/// \brief Set the value of the pixel located at (x, y) position
/// \param x Horizontal position of pixel, where 0 corresponds to left edge
/// \param y Vertical position of pixel, where 0 corresponds to upper edge
/// \param value Value to change pixel to
void Display::set_pixel(const int x, const int y, const PIXEL_TYPE value) {
  screen[y][x] = value;
}

/// \brief Reset the display to its default state
void Display::clear() {
  for (auto &row : screen) {
    for (PIXEL_TYPE &pixel : row) {
      pixel = off_pixel;
    }
  }
}

/// \brief Return the contents of the display as an ASCII representation
/// \return Contents of the display as an ASCII representation
Display::operator std::string() const {
  std::stringstream stream;
  for (auto &row : screen) {
    for (const PIXEL_TYPE &pixel : row) {
      if (pixel == off_pixel) {
        stream << " ";
      } else {
        stream << "X";
      }
    }
    stream << std::endl;
  }
  return stream.str();
}

}  // namespace Emulator

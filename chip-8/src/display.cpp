#include "display.hpp"

Display::Display(int height_, int width_, PIXEL_TYPE off_pixel_)
    : height(height_), width(width_), off_pixel(off_pixel_) {
  screen = std::vector<std::vector<PIXEL_TYPE> >(
      height,
      std::vector<PIXEL_TYPE>(width));
  clear();
}

const PIXEL_TYPE &Display::get_pixel(const int x, const int y) const {
  return screen[y][x];
}

void Display::set_pixel(const int x, const int y, const PIXEL_TYPE value) {
  screen[y][x] = value;
}

void Display::clear() {
  for (auto &row : screen) {
    for (PIXEL_TYPE &pixel : row) {
      pixel = off_pixel;
    }
  }
}

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

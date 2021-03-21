#include "display.hpp"

Display::Display() {
    screen = std::vector< std::vector<PIXEL_TYPE> >(height, std::vector<PIXEL_TYPE>(width));
    clear();
}

const PIXEL_TYPE& Display::get_pixel(const int x, const int y) const {
    return screen[y][x];
}

void Display::set_pixel(const int x, const int y, const PIXEL_TYPE value) {
    screen[y][x] = value;
}

void Display::clear() {
    for (auto & row : screen) {
        for (PIXEL_TYPE & pixel : row) {
            pixel = OFF_PIXEL;
        }
    }
}

Display::operator std::string() const {
    std::stringstream stream;
    for (auto & row : screen) {
        for (const PIXEL_TYPE & pixel : row) {
            if (pixel == OFF_PIXEL) {
                stream << " ";
            } else {
                stream << "X";
            }
        }
        stream << std::endl;
    }
    return stream.str();
}
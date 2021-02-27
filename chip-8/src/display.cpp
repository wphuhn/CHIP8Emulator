#include "display.hpp"

Display::Display() {
    screen = std::vector< std::vector<PIXEL_TYPE> >(height, std::vector<PIXEL_TYPE>(width));
    clear();
}

PIXEL_TYPE Display::get_pixel(int x, int y) {
    return screen[y][x];
}

void Display::set_pixel(int x, int y, PIXEL_TYPE value) {
    screen[y][x] = value;
}

void Display::clear() {
    for (auto & row : screen) {
        for (PIXEL_TYPE & pixel : row) {
            pixel = OFF_PIXEL;
        }
    }
}
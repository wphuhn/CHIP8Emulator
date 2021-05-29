#ifndef CHIP_8_DISPLAY_HPP_
#define CHIP_8_DISPLAY_HPP_

#include <iomanip>
#include <vector>

#include "chip8constants.hpp"

class Display {
public:
    Display();

    static const int width = MAX_WIDTH;
    static const int height = MAX_HEIGHT;

    const PIXEL_TYPE& get_pixel(int, int) const;
    void set_pixel(int, int, PIXEL_TYPE);
    void clear();
    explicit operator std::string() const;
private:
    std::vector< std::vector<PIXEL_TYPE> > screen;
};

#endif //CHIP_8_DISPLAY_HPP_

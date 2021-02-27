#ifndef CHIP_8_DISPLAY_HPP_
#define CHIP_8_DISPLAY_HPP_

#include <vector>

#define MAX_WIDTH 64
#define MAX_HEIGHT 32
#define PIXEL_TYPE int

class Display {
public:
    Display();

    const int width = MAX_WIDTH;
    const int height = MAX_HEIGHT;

    PIXEL_TYPE get_pixel(int, int);
    void set_pixel(int, int, PIXEL_TYPE);
    void clear();
private:
    std::vector< std::vector<PIXEL_TYPE> > screen;
};

#endif //CHIP_8_DISPLAY_HPP_

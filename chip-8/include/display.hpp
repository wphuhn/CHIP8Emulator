#ifndef CHIP_8_DISPLAY_HPP_
#define CHIP_8_DISPLAY_HPP_

#include <vector>

#define MAX_WIDTH 64
#define MAX_HEIGHT 32
#define OFF_PIXEL 0
#define ON_PIXEL 1
#define PIXEL_TYPE int

class Display {
public:
    Display();

    static const int width = MAX_WIDTH;
    static const int height = MAX_HEIGHT;

    const PIXEL_TYPE& get_pixel(int, int) const;
    void set_pixel(int, int, PIXEL_TYPE);
    void clear();
private:
    std::vector< std::vector<PIXEL_TYPE> > screen;
};

#endif //CHIP_8_DISPLAY_HPP_

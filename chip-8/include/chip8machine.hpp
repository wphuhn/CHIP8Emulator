#ifndef CHIP_8_CHIP8MACHINE_HPP_
#define CHIP_8_CHIP8MACHINE_HPP_

#include <cstdint>
#include <iomanip>
#include <stdexcept>
#include <string>

#include "display.hpp"

#define OPCODE_TYPE uint16_t

class Chip8Machine {
public:
    Chip8Machine();

    const int display_width;
    const int display_height;
    const PIXEL_TYPE& get_pixel(const int, const int) const;
    void set_pixel(const int, const int, const PIXEL_TYPE);

    void decode(OPCODE_TYPE);
private:
    Display display;
};

class OpcodeNotSupported : public std::runtime_error
{
public:
    OpcodeNotSupported(const OPCODE_TYPE);
};

#endif //CHIP_8_CHIP8MACHINE_HPP_

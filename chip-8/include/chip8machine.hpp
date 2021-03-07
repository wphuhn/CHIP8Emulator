#ifndef CHIP_8_CHIP8MACHINE_HPP_
#define CHIP_8_CHIP8MACHINE_HPP_

#include <cstdint>
#include <iomanip>
#include <stdexcept>
#include <string>

#include "display.hpp"
#include "register.hpp"

#define OPCODE_TYPE uint16_t
#define NUM_V_REGS 16

class Chip8Machine {
public:
    Chip8Machine();

    const int display_width;
    const int display_height;
    const PIXEL_TYPE& get_pixel(const int, const int) const;
    void set_pixel(const int, const int, const PIXEL_TYPE);

    const int get_i(void);
    const int get_v(const int);

    void set_i(const int);
    void set_v(const int, const int);

    void decode(OPCODE_TYPE);
private:
    Display display;
    Register i_register;
    std::array<Register, NUM_V_REGS> v_register;
};

class OpcodeNotSupported : public std::runtime_error
{
public:
    OpcodeNotSupported(const OPCODE_TYPE);
};

#endif //CHIP_8_CHIP8MACHINE_HPP_

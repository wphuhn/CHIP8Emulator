#ifndef CHIP_8_CHIP8MACHINE_HPP_
#define CHIP_8_CHIP8MACHINE_HPP_

#include <cstdint>
#include <iomanip>
#include <stdexcept>
#include <string>

#include "display.hpp"
#include "memory.hpp"
#include "programcounter.hpp"
#include "register.hpp"

#define OPCODE_TYPE uint16_t
#define NUM_V_REGS 16

class Chip8Machine {
public:
    Chip8Machine();

    const int display_width;
    const int display_height;
    const PIXEL_TYPE& get_pixel(int, int) const;
    void set_pixel(int, int, PIXEL_TYPE);
    void clear_screen();

    void set_memory_byte(int, unsigned char);

    int get_i() const;
    int get_v(int) const;
    int get_flag() const;
    int get_pc() const;

    void set_i(int);
    void set_v(int, int);
    void set_flag(int);

    void decode(OPCODE_TYPE);
private:
    Display display;
    Memory ram;
    ProgramCounter pc;
    Register i_register;
    std::array<Register, NUM_V_REGS> v_register;
};

class OpcodeNotSupported : public std::runtime_error
{
public:
    explicit OpcodeNotSupported(OPCODE_TYPE);
};

#endif //CHIP_8_CHIP8MACHINE_HPP_

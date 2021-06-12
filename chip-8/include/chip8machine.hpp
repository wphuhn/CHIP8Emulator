#ifndef CHIP_8_CHIP8MACHINE_HPP_
#define CHIP_8_CHIP8MACHINE_HPP_

#include <cstdint>
#include <iomanip>
#include <stdexcept>
#include <string>

#include "chip8constants.hpp"
#include "chip8types.hpp"
#include "display.hpp"
#include "memory.hpp"
#include "programcounter.hpp"
#include "register.hpp"

class Chip8Machine {
public:
    Chip8Machine();

    friend class Chip8MachineTester;
    // Bad, but unavoidable for now
    friend Chip8Machine create_machine_for_drawing(OPCODE_TYPE, ADDR_TYPE,
                                            const std::vector<MEM_TYPE>&, int, int);

    const int display_width;
    const int display_height;
    const ADDR_TYPE memory_size;

    const PIXEL_TYPE& get_pixel(int, int) const;
    void* get_pointer_to_ram_start() const;

    void clear_screen();
    void load_rom(const std::vector<MEM_TYPE> &);
    void decode(OPCODE_TYPE);
    void advance();
    void reset();

    std::string display_str() const;
    explicit operator std::string() const;
private:
    Display display;
    Memory ram;
    ProgramCounter pc;
    Register i_register;
    std::array<Register, NUM_V_REGS> v_register;

    OPCODE_TYPE fetch_instruction() const;

    std::vector<MEM_TYPE> get_ram(bool = true) const;
    MEM_TYPE get_memory_byte(ADDR_TYPE) const;
    REG_TYPE get_i() const;
    REG_TYPE get_v(int) const;
    REG_TYPE get_flag() const;
    ADDR_TYPE get_pc() const;

    void set_pixel(int, int, PIXEL_TYPE);
    void set_memory_byte(ADDR_TYPE, MEM_TYPE);
    void set_i(REG_TYPE);
    void set_v(int, REG_TYPE);
    void set_flag(REG_TYPE);
    void set_pc(ADDR_TYPE);
};

class OpcodeNotSupported : public std::runtime_error
{
public:
    explicit OpcodeNotSupported(OPCODE_TYPE);
};

#endif //CHIP_8_CHIP8MACHINE_HPP_

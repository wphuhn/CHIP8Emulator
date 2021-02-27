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
    void decode(OPCODE_TYPE);
    Display display;
};

class OpcodeNotSupported : public std::runtime_error
{
public:
    OpcodeNotSupported(const OPCODE_TYPE);
};

#endif //CHIP_8_CHIP8MACHINE_HPP_

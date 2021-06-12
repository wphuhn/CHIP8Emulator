#ifndef CHIP_8_UTILITIES_HPP_
#define CHIP_8_UTILITIES_HPP_
#include <vector>

#include "chip8machine.hpp"

OPCODE_TYPE gen_XYNN_opcode(int, int, int);
OPCODE_TYPE gen_WXYZ_opcode(int, int, int, int);
Chip8Machine create_machine_for_drawing(OPCODE_TYPE, ADDR_TYPE, const std::vector<MEM_TYPE>&, int, int);
#endif //CHIP_8_UTILITIES_HPP_

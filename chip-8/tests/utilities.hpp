#ifndef CHIP_8_TESTS_UTILITIES_HPP_
#define CHIP_8_TESTS_UTILITIES_HPP_

#include <vector>

#include "chip8machine.hpp"

namespace Emulator {

OPCODE_TYPE gen_XYNN_opcode(int, int, int);
OPCODE_TYPE gen_WXYZ_opcode(int, int, int, int);
Emulator::Chip8Machine create_machine_for_drawing(OPCODE_TYPE, ADDR_TYPE, const std::vector<MEM_TYPE> &, int, int);

}  // namespace Emulator

#endif  // CHIP_8_TESTS_UTILITIES_HPP_

/// \file chip8constants.hpp
/// \brief Constants for machine architecture

#ifndef CHIP_8_INCLUDE_CHIP8CONSTANTS_HPP_
#define CHIP_8_INCLUDE_CHIP8CONSTANTS_HPP_

namespace Emulator {

/// \var INSTRUCTION_LENGTH
/// \brief Number of bytes in instruction
const int INSTRUCTION_LENGTH = 2;

/// \var NUM_V_REGS
/// \brief Number of general-purpose registers
const int NUM_V_REGS = 16;

/// \var MAX_HEIGHT
/// \brief Height of screen (in number of pixels)
const int MAX_HEIGHT = 32;

/// \var MAX_WIDTH
/// \brief Width of screen (in number of pixels)
const int MAX_WIDTH = 64;

/// \var RAM_SIZE
/// \brief Total number of bytes in RAM
const int RAM_SIZE = 0x1000;

/// \var ROM_START_ADDRESS
/// \brief Starting address where ROM will be loaded in RAM
const int ROM_START_ADDRESS = 0x200;

}  // namespace Emulator

#endif  // CHIP_8_INCLUDE_CHIP8CONSTANTS_HPP_

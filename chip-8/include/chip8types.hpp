/// \file chip8types.hpp
/// \brief Typedefs for project

#ifndef CHIP_8_INCLUDE_CHIP8TYPES_HPP_
#define CHIP_8_INCLUDE_CHIP8TYPES_HPP_

#include <cstddef>
#include <cstdint>

namespace Emulator {

/// \var OPCODE_TYPE
/// \brief Type for instructions/opcodes
typedef uint16_t OPCODE_TYPE;

/// \var PIXEL_TYPE
/// \brief Type for pixel values
typedef int PIXEL_TYPE;

/// \var ADDR_TYPE
/// \brief Type for address and address-like quantities (PC register, etc.)
typedef size_t ADDR_TYPE;

/// \var MEM_TYPE
/// \brief Type for values pointed to by memory addresses
typedef unsigned char MEM_TYPE;

/// \var REG_TYPE
/// \brief Type for value in registers
typedef uint16_t REG_TYPE;

}  // namespace Emulator

#endif  // CHIP_8_INCLUDE_CHIP8TYPES_HPP_

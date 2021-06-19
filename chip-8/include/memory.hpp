/// \file memory.hpp
/// \brief Memory space for machine

#ifndef CHIP_8_INCLUDE_MEMORY_HPP_
#define CHIP_8_INCLUDE_MEMORY_HPP_

#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <vector>

#include "chip8types.hpp"

namespace Emulator {

/// \class Memory
/// \brief Memory space for machine
class Memory {
 public:
  Memory(ADDR_TYPE, ADDR_TYPE);

  /// \var size
  /// \brief Number of address in memory
  const ADDR_TYPE size;

  /// \var rom_start_address
  /// \brief Starting address where ROMs will be loaded
  const ADDR_TYPE rom_start_address;

  void load_rom(const std::vector<MEM_TYPE> &);
  std::vector<MEM_TYPE> get_ram(bool = true) const;
  void *get_pointer_to_ram_start() const;
  MEM_TYPE get_byte(ADDR_TYPE) const;
  void set_byte(ADDR_TYPE, MEM_TYPE);

  static std::pair<void *, size_t> get_bytestream_from_file(
      const std::string &);
  static std::vector<MEM_TYPE> convert_bytestream_to_vector(
      const void *bytestream,
      size_t bytestream_size);

 private:
  std::vector<MEM_TYPE> ram;
};

}  // namespace Emulator

#endif  // CHIP_8_INCLUDE_MEMORY_HPP_

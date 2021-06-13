#ifndef CHIP_8_INCLUDE_MEMORY_HPP_
#define CHIP_8_INCLUDE_MEMORY_HPP_

#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <vector>

#include "chip8types.hpp"

class Memory {
 public:
    Memory(ADDR_TYPE, ADDR_TYPE);

    const ADDR_TYPE size;
    const ADDR_TYPE rom_start_address;

    void load_rom(const std::vector<MEM_TYPE> &);
    std::vector<MEM_TYPE> get_ram(bool = true) const;
    void * get_pointer_to_ram_start() const;
    MEM_TYPE get_byte(ADDR_TYPE) const;
    void set_byte(ADDR_TYPE, MEM_TYPE);

    static std::pair<void*, size_t> get_bitstream_from_file(
        const std::string &path);
    static std::vector<MEM_TYPE> convert_bitstream_to_vector(
        const void *bitstream, size_t bitstream_size);
 private:
    std::vector<MEM_TYPE> ram;
};

#endif  // CHIP_8_INCLUDE_MEMORY_HPP_

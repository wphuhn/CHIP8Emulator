#include "memory.hpp"

Memory::Memory() {
   ram.resize(size);
   for (unsigned char& value : ram) {
       value = 0x00;
   }
}

void Memory::load_rom(const std::vector<unsigned char> & rom) {
    int offset = ROM_START_ADDRESS;
    for (unsigned char value : rom) {
        ram[offset] = value;
        offset += 1;
    }
}

// CHIP-8 code can be self-altering, hence the slightly different name
// When include_start is true, returns the entirety of RAM, including the 512 dead
// bytes at the beginning, otherwise return only the writeable portion
std::vector<unsigned char> Memory::get_ram(bool include_start) const {
    auto first = ram.begin();
    if (not include_start) {
        first += ROM_START_ADDRESS;
    }
    auto last = ram.end();
    return std::vector<unsigned char>(first, last);
}

unsigned char Memory::get_byte(const int offset) const {
    return ram[offset];
}

void Memory::set_byte(const int address, const unsigned char value) {
    ram[address] = value;
}

std::pair<void*, size_t> Memory::get_bitstream_from_file(const std::string& path) {
    // Read ROM file into memory
    std::ifstream file(path.c_str(), std::ifstream::binary);

    if (not file) {
        std::string err_msg = std::string("Path '") + path + std::string("' could not be loaded");
        throw std::ifstream::failure(err_msg);
    }

    // Get the file size and read it into a memory buffer
    file.seekg(0, std::ifstream::end);
    size_t data_size = file.tellg();
    char* data = new char [data_size];

    file.seekg(0, std::ifstream::beg);
    file.read(data, data_size);
    // Sanity check, not unit-tested
    if (file.gcount() != data_size) {
        std::string err_msg = std::string("Was unable to read entire ") + path + std::string(" file");
        throw std::ifstream::failure(err_msg);
    }

    file.close();

    return {data, data_size};
}

std::vector<unsigned char> Memory::convert_bitstream_to_vector(const void* bitstream, const size_t bitstream_size) {
    auto rom = (const unsigned char*) bitstream;

    int pc = 0;
    std::vector<unsigned char> rom_vec (bitstream_size, 0x0000);
    while (pc < bitstream_size) {
        rom_vec[pc] = rom[pc];
        pc += 1;
    }

    return rom_vec;
}

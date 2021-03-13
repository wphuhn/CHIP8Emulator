#include "memory.hpp"

Memory::Memory() {
   ram.reserve(RAM_SIZE);
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

unsigned char Memory::get_byte(int offset) const {
    return ram[offset];
}
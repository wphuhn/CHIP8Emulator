#include "gtest/gtest.h"

#include "memory.hpp"

TEST (Memory, HasDefaultConstructor) {
    Memory ram;
}

TEST (Memory, LoadsROMAtCorrectLocation) {
    const std::vector<unsigned char> rom = {0xBE, 0xEF, 0xCA, 0xCE};
    Memory ram;
    ram.load_rom(rom);
    int expected_start_address = 0x200;
    for (int i = 0; i < rom.size(); i++) {
        EXPECT_EQ(ram.get_byte(expected_start_address + i), rom[i]);
    }
}

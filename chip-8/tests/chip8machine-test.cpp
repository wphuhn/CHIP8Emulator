#include "gtest/gtest.h"

#include "chip8machine.hpp"

TEST (Chip8Machine, HasDefaultConstructor) {
    Chip8Machine machine;
}

TEST (Chip8Machine, PassingUnsupportedOpcodeStopsInterpreter) {
    Chip8Machine machine;
    // 0x9000 was chosen because it's not part of CHIP-8 instruction set
    OPCODE_TYPE bad_opcode = 0x9000;
    try {
        machine.decode(bad_opcode);
        FAIL() << "Expected OpcodeNotSupported exception to be thrown for unsupported opcode, none were thrown";
    }
    catch (const OpcodeNotSupported& err) {
        EXPECT_EQ(err.what(),std::string("Opcode 0x9000 not supported"));
    }
    catch (...) {
        FAIL() << "Expected OpcodeNotSupported exception to be thrown for unsupported opcode, another exception was thrown";
    }
}

TEST (Chip8Machine, Opcode00E0ClearsScreen) {
    Chip8Machine machine;
    OPCODE_TYPE opcode = 0x00E0;
    for (int x = 0; x < machine.display.width; x++) {
        for (int y = 0; y < machine.display.height; y++) {
            machine.display.set_pixel(x, y, ON_PIXEL);
        }
    }
    machine.decode(opcode);
    for (int x = 0; x < machine.display.width; x++) {
        for (int y = 0; y < machine.display.height; y++) {
            EXPECT_EQ(machine.display.get_pixel(x, y), OFF_PIXEL);
        }
    }
}
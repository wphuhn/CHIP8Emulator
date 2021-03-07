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
    for (int x = 0; x < machine.display_width; x++) {
        for (int y = 0; y < machine.display_height; y++) {
            machine.set_pixel(x, y, ON_PIXEL);
        }
    }
    machine.decode(opcode);
    for (int x = 0; x < machine.display_width; x++) {
        for (int y = 0; y < machine.display_height; y++) {
            EXPECT_EQ(machine.get_pixel(x, y), OFF_PIXEL);
        }
    }
}

class ANNNParameterizedTestFixture : public ::testing::TestWithParam<OPCODE_TYPE> {};
TEST_P (ANNNParameterizedTestFixture, OpcodeANNNSetsIRegisterToNNN) {
    OPCODE_TYPE opcode = GetParam();
    Chip8Machine machine;
    int value = opcode & 0x0FFF;
    machine.set_i(0x0000);
    machine.decode(opcode);
    EXPECT_EQ(machine.get_i(), value);
}
INSTANTIATE_TEST_CASE_P(
        ANNNTests,
        ANNNParameterizedTestFixture,
        ::testing::Values(
                0xA000, 0xABFA, 0xA212, 0xAFFF
                )
        );

class Opcode6XNNParameterizedTestFixture : public ::testing::TestWithParam<OPCODE_TYPE> {};
TEST_P (Opcode6XNNParameterizedTestFixture, Opcode6XNNSetsIRegisterVXToNN) {
    OPCODE_TYPE opcode = GetParam();
    Chip8Machine machine;
    int value = opcode & 0x00FF;
    int reg_num = (opcode & 0x0F00) >> 8;
    machine.set_v(reg_num, 0x0000);
    machine.decode(opcode);
    EXPECT_EQ(machine.get_v(reg_num), value);
}
INSTANTIATE_TEST_CASE_P(
        Opcode6XNNTests,
        Opcode6XNNParameterizedTestFixture,
        ::testing::Values(
                0x6000, 0x6066, 0x60F7, 0x60FF,
                0x6100, 0x6166, 0x61F7, 0x61FF,
                0x6200, 0x6266, 0x62F7, 0x62FF,
                0x6300, 0x6366, 0x63F7, 0x63FF,
                0x6400, 0x6466, 0x64F7, 0x64FF,
                0x6500, 0x6566, 0x65F7, 0x65FF,
                // I used an RNG to generate the data, I swear
                0x6600, 0x6666, 0x66F7, 0x66FF,
                0x6700, 0x6766, 0x67F7, 0x67FF,
                0x6800, 0x6866, 0x68F7, 0x68FF,
                0x6900, 0x6966, 0x69F7, 0x69FF,
                0x6A00, 0x6A66, 0x6AF7, 0x6AFF,
                0x6B00, 0x6B66, 0x6BF7, 0x6BFF,
                0x6C00, 0x6C66, 0x6CF7, 0x6CFF,
                0x6D00, 0x6D66, 0x6DF7, 0x6DFF,
                0x6E00, 0x6E66, 0x6EF7, 0x6EFF,
                0x6F00, 0x6F66, 0x6FF7, 0x6FFF
        )
);

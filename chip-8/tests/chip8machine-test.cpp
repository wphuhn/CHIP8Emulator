#include "gtest/gtest.h"

#include "chip8machine.hpp"

#include "utilities.hpp"

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

TEST (Chip8Machine, ClearScreenClearsTheScreen) {
    Chip8Machine machine;
    OPCODE_TYPE opcode = 0x00E0;
    for (int x = 0; x < machine.display_width; x++) {
        for (int y = 0; y < machine.display_height; y++) {
            machine.set_pixel(x, y, ON_PIXEL);
        }
    }
    machine.clear_screen();
    for (int x = 0; x < machine.display_width; x++) {
        for (int y = 0; y < machine.display_height; y++) {
            EXPECT_EQ(machine.get_pixel(x, y), OFF_PIXEL);
        }
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
TEST_P (Opcode6XNNParameterizedTestFixture, Opcode6XNNSetsRegisterVXToNN) {
    Chip8Machine machine;
    OPCODE_TYPE value = GetParam();
    for (int reg_num = 0; reg_num <= 0xF; reg_num++) {
        OPCODE_TYPE opcode = gen_XYNN_opcode(0x6, reg_num, value);
        machine.set_v(reg_num, 0x0000);
        machine.decode(opcode);
        EXPECT_EQ(machine.get_v(reg_num), value);
    }
}
INSTANTIATE_TEST_CASE_P(
        Opcode6XNNTests,
        Opcode6XNNParameterizedTestFixture,
        ::testing::Values(0x00, 0x66, 0xF7, 0xFF)
);

class Opcode7XNNParameterizedTestFixture
        : public ::testing::TestWithParam<std::tuple<OPCODE_TYPE, OPCODE_TYPE, OPCODE_TYPE>> {};
TEST_P (Opcode7XNNParameterizedTestFixture, Opcode7XNNAddsNNToRegisterVX) {
    OPCODE_TYPE initial = std::get<0>(GetParam());
    OPCODE_TYPE increment = std::get<1>(GetParam());
    OPCODE_TYPE final = std::get<2>(GetParam());
    Chip8Machine machine;
    for (int reg_num = 0; reg_num <= 0xF; reg_num++) {
        OPCODE_TYPE opcode = gen_XYNN_opcode(0x7, reg_num, increment);
        machine.set_v(reg_num, initial);
        machine.decode(opcode);
        EXPECT_EQ(machine.get_v(reg_num), final);
    }
}
INSTANTIATE_TEST_CASE_P(
        Opcode7XNNTests,
        Opcode7XNNParameterizedTestFixture,
        ::testing::Values(
            std::make_tuple(0x00, 0x00, 0x00),
            std::make_tuple(0x9D, 0x00, 0x9D),
            std::make_tuple(0x00, 0xEF, 0xEF),
            std::make_tuple(0x6F, 0x38, 0xA7),
            std::make_tuple(0xA2, 0x65, 0x07)
        )
);

class OneNNNParameterizedTestFixture : public ::testing::TestWithParam<OPCODE_TYPE> {};
TEST_P (OneNNNParameterizedTestFixture, Opcode1NNNSetsPCToNNN) {
    OPCODE_TYPE opcode = GetParam();
    Chip8Machine machine;
    int value = opcode & 0x0FFF;
    machine.decode(opcode);
    EXPECT_EQ(machine.get_pc(), value);
}
INSTANTIATE_TEST_CASE_P(
        OneNNNTests,
        OneNNNParameterizedTestFixture,
        ::testing::Values(
                0x1000, 0x1F5D, 0x129A, 0x1FFF
        )
);

/*
TEST (OpcodeDXYN, OpcodeDXYNDrawsToScreen) {
    OPCODE_TYPE opcode = 0xD001;
    int font_address = 0x050;
    unsigned char font_value = 0xFF;
    Chip8Machine machine;
    machine.clear_screen();
    machine.set_flag(0);
    machine.set_i(font_address);
    machine.set_memory_byte(font_address, font_value);
    machine.decode(opcode);
    for (int x = 0; x < 8; x++) {
        int y = 0;
        EXPECT_EQ(machine.get_pixel(x, y), ON_PIXEL);
    }
    EXPECT_EQ(machine.get_flag(), 0);
}
*/
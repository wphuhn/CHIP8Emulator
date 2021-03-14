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

class DXYNRowsParameterizedTestFixture : public ::testing::TestWithParam<int> {};
TEST_P (DXYNRowsParameterizedTestFixture, OpcodeDXYNDrawsCorrectNumberOfRows) {
    int n_rows = GetParam();
    OPCODE_TYPE opcode = 0xD000 + n_rows;
    int font_address = 0x050;
    unsigned char font_value = 0xFF;
    Chip8Machine machine;
    machine.clear_screen();
    machine.set_flag(0);
    machine.set_v(0, 0);
    machine.set_i(font_address);
    for (int row = 0; row < n_rows; row++) {
        machine.set_memory_byte(font_address + row, font_value);
    }
    machine.decode(opcode);
    for (int y = 0; y < n_rows; y++) {
        for (int x = 0; x < 8; x++) {
            EXPECT_EQ(machine.get_pixel(x, y), ON_PIXEL);
        }
    }
    EXPECT_EQ(machine.get_flag(), 0);
}
INSTANTIATE_TEST_CASE_P(
        DXYNTests,
        DXYNRowsParameterizedTestFixture,
        ::testing::Values(
                0x1, 0x8, 0xF
        )
);

class DXYNRegistersParameterizedTestFixture
    : public ::testing::TestWithParam<std::tuple<int, int, int> > {};
TEST_P (DXYNRegistersParameterizedTestFixture, OpcodeDXYNDrawsToCorrectPositionBasedOnRegisterValues) {
    int x_offset = std::get<0>(GetParam());
    int y_offset = std::get<1>(GetParam());
    int n_rows = std::get<2>(GetParam());
    // We omit register VF, as it is used as a flag register here
    for (int x_reg = 0; x_reg < 0xF; x_reg++) {
        for (int y_reg = 0; y_reg < 0xF; y_reg++) {
            OPCODE_TYPE opcode = gen_WXYZ_opcode(0xD, x_reg, y_reg, n_rows);
            int font_address = 0x050;
            unsigned char font_value = 0xFF;
            Chip8Machine machine;
            machine.clear_screen();
            machine.set_flag(0);
            machine.set_v(x_reg, x_offset);
            machine.set_v(y_reg, y_offset);
            if (x_reg == y_reg) {
                // Corner case within test; if the x and y registers are the same,
                // x_offset and y_offset are the same, so pick y_offset
                x_offset = y_offset;
            }
            machine.set_i(font_address);
            for (int row = 0; row < n_rows; row++) {
                machine.set_memory_byte(font_address + row, font_value);
            }
            machine.decode(opcode);
            for (int y = y_offset; y < y_offset + n_rows; y++) {
                for (int x = x_offset; x < x_offset + 8; x++) {
                    EXPECT_EQ(machine.get_pixel(x, y), ON_PIXEL);
                }
            }
            EXPECT_EQ(machine.get_flag(), 0);
        }
    }
}
INSTANTIATE_TEST_CASE_P(
        DXYNTests,
        DXYNRegistersParameterizedTestFixture,
        ::testing::Values(
                std::make_tuple(0x00, 0x00, 0x1),
                std::make_tuple(0xB4, 0x0D, 0x9),
                std::make_tuple(0x37, 0x10, 0xF)
        )
);

class DXYNHorizTruncationParameterizedTestFixture
        : public ::testing::TestWithParam<std::tuple<int, int, int> > {};
TEST_P (DXYNHorizTruncationParameterizedTestFixture, OpcodeDXYNTruncatesDrawingWhenReachesHorizontalEdgeOfScreen) {
    int x_offset = std::get<0>(GetParam());
    int y_offset = std::get<1>(GetParam());
    int n_rows = std::get<2>(GetParam());
    int x_reg = 1;
    int y_reg = 2;
    OPCODE_TYPE opcode = gen_WXYZ_opcode(0xD, x_reg, y_reg, n_rows);
    int font_address = 0x050;
    unsigned char font_value = 0xFF;
    Chip8Machine machine;
    machine.clear_screen();
    machine.set_flag(0);
    machine.set_v(x_reg, x_offset);
    machine.set_v(y_reg, y_offset);
    machine.set_i(font_address);
    for (int row = 0; row < n_rows; row++) {
        machine.set_memory_byte(font_address + row, font_value);
    }
    machine.decode(opcode);
    for (int y = y_offset; y < y_offset + n_rows; y++) {
        for (int x = x_offset; x < machine.display_width; x++) {
            EXPECT_EQ(machine.get_pixel(x, y), ON_PIXEL);
        }
        for (int x = 0; x < (x_offset + 8) - machine.display_width; x++) {
            EXPECT_EQ(machine.get_pixel(x, y), OFF_PIXEL);
        }
    }
    EXPECT_EQ(machine.get_flag(), 0);
}
INSTANTIATE_TEST_CASE_P(
        DXYNTests,
        DXYNHorizTruncationParameterizedTestFixture,
        ::testing::Values(
                std::make_tuple(0x3F, 0x00, 0x1),
                std::make_tuple(0x3D, 0x08, 0x5),
                std::make_tuple(0x39, 0x01, 0xA)
        )
);

class DXYNVertTruncationParameterizedTestFixture
        : public ::testing::TestWithParam<std::tuple<int, int, int> > {};
TEST_P (DXYNVertTruncationParameterizedTestFixture, OpcodeDXYNWrapsDrawingAroundWhenReachesVerticalEdgeOfScreen) {
    int x_offset = std::get<0>(GetParam());
    int y_offset = std::get<1>(GetParam());
    int n_rows = std::get<2>(GetParam());
    int x_reg = 0xB;
    int y_reg = 0xD;
    OPCODE_TYPE opcode = gen_WXYZ_opcode(0xD, x_reg, y_reg, n_rows);
    int font_address = 0x050;
    unsigned char font_value = 0xFF;
    Chip8Machine machine;
    machine.clear_screen();
    machine.set_flag(0);
    machine.set_v(x_reg, x_offset);
    machine.set_v(y_reg, y_offset);
    machine.set_i(font_address);
    for (int row = 0; row < n_rows; row++) {
        machine.set_memory_byte(font_address + row, font_value);
    }
    machine.decode(opcode);
    for (int x = x_offset; x < x_offset + 8; x++) {
        for (int y = y_offset; y < machine.display_height; y++) {
            EXPECT_EQ(machine.get_pixel(x, y), ON_PIXEL);
        }
        for (int y = 0; y < y_offset + n_rows - machine.display_height; y++) {
            EXPECT_EQ(machine.get_pixel(x, y), OFF_PIXEL);
        }
    }
    EXPECT_EQ(machine.get_flag(), 0);
}
INSTANTIATE_TEST_CASE_P(
        DXYNTests,
        DXYNVertTruncationParameterizedTestFixture,
        ::testing::Values(
                std::make_tuple(0x00, 0x1F, 0x2),
                std::make_tuple(0x25, 0x13, 0xF)
        )
);

class DXYNValuesParameterizedTestFixture
        : public ::testing::TestWithParam<std::tuple<unsigned char, unsigned char> > {};
TEST_P (DXYNValuesParameterizedTestFixture, OpcodeDXYNDrawsFontPointedToByIRegister) {
    int n_rows = 2;
    OPCODE_TYPE opcode = 0xD000 + n_rows;
    int font_address = 0x050;
    unsigned char font_value_1 = std::get<0>(GetParam());
    unsigned char font_value_2 = std::get<1>(GetParam());
    Chip8Machine machine;
    machine.clear_screen();
    machine.set_flag(0);
    machine.set_v(0, 0);
    machine.set_i(font_address);
    machine.set_memory_byte(font_address, font_value_1);
    machine.set_memory_byte(font_address + 1, font_value_2);
    machine.decode(opcode);
    for (int x = 0; x < 8; x++) {
        EXPECT_EQ(machine.get_pixel(x, 0), (font_value_1 >> (7-x)) & 0x01);
        EXPECT_EQ(machine.get_pixel(x, 1), (font_value_2 >> (7-x)) & 0x01);
    }
    EXPECT_EQ(machine.get_flag(), 0);
}
INSTANTIATE_TEST_CASE_P(
        DXYNTests,
        DXYNValuesParameterizedTestFixture,
        ::testing::Values(
                std::make_tuple(0xA5, 0x5A)
        )
);

#include "gtest/gtest.h"

#include "chip8machine.hpp"

#include "chip8machinetester.hpp"
#include "utilities.hpp"

#define OFF_PIXEL 0
#define ON_PIXEL 1

class Chip8MachineFixture : public ::testing::Test {
protected:
    Chip8MachineFixture() {
        tester.set_machine(&machine);
    }

    Chip8Machine machine;
    Chip8MachineTester tester;
};

TEST_F (Chip8MachineFixture, HasDefaultConstructor) {}

TEST_F (Chip8MachineFixture, ResetSetsPCToPointToStartOfROM) {
    tester.set_pc(0);
    machine.reset();
    int expected = 0x200;
    EXPECT_EQ(tester.get_pc(), expected);
}

TEST_F (Chip8MachineFixture, PassingUnsupportedOpcodeStopsInterpreter) {
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

TEST_F (Chip8MachineFixture, ClearScreenClearsTheScreen) {
    for (int x = 0; x < machine.display_width; x++) {
        for (int y = 0; y < machine.display_height; y++) {
            tester.set_pixel(x, y, ON_PIXEL);
        }
    }
    machine.clear_screen();
    for (int x = 0; x < machine.display_width; x++) {
        for (int y = 0; y < machine.display_height; y++) {
            EXPECT_EQ(machine.get_pixel(x, y), OFF_PIXEL);
        }
    }
}

TEST_F (Chip8MachineFixture, Opcode00E0ClearsScreen) {
    OPCODE_TYPE opcode = 0x00E0;
    for (int x = 0; x < machine.display_width; x++) {
        for (int y = 0; y < machine.display_height; y++) {
            tester.set_pixel(x, y, ON_PIXEL);
        }
    }
    machine.decode(opcode);
    for (int x = 0; x < machine.display_width; x++) {
        for (int y = 0; y < machine.display_height; y++) {
            EXPECT_EQ(machine.get_pixel(x, y), OFF_PIXEL);
        }
    }
}

class ANNNParameterizedTestFixture : public Chip8MachineFixture,
                                     public ::testing::WithParamInterface<OPCODE_TYPE> {};
TEST_P (ANNNParameterizedTestFixture, OpcodeANNNSetsIRegisterToNNN) {
    OPCODE_TYPE opcode = GetParam();
    int value = opcode & 0x0FFF;
    tester.set_i(0x0000);
    machine.decode(opcode);
    EXPECT_EQ(tester.get_i(), value);
}
INSTANTIATE_TEST_CASE_P(
        ANNNTests,
        ANNNParameterizedTestFixture,
        ::testing::Values(
                0xA000, 0xABFA, 0xA212, 0xAFFF
                )
        );

class Opcode6XNNParameterizedTestFixture : public Chip8MachineFixture,
                                           public ::testing::WithParamInterface<OPCODE_TYPE> {};
TEST_P (Opcode6XNNParameterizedTestFixture, Opcode6XNNSetsRegisterVXToNN) {
    OPCODE_TYPE value = GetParam();
    for (int reg_num = 0; reg_num <= 0xF; reg_num++) {
        OPCODE_TYPE opcode = gen_XYNN_opcode(0x6, reg_num, value);
        tester.set_v(reg_num, 0x0000);
        machine.decode(opcode);
        EXPECT_EQ(tester.get_v(reg_num), value);
    }
}
INSTANTIATE_TEST_CASE_P(
        Opcode6XNNTests,
        Opcode6XNNParameterizedTestFixture,
        ::testing::Values(0x00, 0x66, 0xF7, 0xFF)
);

class Opcode7XNNParameterizedTestFixture : public Chip8MachineFixture,
        public ::testing::WithParamInterface<std::tuple<OPCODE_TYPE, OPCODE_TYPE, OPCODE_TYPE>> {};
TEST_P (Opcode7XNNParameterizedTestFixture, Opcode7XNNAddsNNToRegisterVX) {
    OPCODE_TYPE initial = std::get<0>(GetParam());
    OPCODE_TYPE increment = std::get<1>(GetParam());
    OPCODE_TYPE final = std::get<2>(GetParam());
    for (int reg_num = 0; reg_num <= 0xF; reg_num++) {
        OPCODE_TYPE opcode = gen_XYNN_opcode(0x7, reg_num, increment);
        tester.set_v(reg_num, initial);
        machine.decode(opcode);
        EXPECT_EQ(tester.get_v(reg_num), final);
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

class OneNNNParameterizedTestFixture : public Chip8MachineFixture,
        public ::testing::WithParamInterface<OPCODE_TYPE> {};
TEST_P (OneNNNParameterizedTestFixture, Opcode1NNNSetsPCToNNN) {
    OPCODE_TYPE opcode = GetParam();
    int value = opcode & 0x0FFF;
    machine.decode(opcode);
    EXPECT_EQ(tester.get_pc(), value);
}
INSTANTIATE_TEST_CASE_P(
        OneNNNTests,
        OneNNNParameterizedTestFixture,
        ::testing::Values(
                0x1000, 0x1F5D, 0x129A, 0x1FFF
        )
);

class DXYNRowsParameterizedTestFixture : public ::testing::TestWithParam<int> {};
TEST_P (DXYNRowsParameterizedTestFixture, OpcodeDXYNDrawsCorrectNumberOfRowsToBlankScreen) {
    int x_reg = 1;
    int y_reg = 3;
    int n_rows = GetParam();
    OPCODE_TYPE opcode = gen_WXYZ_opcode(0xD, x_reg, y_reg, n_rows);

    unsigned char font_value = 0xFF;
    std::vector<unsigned char> font(n_rows, font_value);

    int font_address = 0x050;
    int x_offset = 2;
    int y_offset = 4;
    Chip8Machine machine = create_machine_for_drawing(opcode, font_address, font, x_offset, y_offset);
    Chip8MachineTester tester;
    tester.set_machine(&machine);

    machine.decode(opcode);

    for (int y = y_offset; y < y_offset + n_rows; y++) {
        for (int x = x_offset; x < x_offset + 8; x++) {
            EXPECT_EQ(machine.get_pixel(x, y), ON_PIXEL);
        }
    }
    EXPECT_EQ(tester.get_flag(), 0);
}
TEST_P (DXYNRowsParameterizedTestFixture, OpcodeDXYNFlipsPixelsOnCompletelyFilledScreen) {
    int x_reg = 1;
    int y_reg = 3;
    int n_rows = GetParam();
    OPCODE_TYPE opcode = gen_WXYZ_opcode(0xD, x_reg, y_reg, n_rows);

    unsigned char font_value = 0xFF;
    std::vector<unsigned char> font(n_rows, font_value);

    int font_address = 0x050;
    int x_offset = 2;
    int y_offset = 4;
    Chip8Machine machine = create_machine_for_drawing(opcode, font_address, font, x_offset, y_offset);
    Chip8MachineTester tester;
    tester.set_machine(&machine);

    for (int y = 0; y < machine.display_height; y++) {
        for (int x = 0; x < machine.display_width; x++) {
            tester.set_pixel(x, y, ON_PIXEL);
        }
    }

    machine.decode(opcode);

    for (int y = y_offset; y < y_offset + n_rows; y++) {
        for (int x = x_offset; x < x_offset + 8; x++) {
            EXPECT_EQ(machine.get_pixel(x, y), OFF_PIXEL);
        }
    }
    EXPECT_EQ(tester.get_flag(), 1);
}
TEST_P (DXYNRowsParameterizedTestFixture, OpcodeDXYNDrawsModuloOffsetFromRegisters) {
    int x_reg = 1;
    int y_reg = 2;
    int n_rows = GetParam();
    OPCODE_TYPE opcode = gen_WXYZ_opcode(0xD, x_reg, y_reg, n_rows);

    unsigned char font_value = 0xFF;
    std::vector<unsigned char> font(n_rows, font_value);

    int font_address = 0x050;
    int modulo_x_offset = 1;
    int modulo_y_offset = 4;
    // We need to know the machine's width/height to be able to set the offset
    // past it, so we can't set it in the factory method
    int dummy_offset = 0;
    Chip8Machine machine = create_machine_for_drawing(opcode, font_address, font, dummy_offset, dummy_offset);
    Chip8MachineTester tester;
    tester.set_machine(&machine);

    tester.set_v(x_reg, machine.display_width + modulo_x_offset);
    tester.set_v(y_reg, machine.display_height + modulo_y_offset);

    machine.decode(opcode);

    for (int y = modulo_y_offset; y < modulo_y_offset + n_rows; y++) {
        for (int x = modulo_x_offset; x < modulo_x_offset + 8; x++) {
            EXPECT_EQ(machine.get_pixel(x, y), ON_PIXEL);
        }
    }
    EXPECT_EQ(tester.get_flag(), 0);
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

            unsigned char font_value = 0xFF;
            std::vector<unsigned char> font(n_rows, font_value);

            int font_address = 0x050;
            int x_offset_ = x_offset;
            if (x_reg == y_reg) {
                // Corner case within test; if the x and y registers are the same,
                // x_offset and y_offset are the same, so pick y_offset
                x_offset_ = y_offset;
            }
            Chip8Machine machine = create_machine_for_drawing(opcode, font_address, font, x_offset_, y_offset);
            Chip8MachineTester tester;
            tester.set_machine(&machine);

            machine.decode(opcode);

            for (int y = y_offset; y < y_offset + n_rows; y++) {
                for (int x = x_offset_; x < x_offset_ + 8; x++) {
                    EXPECT_EQ(machine.get_pixel(x, y), ON_PIXEL);
                }
            }
            EXPECT_EQ(tester.get_flag(), 0);
        }
    }
}
INSTANTIATE_TEST_CASE_P(
        DXYNTests,
        DXYNRegistersParameterizedTestFixture,
        ::testing::Values(
                std::make_tuple(0x00, 0x00, 0x1),
                std::make_tuple(0x1B, 0x0D, 0x9),
                std::make_tuple(0x37, 0x10, 0xF)
        )
);

class DXYNHorizTruncationParameterizedTestFixture
        : public ::testing::TestWithParam<std::tuple<int, int, int> > {};
TEST_P (DXYNHorizTruncationParameterizedTestFixture, OpcodeDXYNTruncatesDrawingWhenReachesHorizontalEdgeOfScreen) {
    int x_reg = 1;
    int y_reg = 2;
    int n_rows = std::get<2>(GetParam());
    OPCODE_TYPE opcode = gen_WXYZ_opcode(0xD, x_reg, y_reg, n_rows);

    unsigned char font_value = 0xFF;
    std::vector<unsigned char> font(n_rows, font_value);

    int x_offset = std::get<0>(GetParam());
    int y_offset = std::get<1>(GetParam());
    int font_address = 0x050;
    Chip8Machine machine = create_machine_for_drawing(opcode, font_address, font, x_offset, y_offset);
    Chip8MachineTester tester;
    tester.set_machine(&machine);

    machine.decode(opcode);

    // Return offsets back into the screen
    x_offset = x_offset % machine.display_width;
    y_offset = y_offset % machine.display_height;
    for (int y = y_offset; y < y_offset + n_rows; y++) {
        for (int x = x_offset; x < machine.display_width; x++) {
            EXPECT_EQ(machine.get_pixel(x, y), ON_PIXEL);
        }
        for (int x = 0; x < (x_offset + 8) - machine.display_width; x++) {
            EXPECT_EQ(machine.get_pixel(x, y), OFF_PIXEL);
        }
    }
    EXPECT_EQ(tester.get_flag(), 0);
}
INSTANTIATE_TEST_CASE_P(
        DXYNTests,
        DXYNHorizTruncationParameterizedTestFixture,
        ::testing::Values(
                std::make_tuple(0x3F, 0x00, 0x1),
                std::make_tuple(0x3D, 0x08, 0x5),
                std::make_tuple(0x39, 0x01, 0xA),
                // Testing for initial offsets modulo the screen size
                std::make_tuple(0x39 + 0x40, 0x01, 0xA),
                std::make_tuple(0x39, 0x01 + 0x20, 0xA),
                std::make_tuple(0x39 + 0x40, 0x01 + 0x20, 0xA)
        )
);

class DXYNVertTruncationParameterizedTestFixture
        : public ::testing::TestWithParam<std::tuple<int, int, int> > {};
TEST_P (DXYNVertTruncationParameterizedTestFixture, OpcodeDXYNWrapsDrawingAroundWhenReachesVerticalEdgeOfScreen) {
    int x_reg = 0xB;
    int y_reg = 0xD;
    int n_rows = std::get<2>(GetParam());
    OPCODE_TYPE opcode = gen_WXYZ_opcode(0xD, x_reg, y_reg, n_rows);

    unsigned char font_value = 0xFF;
    std::vector<unsigned char> font(n_rows, font_value);

    int font_address = 0x050;
    int x_offset = std::get<0>(GetParam());
    int y_offset = std::get<1>(GetParam());
    Chip8Machine machine = create_machine_for_drawing(opcode, font_address, font, x_offset, y_offset);
    Chip8MachineTester tester;
    tester.set_machine(&machine);

    machine.decode(opcode);

    for (int x = x_offset; x < x_offset + 8; x++) {
        for (int y = y_offset; y < machine.display_height; y++) {
            EXPECT_EQ(machine.get_pixel(x, y), ON_PIXEL);
        }
        for (int y = 0; y < y_offset + n_rows - machine.display_height; y++) {
            EXPECT_EQ(machine.get_pixel(x, y), OFF_PIXEL);
        }
    }
    EXPECT_EQ(tester.get_flag(), 0);
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
    int x_reg = 1;
    int y_reg = 2;
    int n_rows = 2;
    OPCODE_TYPE opcode = gen_WXYZ_opcode(0xD, x_reg, y_reg, n_rows);

    unsigned char font_value_1 = std::get<0>(GetParam());
    unsigned char font_value_2 = std::get<1>(GetParam());
    std::vector<unsigned char> font = {font_value_1, font_value_2};

    int font_address = 0x050;
    int x_offset = 2;
    int y_offset = 4;
    Chip8Machine machine = create_machine_for_drawing(opcode, font_address, font, x_offset, y_offset);
    Chip8MachineTester tester;
    tester.set_machine(&machine);

    machine.decode(opcode);

    for (int x = 0; x < 8; x++) {
        EXPECT_EQ(machine.get_pixel(x_offset + x, y_offset), (font_value_1 >> (7-x)) & 0x01);
        EXPECT_EQ(machine.get_pixel(x_offset + x, y_offset + 1), (font_value_2 >> (7-x)) & 0x01);
    }
    EXPECT_EQ(tester.get_flag(), 0);
}
INSTANTIATE_TEST_CASE_P(
        DXYNTests,
        DXYNValuesParameterizedTestFixture,
        ::testing::Values(
                std::make_tuple(0xA5, 0x5A)
        )
);

TEST_F (Chip8MachineFixture, LoadsROMAtCorrectLocation) {
    const std::vector<unsigned char> rom = {0xBE, 0xEF, 0xCA, 0xCE};
    machine.reset();
    machine.load_rom(rom);
    int pc_start = tester.get_pc();
    for (int i = 0; i < rom.size(); i++) {
        EXPECT_EQ(tester.get_memory_byte(pc_start + i), rom[i]);
    }
}

class FetchInstructionParameterizedTestFixture : public Chip8MachineFixture,
        public ::testing::WithParamInterface<OPCODE_TYPE> {};
TEST_P (FetchInstructionParameterizedTestFixture, FetchInstructionGrabsInstructionAtPCFromMemory) {
    int opcode = GetParam();

    unsigned char byte_one = (opcode >> 8) & 0x00FF;
    unsigned char byte_two = opcode & 0x00FF;
    std::vector<unsigned char> rom = {byte_one, byte_two};

    machine.reset();
    machine.load_rom(rom);

    int actual = tester.fetch_instruction();
    EXPECT_EQ(opcode, actual);
}
INSTANTIATE_TEST_CASE_P(
        FetchInstrutionTests,
        FetchInstructionParameterizedTestFixture,
        ::testing::Values(
                0x0000, 0x5302, 0xFFFF
        )
);

TEST_F (Chip8MachineFixture, AdvanceIncrementsPCForNonJumpInstructions) {
    int reg_int = 1;
    int reg_value = 63;
    OPCODE_TYPE opcode = gen_XYNN_opcode(0x6, reg_int, reg_value);

    unsigned char byte_one = (opcode >> 8) & 0x00FF;
    unsigned char byte_two = opcode & 0x00FF;
    std::vector<unsigned char> rom = {byte_one, byte_two};

    machine.reset();
    machine.load_rom(rom);

    int pc_start = tester.get_pc();
    machine.advance();
    EXPECT_EQ(pc_start + 2, tester.get_pc());
}

TEST_F (Chip8MachineFixture, AdvanceSetsPCExplicitlyForJumpInstructions) {
    int jump_address = 0x227;
    OPCODE_TYPE opcode = 0x1000 + jump_address;

    unsigned char byte_one = (opcode >> 8) & 0x00FF;
    unsigned char byte_two = opcode & 0x00FF;
    std::vector<unsigned char> rom = {byte_one, byte_two};

    machine.reset();
    machine.load_rom(rom);

    machine.advance();
    EXPECT_EQ(jump_address, tester.get_pc());
}

TEST_F (Chip8MachineFixture, AdvanceExecutesInstructionPointedToByPC) {
    // Chose a simple instruction (set register to NN) for this
    int reg_int = 1;
    int reg_value = 63;
    OPCODE_TYPE opcode = gen_XYNN_opcode(0x6, reg_int, reg_value);

    unsigned char byte_one = (opcode >> 8) & 0x00FF;
    unsigned char byte_two = opcode & 0x00FF;
    std::vector<unsigned char> rom = {byte_one, byte_two};

    machine.reset();
    machine.load_rom(rom);

    tester.set_v(reg_int, 0);
    machine.advance();
    EXPECT_EQ(reg_value, tester.get_v(reg_int));
}

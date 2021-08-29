#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"
#include "gtest/gtest.h"

#include "chip8machine.hpp"

#include "chip8machinetester.hpp"
#include "test-constants.hpp"
#include "utilities.hpp"

class Chip8MachineFixture : public ::testing::Test {
 protected:
  Chip8MachineFixture() : tester(Emulator::Chip8MachineTester()) {
    tester.set_machine(&machine);
  }

  Emulator::Chip8Machine machine;
  Emulator::Chip8MachineTester tester;
};

TEST_F(Chip8MachineFixture, Opcode00E0ClearsScreen) {
  Emulator::OPCODE_TYPE opcode = 0x00E0;
  for (int x = 0; x < machine.display_width; x++) {
    for (int y = 0; y < machine.display_height; y++) {
      tester.set_pixel(x, y, TEST_ON_PIXEL);
    }
  }
  machine.decode(opcode);
  for (int x = 0; x < machine.display_width; x++) {
    for (int y = 0; y < machine.display_height; y++) {
      EXPECT_EQ(machine.get_pixel(x, y), TEST_OFF_PIXEL);
    }
  }
}

class ANNNParameterizedTestFixture : public Chip8MachineFixture,
                                     public ::testing::WithParamInterface<Emulator::OPCODE_TYPE> {
};
TEST_P(ANNNParameterizedTestFixture, OpcodeANNNSetsIRegisterToNNN) {
  Emulator::OPCODE_TYPE opcode = GetParam();
  int value = opcode & 0x0FFF;
  tester.set_i(0x0000);
  machine.decode(opcode);
  EXPECT_EQ(tester.get_i(), value);
}
INSTANTIATE_TEST_SUITE_P
(
    ANNNTests,
    ANNNParameterizedTestFixture,
    ::testing::Values(0xA000, 0xABFA, 0xA212, 0xAFFF));

class Opcode00EEParameterizedTestFixture : public Chip8MachineFixture,
                                           public ::testing::WithParamInterface< std::tuple<Emulator::ADDR_TYPE, Emulator::OPCODE_TYPE> > {
};
TEST_P(Opcode00EEParameterizedTestFixture, Opcode00EEPopsTopOfStack) {
  auto stack_top = std::get<1>(GetParam());
  Emulator::OPCODE_TYPE opcode = 0x00EE;
  Emulator::ADDR_TYPE padding = 0x0000;
  tester.add_to_stack(padding);
  tester.add_to_stack(stack_top);
  EXPECT_EQ(tester.get_top_of_stack(), stack_top);
  machine.decode(opcode);
  EXPECT_EQ(tester.get_top_of_stack(), padding);
}
TEST_P(Opcode00EEParameterizedTestFixture, Opcode00EESetsPCToValue) {
  auto new_addr = std::get<0>(GetParam());
  Emulator::OPCODE_TYPE opcode = 0x00EE;
  Emulator::ADDR_TYPE old_addr = 0x0000;
  tester.set_pc(old_addr);
  tester.add_to_stack(new_addr);
  machine.decode(opcode);
  EXPECT_EQ(tester.get_pc(), new_addr);
}
INSTANTIATE_TEST_SUITE_P
(
    Opcode00EETests,
    Opcode00EEParameterizedTestFixture,
    ::testing::Values(std::make_tuple(0x24DC, 0x2383),
                      std::make_tuple(0x2678, 0x2536),
                      std::make_tuple(0x2C62, 0x2752),
                      std::make_tuple(0x2D8E, 0x289F))
);

class Opcode2NNNParameterizedTestFixture : public Chip8MachineFixture,
 public ::testing::WithParamInterface< std::tuple<Emulator::ADDR_TYPE, Emulator::OPCODE_TYPE> > {
};
TEST_P(Opcode2NNNParameterizedTestFixture, Opcode2NNNPutsCurrentAddressOnStack) {
  auto pc = std::get<0>(GetParam());
  auto opcode = std::get<1>(GetParam());
  tester.set_pc(pc);
  machine.decode(opcode);
  EXPECT_EQ(tester.get_top_of_stack(), pc);
}
TEST_P(Opcode2NNNParameterizedTestFixture, Opcode2NNNSetsPCToCorrectValue) {
  auto pc = std::get<0>(GetParam());
  auto opcode = std::get<1>(GetParam());
  int value = opcode & 0x0FFF;
  tester.set_pc(pc);
  machine.decode(opcode);
  EXPECT_EQ(tester.get_pc(), value);
}
INSTANTIATE_TEST_SUITE_P
(
    Opcode2NNNTests,
    Opcode2NNNParameterizedTestFixture,
    ::testing::Values(std::make_tuple(0x2537, 0x2000),
                      std::make_tuple(0x2C14, 0x2BFA),
                      std::make_tuple(0x28DB, 0x2212),
                      std::make_tuple(0x2F94, 0x2FFF))
);

class Opcode3NNNParameterizedTestFixture : public Chip8MachineFixture,
 public ::testing::WithParamInterface< std::tuple<Emulator::ADDR_TYPE, Emulator::OPCODE_TYPE> > {
};
TEST_P(Opcode3NNNParameterizedTestFixture, Opcode3NNNIncrementsPCWhenValuesEqual) {
  auto pc = std::get<0>(GetParam());
  auto opcode = std::get<1>(GetParam());

  int reg_num = (opcode & 0x0F00) >> 8;
  int value = opcode & 0x00FF;

  tester.set_pc(pc);
  tester.set_v(reg_num, value);
  machine.decode(opcode);
  EXPECT_EQ(tester.get_pc(), pc + TEST_INSTRUCTION_LENGTH);
}
TEST_P(Opcode3NNNParameterizedTestFixture, Opcode3NNNDoesNothingWhenValuesNotEqual) {
  auto pc = std::get<0>(GetParam());
  auto opcode = std::get<1>(GetParam());

  int reg_num = (opcode & 0x0F00) >> 8;
  int value = opcode & 0x00FF;

  tester.set_pc(pc);
  tester.set_v(reg_num, value + 1);
  machine.decode(opcode);
  EXPECT_EQ(tester.get_pc(), pc);
}
INSTANTIATE_TEST_SUITE_P
(
    Opcode3NNNTests,
    Opcode3NNNParameterizedTestFixture,
    ::testing::Values(std::make_tuple(0x215F, 0x3B83),
                      std::make_tuple(0x27C7, 0x3029),
                      std::make_tuple(0x268D, 0x3898),
                      std::make_tuple(0x28C4, 0x3630))
);

class Opcode6XNNParameterizedTestFixture : public Chip8MachineFixture,
                                           public ::testing::WithParamInterface<Emulator::OPCODE_TYPE> {
};
TEST_P(Opcode6XNNParameterizedTestFixture, Opcode6XNNSetsRegisterVXToNN) {
  Emulator::OPCODE_TYPE value = GetParam();
  for (int reg_num = 0; reg_num < TEST_NUM_REGISTERS; reg_num++) {
    Emulator::OPCODE_TYPE opcode = Emulator::gen_XYNN_opcode(0x6, reg_num, value);
    tester.set_v(reg_num, 0x0000);
    machine.decode(opcode);
    EXPECT_EQ(tester.get_v(reg_num), value);
  }
}
INSTANTIATE_TEST_SUITE_P
(
    Opcode6XNNTests,
    Opcode6XNNParameterizedTestFixture,
    ::testing::Values(0x00, 0x66, 0xF7, 0xFF)
);

class Opcode7XNNParameterizedTestFixture : public Chip8MachineFixture,
                                           public ::testing::WithParamInterface<std::tuple<Emulator::OPCODE_TYPE,
                                                                                           Emulator::OPCODE_TYPE,
                                                                                           Emulator::OPCODE_TYPE>> {
};
TEST_P(Opcode7XNNParameterizedTestFixture, Opcode7XNNAddsNNToRegisterVX) {
  Emulator::OPCODE_TYPE initial = std::get<0>(GetParam());
  Emulator::OPCODE_TYPE increment = std::get<1>(GetParam());
  Emulator::OPCODE_TYPE final = std::get<2>(GetParam());
  for (int reg_num = 0; reg_num < TEST_NUM_REGISTERS; reg_num++) {
    Emulator::OPCODE_TYPE opcode = Emulator::gen_XYNN_opcode(0x7, reg_num, increment);
    tester.set_v(reg_num, initial);
    machine.decode(opcode);
    EXPECT_EQ(tester.get_v(reg_num), final);
  }
}
INSTANTIATE_TEST_SUITE_P
(
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

class Opcode8XY2ParameterizedTestFixture : public Chip8MachineFixture,
    public ::testing::WithParamInterface<std::tuple<Emulator::OPCODE_TYPE,
    Emulator::REG_TYPE,
    Emulator::REG_TYPE,
    Emulator::REG_TYPE>> {
};
TEST_P(Opcode8XY2ParameterizedTestFixture, Opcode8XY2StoresANDIntoVXAndLeaveVYUnchanged) {
  Emulator::OPCODE_TYPE opcode = std::get<0>(GetParam());
  Emulator::REG_TYPE value_x = std::get<1>(GetParam());
  Emulator::REG_TYPE value_y = std::get<2>(GetParam());
  Emulator::OPCODE_TYPE result = std::get<3>(GetParam());
  int reg_num_x = (opcode & 0x0F00) >> 8;
  int reg_num_y = (opcode & 0x00F0) >> 4;
  // Sanity check for tests
  if ((reg_num_x == reg_num_y) && (value_x != value_y)) {
    ASSERT_EQ(0, 1);
  }
  tester.set_v(reg_num_x, value_x);
  tester.set_v(reg_num_y, value_y);
  machine.decode(opcode);
  int actual_x = tester.get_v(reg_num_x);
  int actual_y = tester.get_v(reg_num_y);
  ASSERT_EQ(result, actual_x);
  ASSERT_EQ(value_y, actual_y);
}
INSTANTIATE_TEST_SUITE_P
(
    Opcode8XY2Tests,
    Opcode8XY2ParameterizedTestFixture,
    ::testing::Values(
        std::make_tuple(0x8002, 0x00, 0x00, 0x00),
        std::make_tuple(0x8002, 0xFF, 0xFF, 0xFF),
        std::make_tuple(0x8012, 0x00, 0xFF, 0x00),
        std::make_tuple(0x8012, 0xFF, 0x00, 0x00),
        std::make_tuple(0x8012, 0xFF, 0xFF, 0xFF),
        std::make_tuple(0x8282, 0xE4, 0xB9, 0xA0),
        std::make_tuple(0x8992, 0x71, 0x71, 0x71),
        std::make_tuple(0x8342, 0xD7, 0x59, 0x51),
        std::make_tuple(0x8722, 0x56, 0x57, 0x56),
        std::make_tuple(0x8B12, 0x70, 0x4F, 0x40)
    )
);

class OneNNNParameterizedTestFixture : public Chip8MachineFixture,
                                       public ::testing::WithParamInterface<Emulator::OPCODE_TYPE> {
};
TEST_P(OneNNNParameterizedTestFixture, Opcode1NNNSetsPCToNNN) {
  Emulator::OPCODE_TYPE opcode = GetParam();
  int value = opcode & 0x0FFF;
  machine.decode(opcode);
  EXPECT_EQ(tester.get_pc(), value);
}
INSTANTIATE_TEST_SUITE_P
(
    OneNNNTests,
    OneNNNParameterizedTestFixture,
    ::testing::Values(
        0x1000, 0x1F5D, 0x129A, 0x1FFF
    )
);

class DXYNRowsParameterizedTestFixture : public ::testing::TestWithParam<int> {};
TEST_P(DXYNRowsParameterizedTestFixture, OpcodeDXYNDrawsCorrectNumberOfRowsToBlankScreen) {
  int x_reg = 1;
  int y_reg = 3;
  int n_rows = GetParam();
  Emulator::OPCODE_TYPE opcode = Emulator::gen_WXYZ_opcode(0xD, x_reg, y_reg, n_rows);

  unsigned char font_value = 0xFF;
  std::vector<unsigned char> font(n_rows, font_value);

  int font_address = 0x050;
  int x_offset = 2;
  int y_offset = 4;
  Emulator::Chip8Machine machine = Emulator::create_machine_for_drawing(opcode, font_address, font, x_offset, y_offset);
  Emulator::Chip8MachineTester tester = Emulator::Chip8MachineTester();
  tester.set_machine(&machine);

  machine.decode(opcode);

  for (int y = y_offset; y < y_offset + n_rows; y++) {
    for (int x = x_offset; x < x_offset + TEST_FONT_WIDTH; x++) {
      EXPECT_EQ(machine.get_pixel(x, y), TEST_ON_PIXEL);
    }
  }
  EXPECT_EQ(tester.get_flag(), 0);
}
TEST_P(DXYNRowsParameterizedTestFixture, OpcodeDXYNFlipsPixelsOnCompletelyFilledScreen) {
  int x_reg = 1;
  int y_reg = 3;
  int n_rows = GetParam();
  Emulator::OPCODE_TYPE opcode = Emulator::gen_WXYZ_opcode(0xD, x_reg, y_reg, n_rows);

  unsigned char font_value = 0xFF;
  std::vector<unsigned char> font(n_rows, font_value);

  int font_address = 0x050;
  int x_offset = 2;
  int y_offset = 4;
  Emulator::Chip8Machine machine = Emulator::create_machine_for_drawing(opcode, font_address, font, x_offset, y_offset);
  Emulator::Chip8MachineTester tester = Emulator::Chip8MachineTester();
  tester.set_machine(&machine);

  for (int y = 0; y < machine.display_height; y++) {
    for (int x = 0; x < machine.display_width; x++) {
      tester.set_pixel(x, y, TEST_ON_PIXEL);
    }
  }

  machine.decode(opcode);

  for (int y = y_offset; y < y_offset + n_rows; y++) {
    for (int x = x_offset; x < x_offset + TEST_FONT_WIDTH; x++) {
      EXPECT_EQ(machine.get_pixel(x, y), TEST_OFF_PIXEL);
    }
  }
  EXPECT_EQ(tester.get_flag(), 1);
}
TEST_P(DXYNRowsParameterizedTestFixture, OpcodeDXYNDrawsModuloOffsetFromRegisters) {
  int x_reg = 1;
  int y_reg = 2;
  int n_rows = GetParam();
  Emulator::OPCODE_TYPE opcode = Emulator::gen_WXYZ_opcode(0xD, x_reg, y_reg, n_rows);

  unsigned char font_value = 0xFF;
  std::vector<unsigned char> font(n_rows, font_value);

  int font_address = 0x050;
  int modulo_x_offset = 1;
  int modulo_y_offset = 4;
  // We need to know the machine's width/height to be able to set the offset
  // past it, so we can't set it in the factory method
  int dummy_offset = 0;
  Emulator::Chip8Machine machine = Emulator::create_machine_for_drawing(opcode, font_address, font, dummy_offset, dummy_offset);
  Emulator::Chip8MachineTester tester = Emulator::Chip8MachineTester();
  tester.set_machine(&machine);

  tester.set_v(x_reg, machine.display_width + modulo_x_offset);
  tester.set_v(y_reg, machine.display_height + modulo_y_offset);

  machine.decode(opcode);

  for (int y = modulo_y_offset; y < modulo_y_offset + n_rows; y++) {
    for (int x = modulo_x_offset; x < modulo_x_offset + TEST_FONT_WIDTH; x++) {
      EXPECT_EQ(machine.get_pixel(x, y), TEST_ON_PIXEL);
    }
  }
  EXPECT_EQ(tester.get_flag(), 0);
}
INSTANTIATE_TEST_SUITE_P
(
    DXYNTests,
    DXYNRowsParameterizedTestFixture,
    ::testing::Values(
        0x1, 0x8, 0xF
    )
);

class DXYNRegistersParameterizedTestFixture
    : public ::testing::TestWithParam<std::tuple<int, int, int> > {
};
TEST_P(DXYNRegistersParameterizedTestFixture, OpcodeDXYNDrawsToCorrectPositionBasedOnRegisterValues) {
  int x_offset = std::get<0>(GetParam());
  int y_offset = std::get<1>(GetParam());
  int n_rows = std::get<2>(GetParam());

  // We omit register VF, as it is used as a flag register here
  for (int x_reg = 0; x_reg < TEST_NUM_REGISTERS - 1; x_reg++) {
    for (int y_reg = 0; y_reg < TEST_NUM_REGISTERS - 1; y_reg++) {
      Emulator::OPCODE_TYPE opcode = Emulator::gen_WXYZ_opcode(0xD, x_reg, y_reg, n_rows);

      unsigned char font_value = 0xFF;
      std::vector<unsigned char> font(n_rows, font_value);

      int font_address = 0x050;
      int x_offset_ = x_offset;
      if (x_reg == y_reg) {
        // Corner case within test; if the x and y registers are the same,
        // x_offset and y_offset are the same, so pick y_offset
        x_offset_ = y_offset;
      }
      Emulator::Chip8Machine machine = Emulator::create_machine_for_drawing(opcode, font_address, font, x_offset_, y_offset);
      Emulator::Chip8MachineTester tester = Emulator::Chip8MachineTester();
      tester.set_machine(&machine);

      machine.decode(opcode);

      for (int y = y_offset; y < y_offset + n_rows; y++) {
        for (int x = x_offset_; x < x_offset_ + TEST_FONT_WIDTH; x++) {
          EXPECT_EQ(machine.get_pixel(x, y), TEST_ON_PIXEL);
        }
      }
      EXPECT_EQ(tester.get_flag(), 0);
    }
  }
}
INSTANTIATE_TEST_SUITE_P
(
    DXYNTests,
    DXYNRegistersParameterizedTestFixture,
    ::testing::Values(
        std::make_tuple(0x00, 0x00, 0x1),
        std::make_tuple(0x1B, 0x0D, 0x9),
        std::make_tuple(0x37, 0x10, 0xF)
    )
);

class DXYNHorizTruncationParameterizedTestFixture
    : public ::testing::TestWithParam<std::tuple<int, int, int> > {
};
TEST_P(DXYNHorizTruncationParameterizedTestFixture, OpcodeDXYNTruncatesDrawingWhenReachesHorizontalEdgeOfScreen) {
  int x_reg = 1;
  int y_reg = 2;
  int n_rows = std::get<2>(GetParam());
  Emulator::OPCODE_TYPE opcode = Emulator::gen_WXYZ_opcode(0xD, x_reg, y_reg, n_rows);

  unsigned char font_value = 0xFF;
  std::vector<unsigned char> font(n_rows, font_value);

  int x_offset = std::get<0>(GetParam());
  int y_offset = std::get<1>(GetParam());
  int font_address = 0x050;
  Emulator::Chip8Machine machine = Emulator::create_machine_for_drawing(opcode, font_address, font, x_offset, y_offset);
  Emulator::Chip8MachineTester tester = Emulator::Chip8MachineTester();
  tester.set_machine(&machine);

  machine.decode(opcode);

  // Return offsets back into the screen
  x_offset = x_offset % machine.display_width;
  y_offset = y_offset % machine.display_height;
  for (int y = y_offset; y < y_offset + n_rows; y++) {
    for (int x = x_offset; x < machine.display_width; x++) {
      EXPECT_EQ(machine.get_pixel(x, y), TEST_ON_PIXEL);
    }
    for (int x = 0; x < (x_offset + TEST_FONT_WIDTH) - machine.display_width; x++) {
      EXPECT_EQ(machine.get_pixel(x, y), TEST_OFF_PIXEL);
    }
  }
  EXPECT_EQ(tester.get_flag(), 0);
}
INSTANTIATE_TEST_SUITE_P
(
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
    : public ::testing::TestWithParam<std::tuple<int, int, int> > {
};
TEST_P(DXYNVertTruncationParameterizedTestFixture, OpcodeDXYNWrapsDrawingAroundWhenReachesVerticalEdgeOfScreen) {
  int x_reg = 0xB;
  int y_reg = 0xD;
  int n_rows = std::get<2>(GetParam());
  Emulator::OPCODE_TYPE opcode = Emulator::gen_WXYZ_opcode(0xD, x_reg, y_reg, n_rows);

  unsigned char font_value = 0xFF;
  std::vector<unsigned char> font(n_rows, font_value);

  int font_address = 0x050;
  int x_offset = std::get<0>(GetParam());
  int y_offset = std::get<1>(GetParam());
  Emulator::Chip8Machine machine = Emulator::create_machine_for_drawing(opcode, font_address, font, x_offset, y_offset);
  Emulator::Chip8MachineTester tester = Emulator::Chip8MachineTester();
  tester.set_machine(&machine);

  machine.decode(opcode);

  for (int x = x_offset; x < x_offset + TEST_FONT_WIDTH; x++) {
    for (int y = y_offset; y < machine.display_height; y++) {
      EXPECT_EQ(machine.get_pixel(x, y), TEST_ON_PIXEL);
    }
    for (int y = 0; y < y_offset + n_rows - machine.display_height; y++) {
      EXPECT_EQ(machine.get_pixel(x, y), TEST_OFF_PIXEL);
    }
  }
  EXPECT_EQ(tester.get_flag(), 0);
}
INSTANTIATE_TEST_SUITE_P
(
    DXYNTests,
    DXYNVertTruncationParameterizedTestFixture,
    ::testing::Values(
        std::make_tuple(0x00, 0x1F, 0x2),
        std::make_tuple(0x25, 0x13, 0xF)
    )
);

class DXYNValuesParameterizedTestFixture
    : public ::testing::TestWithParam<std::tuple<unsigned char, unsigned char> > {
};
TEST_P(DXYNValuesParameterizedTestFixture, OpcodeDXYNDrawsFontPointedToByIRegister) {
  int x_reg = 1;
  int y_reg = 2;
  int n_rows = 2;
  Emulator::OPCODE_TYPE opcode = Emulator::gen_WXYZ_opcode(0xD, x_reg, y_reg, n_rows);

  unsigned char font_value_1 = std::get<0>(GetParam());
  unsigned char font_value_2 = std::get<1>(GetParam());
  std::vector<unsigned char> font = {font_value_1, font_value_2};

  int font_address = 0x050;
  int x_offset = 2;
  int y_offset = 4;
  Emulator::Chip8Machine machine = Emulator::create_machine_for_drawing(opcode, font_address, font, x_offset, y_offset);
  Emulator::Chip8MachineTester tester = Emulator::Chip8MachineTester();
  tester.set_machine(&machine);

  machine.decode(opcode);

  for (int x = 0; x < TEST_FONT_WIDTH; x++) {
    EXPECT_EQ(machine.get_pixel(x_offset + x, y_offset), (font_value_1 >> (7 - x)) & 0x01);
    EXPECT_EQ(machine.get_pixel(x_offset + x, y_offset + 1), (font_value_2 >> (7 - x)) & 0x01);
  }
  EXPECT_EQ(tester.get_flag(), 0);
}
INSTANTIATE_TEST_SUITE_P
(
    DXYNTests,
    DXYNValuesParameterizedTestFixture,
    ::testing::Values(
        std::make_tuple(0xA5, 0x5A)
    )
);

class OpcodeFX15ParameterizedTestFixture : public Chip8MachineFixture,
                                           public ::testing::WithParamInterface< std::tuple<Emulator::OPCODE_TYPE, Emulator::ADDR_TYPE> > {
};
TEST_P(OpcodeFX15ParameterizedTestFixture, OpcodeFX15SetsDelayTimerToValueInInVRegister) {
  auto opcode = std::get<0>(GetParam());
  auto value = std::get<1>(GetParam());

  int v_num = (opcode & 0x0F00) >> 8;

  tester.set_delay_timer(0);
  tester.set_v(v_num, value);

  machine.decode(opcode);
  EXPECT_EQ(tester.get_delay_timer(), value);
}
INSTANTIATE_TEST_SUITE_P
(
    OpcodeFX15Tests,
    OpcodeFX15ParameterizedTestFixture,
    ::testing::Values(std::make_tuple(0xF015, 0xFB),
                      std::make_tuple(0xF215, 0x86),
                      std::make_tuple(0xF615, 0xC5),
                      std::make_tuple(0xFF15, 0xFA))
);

class OpcodeFX07ParameterizedTestFixture : public Chip8MachineFixture,
                                           public ::testing::WithParamInterface< std::tuple<Emulator::OPCODE_TYPE, Emulator::ADDR_TYPE> > {
};
TEST_P(OpcodeFX07ParameterizedTestFixture, OpcodeFX07SetsVRegisterToValueInDelayTimer) {
  auto opcode = std::get<0>(GetParam());
  auto value = std::get<1>(GetParam());

  int v_num = (opcode & 0x0F00) >> 8;

  tester.set_delay_timer(value);
  tester.set_v(v_num, 0);

  machine.decode(opcode);
  EXPECT_EQ(tester.get_v(v_num), value);
}
INSTANTIATE_TEST_SUITE_P
(
    OpcodeFX07Tests,
    OpcodeFX07ParameterizedTestFixture,
    ::testing::Values(std::make_tuple(0xF007, 0x10),
                      std::make_tuple(0xFE07, 0xDF),
                      std::make_tuple(0xF407, 0x81),
                      std::make_tuple(0xFF07, 0x89))
);

class OpcodeFX29ParameterizedTestFixture : public Chip8MachineFixture,
    public ::testing::WithParamInterface< std::tuple<Emulator::OPCODE_TYPE, Emulator::ADDR_TYPE> > {
};
TEST_P(OpcodeFX29ParameterizedTestFixture, OpcodeFX29SetsIRegisterToAddressInVRegister) {
  auto opcode = std::get<0>(GetParam());
  auto addr = std::get<1>(GetParam());

  int v_num = (opcode & 0x0F00) >> 8;

  tester.set_i(0);
  tester.set_v(v_num, addr);

  machine.decode(opcode);
  EXPECT_EQ(tester.get_i(), addr);
}
INSTANTIATE_TEST_SUITE_P
(
    OpcodeFX29Tests,
    OpcodeFX29ParameterizedTestFixture,
    ::testing::Values(std::make_tuple(0xF029, 0x2E12),
                      std::make_tuple(0xF929, 0x27AE),
                      std::make_tuple(0xF429, 0x27E7),
                      std::make_tuple(0xFF29, 0x22BE))
);

class OpcodeFX33ParameterizedTestFixture : public Chip8MachineFixture,
 public ::testing::WithParamInterface< std::tuple<int, Emulator::ADDR_TYPE, Emulator::OPCODE_TYPE> > {
};
TEST_P(OpcodeFX33ParameterizedTestFixture, OpcodeFX33SetsMemoryValues) {
  auto value = std::get<0>(GetParam());
  auto i_addr = std::get<1>(GetParam());
  auto opcode = std::get<2>(GetParam());

  int v_num = (opcode & 0x0F00) >> 8;
  int digit_hundred = (value / 100) % 10;
  int digit_ten = (value / 10) % 10;
  int digit_one = value % 10;

  tester.set_i(i_addr);
  tester.set_v(v_num, value);

  machine.decode(opcode);
  EXPECT_EQ(tester.get_memory_byte(i_addr), digit_hundred);
  EXPECT_EQ(tester.get_memory_byte(i_addr+1), digit_ten);
  EXPECT_EQ(tester.get_memory_byte(i_addr+2), digit_one);
}
INSTANTIATE_TEST_SUITE_P
(
    OpcodeFX33Tests,
    OpcodeFX33ParameterizedTestFixture,
    ::testing::Values(std::make_tuple(0, 0x2E9C, 0xFA33),
                      std::make_tuple(7, 0x204A, 0xF733),
                      std::make_tuple(98, 0x2D62, 0xFB33),
                      std::make_tuple(255, 0x24AA, 0xF933))
);

class OpcodeFX65ParameterizedTestFixture : public Chip8MachineFixture,
 public ::testing::WithParamInterface< std::tuple<Emulator::OPCODE_TYPE, Emulator::ADDR_TYPE> > {
};
TEST_P(OpcodeFX65ParameterizedTestFixture, OpcodeFX65SetsRegistersFromMemory) {
  auto opcode = std::get<0>(GetParam());
  auto i_addr = std::get<1>(GetParam());

  int v_num = (opcode & 0x0F00) >> 8;

  tester.set_i(i_addr);
  for (int i = 0; i <= v_num; i++) {
    tester.set_v(i, 0);
    tester.set_memory_byte(i_addr + i, i + 1);
  }

  machine.decode(opcode);
  for (int i = 0; i <= v_num; i++) {
    EXPECT_EQ(tester.get_v(i), i + 1);
  }
  for (int i = v_num + 1; i <= 0xF; i++) {
    EXPECT_EQ(tester.get_v(i), 0);
  }
}
INSTANTIATE_TEST_SUITE_P
(
    OpcodeFX65Tests,
    OpcodeFX65ParameterizedTestFixture,
    ::testing::Values(std::make_tuple(0xF065, 0x20D2),
                      std::make_tuple(0xFC65, 0x22CD),
                      std::make_tuple(0xF565, 0x27D3),
                      std::make_tuple(0xFF65, 0x2BDD))
);

namespace {
// Testing pseudo-randomness is always fun
// The tests for opcode 0xCXNN assume we are using std::mt19937 as the generator engine,
// std::uniform_int_distribution(0, 0xFF) as the distribution, and the seed value is
// fixed to a particular value (here 0 because why not).
int expected_vals[3] = {0x8C, 0x97, 0xB7};
int seed_val = 0;

class OpcodeCXNNParameterizedTestFixture : public Chip8MachineFixture,
    public ::testing::WithParamInterface<Emulator::OPCODE_TYPE> {
};
TEST_P(OpcodeCXNNParameterizedTestFixture, OpcodeCXNNGeneratesRandomNumberWithNoMask) {
  // Putting as part of the fixture (despite not using parameterized tests) so that it
  // runs multiple times, verifying PRNG initialization behaves deterministically
  Emulator::OPCODE_TYPE opcode = 0xC0FF;
  machine.set_seed(seed_val);
  for (auto expected : expected_vals) {
    machine.decode(opcode);
    int result = tester.get_v(0);
    EXPECT_EQ(result, expected);
  }
}
TEST_P(OpcodeCXNNParameterizedTestFixture, OpcodeCXNNGeneratesRandomNumberWithMask) {
  Emulator::OPCODE_TYPE opcode = GetParam();
  int reg_num = (opcode & 0x0F00) >> 8;
  int mask = opcode & 0x00FF;
  machine.set_seed(seed_val);
  for (auto expected : expected_vals) {
    expected = expected & mask;
    machine.decode(opcode);
    int result = tester.get_v(reg_num);
    EXPECT_EQ(result, expected);
  }
}
INSTANTIATE_TEST_SUITE_P
(
    OpcodeCXNNTests,
    OpcodeCXNNParameterizedTestFixture,
    ::testing::Values(0xC6FF, 0xC300, 0xCC21, 0xCF56)
);
}  // namespace

#ifndef __CLION_IDE_
#pragma clang diagnostic pop
#endif

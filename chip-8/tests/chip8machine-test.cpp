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

TEST_F(Chip8MachineFixture, HasDefaultConstructor) {}

TEST_F(Chip8MachineFixture, ResetSetsPCToPointToStartOfROM) {
  Emulator::ADDR_TYPE start_pc = 0x201;
  assert(start_pc != TEST_ROM_START_ADDRESS);
  tester.set_pc(start_pc);
  machine.reset();
  Emulator::ADDR_TYPE expected = TEST_ROM_START_ADDRESS;
  EXPECT_EQ(tester.get_pc(), expected);
}

TEST_F(Chip8MachineFixture, PassingUnsupportedOpcodeStopsInterpreter) {
  // 0x9000 was chosen because it's not part of CHIP-8 instruction set
  Emulator::OPCODE_TYPE bad_opcode = 0x9000;
  try {
    machine.decode(bad_opcode);
    FAIL() << "Expected OpcodeNotSupported exception to be thrown for unsupported opcode, none were thrown";
  }
  catch (const Emulator::OpcodeNotSupported &err) {
    EXPECT_EQ(err.what(), std::string("Opcode 0x9000 not supported"));
  }
  catch (...) {
    FAIL() << "Expected OpcodeNotSupported exception to be thrown for unsupported opcode, another exception was thrown";
  }
}

TEST_F(Chip8MachineFixture, ClearScreenClearsTheScreen) {
  for (int x = 0; x < machine.display_width; x++) {
    for (int y = 0; y < machine.display_height; y++) {
      tester.set_pixel(x, y, TEST_ON_PIXEL);
    }
  }
  machine.clear_screen();
  for (int x = 0; x < machine.display_width; x++) {
    for (int y = 0; y < machine.display_height; y++) {
      EXPECT_EQ(machine.get_pixel(x, y), TEST_OFF_PIXEL);
    }
  }
}

TEST_F(Chip8MachineFixture, LoadsROMAtCorrectLocation) {
  const std::vector<unsigned char> rom = {0xBE, 0xEF, 0xCA, 0xCE};
  machine.reset();
  machine.load_rom(rom);
  Emulator::ADDR_TYPE pc_start = tester.get_pc();
  for (int i = 0; i < rom.size(); i++) {
    EXPECT_EQ(tester.get_memory_byte(pc_start + i), rom[i]);
  }
}

class FetchInstructionParameterizedTestFixture : public Chip8MachineFixture,
                                                 public ::testing::WithParamInterface<Emulator::OPCODE_TYPE> {
};
TEST_P(FetchInstructionParameterizedTestFixture, FetchInstructionGrabsInstructionAtPCFromMemory) {
  int opcode = GetParam();

  unsigned char byte_one = (opcode >> 8) & 0x00FF;
  unsigned char byte_two = opcode & 0x00FF;
  std::vector<unsigned char> rom = {byte_one, byte_two};

  machine.reset();
  machine.load_rom(rom);

  int actual = tester.fetch_instruction();
  EXPECT_EQ(opcode, actual);
}
INSTANTIATE_TEST_SUITE_P
(
    FetchInstrutionTests,
    FetchInstructionParameterizedTestFixture,
    ::testing::Values(
        0x0000, 0x5302, 0xFFFF
    )
);

TEST_F(Chip8MachineFixture, AdvanceIncrementsPCForNonJumpInstructions) {
  int reg_int = 1;
  int reg_value = 63;
  Emulator::OPCODE_TYPE opcode = Emulator::gen_XYNN_opcode(0x6, reg_int, reg_value);

  unsigned char byte_one = (opcode >> 8) & 0x00FF;
  unsigned char byte_two = opcode & 0x00FF;
  std::vector<unsigned char> rom = {byte_one, byte_two};

  machine.reset();
  machine.load_rom(rom);

  Emulator::ADDR_TYPE pc_start = tester.get_pc();
  machine.advance();
  EXPECT_EQ(pc_start + 2, tester.get_pc());
}

TEST_F(Chip8MachineFixture, AdvanceSetsPCExplicitlyForJumpInstructions) {
  int jump_address = 0x227;
  Emulator::OPCODE_TYPE opcode = 0x1000 + jump_address;

  unsigned char byte_one = (opcode >> 8) & 0x00FF;
  unsigned char byte_two = opcode & 0x00FF;
  std::vector<unsigned char> rom = {byte_one, byte_two};

  machine.reset();
  machine.load_rom(rom);

  machine.advance();
  EXPECT_EQ(jump_address, tester.get_pc());
}

TEST_F(Chip8MachineFixture, AdvanceExecutesInstructionPointedToByPC) {
  // Chose a simple instruction (set register to NN) for this
  int reg_int = 1;
  int reg_value = 63;
  Emulator::OPCODE_TYPE opcode = Emulator::gen_XYNN_opcode(0x6, reg_int, reg_value);

  unsigned char byte_one = (opcode >> 8) & 0x00FF;
  unsigned char byte_two = opcode & 0x00FF;
  std::vector<unsigned char> rom = {byte_one, byte_two};

  machine.reset();
  machine.load_rom(rom);

  tester.set_v(reg_int, 0);
  machine.advance();
  EXPECT_EQ(reg_value, tester.get_v(reg_int));
}

TEST_F(Chip8MachineFixture, TriggerDelayTimerDoesNothingWhenTimerIsZero) {
  tester.set_delay_timer(0);
  machine.trigger_delay_timer();
  EXPECT_EQ(0, tester.get_delay_timer());
}

TEST_F(Chip8MachineFixture, TriggerDelayTimerDecrementsTimerWhenTimerGreaterThanZero) {
  Emulator::REG_TYPE some_value = 10;
  tester.set_delay_timer(some_value);
  machine.trigger_delay_timer();
  EXPECT_EQ(some_value - 1, tester.get_delay_timer());
}

#ifndef __CLION_IDE_
#pragma clang diagnostic pop
#endif

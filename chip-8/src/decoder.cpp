#include "chip8machine.hpp"

namespace Emulator {

/// \brief Executes an instruction for the current machine state
///
/// This subroutine assumes the program counter has already been incremented
/// for the current instruction cycle;  may have unintended consequences when
/// decoding instructions that modify the program counter!
///
/// \param opcode Instruction to execute
void Chip8Machine::decode(const OPCODE_TYPE opcode) {
  // Are ya coding, son?
  if (opcode == 0x00E0) {
    display.clear();
    return;
  }
  if ((opcode & 0xF000) == 0x1000) {
    int value = opcode & 0x0FFF;
    pc.set(value);
    return;
  }
  if ((opcode & 0xF000) == 0x2000) {
    int value = opcode & 0x0FFF;
    call_stack.push(get_pc());
    pc.set(value);
    return;
  }
  if ((opcode & 0xF000) == 0x6000) {
    int reg_num = (opcode & 0x0F00) >> 8;
    // In principle, the else condition should never trigger, since there
    // If invalid register specified, should be considered a bad opcode
    if (reg_num < v_register.size()) {
      v_register[reg_num].set(opcode & 0x00FF);
      return;
    }
  }
  if ((opcode & 0xF000) == 0x7000) {
    int reg_num = (opcode & 0x0F00) >> 8;
    if (reg_num < v_register.size()) {
      int value = opcode & 0x00FF;
      value += v_register[reg_num].get();
      v_register[reg_num].set(value & 0xFF);
      return;
    }
  }
  if ((opcode & 0xF000) == 0xA000) {
    i_register.set(opcode & 0x0FFF);
    return;
  }
  if ((opcode & 0xF000) == 0xD000) {
    int x_reg = (opcode & 0x0F00) >> 8;
    int y_reg = (opcode & 0x00F0) >> 4;
    int n_rows = opcode & 0x000F;
    int x_offset = v_register[x_reg].get() % display_width;
    int y_offset = v_register[y_reg].get() % display_height;
    int address = i_register.get();
    for (int y = y_offset; y < y_offset + n_rows; y++) {
      if (y >= display_height) break;
      MEM_TYPE byte_to_draw = ram.get_byte(address);
      for (int x = 0; x < 8; x++) {
        if (x + x_offset >= display_width) break;
        PIXEL_TYPE current = display.get_pixel(x + x_offset, y);
        PIXEL_TYPE bit_to_draw = (byte_to_draw >> (7 - x)) & 0x1;
        PIXEL_TYPE new_value = current ^bit_to_draw;
        display.set_pixel(x + x_offset, y, new_value);
        if (current != 0x0 && new_value == 0x0) set_flag(0x1);
      }
      address += 1;
    }
    return;
  }
  throw OpcodeNotSupported(opcode);
}

}  // namespace Emulator

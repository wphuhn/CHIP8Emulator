#include "chip8machinetester.hpp"

namespace Emulator {

void Chip8MachineTester::set_machine(Chip8Machine *machine_) { machine = machine_; }

OPCODE_TYPE Chip8MachineTester::fetch_instruction() const {
  return machine->fetch_instruction();
}

std::vector<MEM_TYPE> Chip8MachineTester::get_ram(bool include_start) const {
  return machine->get_ram(include_start);
}
MEM_TYPE Chip8MachineTester::get_memory_byte(ADDR_TYPE address) const {
  return machine->get_memory_byte(address);
}
REG_TYPE Chip8MachineTester::get_i() const { return machine->get_i(); }
REG_TYPE Chip8MachineTester::get_flag() const { return machine->get_flag(); }
REG_TYPE Chip8MachineTester::get_v(const int reg_num) const {
  return machine->get_v(reg_num);
}
ADDR_TYPE Chip8MachineTester::get_pc() const { return machine->get_pc(); }
ADDR_TYPE Chip8MachineTester::get_top_of_stack() const {
  return machine->get_top_of_stack();
}

void Chip8MachineTester::set_pixel(const int x, const int y, const PIXEL_TYPE value) {
  machine->set_pixel(x, y, value);
}
void Chip8MachineTester::set_i(const REG_TYPE new_value) { machine->set_i(new_value); }
void Chip8MachineTester::set_v(const int reg_num, const REG_TYPE new_value) {
  machine->set_v(reg_num, new_value);
}
void Chip8MachineTester::set_pc(const ADDR_TYPE new_pc) { machine->set_pc(new_pc); }
void Chip8MachineTester::add_to_stack(const ADDR_TYPE new_stack_top) {
  machine->add_to_stack(new_stack_top);
}

}  // namespace Emulator

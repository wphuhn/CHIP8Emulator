#ifndef CHIP_8_TESTS_CHIP8MACHINETESTER_HPP_
#define CHIP_8_TESTS_CHIP8MACHINETESTER_HPP_

#include <vector>

#include "chip8machine.hpp"
#include "chip8types.hpp"

class Chip8MachineTester {
 public:
  void set_machine(Chip8Machine *);

  OPCODE_TYPE fetch_instruction() const;

  std::vector<MEM_TYPE> get_ram(bool = true) const;
  MEM_TYPE get_memory_byte(ADDR_TYPE) const;
  REG_TYPE get_i() const;
  REG_TYPE get_flag() const;
  REG_TYPE get_v(int) const;
  ADDR_TYPE get_pc() const;

  void set_pixel(int, int, PIXEL_TYPE);
  void set_i(REG_TYPE);
  void set_v(int, REG_TYPE);
  void set_pc(ADDR_TYPE);
 private:
  Chip8Machine *machine;
};

#endif  // CHIP_8_TESTS_CHIP8MACHINETESTER_HPP_


#ifndef CHIP_8_INCLUDE_PROGRAMCOUNTER_HPP_
#define CHIP_8_INCLUDE_PROGRAMCOUNTER_HPP_

#include "chip8types.hpp"

class ProgramCounter {
 public:
  ProgramCounter();

  ADDR_TYPE get() const;
  void set(ADDR_TYPE);

  void add(ADDR_TYPE);
 private:
  ADDR_TYPE value_;
};
#endif  // CHIP_8_INCLUDE_PROGRAMCOUNTER_HPP_

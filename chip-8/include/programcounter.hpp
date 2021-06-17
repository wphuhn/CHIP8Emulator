/// \file programcounter.hpp
/// \brief Program counter for machine

#ifndef CHIP_8_INCLUDE_PROGRAMCOUNTER_HPP_
#define CHIP_8_INCLUDE_PROGRAMCOUNTER_HPP_

#include "chip8types.hpp"

namespace Emulator {

/// \class ProgramCounter
/// \brief Program counter for machine
class ProgramCounter {
 public:
  ProgramCounter();

  ADDR_TYPE get() const;
  void set(ADDR_TYPE);

  void add(ADDR_TYPE);
 private:
  ADDR_TYPE value_;
};

}  // namespace Emulator

#endif  // CHIP_8_INCLUDE_PROGRAMCOUNTER_HPP_

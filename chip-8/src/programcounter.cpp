#include "programcounter.hpp"

namespace Emulator {

ProgramCounter::ProgramCounter() : value_(0) {}

/// \brief Get address in program counter
/// \return Address in program counter
ADDR_TYPE ProgramCounter::get() const {
  return value_;
}

/// \brief Set program counter to new address
/// \param new_value New address for program counter
void ProgramCounter::set(const ADDR_TYPE new_value) {
  value_ = new_value;
}

/// \brief Increase the program counter by a fixed amount
/// \param increment Amount to increment program counter by
void ProgramCounter::add(const ADDR_TYPE increment) {
  value_ += increment;
  value_ &= 0xFFFF;
}

}  // namespace Emulator

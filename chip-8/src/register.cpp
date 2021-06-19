#include "register.hpp"

namespace Emulator {

Register::Register() : value_(0) {}

/// \brief Get value in register
/// \return Value in register
REG_TYPE Register::get() const { return value_; }

/// \brief Set register to new value
/// \param new_value  New value for register
void Register::set(const REG_TYPE new_value) { value_ = new_value; }

}  // namespace Emulator

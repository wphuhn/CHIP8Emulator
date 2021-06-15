#include "register.hpp"

namespace Emulator {

Register::Register() : value_(0) {}

REG_TYPE Register::get() const { return value_; }
void Register::set(const REG_TYPE new_value) { value_ = new_value; }

}  // namespace Emulator

#include "register.hpp"

Register::Register() {}

const int Register::get(void) {return value_;}
void Register::set(const int new_value) {value_ = new_value;}

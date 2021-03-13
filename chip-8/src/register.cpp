#include "register.hpp"

Register::Register() : value_(0) {}

int Register::get() const {return value_;}
void Register::set(const int new_value) {value_ = new_value;}

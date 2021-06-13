#include "programcounter.hpp"

ProgramCounter::ProgramCounter() : value_(0) {}

ADDR_TYPE ProgramCounter::get() const {
    return value_;
}

void ProgramCounter::set(const ADDR_TYPE new_value) {
    value_ = new_value;
}

void ProgramCounter::add(const ADDR_TYPE increment) {
    value_ += increment;
    value_ &= 0xFFFF;
}

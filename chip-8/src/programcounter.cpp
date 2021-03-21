#include "programcounter.hpp"

ProgramCounter::ProgramCounter() : value_(0) {}

int ProgramCounter::get() const {
    return value_;
}

void ProgramCounter::set(const int new_value) {
    value_ = new_value;
}

void ProgramCounter::add(const int increment) {
    value_ += increment;
    value_ &= 0xFFFF;
}
#ifndef CHIP_8_INCLUDE_REGISTER_HPP_
#define CHIP_8_INCLUDE_REGISTER_HPP_

#include "chip8types.hpp"

// For advanced architectures, this is a candidate for templating
// For now, I'm going to keep it simple and assume that all registers have
// the same underlying type
class Register {
 public:
    Register();

    REG_TYPE get() const;
    void set(REG_TYPE);
 private:
    REG_TYPE value_{};
};

#endif  // CHIP_8_INCLUDE_REGISTER_HPP_

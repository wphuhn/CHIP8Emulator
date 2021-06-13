#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"
#include "gtest/gtest.h"

#include "register.hpp"

TEST(Register, HasDefaultConstructor) {
  Register the_register;
}

#ifndef __CLION_IDE_
#pragma clang diagnostic pop
#endif

#include "gtest/gtest.h"

#include "register.hpp"

TEST (Register, HasDefaultConstructor) {
    Register the_register;
}
 
int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
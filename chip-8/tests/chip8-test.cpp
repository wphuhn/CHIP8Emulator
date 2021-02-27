#include "gtest/gtest.h"

#include "chip8.hpp"

TEST (Chip8, HasDefaultConstructor) {
    Chip8 interpreter;
}

int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

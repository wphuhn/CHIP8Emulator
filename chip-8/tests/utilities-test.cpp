#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"
#include "gtest/gtest.h"

#include "chip8machine.hpp"
#include "utilities.hpp"

class GenXYNNParameterizedTestFixture
    : public ::testing::TestWithParam<std::tuple<int, int, int, Emulator::OPCODE_TYPE> > {
};
TEST_P(GenXYNNParameterizedTestFixture, GenAXNNGeneratesExpectedCode) {
  int A = std::get<0>(GetParam());
  int X = std::get<1>(GetParam());
  int NN = std::get<2>(GetParam());
  Emulator::OPCODE_TYPE expected = std::get<3>(GetParam());
  Emulator::OPCODE_TYPE actual = Emulator::gen_XYNN_opcode(A, X, NN);
  EXPECT_EQ(expected, actual);
}
INSTANTIATE_TEST_SUITE_P
(
    GenXYNNTests,
    GenXYNNParameterizedTestFixture,
    ::testing::Values(
        std::make_tuple(0x0, 0x0, 0x00, 0x0000),
        std::make_tuple(0x6, 0x0, 0x15, 0x6015),
        std::make_tuple(0xFE, 0xA, 0x5D, 0xEA5D),
        std::make_tuple(0x8, 0xAB, 0xC7, 0x8BC7),
        std::make_tuple(0xD, 0x0, 0x741, 0xD041)
    )
);

class GenWXYZParameterizedTestFixture
    : public ::testing::TestWithParam<std::tuple<int, int, int, int, Emulator::OPCODE_TYPE> > {
};
TEST_P(GenWXYZParameterizedTestFixture, GenAXNNGeneratesExpectedCode) {
  int W = std::get<0>(GetParam());
  int X = std::get<1>(GetParam());
  int Y = std::get<2>(GetParam());
  int Z = std::get<3>(GetParam());
  Emulator::OPCODE_TYPE expected = std::get<4>(GetParam());
  Emulator::OPCODE_TYPE actual = Emulator::gen_WXYZ_opcode(W, X, Y, Z);
  EXPECT_EQ(expected, actual);
}
INSTANTIATE_TEST_SUITE_P
(
    GenWXYZTests,
    GenWXYZParameterizedTestFixture,
    ::testing::Values(
        std::make_tuple(0x0, 0x0, 0x00, 0x0, 0x0000),
        std::make_tuple(0xC, 0x5, 0x1, 0xF, 0xC51F),
        std::make_tuple(0x4E, 0x4, 0xC, 0x6, 0xE4C6),
        std::make_tuple(0x1, 0xCC, 0xD, 0xC, 0x1CDC),
        std::make_tuple(0x5, 0x8, 0x7B, 0x3, 0x58B3),
        std::make_tuple(0x3, 0x6, 0xD, 0xDB, 0x36DB)
    )
);

#ifndef __CLION_IDE_
#pragma clang diagnostic pop
#endif

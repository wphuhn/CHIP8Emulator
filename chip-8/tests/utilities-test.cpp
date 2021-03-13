#include "gtest/gtest.h"

#include "chip8machine.hpp"
#include "utilities.hpp"

class GenXYNNParameterizedTestFixture
    : public ::testing::TestWithParam<std::tuple<int, int, int, OPCODE_TYPE> >{};
TEST_P (GenXYNNParameterizedTestFixture, GenAXNNGeneratesExpectedCode) {
    int A = std::get<0>(GetParam());
    int X = std::get<1>(GetParam());
    int NN = std::get<2>(GetParam());
    OPCODE_TYPE expected = std::get<3>(GetParam());
    OPCODE_TYPE actual = gen_XYNN_opcode(A, X, NN);
    EXPECT_EQ(expected, actual);
}
INSTANTIATE_TEST_CASE_P(
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

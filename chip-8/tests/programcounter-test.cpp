#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"
#include "gtest/gtest.h"

#include "programcounter.hpp"

class ProgramCounterParameterizedTestFixture
    : public ::testing::TestWithParam<std::tuple<int, int, int> > {
};
TEST_P(ProgramCounterParameterizedTestFixture, AddInstructionAddsValueToCurrentValue) {
  int initial = std::get<0>(GetParam());
  int increment = std::get<1>(GetParam());
  int expected = std::get<2>(GetParam());

  ProgramCounter pc;
  pc.set(initial);

  pc.add(increment);
  EXPECT_EQ(pc.get(), expected);
}
INSTANTIATE_TEST_CASE_P
(
    ProgramCounterTests,
    ProgramCounterParameterizedTestFixture,
    ::testing::Values(
        std::make_tuple(0, 0, 0),
        std::make_tuple(0, 0x6A39, 0x6A39),
        std::make_tuple(0x0C04, 0x6F45, 0x7B49),
        std::make_tuple(0xFFFF, 0x0001, 0x0000),
        std::make_tuple(0xF58A, 0x619B, 0x5725)
    )
);

#ifndef __CLION_IDE_
#pragma clang diagnostic pop
#endif

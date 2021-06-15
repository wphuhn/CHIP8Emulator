#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"
#include "gtest/gtest.h"

#include "display.hpp"

#include "test-constants.hpp"

static Emulator::Display get_display() {
  return Emulator::Display(TEST_SCREEN_HEIGHT, TEST_SCREEN_WIDTH);
}

class DisplayFixture : public ::testing::Test {
 protected:
  DisplayFixture() : display(get_display()) {}

  Emulator::Display display;
};

TEST_F(DisplayFixture, InitiallyIsClear) {
  for (int x = 0; x < display.width; x++) {
    for (int y = 0; y < display.height; y++) {
      EXPECT_EQ(display.get_pixel(x, y), TEST_OFF_PIXEL);
    }
  }
}

TEST_F(DisplayFixture, PixelCanBeChanged) {
  int x = 5;
  int y = 3;
  int expected = TEST_ON_PIXEL;
  display.set_pixel(x, y, expected);
  int actual = display.get_pixel(x, y);
  EXPECT_EQ(expected, actual);
}

TEST_F(DisplayFixture, ClearClearsTheScreen) {
  for (int x = 0; x < display.width; x++) {
    for (int y = 0; y < display.height; y++) {
      display.set_pixel(x, y, TEST_ON_PIXEL);
    }
  }
  display.clear();
  for (int x = 0; x < display.width; x++) {
    for (int y = 0; y < display.height; y++) {
      EXPECT_EQ(display.get_pixel(x, y), TEST_OFF_PIXEL);
    }
  }
}

#ifndef __CLION_IDE_
#pragma clang diagnostic pop
#endif

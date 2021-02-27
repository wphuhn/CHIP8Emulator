#include "gtest/gtest.h"

#include "display.hpp"

TEST (Display, HasDefaultConstructor) {
    Display display;
}

TEST (Display, InitiallyIsClear) {
    Display display;
    for (int x = 0; x < display.width; x++) {
        for (int y = 0; y < display.height; y++) {
            EXPECT_EQ(display.get_pixel(x, y), 0);
        }
    }
}

TEST (Display, PixelCanBeChanged) {
    Display display;
    int x = 5;
    int y = 3;
    int expected = 1;
    display.set_pixel(x, y, expected);
    int actual = display.get_pixel(x, y);
    EXPECT_EQ(expected, actual);
}

TEST (Display, ClearClearsTheScreen) {
    Display display;
    for (int x = 0; x < display.width; x++) {
        for (int y = 0; y < display.height; y++) {
            display.set_pixel(x, y, 1);
        }
    }
    display.clear();
    for (int x = 0; x < display.width; x++) {
        for (int y = 0; y < display.height; y++) {
            EXPECT_EQ(display.get_pixel(x, y), 0);
        }
    }
}

int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

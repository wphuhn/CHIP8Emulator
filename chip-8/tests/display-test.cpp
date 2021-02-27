#include "gtest/gtest.h"

#include "display.hpp"

TEST (Display, HasDefaultConstructor) {
    Display display;
}

TEST (Display, InitiallyIsClear) {
    Display display;
    for (int x = 0; x < display.width; x++) {
        for (int y = 0; y < display.height; y++) {
            EXPECT_EQ(display.get_pixel(x, y), OFF_PIXEL);
        }
    }
}

TEST (Display, PixelCanBeChanged) {
    Display display;
    int x = 5;
    int y = 3;
    int expected = ON_PIXEL;
    display.set_pixel(x, y, expected);
    int actual = display.get_pixel(x, y);
    EXPECT_EQ(expected, actual);
}

TEST (Display, ClearClearsTheScreen) {
    Display display;
    for (int x = 0; x < display.width; x++) {
        for (int y = 0; y < display.height; y++) {
            display.set_pixel(x, y, ON_PIXEL);
        }
    }
    display.clear();
    for (int x = 0; x < display.width; x++) {
        for (int y = 0; y < display.height; y++) {
            EXPECT_EQ(display.get_pixel(x, y), OFF_PIXEL);
        }
    }
}

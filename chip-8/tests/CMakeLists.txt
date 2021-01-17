include(GoogleTest)

add_executable(libretro-test libretro-test.cpp)
target_link_libraries(libretro-test pthread chip-8-libretro gtest)
gtest_discover_tests(libretro-test)

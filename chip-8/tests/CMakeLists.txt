include(GoogleTest)

file(GLOB TESTS_SRC "*.cpp")
list(REMOVE_ITEM TESTS_SRC "${CMAKE_CURRENT_SOURCE_DIR}/libretro-test.cpp")
add_executable(chip8-tests ${TESTS_SRC})
target_link_libraries(chip8-tests chip-8 gtest)
gtest_discover_tests(chip8-tests)

add_executable(libretro-test libretro-test.cpp)
target_link_libraries(libretro-test chip-8-libretro gtest)
gtest_discover_tests(libretro-test)

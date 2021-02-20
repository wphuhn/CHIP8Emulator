include(GoogleTest)

add_executable(libretro-test libretro-test.cpp)
target_link_libraries(libretro-test chip-8-libretro gtest)
gtest_discover_tests(libretro-test)

add_executable(register-test register-test.cpp)
target_link_libraries(register-test chip-8 gtest)
gtest_discover_tests(register-test)

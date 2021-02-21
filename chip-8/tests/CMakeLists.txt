include(GoogleTest)

add_executable(libretro-test libretro-test.cpp)
target_link_libraries(libretro-test chip-8-libretro gtest)
gtest_discover_tests(libretro-test)

function(add_chip8_test name)
  add_executable(${name} ${name}.cpp)
  target_link_libraries(${name} chip-8 gtest)
  gtest_discover_tests(${name})
endfunction()

add_chip8_test(register-test)
add_chip8_test(memory-test)

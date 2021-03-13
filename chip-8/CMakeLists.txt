include_directories(include)

add_library(chip-8-libretro SHARED src/libretro.cpp)
set_target_properties(chip-8-libretro PROPERTIES PUBLIC_HEADER include/libretro.h)
install(TARGETS chip-8-libretro
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

add_library(chip-8 SHARED src/register.cpp src/memory.cpp src/chip8machine.cpp src/display.cpp src/utilities.cpp)
install(TARGETS chip-8
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

add_executable(test-rom src/test_rom.cpp)
target_link_libraries(test-rom chip-8)
install(TARGETS test-rom
		RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

add_subdirectory(tests)

include_directories(include)

add_library(chip8-only OBJECT include/chip8constants.hpp include/chip8types.hpp src/register.cpp src/memory.cpp
		    src/chip8machine.cpp src/display.cpp src/programcounter.cpp src/decoder.cpp)
add_library(libretro-only OBJECT src/libretro.cpp src/upscaler.cpp)
set_property(TARGET chip8-only PROPERTY POSITION_INDEPENDENT_CODE ON)
set_property(TARGET libretro-only PROPERTY POSITION_INDEPENDENT_CODE ON)

add_library(chip-8 SHARED $<TARGET_OBJECTS:chip8-only>)
install(TARGETS chip-8
		LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

add_library(chip-8-libretro SHARED $<TARGET_OBJECTS:libretro-only> $<TARGET_OBJECTS:chip8-only>)
set_target_properties(chip-8-libretro PROPERTIES PUBLIC_HEADER include/libretro.h)
install(TARGETS chip-8-libretro
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

add_executable(test-rom src/test_rom.cpp)
target_link_libraries(test-rom chip-8)
install(TARGETS test-rom
		RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

add_subdirectory(tests)

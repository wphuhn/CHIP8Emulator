include_directories(include)

add_library(chip-8-libretro SHARED src/libretro.cpp)
set_target_properties(chip-8-libretro PROPERTIES PUBLIC_HEADER include/libretro.h)
install(TARGETS chip-8-libretro
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

add_library(chip-8 SHARED src/register.cpp)
install(TARGETS chip-8
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

add_subdirectory(tests)

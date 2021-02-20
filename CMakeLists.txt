cmake_minimum_required(VERSION 3.10)
project(CHIP-8 
        VERSION 0.0.1
		DESCRIPTION "CHIP-8 libretro core"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(googletest)
include_directories(googletest/googletest/include/)
enable_testing()

add_subdirectory(chip-8)

#include "gtest/gtest.h"

#include "memory.hpp"

TEST (Memory, HasDefaultConstructor) {
    Memory ram;
}

TEST (Memory, GetBitstreamFromFileThrowsExceptionIfFileNotFound) {
    void* dummy;
    size_t dummy_s;
    std::string bad_path = "dummy.CH8";
    try {
        std::tie(dummy, dummy_s) = Memory::get_bitstream_from_file(bad_path);
        FAIL() << "Expected ifstream::failure exception to be thrown for bad path, none were thrown";
    }
    catch (const std::ifstream::failure& err) {
        EXPECT_EQ(err.what(),std::string("Path 'dummy.CH8' could not be loaded: iostream error"));
    }
    catch (...) {
        FAIL() << "Expected ifstream::failure exception to be thrown for bad path, another exception was thrown";
    }
}

TEST (Memory, GetBitstreamFromFileGivesExpectedSize) {
    std::string rom_path = "./beefcake.ch8";
    std::vector<unsigned char> rom_contents = {0xBE, 0xEF, 0xCA, 0xCE};

    std::ofstream file(rom_path.c_str(), std::ifstream::binary);
    file.write((char*) &rom_contents[0], rom_contents.size());
    file.close();

    void* dummy;
    size_t size;
    std::tie(dummy, size) = Memory::get_bitstream_from_file(rom_path);
    EXPECT_EQ(size, rom_contents.size());
}

TEST (Memory, GetBitstreamFromFileGivesExpectedContent) {
    std::string rom_path = "./beefcake.ch8";
    std::vector<unsigned char> rom_contents = {0xBE, 0xEF, 0xCA, 0xCE};

    std::ofstream file(rom_path.c_str(), std::ifstream::binary);
    file.write((char*) &rom_contents[0], rom_contents.size());
    file.close();

    void* content;
    size_t size;
    std::tie(content, size) = Memory::get_bitstream_from_file(rom_path);
    for (int i = 0; i < rom_contents.size(); i++) {
        EXPECT_EQ(((unsigned char*)content)[i], rom_contents[i]);
    }
}

TEST (Memory, ConvertBitstreamToVectorGivesExpectedSize) {
    std::vector<unsigned char> rom_vector = {0xDE, 0xAD, 0xBE, 0xEF};
    size_t rom_size = rom_vector.size();
    unsigned char* rom_stream = new unsigned char[rom_size];
    // Make sure to copy by value
    for (int i = 0; i < rom_vector.size(); i++) {
        rom_stream[i] = rom_vector[i];
    }

    std::vector<unsigned char> converted_stream;
    converted_stream = Memory::convert_bitstream_to_vector(rom_stream, rom_size);
    EXPECT_EQ(converted_stream.size(), rom_size);
}

TEST (Memory, ConvertBitstreamToVectorGivesExpectedContent) {
    std::vector<unsigned char> rom_vector = {0xDE, 0xAD, 0xBE, 0xEF};
    size_t rom_size = rom_vector.size();
    unsigned char* rom_stream = new unsigned char[rom_size];
    // Make sure to copy by value
    for (int i = 0; i < rom_vector.size(); i++) {
        rom_stream[i] = rom_vector[i];
    }

    std::vector<unsigned char> converted_stream;
    converted_stream = Memory::convert_bitstream_to_vector(rom_stream, rom_size);
    for (int i = 0; i < rom_size; i++) {
        EXPECT_EQ(rom_stream[i], converted_stream[i]);
    }
}

TEST (Memory, LoadROMLoadsROMAtCorrectLocation) {
    const std::vector<unsigned char> rom = {0xBE, 0xEF, 0xCA, 0xCE};
    Memory ram;
    ram.load_rom(rom);
    int expected_start_address = 0x200;
    for (int i = 0; i < rom.size(); i++) {
        EXPECT_EQ(ram.get_byte(expected_start_address + i), rom[i]);
    }
}

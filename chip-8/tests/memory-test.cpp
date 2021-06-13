#include "gtest/gtest.h"

#include "memory.hpp"

#include "test-constants.hpp"

static Memory get_memory() {
  return Memory(TEST_RAM_SIZE, TEST_ROM_START_ADDRESS);
}

class MemoryFixture : public ::testing::Test {
 protected:
  MemoryFixture() : ram(get_memory()) {}

  Memory ram;
};

TEST(Memory, GetBitstreamFromFileThrowsExceptionIfFileNotFound) {
  void *dummy;
  size_t dummy_s;
  std::string bad_path = "dummy.CH8";
  try {
    std::tie(dummy, dummy_s) = Memory::get_bitstream_from_file(bad_path);
    FAIL() << "Expected ifstream::failure exception to be thrown for bad path, none were thrown";
  }
  catch (const std::ifstream::failure &err) {
    EXPECT_EQ(err.what(), std::string("Path 'dummy.CH8' could not be loaded: iostream error"));
  }
  catch (...) {
    FAIL() << "Expected ifstream::failure exception to be thrown for bad path, another exception was thrown";
  }
}

TEST(Memory, GetBitstreamFromFileGivesExpectedSize) {
  std::string rom_path = "./beefcake.ch8";
  std::vector<unsigned char> rom_contents = {0xBE, 0xEF, 0xCA, 0xCE};

  std::ofstream file(rom_path.c_str(), std::ifstream::binary);
  file.write((char *) &rom_contents[0], rom_contents.size());
  file.close();

  void *dummy;
  size_t size;
  std::tie(dummy, size) = Memory::get_bitstream_from_file(rom_path);
  EXPECT_EQ(size, rom_contents.size());
}

TEST(Memory, GetBitstreamFromFileGivesExpectedContent) {
  std::string rom_path = "./beefcake.ch8";
  std::vector<unsigned char> rom_contents = {0xBE, 0xEF, 0xCA, 0xCE};

  std::ofstream file(rom_path.c_str(), std::ifstream::binary);
  file.write((char *) &rom_contents[0], rom_contents.size());
  file.close();

  void *content;
  size_t size;
  std::tie(content, size) = Memory::get_bitstream_from_file(rom_path);
  for (int i = 0; i < rom_contents.size(); i++) {
    EXPECT_EQ(((unsigned char *) content)[i], rom_contents[i]);
  }
}

TEST(Memory, ConvertBitstreamToVectorGivesExpectedSize) {
  std::vector<unsigned char> rom_vector = {0xDE, 0xAD, 0xBE, 0xEF};
  size_t rom_size = rom_vector.size();
  auto rom_stream = new unsigned char[rom_size];
  // Make sure to copy by value
  for (int i = 0; i < rom_vector.size(); i++) {
    rom_stream[i] = rom_vector[i];
  }

  std::vector<unsigned char> converted_stream;
  converted_stream = Memory::convert_bitstream_to_vector(rom_stream, rom_size);
  EXPECT_EQ(converted_stream.size(), rom_size);
}

TEST(Memory, ConvertBitstreamToVectorGivesExpectedContent) {
  std::vector<unsigned char> rom_vector = {0xDE, 0xAD, 0xBE, 0xEF};
  size_t rom_size = rom_vector.size();
  auto rom_stream = new unsigned char[rom_size];
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

// TODO(WPH):  Just remove this test?
TEST_F(MemoryFixture, LoadROMLoadsROMAtCorrectLocation_AccessByByte) {
  const std::vector<unsigned char> rom = {0xBE, 0xEF, 0xCA, 0xCE};
  ram.load_rom(rom);
  ADDR_TYPE expected_start_address = TEST_ROM_START_ADDRESS;
  for (int i = 0; i < rom.size(); i++) {
    EXPECT_EQ(ram.get_byte(expected_start_address + i), rom[i]);
  }
}

// This is *not* a redundant test case, as it verifies that the STL container
// is properly set up, which the AccessByByte variant does not (it's possible
// to write past the boundaries of the original STL container and then access
// memory beyond the boundary of the original STL container)
TEST_F(MemoryFixture, LoadROMLoadsROMAtCorrectLocation_AccessByRam_GetAllRAM) {
  const std::vector<unsigned char> rom = {0xBE, 0xEF, 0xCA, 0xCE};
  ram.load_rom(rom);
  const std::vector<unsigned char> returned_ram = ram.get_ram();
  ADDR_TYPE expected_start_address = TEST_ROM_START_ADDRESS;
  for (int i = 0; i < rom.size(); i++) {
    EXPECT_EQ(returned_ram[expected_start_address + i], rom[i]);
  }
}

TEST_F(MemoryFixture, LoadROMLoadsROMAtCorrectLocation_AccessByRam_GetActivePortion) {
  const std::vector<unsigned char> rom = {0xBE, 0xEF, 0xCA, 0xCE};
  ram.load_rom(rom);
  const std::vector<unsigned char> returned_ram = ram.get_ram(false);
  for (int i = 0; i < rom.size(); i++) {
    EXPECT_EQ(returned_ram[i], rom[i]);
  }
}

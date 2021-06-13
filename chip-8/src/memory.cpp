#include "memory.hpp"

Memory::Memory(ADDR_TYPE size_, ADDR_TYPE rom_start_address_)
    : size(size_), rom_start_address(rom_start_address_) {
  ram.resize(size);
  for (MEM_TYPE &value : ram) {
    value = 0x00;
  }
}

void Memory::load_rom(const std::vector<MEM_TYPE> &rom) {
  ADDR_TYPE offset = rom_start_address;
  for (MEM_TYPE value : rom) {
    ram[offset] = value;
    offset += 1;
  }
}

// CHIP-8 code can be self-altering, hence the slightly different name
// When include_start is true, returns the entirety of RAM, including
// the 512 dead bytes at the beginning, otherwise return only the
// writeable portion
std::vector<MEM_TYPE> Memory::get_ram(bool include_start) const {
  auto first = ram.begin();
  if (!include_start) {
    first += rom_start_address;
  }
  auto last = ram.end();
  return std::vector<MEM_TYPE>(first, last);
}

void *Memory::get_pointer_to_ram_start() const {
  return (void *) &ram[0];
}

MEM_TYPE Memory::get_byte(const ADDR_TYPE offset) const {
  return ram[offset];
}

void Memory::set_byte(const ADDR_TYPE address, const MEM_TYPE value) {
  ram[address] = value;
}

std::pair<void *, size_t> Memory::get_bitstream_from_file(
    const std::string &path) {
  // Read ROM file into memory
  std::ifstream file(path.c_str(), std::ifstream::binary);

  if (!file) {
    std::string err_msg =
        std::string("Path '") + path + std::string("' could not be loaded");
    throw std::ifstream::failure(err_msg);
  }

  // Get the file size and read it into a memory buffer
  file.seekg(0, std::ifstream::end);
  size_t data_size = file.tellg();
  char *data = new char[data_size];

  file.seekg(0, std::ifstream::beg);
  file.read(data, data_size);
  // Sanity check, not unit-tested
  if (file.gcount() != data_size) {
    std::string err_msg = std::string(
        "Was unable to read entire ") + path + std::string(" file");
    throw std::ifstream::failure(err_msg);
  }

  file.close();

  return {data, data_size};
}

std::vector<MEM_TYPE> Memory::convert_bitstream_to_vector(
    const void *bitstream,
    const size_t bitstream_size) {
  auto rom = (const MEM_TYPE *) bitstream;

  size_t pc = 0;
  std::vector<MEM_TYPE> rom_vec(bitstream_size, 0x0000);
  while (pc < bitstream_size) {
    rom_vec[pc] = rom[pc];
    pc += 1;
  }

  return rom_vec;
}

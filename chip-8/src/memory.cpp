#include "memory.hpp"

namespace Emulator {

/// \brief Create memory size with pre-defined size
/// \param size_ Number of addresses in memory
/// \param rom_start_address_ Starting address where ROMs will be loaded
Memory::Memory(ADDR_TYPE size_, ADDR_TYPE rom_start_address_)
    : size(size_), rom_start_address(rom_start_address_) {
  ram.resize(size);
  for (MEM_TYPE &value : ram) {
    value = 0x00;
  }
}

/// \brief Load ROM into memory
///
/// The address where the ROM will be loaded into memory is determined
/// at object creation
///
/// \param rom ROM to be loaded into RAM
void Memory::load_rom(const std::vector<MEM_TYPE> &rom) {
  ADDR_TYPE offset = rom_start_address;
  for (MEM_TYPE value : rom) {
    ram[offset] = value;
    offset += 1;
  }
}

/// \brief Return the contents of the memory space
///
/// CHIP-8 code can be self-altering, hence the slightly different name
/// When include_start is true, returns the entirety of RAM, including
/// the 512 dead bytes at the beginning, otherwise return only the
/// writeable portion
///
/// \param include_start Whether to output the full memory space or not
/// \return Contents of the memory space
std::vector<MEM_TYPE> Memory::get_ram(bool include_start) const {
  auto first = ram.begin();
  if (!include_start) {
    first += rom_start_address;
  }
  auto last = ram.end();
  return std::vector<MEM_TYPE>(first, last);
}

/// \brief Return a pointer to the memory space
///
/// This subroutine exists solely for libretro compatibility!
///
/// \return Pointer to first element in memory space
void *Memory::get_pointer_to_ram_start() const {
  return const_cast<unsigned char *>(&ram[0]);
}

/// \brief Get the contents of a memory address
/// \param offset Memory address to inspect
/// \return The contents of the memory address
MEM_TYPE Memory::get_byte(const ADDR_TYPE offset) const {
  return ram[offset];
}

/// \brief Set the contents of a memory address to a new value
/// \param address Memory address to change
/// \param value New value for contents of memory address
void Memory::set_byte(const ADDR_TYPE address, const MEM_TYPE value) {
  ram[address] = value;
}

/// \brief Extract contents of file as a bytestream
/// \param path Filename to load
/// \return Contents of file as a bytestream, as well as the size
std::pair<void *, size_t> Memory::get_bytestream_from_file(
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

/// \brief Convert bytestream to STL vector of bytes
/// \param bytestream Bytestream to convert
/// \param bytestream_size Size of bytestream
/// \return STL vector containing contents of bytestream
std::vector<MEM_TYPE> Memory::convert_bytestream_to_vector(
    const void *bytestream,
    const size_t bytestream_size) {
  auto rom = (const MEM_TYPE *) bytestream;

  size_t pc = 0;
  std::vector<MEM_TYPE> rom_vec(bytestream_size, 0x0000);
  while (pc < bytestream_size) {
    rom_vec[pc] = rom[pc];
    pc += 1;
  }

  return rom_vec;
}

}  // namespace Emulator

#include <chrono>  // NOLINT [build/c++11]
#include <cstdint>
#include <iostream>
#include <map>
#include <thread>  // NOLINT [build/c++11]

#include "chip8machine.hpp"

#define N_BYTES_IN_OP sizeof(OPCODE_TYPE)

#define SLEEP_MS 500

void output_stats(const std::map<std::string, int> &counter,
                  const unsigned int n_total) {
  int n_ops = 0;
  for (const auto &pair : counter) {
    n_ops += pair.second;
  }

  std::cout << "Number of instructions in set: "
            << n_ops
            << " ("
            << 100.0 * n_ops / static_cast<float>(n_total)
            << "%)"
            << std::endl;
  for (const auto &pair : counter) {
    std::cout << pair.first
              << " "
              << pair.second
              << " ("
              << 100.0 * pair.second / static_cast<float>(n_total)
              << "%)"
              << std::endl;
  }
}

OPCODE_TYPE extract_big_endian_opcode(const std::vector<MEM_TYPE> &rom,
                                      const ADDR_TYPE pc) {
  OPCODE_TYPE opcode = 0;

  for (int i = 0; i < N_BYTES_IN_OP; i++) {
    // opcode = (rom[pc] << 8) | (rom[pc+1] << 0);
    unsigned int shift = 8 * (N_BYTES_IN_OP - i - 1);
    opcode = opcode | (rom[pc + i] << shift);
  }

  return opcode;
}

std::string convert_opcode_to_str(const OPCODE_TYPE opcode) {
  std::stringstream stream;
  stream << "0x"
         << std::hex
         << std::setw(2 * N_BYTES_IN_OP)
         << std::setfill('0')
         << opcode;
  return stream.str();
}

void check_implemented_instructions(const std::vector<MEM_TYPE> &rom) {
  Chip8Machine machine;
  OPCODE_TYPE opcode;

  std::cout << "Parsing ROM to check for unimplemented instructions"
            << std::endl;
  std::cout << "Note: we are assuming that the data stream is little-endian"
            << std::endl;
  std::cout << "      (i.e. Linux on x86) and are converting to big-endian."
            << std::endl;

  std::map<std::string, int> implemented;
  std::map<std::string, int> unimplemented;

  int pc = 0;
  std::string first_unimplemented;
  bool has_encountered_unimplemented = false;

  while (pc < rom.size()) {
    opcode = extract_big_endian_opcode(rom, pc);
    std::string opcode_str = convert_opcode_to_str(opcode);

    try {
      machine.decode(opcode);
      if (implemented.count(opcode_str) == 0) {
        implemented[opcode_str] = 0;
      }
      implemented[opcode_str] += 1;
    }
    catch (OpcodeNotSupported &) {
      if (unimplemented.count(opcode_str) == 0) {
        unimplemented[opcode_str] = 0;
      }
      unimplemented[opcode_str] += 1;

      if (!has_encountered_unimplemented) {
        first_unimplemented = opcode_str;
        has_encountered_unimplemented = true;
      }
    }
    catch (...) {
      throw std::runtime_error(
          "Unknown error encountered when parsing " + opcode_str);
    }
    pc += N_BYTES_IN_OP;
  }

  unsigned int n_total = rom.size() / N_BYTES_IN_OP;

  std::cout << std::endl;
  std::cout << "==============================" << std::endl;
  std::cout << "=  Implemented instructions  =" << std::endl;
  std::cout << "==============================" << std::endl;
  output_stats(implemented, n_total);

  std::cout << std::endl;
  std::cout << "==============================" << std::endl;
  std::cout << "= Unimplemented instructions =" << std::endl;
  std::cout << "==============================" << std::endl;
  output_stats(unimplemented, n_total);

  std::cout << std::endl;
  std::cout << "First unimplemented opcode encountered: "
            << first_unimplemented
            << std::endl;
}

[[noreturn]] void run_rom(const std::vector<MEM_TYPE> &rom) {
  Chip8Machine machine;

  std::cout << std::endl;
  std::cout << "Running ROM" << std::endl;
  std::cout << std::endl;

  machine.reset();
  machine.load_rom(rom);
  while (true) {
    std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_MS));
    std::cout << std::string(machine) << std::endl;
    std::cout << machine.display_str() << std::endl;
    machine.advance();
  }
}

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cout << "Please specify a file as an argument" << std::endl;
    exit(1);
  }

  void *data;
  size_t size;
  std::tie(data, size) = Memory::get_bitstream_from_file(std::string(argv[1]));
  std::vector<MEM_TYPE> rom = Memory::convert_bitstream_to_vector(data, size);

  check_implemented_instructions(rom);
  run_rom(rom);
}

#include <cstdint>
#include <iostream>
#include <map>

#include "chip8machine.hpp"

#define N_BYTES_IN_OP sizeof(OPCODE_TYPE)

size_t get_rom_from_file(const char *filename, void **data) {
    // Read ROM file into memory
    FILE *f = fopen(filename, "rb");
    if (f == nullptr) {
        std::cout << "error: Couldn't open " << filename << std::endl;
        exit(1);
    }

    // Get the file size and read it into a memory buffer
    fseek(f, 0L, SEEK_END);
    size_t size = ftell(f);
    (*data) = (void*) malloc(size);
    fseek(f, 0L, SEEK_SET);
    fread(*data, size, 1, f);
    fclose(f);
    return size;
}

void output_stats(const std::map<std::string, int> & counter, unsigned int n_total) {
    int n_ops = 0;
    for (const auto & pair : counter) {
        n_ops += pair.second;
    }

    std::cout << "Number of instructions in set: " << n_ops << " (" << 100.0 * n_ops / (float)n_total << "%)" << std::endl;
    for (const auto & pair : counter) {
        std::cout << pair.first << " " << pair.second << " (" << 100.0 * pair.second / (float)n_total <<  "%)" << std::endl;
    }
}

OPCODE_TYPE extract_big_endian_opcode(const uint8_t* rom, int pc) {
    OPCODE_TYPE opcode = 0;

    for (int i = 0; i < N_BYTES_IN_OP; i++) {
        // opcode = (rom[pc] << 8) | (rom[pc+1] << 0);
        unsigned int shift = 8 * (N_BYTES_IN_OP - i - 1);
        opcode = opcode | (rom[pc + i] << shift);
    }

    return opcode;
}

std::string convert_opcode_to_str(OPCODE_TYPE opcode) {
    std::stringstream stream;
    stream << "0x" << std::hex << std::setw(2*N_BYTES_IN_OP) << std::setfill('0') << opcode;
    return stream.str();
}

void test_rom(const void *data, size_t size) {
    Chip8Machine machine;
    auto rom = (const uint8_t*) data;
    OPCODE_TYPE opcode;

    std::cout << "Parsing ROM to check for unimplemented instructions" << std::endl;
    std::cout << "Note: we are assuming that the data stream is little-endian" << std::endl;
    std::cout << "      (i.e. Linux on x86) and are converting to big-endian." << std::endl;

    std::map<std::string, int> implemented;
    std::map<std::string, int> unimplemented;

    int pc = 0;
    std::string first_unimplemented;
    bool has_encountered_unimplemented = false;

    while (pc < size) {
        opcode = extract_big_endian_opcode(rom, pc);
        std::string opcode_str = convert_opcode_to_str(opcode);

        try {
            machine.decode(opcode);
            if (implemented.count(opcode_str) == 0) {
                implemented[opcode_str] = 0;
            }
            implemented[opcode_str] += 1;
        }
        catch (OpcodeNotSupported&) {
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
            throw std::runtime_error("Unknown error encountered when parsing " + opcode_str);
        }
        pc += N_BYTES_IN_OP;
    }

    unsigned int n_total = size / N_BYTES_IN_OP;

    std::cout <<  std::endl;
    std::cout << "==============================" << std::endl;
    std::cout << "=  Implemented instructions  =" << std::endl;
    std::cout << "==============================" << std::endl;
    output_stats(implemented, n_total);

    std::cout <<  std::endl;
    std::cout << "==============================" << std::endl;
    std::cout << "= Unimplemented instructions =" << std::endl;
    std::cout << "==============================" << std::endl;
    output_stats(unimplemented, n_total);

    std::cout <<  std::endl;
    std::cout << "First unimplemented opcode encountered: " << first_unimplemented << std::endl;
}

int main (int argc, char** argv) {
  if (argc < 2) {
    std::cout << "Please specify a file as an argument" << std::endl;
    exit(1);
  }

  void* data;
  size_t size = get_rom_from_file(argv[1], &data);

  test_rom(data, size);
  return 0;
}

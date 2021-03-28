#ifndef CHIP_8_MEMORY_HPP_
#define CHIP_8_MEMORY_HPP_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#define RAM_SIZE 0x1000
#define ROM_START_ADDRESS 0x200

class Memory {
public:
    Memory();

    static const int size = RAM_SIZE;

    void load_rom(const std::vector<unsigned char> &);
    unsigned char get_byte(int) const;
    void set_byte(int, unsigned char);

    static std::pair<void*, size_t> get_bitstream_from_file(const std::string &path);
    static std::vector<unsigned char> convert_bitstream_to_vector(const void *bitstream, size_t bitstream_size);
private:
    std::vector<unsigned char> ram;
};

#endif //CHIP_8_MEMORY_HPP_

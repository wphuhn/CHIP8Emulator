#ifndef CHIP_8_MEMORY_HPP_
#define CHIP_8_MEMORY_HPP_

#include <vector>

#define RAM_SIZE 0x1000
#define ROM_START_ADDRESS 0x200

class Memory {
public:
    Memory();
    void load_rom(const std::vector<unsigned char> &);
    unsigned char get_byte(int) const;
    void set_byte(int, unsigned char);
private:
    std::vector<unsigned char> ram;
};

#endif //CHIP_8_MEMORY_HPP_

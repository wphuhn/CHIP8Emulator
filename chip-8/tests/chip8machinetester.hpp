//
// Created by will on 6/6/21.
//

#ifndef CHIP_8_CHIP8MACHINETESTER_HPP_
#define CHIP_8_CHIP8MACHINETESTER_HPP_

#include "chip8machine.hpp"

#include "chip8types.hpp"

class Chip8MachineTester {
public:
    void set_machine(Chip8Machine*);

    void set_pixel(int, int, PIXEL_TYPE);
    void set_i(REG_TYPE);
    void set_v(int, REG_TYPE);
    void set_pc(ADDR_TYPE);
private:
    Chip8Machine* machine;
};

#endif //CHIP_8_CHIP8MACHINETESTER_HPP_

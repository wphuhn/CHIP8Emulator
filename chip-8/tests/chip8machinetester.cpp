//
// Created by will on 6/6/21.
//

#include "chip8machinetester.hpp"

void Chip8MachineTester::set_machine(Chip8Machine* machine_) {machine = machine_;}

void Chip8MachineTester::set_pixel(const int x, const int y, const PIXEL_TYPE value) {
    machine->display.set_pixel(x, y, value);
}

void Chip8MachineTester::set_i(const REG_TYPE new_value) {machine->i_register.set(new_value);}
void Chip8MachineTester::set_pc(const ADDR_TYPE new_pc) {machine->pc.set(new_pc);}
void Chip8MachineTester::set_v(const int reg_num, const REG_TYPE new_value) {
    if (reg_num < machine->v_register.size()) {
        machine->v_register[reg_num].set(new_value);
        return;
    }
    throw std::runtime_error("Invalid register V" + std::to_string(reg_num) + " specified.");
}
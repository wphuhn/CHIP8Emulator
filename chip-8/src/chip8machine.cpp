#include "chip8machine.hpp"

Chip8Machine::Chip8Machine()
    : display_height(Display::height), display_width(Display::width) {}

void Chip8Machine::decode(const OPCODE_TYPE opcode) {
    // Are ya coding, son?
    if ((opcode & 0xF000) == 0x6000) {
        int reg_num = (opcode & 0x0F00) >> 8;
        // In principle, the else condition should never trigger, since there
        // If invalid register specified, should be considered a bad opcode
        if (reg_num < v_register.size()) {
            v_register[reg_num].set(opcode & 0x00FF);
            return;
        }
    }
    if ((opcode & 0xF000) == 0x7000) {
        int reg_num = (opcode & 0x0F00) >> 8;
        if (reg_num < v_register.size()) {
            int value = opcode & 0x00FF;
            value += v_register[reg_num].get();
            v_register[reg_num].set(value & 0xFF);
            return;
        }
    }
    if ((opcode & 0xF000) == 0xA000) {
        i_register.set(opcode & 0x0FFF);
        return;
    }
    if (opcode == 0x00E0) {
        display.clear();
        return;
    }
    throw OpcodeNotSupported(opcode);
}

const PIXEL_TYPE& Chip8Machine::get_pixel(const int x, const int y) const {
    return display.get_pixel(x, y);
}

void Chip8Machine::set_pixel(const int x, const int y, const PIXEL_TYPE value) {
    display.set_pixel(x, y, value);
}

void Chip8Machine::clear_screen() {
    display.clear();
}

int Chip8Machine::get_i() const {return i_register.get();}
int Chip8Machine::get_v(const int reg_num) const {
    if (reg_num < v_register.size()) return v_register[reg_num].get();
    throw std::runtime_error("Invalid register V" + std::to_string(reg_num) + " specified.");
}
int Chip8Machine::get_flag() const {return get_v(0xF);}

void Chip8Machine::set_i(const int new_value) {i_register.set(new_value);}
void Chip8Machine::set_v(const int reg_num, const int new_value) {
    if (reg_num < v_register.size()) {
        v_register[reg_num].set(new_value);
        return;
    }
    throw std::runtime_error("Invalid register V" + std::to_string(reg_num) + " specified.");
}
void Chip8Machine::set_flag(const int new_value) {set_v(0xF, new_value);}

static std::string opcode_to_hex_str(const OPCODE_TYPE value) {
    std::stringstream stream;
    stream << "0x" << std::hex << value;
    return stream.str();
}

OpcodeNotSupported::OpcodeNotSupported(const OPCODE_TYPE opcode)
    : std::runtime_error("Opcode " + opcode_to_hex_str(opcode) + " not supported") {}

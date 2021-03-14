#include "utilities.hpp"

#include <stdexcept>

OPCODE_TYPE gen_XYNN_opcode(const int A, const int X, const int NN) {
    int A_ = 0xF & A;
    int X_ = 0xF & X;
    int NN_ = 0xFF & NN;
    return (A_ << 12) + (X_ << 8) + NN_;
}

OPCODE_TYPE gen_WXYZ_opcode(const int W, const int X, const int Y, const int Z) {
    int W_ = 0xF & W;
    int X_ = 0xF & X;
    int Y_ = 0xF & Y;
    int Z_ = 0xF & Z;
    return (W_ << 12) + (X_ << 8) + (Y_ << 4) + Z_;
}

Chip8Machine create_machine_for_drawing(const OPCODE_TYPE opcode, const int font_address,
                                        const std::vector<unsigned char>& font, const int x_offset, const int y_offset) {
    Chip8Machine machine;
    machine.clear_screen();
    machine.set_flag(0);
    machine.set_i(font_address);
    int vx = (opcode >> 8) & 0xF;
    int vy = (opcode >> 4) & 0xF;
    // Note:  if vx == vy, y_offset will be used for both!
    if ((vx == vy) && (x_offset != y_offset)) {
        throw std::invalid_argument("Offsets cannot be different when registers for offsets are identical");
    }
    machine.set_v(vx, x_offset);
    machine.set_v(vy, y_offset);
    int offset = 0;
    for (auto value : font) {
        machine.set_memory_byte(font_address + offset, value);
        offset += 1;
    }
    return machine;
}
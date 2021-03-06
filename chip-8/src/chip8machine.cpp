#include "chip8machine.hpp"

Chip8Machine::Chip8Machine()
    : display_height(display.height), display_width(display.width) {};

void Chip8Machine::decode(const OPCODE_TYPE opcode) {
    switch (opcode) {
        // Clean screen
        case 0x00E0:
            display.clear();
            break;
        default:
            throw OpcodeNotSupported(opcode);
    }
}

const PIXEL_TYPE& Chip8Machine::get_pixel(const int x, const int y) const {
    return display.get_pixel(x, y);
}

void Chip8Machine::set_pixel(const int x, const int y, const PIXEL_TYPE value) {
    display.set_pixel(x, y, value);
}

static std::string opcode_to_hex_str(const OPCODE_TYPE value) {
    std::stringstream stream;
    stream << "0x" << std::hex << value;
    return stream.str();
}

OpcodeNotSupported::OpcodeNotSupported(const OPCODE_TYPE opcode)
    : std::runtime_error("Opcode " + opcode_to_hex_str(opcode) + " not supported") {}

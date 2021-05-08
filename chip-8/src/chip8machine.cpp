#include "chip8machine.hpp"

Chip8Machine::Chip8Machine()
    : display_height(Display::height), display_width(Display::width), memory_size(Memory::size) {}

void Chip8Machine::decode(const OPCODE_TYPE opcode) {
    // Are ya coding, son?
    if (opcode == 0x00E0) {
        display.clear();
        return;
    }
    if ((opcode & 0xF000) == 0x1000) {
        int value = opcode & 0x0FFF;
        pc.set(value);
        return;
    }
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
    if ((opcode & 0xF000) == 0xD000) {
        int x_reg = (opcode & 0x0F00) >> 8;
        int y_reg = (opcode & 0x00F0) >> 4;
        int n_rows = opcode & 0x000F;
        int x_offset = v_register[x_reg].get() % display_width;
        int y_offset = v_register[y_reg].get() % display_height;
        int addr = i_register.get();
        for (int y = y_offset; y < y_offset + n_rows; y++) {
            if (y >= display_height) break;
            unsigned char byte_to_draw = ram.get_byte(addr);
            for (int x = 0; x < 8; x++) {
                if (x + x_offset >= display_width) break;
                PIXEL_TYPE current = display.get_pixel(x + x_offset, y);
                PIXEL_TYPE bit_to_draw = (byte_to_draw >> (7-x)) & 0x1;
                PIXEL_TYPE new_value = current ^ bit_to_draw;
                display.set_pixel(x + x_offset, y, new_value);
                if (current != 0x0 && new_value == 0x0) set_flag(0x1);
            }
            addr += 1;
        }
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

int Chip8Machine::get_pc() const {
    return pc.get();
}

std::vector<unsigned char> Chip8Machine::get_ram(bool include_start) const {
    return ram.get_ram(include_start);
}

void Chip8Machine::set_pc(const int new_pc) {
    pc.set(new_pc);
}

void Chip8Machine::clear_screen() {
    display.clear();
}

void Chip8Machine::set_memory_byte(int address, unsigned char value) {
    ram.set_byte(address, value);
}

unsigned char Chip8Machine::get_memory_byte(int address) const {
    return ram.get_byte(address);
}

void Chip8Machine::load_rom(const std::vector<unsigned char> &rom) {
    ram.load_rom(rom);
}

OPCODE_TYPE Chip8Machine::fetch_instruction() const {
    int pc_ = pc.get();
    unsigned char byte_one = ram.get_byte(pc_);
    unsigned char byte_two = ram.get_byte(pc_ + 1);
    return (byte_one << 8) + byte_two;
}

void Chip8Machine::advance() {
    OPCODE_TYPE opcode = fetch_instruction();
    pc.add(INSTRUCTION_LENGTH);
    decode(opcode);
}

int Chip8Machine::get_i() const {return i_register.get();}
int Chip8Machine::get_v(const int reg_num) const {
    if (reg_num < v_register.size()) return v_register[reg_num].get();
    throw std::runtime_error("Invalid register V" + std::to_string(reg_num) + " specified.");
}
int Chip8Machine::get_flag() const {return v_register[0xF].get();}

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

Chip8Machine::operator std::string() const {
    std::stringstream stream;
    stream << "Current status of Chip8Machine:" << std::endl;
    stream << "- PC: " << opcode_to_hex_str(pc.get()) << " , ";
    stream << "I: " << opcode_to_hex_str(i_register.get()) << " , ";
    stream << "Flag: " << opcode_to_hex_str(get_flag()) << " , ";
    stream << "Opcode: " << opcode_to_hex_str(fetch_instruction()) << std::endl;
    stream <<  "- V[0]: " << opcode_to_hex_str(v_register[0].get());
    for (int n = 1; n < NUM_V_REGS - 1; n++) {
        stream << " , V[" << n << "]: " << opcode_to_hex_str(v_register[n].get());
    }
    return stream.str();
}

void Chip8Machine::reset() {
    pc.set(ROM_START_ADDRESS);
}

std::string Chip8Machine::display_str() const {
    return std::string(display);
};

OpcodeNotSupported::OpcodeNotSupported(const OPCODE_TYPE opcode)
    : std::runtime_error("Opcode " + opcode_to_hex_str(opcode) + " not supported") {}

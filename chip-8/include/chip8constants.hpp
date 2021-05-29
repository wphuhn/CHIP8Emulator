#ifndef CHIP_8_CHIP8CONSTANTS_HPP_
#define CHIP_8_CHIP8CONSTANTS_HPP_

// Chip8Machine
#define OPCODE_TYPE uint16_t
#define INSTRUCTION_LENGTH 2
#define NUM_V_REGS 16

// Display
#define MAX_WIDTH 64
#define MAX_HEIGHT 32
#define OFF_PIXEL 0
#define ON_PIXEL 1
#define PIXEL_TYPE int

// Memory
#define RAM_SIZE 0x1000
#define ROM_START_ADDRESS 0x200

#endif //CHIP_8_CHIP8CONSTANTS_HPP_

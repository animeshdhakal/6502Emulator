
#include <stdint.h>
#include <sys/types.h>

#ifndef EMULATOR_H
#define EMULATOR_H

#define MAX_MEMORY 64 * 1024

// Opcodes
#define INS_LDA_IM 0xa9
#define INS_LDA_ZP 0xa5
#define INS_LDA_ZPX 0xb5

#define INS_JSR 0x20

typedef struct {
    uint8_t data[MAX_MEMORY];
} memory_t;

typedef struct {
    uint16_t program_counter;
    uint8_t stack_pointer;

    uint8_t accumulator;
    uint8_t index_reg_x; // Index Register X
    uint8_t index_reg_y; // Index Register Y

    // Processor Status
    uint8_t carry_flag;
    uint8_t zero_flag;
    uint8_t interrupt_disable;
    uint8_t decimal_mode;
    uint8_t break_command;
    uint8_t overflow_flag;
    uint8_t negative_flag;
} cpu_t;

void lda_set_status(cpu_t *cpu);

void reset_cpu(cpu_t *cpu);

void initialize_memory(memory_t *memory);

void write_word(memory_t *memory, uint32_t address, uint32_t value, uint32_t *cycles);

uint8_t fetch_byte(cpu_t *cpu, memory_t *memory, uint32_t *cycles);

uint8_t read_byte(cpu_t *cpu, memory_t *memory, uint32_t *cycles, uint8_t address);

uint16_t fetch_word(cpu_t *cpu, memory_t *memory, uint32_t *cycles);

void execute_instruction(cpu_t *cpu, memory_t *memory, uint32_t cycles);

#endif

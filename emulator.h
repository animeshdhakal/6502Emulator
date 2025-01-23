#ifndef EMULATOR_H
#define EMULATOR_H

#include <stdint.h>

#define MAX_MEMORY 64*1024

#define INS_LDA_IM 0xa9
#define INS_LDA_ZP 0xa5
#define INS_LDA_ZPX 0xb5 

typedef struct {
    // Internal Registers
    uint16_t program_counter;
    uint8_t stack_pointer;

    // General Purpose Registers
    uint8_t accumulator;
    uint8_t index_reg_x;
    uint8_t index_reg_y;

    uint8_t carry_flag: 1;
    uint8_t zero_flag: 1;
    uint8_t interrupt_disable: 1;
    uint8_t decimal_mode: 1;
    uint8_t break_command: 1;
    uint8_t overflow_flag: 1;
    uint8_t negative_flag: 1; 
} cpu_t;

typedef struct {
    uint8_t data[MAX_MEMORY];
} memory_t;


void set_lda_status(cpu_t *cpu);
void reset_memory(memory_t* memory);
void reset_cpu(cpu_t* cpu);
uint8_t read_byte(cpu_t *cpu, memory_t *memory, uint8_t address, uint8_t *cycles);
uint8_t fetch_byte(cpu_t *cpu, memory_t *memory, uint8_t *cycles);
void print_cpu_info(cpu_t* cpu);
void execute_instructions(cpu_t *cpu, memory_t *memory);

#endif

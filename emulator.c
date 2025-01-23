#include "emulator.h"
#include <stdio.h>

void print_cpu_info(cpu_t *cpu)
{
    printf("----------------------CPU INFO----------------------\n");
    printf("Program Counter: 0x%x\n", cpu->program_counter);
    printf("Stack Pointer: 0x%x\n\n", cpu->stack_pointer);

    printf("Index Register X: 0x%x\n", cpu->index_reg_x);
    printf("Index Register Y: 0x%x\n", cpu->index_reg_y);
    printf("Accumulator: 0x%x\n\n", cpu->accumulator);

    printf("Carry Flag: 0x%x\n", cpu->carry_flag);
    printf("Zero Flag: 0x%x\n", cpu->zero_flag);
    printf("Interrupt Disable: 0x%x\n", cpu->interrupt_disable);
    printf("Decimal Mode: 0x%x\n", cpu->decimal_mode);
    printf("Break Command: 0x%x\n", cpu->break_command);
    printf("Overflow Flag: 0x%x\n", cpu->overflow_flag);
    printf("Negative Flag: 0x%x\n", cpu->negative_flag);
}

void reset_cpu(cpu_t *cpu)
{
    cpu->program_counter = 0xfffc;
    cpu->stack_pointer = 0x01;

    cpu->accumulator = 0;
    cpu->index_reg_x = 0;
    cpu->index_reg_y = 0;

    cpu->carry_flag = 0;
    cpu->zero_flag = 0;
    cpu->interrupt_disable = 0;
    cpu->decimal_mode = 0;
    cpu->break_command = 0;
    cpu->overflow_flag = 0;
    cpu->negative_flag = 0;
}

void reset_memory(memory_t *memory)
{
    for (int i = 0; i < MAX_MEMORY; i++) {
        memory->data[i] = 0;
    }
}

uint8_t fetch_byte(cpu_t *cpu, memory_t *memory, uint8_t *cycles)
{
    uint8_t ins = memory->data[cpu->program_counter];
    (*cycles)++;
    cpu->program_counter++;
    return ins;
}

uint8_t read_byte(cpu_t *cpu, memory_t *memory, uint8_t address, uint8_t *cycles)
{
    uint8_t value = memory->data[address];
    (*cycles)++;
    return value;
}

void set_lda_status(cpu_t *cpu)
{
    cpu->zero_flag = cpu->accumulator == 0;
    cpu->negative_flag = (cpu->accumulator >> 6) & 1;
}

void execute_instructions(cpu_t *cpu, memory_t *memory)
{
    while (1) {
        uint8_t cycles = 0;
        uint8_t ins = fetch_byte(cpu, memory, &cycles);

        switch (ins) {
        case INS_LDA_IM: {
            uint8_t value = fetch_byte(cpu, memory, &cycles);
            cpu->accumulator = value;
            set_lda_status(cpu);
        } break;

        case INS_LDA_ZP: {
            uint8_t address = fetch_byte(cpu, memory, &cycles);
            cpu->accumulator = read_byte(cpu, memory, address, &cycles);
            set_lda_status(cpu);
        } break;

        case INS_LDA_ZPX: {
            uint8_t address = fetch_byte(cpu, memory, &cycles);
            cpu->accumulator = read_byte(cpu, memory, address + cpu->index_reg_x, &cycles);
            cycles++;
            set_lda_status(cpu);
        } break;

        default:
            printf("Invalid Instruction: 0x%x\n", ins);
        }

        printf("Cycles Consumed: %d\n", cycles);

        break;
    }
}

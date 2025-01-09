#include <stdio.h>
#include "emulator.h"

int main()
{
    cpu_t cpu;
    memory_t memory;

    reset_cpu(&cpu);
    initialize_memory(&memory);

    memory.data[cpu.program_counter] = INS_JSR;
    memory.data[cpu.program_counter + 1] = 0x42;
    memory.data[cpu.program_counter + 2] = 0x42;
    memory.data[0x4242] = INS_LDA_IM;
    memory.data[0x4243] = 0x84;

    execute_instruction(&cpu, &memory, 8);

    printf("-----------------------------------CPU Status-----------------------------\n");
    printf("Program Counter: %d\n", cpu.program_counter);
    printf("Stack Pointer: %d\n", cpu.stack_pointer);
    printf("Accumulator: %d\n", cpu.accumulator);
    printf("Index Register X: %d\n", cpu.index_reg_x);
    printf("Index Register Y: %d\n", cpu.index_reg_y);
    printf("Index Register Y: %d\n", cpu.index_reg_y);

    printf("Carry Flag: %d\n", cpu.carry_flag);
    printf("Zero Flag: %d\n", cpu.zero_flag);
    printf("Interrupt Disable: %d\n", cpu.interrupt_disable);
    printf("Decimal Mode: %d\n", cpu.decimal_mode);
    printf("Break Command: %d\n", cpu.break_command);
    printf("Overflow Flag: %d\n", cpu.overflow_flag);
    printf("Negative Flag: %d\n", cpu.negative_flag);
    printf("-----------------------------------CPU Status-----------------------------\n");

    return 0;
}

#include <stdint.h>
#include <stdio.h>

#define MAX_MEMORY 64 * 1024

typedef struct {
    uint8_t data[MAX_MEMORY];
} memory_t;

typedef struct {
    uint16_t program_counter;
    uint8_t stack_pointer;

    uint8_t accumulator;
    uint8_t x; // Index Register X
    uint8_t y; // Index Register Y

    // Processor Status
    // 1st Bit - Carry Flag
    // 2nd Bit - Zero Flag
    // 3rd Bit - Interrupt Disable
    // 4th Bit - Decimal Mode
    // 5th Bit - Break Command
    // 6th Bit - Overflow Flag
    // 7th Bit - Negative Flag
    // 8th Bit - Redundant Bit
    uint8_t processor_status;
} cpu_t;

void reset_cpu(cpu_t *cpu)
{
    cpu->program_counter = 0xfffc;
    cpu->stack_pointer = 0x00ff;

    cpu->accumulator = 0;
    cpu->x = 0;
    cpu->y = 0;

    cpu->processor_status = 0;
}

void initialize_memory(memory_t *memory)
{
    for (int i = 0; i < MAX_MEMORY; i++) {
        memory->data[i] = 0;
    }
}

uint8_t fetch_instruction(cpu_t* cpu, memory_t *memory, uint32_t cycles)
{
    uint8_t instruction = memory->data[cycles];
    cpu->program_counter++;
    return instruction;
}

void execute_instruction(cpu_t *cpu, memory_t *memory, uint32_t cycles)
{
    while(cycles > 0) 
    {
        uint8_t instruction = fetch_instruction(cpu, memory, cycles);
        printf("Instruction is %d\n", instruction);
        cycles--;
    }
}

int main()
{
    cpu_t cpu;
    memory_t memory;

    reset_cpu(&cpu);
    initialize_memory(&memory);

    execute_instruction(&cpu, &memory, 2);

    return 0;
}

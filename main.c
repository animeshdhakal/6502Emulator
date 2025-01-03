#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>

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

void lda_set_status(cpu_t *cpu)
{
    cpu->zero_flag = cpu->accumulator == 0;
    cpu->negative_flag = (cpu->accumulator & 0b10000000) > 0;
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

void initialize_memory(memory_t *memory)
{
    for (int i = 0; i < MAX_MEMORY; i++) {
        memory->data[i] = 0;
    }
}

void write_word(memory_t *memory, uint32_t address, uint32_t value, uint32_t *cycles)
{
    memory->data[address] = (value & 0xff);
    memory->data[address] = (value >> 8);
    (*cycles) -= 2;
}

uint8_t fetch_byte(cpu_t *cpu, memory_t *memory, uint32_t *cycles)
{
    uint8_t instruction = memory->data[cpu->program_counter];
    cpu->program_counter++;
    (*cycles) -= 1;
    return instruction;
}

uint8_t read_byte(cpu_t *cpu, memory_t *memory, uint32_t *cycles, uint8_t address)
{
    uint8_t instruction = memory->data[address];
    (*cycles) -= 1;
    return instruction;
}

uint16_t fetch_word(cpu_t *cpu, memory_t *memory, uint32_t *cycles)
{
    uint16_t instruction = memory->data[cpu->program_counter];
    instruction |= (instruction << 8);

    (*cycles) -= 2;

    return instruction;
}

void execute_instruction(cpu_t *cpu, memory_t *memory, uint32_t cycles)
{
    while (cycles > 0) {
        uint8_t instruction = fetch_byte(cpu, memory, &cycles);

        switch (instruction) {
        case INS_LDA_IM: {
            uint8_t value = fetch_byte(cpu, memory, &cycles);
            cpu->accumulator = value;
            lda_set_status(cpu);
        } break;

        case INS_LDA_ZP: {
            uint8_t zero_page_address = fetch_byte(cpu, memory, &cycles);
            cpu->accumulator = read_byte(cpu, memory, &cycles, zero_page_address);
            lda_set_status(cpu);
        } break;

        case INS_LDA_ZPX: {
            uint8_t zero_page_address = fetch_byte(cpu, memory, &cycles);
            zero_page_address += cpu->index_reg_x;
            cycles--;
            cpu->accumulator = read_byte(cpu, memory, &cycles, zero_page_address);
            lda_set_status(cpu);
        } break;

        case INS_JSR: {
            uint16_t address = fetch_word(cpu, memory, &cycles);
            write_word(memory, cpu->stack_pointer, cpu->program_counter - 1, &cycles);
            cpu->program_counter = address;
            cycles--;
        } break;

        default: {
            printf("Invalid Instruction %d\n", instruction);
        }
        }
    }
}

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

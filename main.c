#include "emulator.h"


int main() {
    cpu_t cpu;
    memory_t memory;

    reset_cpu(&cpu);
    reset_memory(&memory);

    memory.data[0x1] = 0x38;

    memory.data[cpu.program_counter] = INS_LDA_ZPX;
    memory.data[cpu.program_counter+1] = 0x1;

    execute_instructions(&cpu, &memory);

    print_cpu_info(&cpu);

    return 0;
}   

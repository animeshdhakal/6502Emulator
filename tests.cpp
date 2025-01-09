#include <gtest/gtest.h>

extern "C" {
    #include "emulator.h"
}

class M6502Test1 : public testing::Test {
public:
    cpu_t cpu;
    memory_t memory;

    virtual void SetUp()
    {
        reset_cpu(&cpu);
        initialize_memory(&memory);
    }

    virtual void TearDown()
    {
    }
};

TEST_F(M6502Test1, LDAImmediateLoadARegister)
{
    memory.data[cpu.program_counter] = INS_LDA_IM;
    memory.data[cpu.program_counter + 1] = 0x69;

    execute_instruction(&cpu, &memory, 2);

    EXPECT_EQ(cpu.accumulator, 0x69);
}

TEST_F(M6502Test1, LDAZeroPageLoadARegister)
{
    memory.data[cpu.program_counter] = INS_LDA_ZP;
    memory.data[cpu.program_counter + 1] = 0;
    memory.data[0] = 0x29;

    execute_instruction(&cpu, &memory, 3);

    EXPECT_EQ(cpu.accumulator, 0x29);
}

TEST_F(M6502Test1, LDAZeroPageXLoadARegister)
{
    cpu.index_reg_x = 4;

    memory.data[cpu.program_counter] = INS_LDA_ZPX;
    memory.data[cpu.program_counter + 1] = 5;
    memory.data[cpu.index_reg_x + 5] = 0x11;

    execute_instruction(&cpu, &memory, 4);

    EXPECT_EQ(cpu.accumulator, 0x11);
}

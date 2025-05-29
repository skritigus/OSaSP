#ifndef CPU_H
#define CPU_H

#include "register.h"
#include "instruction.h"

#define REGISTER_COUNT 4
#define MEMORY_SIZE 256

typedef struct CPU
{
    Reg64 registers[REGISTER_COUNT];
    int memory[MEMORY_SIZE];
    int lastInstruction;
    int currentInstruction;
} CPU;

CPU initCpu();
void execute(CPU* cpu);

int writeToMemory(CPU* cpu, Instruction instruction, int opCount, void* fileOpened);

#endif

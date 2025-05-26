#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include "register.h"

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

#endif

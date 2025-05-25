#ifndef CPU_H
#define CPU_H

#include <stdint.h>

#define REGISTER_COUNT 5
#define MEMORY_SIZE 256

typedef enum 
{
	INVALID_REG = -1,
    RAX,
    RBX,
    RCX,
	RDX,
    RFX
} Register;

typedef struct 
{
    int registers[REGISTER_COUNT];
    int64_t RCX;
    int memory[MEMORY_SIZE];
    int lastInstruction;
    int currentInstruction;
} CPU;

CPU initCpu();
void execute(CPU* cpu);

#endif

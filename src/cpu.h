#ifndef CPU_H
#define CPU_H

#define REGISTER_COUNT 5
#define MEMORY_SIZE 256

typedef struct 
{
    int registers[REGISTER_COUNT];
    int memory[MEMORY_SIZE];
    int lastInstruction;
    int currentInstruction;
} CPU;

CPU initCpu();
void execute(CPU* cpu);

#endif

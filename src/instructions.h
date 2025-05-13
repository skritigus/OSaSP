#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "cpu.h"

typedef enum 
{
	HELP = 1,
	STAT,
    END,
    PRINT,
    CLR,
    MOV,
    ADD,
    SUB,
    MUL,
    DIV
} Operations;

void handleHelp(CPU *cpu);
void handleStat(CPU *cpu);
void handlePrint(CPU *cpu);
void handleMov(CPU *cpu);
void handleAdd(CPU *cpu);
void handleSub(CPU *cpu);
void handleMul(CPU *cpu);
void handleDiv(CPU *cpu);

#endif

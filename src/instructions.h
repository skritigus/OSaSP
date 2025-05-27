#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "cpu.h"

typedef enum 
{
	INVALID_OP = -1,
	HELP,
	STAT,
    END,
    PRINT,
    CLR,
    MOV,
    MOVR,
    ADD,
    SUB,
    MUL,
    DIV
} Operation;

void handleHelp(CPU *cpu);
void handleStat(CPU *cpu);
void handlePrint(CPU *cpu);
void handleClr(CPU* cpu);
void handleMov(CPU *cpu);
void handleMovr(CPU *cpu);
int handleAdd(CPU *cpu);
void handleSub(CPU *cpu);
int handleMul(CPU *cpu);
void handleDiv(CPU *cpu);

void printNum(double num);

#endif

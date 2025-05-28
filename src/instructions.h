#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "register.h"

struct CPU;
typedef struct CPU CPU;

typedef enum 
{
	INVALID_OP = -1,
	HELP,
	STAT,
    END,
    PRINT,
    CLR,
    CHS,
    MOV,
    MOVR,
    ADD,
    SUB,
    MUL,
    DIV
} Operation;

typedef struct 
{
	Operation opcode;
	Register operand1;
	int operand2;
} Instruction;

void handleHelp(CPU* cpu);
void handleStat(CPU* cpu);
void handleClr(CPU* cpu);
void handleMov(CPU* cpu);
void handleMovr(CPU* cpu);
int handleChs(CPU* cpu);
int handleAdd(CPU* cpu);
int handleSub(CPU* cpu);
int handleMul(CPU* cpu);
int handleDiv(CPU* cpu);

void printNum(double num);
void printReg(CPU* cpu);

#endif

#ifndef CLI_H
#define CLI_H

#include "cpu.h"

typedef struct 
{
	int opcode;
	int operand1;
	int operand2;
} Instruction;

void trim(char* str);
void parseInstruction(CPU* cpu);
int convertToRegister(char* str);
int convertToOperation(char* str);

#endif

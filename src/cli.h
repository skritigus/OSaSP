#ifndef CLI_H
#define CLI_H

#include "cpu.h"
#include "instructions.h"

typedef struct 
{
	Operation opcode;
	int operand1;
} Instruction;

void trim(char* str);
void deleteEnter(char* str);
char* parseWord(char* str, const char* delimeters);
float parseNum(char* str);

void parseInstruction(CPU* cpu);
Register convertToRegister(char* str);
Operation convertToOperation(char* str);

#endif

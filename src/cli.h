#ifndef CLI_H
#define CLI_H

#include "cpu.h"
#include "instructions.h"

typedef struct 
{
	Operation opcode;
	Register operand1;
	int operand2;
} Instruction;

void trim(char* str);
void deleteEnter(char* str);
char* parseWord(char* str, const char* delimeters);
double parseNum(char* str);

void parseInstruction(CPU* cpu);
Register convertToRegister(char* str);
Operation convertToOperation(char* str);

#endif

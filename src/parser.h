#ifndef PARSER_H
#define PARSER_H

#include "cpu.h"
#include <stdio.h>

void trim(char* str);
void deleteEnter(char* str);
void deleteComment(char* str);
char* parseWord(char* str, const char* delimeters, char required, int lineNumber);
double parseNum(char* str, int lineNumber);

int parseInstruction(CPU* cpu, FILE* file, int lineNumber);
Register convertToRegister(char* str, int lineNumber);
Operation convertToOperation(char* str, int lineNumber);

#endif

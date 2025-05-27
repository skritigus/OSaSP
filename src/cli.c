#include "cli.h"
#include "instructions.h"
#include "file.h"
#include "operations.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#define MAX_INPUT_LEN 256

void trim(char* str) 
{
	char* start = str;
	
    while (isspace(*str)) 
    {
		++str;
	}

	if (start != str) 
	{
		memmove(start, str, strlen(str) + 1);
	}

    int len = strlen(str);
    while (isspace(str[len - 1])) 
    {
        str[--len] = '\0';
    }
}

void deleteEnter(char* str)
{
	char* end;
	
	end = strchr(str, '\n');
	if (end != NULL)
	{
		*end = '\0';
	}
}

char* parseWord(char* str, const char* delimeters)
{
	char* word = strtok(str, delimeters);
	if (word == NULL) 
	{
		printf("\nMissing operation or operand\n");
		printf("For instructions' list print \"HELP\"\n\n");
	}
	else
	{
		trim(word);
	}
	
	return word;
}

double parseNum(char* str)
{
	char* numEnd;
	double value = strtod(str, &numEnd);
	if(*numEnd != 0)
	{
		printf("\nThe second operand of operation must be number\n");
		printf("For instructions' list print \"HELP\"\n\n");
	}
	
	return value;
}

void parseInstruction(CPU* cpu) 
{
    char input[MAX_INPUT_LEN];
    char* word;
    Instruction instruction = {0};
    char success;

	do
	{
		success = 0;
		
		if (fgets(input, sizeof(input), stdin) == NULL) 
		{
			perror("Error in getting instruction");
			exit(EXIT_FAILURE);
		}
		deleteEnter(input);

		if((word = parseWord(input, " ")) == NULL)
		{
			continue;
		}
		
		instruction.opcode = convertToOperation(word);
		switch(instruction.opcode)
		{
			case HELP:
				if((word = parseWord(NULL, "\0")) != NULL)
				{
					break;
				}
				loadHelp();
				break;
			case STAT:
				if((word = parseWord(NULL, "\0")) != NULL)
				{
					break;
				}
				printStat(cpu);
				break;
			case END:
				if((word = parseWord(NULL, "\0")) != NULL)
				{
					break;
				}
				exit(EXIT_SUCCESS);
			case PRINT:
			case CLR:
				if((word = parseWord(NULL, "\0")) == NULL)
				{
					break;
				}
				
				if((instruction.operand1 = convertToRegister(word)) == INVALID_REG)
				{
					break;
				}
				
				cpu->memory[cpu->lastInstruction++] = instruction.opcode;
				cpu->memory[cpu->lastInstruction++] = instruction.operand1;
				
				success = 1;
				break;
			case MOV:
				if((word = parseWord(NULL, ",")) == NULL)
				{
					break;
				}
				
				if((instruction.operand1 = convertToRegister(word)) == INVALID_REG)
				{
					break;
				}
				
				if((word = parseWord(NULL, "\0")) == NULL)
				{
					break;
				}
				
				double value = parseNum(word);
				if(errno != 0)
				{
					break;
				} 
				
				cpu->memory[cpu->lastInstruction++] = instruction.opcode;
				cpu->memory[cpu->lastInstruction++] = instruction.operand1;
				cpu->memory[cpu->lastInstruction++] = floatToFixed(value);
				
				success = 1;
				break;
			case MOVR:
			case ADD:
			case SUB:
			case MUL:
			case DIV:
				if((word = parseWord(NULL, ",")) == NULL)
				{
					break;
				}
				
				if((instruction.operand1 = convertToRegister(word)) == INVALID_REG)
				{
					break;
				}
				
				if((word = parseWord(NULL, "\0")) == NULL)
				{
					break;
				}
				
				if((instruction.operand2 = convertToRegister(word)) == INVALID_REG)
				{
					break;
				}
				
				if((instruction.operand2 < 4 || instruction.operand1 < 4) && ((instruction.operand2 > 3 || instruction.operand1 > 3)))
				{
					printf("\nError. Differnt register size\n");
					printf("For instructions' list print \"HELP\"\n\n");
				}
				
				if(instruction.opcode == MUL && instruction.operand1 < 4)
				{
					printf("\nError. You can multiply only 32-bit registers\n");
					printf("For instructions' list print \"HELP\"\n\n");
				}
				
				cpu->memory[cpu->lastInstruction++] = instruction.opcode;
				cpu->memory[cpu->lastInstruction++] = instruction.operand1;
				cpu->memory[cpu->lastInstruction++] = instruction.operand2;
				
				success = 1;
				break;
			default:
				break;
		}	
	} while(success == 0);
	
}

Register convertToRegister(char* str)
{
	char* start = str;
	
	while (*str) 
	{
        *str = toupper(*str);
        ++str;
    }
    str = start;
	
	if (strcmp(str, "RAX") == 0) 
	{
		return RAX;
    }
    else if (strcmp(str, "RBX") == 0) 
    {
		return RBX;
    }
    else if (strcmp(str, "RCX") == 0) 
    {
		return RCX;
    }
    else if (strcmp(str, "RDX") == 0) 
    {
		return RDX;
    }
    else if (strcmp(str, "RAH") == 0) 
    {
		return RAH;
    }
    else if (strcmp(str, "RBH") == 0) 
    {
		return RBH;
    }
    else if (strcmp(str, "RCH") == 0) 
    {
		return RCH;
    }
    else if (strcmp(str, "RDH") == 0) 
    {
		return RDH;
    }
    else if (strcmp(str, "RAL") == 0) 
    {
		return RAL;
    }
    else if (strcmp(str, "RBL") == 0) 
    {
		return RBL;
    }
    else if (strcmp(str, "RCL") == 0) 
    {
		return RCL;
    }
    else if (strcmp(str, "RDL") == 0) 
    {
		return RDL;
    }
    else
    {
		printf("Unknown register \"%s\"\n", str);
		printf("For registers' list print \"HELP\"\n\n");
	}

    return INVALID_REG;
}

Operation convertToOperation(char* str)
{
	char* start = str;
	
	while (*str) 
	{
        *str = toupper(*str);
        ++str;
    }
    
    str = start;
	
	if (strcmp(str, "HELP") == 0) 
	{
		return HELP;
    }
    else if (strcmp(str, "STAT") == 0) 
    {
		return STAT;
    }
    else if (strcmp(str, "END") == 0) 
    {
		return END;
    }
    else if (strcmp(str, "PRINT") == 0) 
    {
		return PRINT;
    }
    else if (strcmp(str, "CLR") == 0) 
    {
		return CLR;
    }
    else if (strcmp(str, "MOV") == 0) 
    {
		return MOV;
    }
    else if (strcmp(str, "MOVR") == 0) 
    {
		return MOVR;
    }
    else if (strcmp(str, "ADD") == 0) 
    {
		return ADD;
    }
    else if (strcmp(str, "SUB") == 0) 
    {
        return SUB;
    }
    else if (strcmp(str, "MUL") == 0) 
    {
        return MUL;
    }
    else if (strcmp(str, "DIV") == 0) 
    {
        return DIV;
    }
    else
    {
		printf("Unknown operation \"%s\"\n", str);
		printf("For instructions' list print \"HELP\"\n\n");
	}

    return INVALID_OP;
}

void printStat(CPU* cpu)
{
	printf("Registers:\n");
	printf("RAX: ");
	printNum(fixed64ToFloat(cpu->registers[RAX].value));
	printf("RAH: ");
	printNum(fixed32ToFloat(cpu->registers[RAX].high));
	printf("RAL: ");
	printNum(fixed32ToFloat(cpu->registers[RAX].low));
	
	printf("RBX: ");
	printNum(fixed64ToFloat(cpu->registers[RBX].value));
	printf("RBH: ");
	printNum(fixed32ToFloat(cpu->registers[RBX].high));
	printf("RBL: ");
	printNum(fixed32ToFloat(cpu->registers[RBX].low));
	
	printf("RCX: ");
	printNum(fixed64ToFloat(cpu->registers[RCX].value));
	printf("RCH: ");
	printNum(fixed32ToFloat(cpu->registers[RCX].high));
	printf("RCL: ");
	printNum(fixed32ToFloat(cpu->registers[RCX].low));
	
	printf("RDX: ");
	printNum(fixed64ToFloat(cpu->registers[RDX].value));
	printf("RDH: ");
	printNum(fixed32ToFloat(cpu->registers[RDX].high));
	printf("RDL: ");
	printNum(fixed32ToFloat(cpu->registers[RDX].low));
}

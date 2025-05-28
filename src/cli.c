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
	if(str == NULL)
	{
		return;
	}
	
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

void deleteComment(char* str)
{
	char* commentPos = strchr(str, ';');
	if (commentPos != NULL) 
	{
		*commentPos = '\0';
	}
}

char* parseWord(char* str, const char* delimeters, char required, int lineNumber)
{	
	char* word = strtok(str, delimeters);
	char result = required == 0 ? word == NULL : word != NULL;
	
	if (result == 0) 
	{
		printf("\n");
		if(lineNumber != 0)
		{
			printf("Line %d: ", lineNumber);
		}
		printf("Wrong instruction\n");
		printf("For instructions' list print \"HELP\"\n\n");
	}
	else
	{
		trim(word);
	}
	
	return word;
}

double parseNum(char* str, int lineNumber)
{
	char* numEnd;
	double value = strtod(str, &numEnd);
	if(*numEnd != 0)
	{
		printf("\n");
		if(lineNumber != 0)
		{
			printf("Line %d: ", lineNumber);
		}
		printf("The second operand of operation must be number\n");
		printf("For instructions' list print \"HELP\"\n\n");
	}
	
	return value;
}

int parseInstruction(CPU* cpu, FILE* file, int lineNumber) 
{
    char input[MAX_INPUT_LEN];
    char* word;
    Instruction instruction = {0};

	if (fgets(input, sizeof(input), file == NULL ? stdin : file) == NULL) 
	{
		if(errno != 0)
		{
			perror("Error in getting instruction");
			exit(EXIT_FAILURE);
		}
		else
		{
			return -2;
		} 
	}
	 
	deleteEnter(input);
	deleteComment(input);

	trim(input);
	if (strlen(input) == 0)
	{
		return 2;
	}
	
	if((word = parseWord(input, " ", 1, lineNumber)) == NULL)
	{
		return -1;
	}
	
	instruction.opcode = convertToOperation(word, lineNumber);
	switch(instruction.opcode)
	{
		case HELP:
		case STAT:
			if((word = parseWord(NULL, "\0", 0, lineNumber)) != NULL)
			{
				return -1;
			}
			writeToMemory(cpu, instruction, 1, file);	
			break;
		case END:
			if((word = parseWord(NULL, "\0", 0, lineNumber)) != NULL)
			{
				return -1;
			}
			
			if(writeToMemory(cpu, instruction, 1, file) == -1)
			{
				return -1;
			}	
			return 1;
		case PRINT:
		case CLR:
		case CHS:
			if((word = parseWord(NULL, "\0", 1, lineNumber)) == NULL)
			{
				return -1;
			}
			
			if((instruction.operand1 = convertToRegister(word, lineNumber)) == INVALID_REG)
			{
				break;
			}
			
			if(writeToMemory(cpu, instruction, 2, file) == -1)
			{
				return -1;
			}
			break;
		case MOV:
			if((word = parseWord(NULL, ",", 1, lineNumber)) == NULL)
			{
				return -1;
			}
			
			if((instruction.operand1 = convertToRegister(word, lineNumber)) == INVALID_REG)
			{
				return -1;
			}
			
			if((word = parseWord(NULL, "\0", 1, lineNumber)) == NULL)
			{
				return -1;
			}
			
			double value = parseNum(word, lineNumber);
			if(errno != 0)
			{
				return -1;
			} 
			
			instruction.operand2 = floatToFixed(value);
			if(writeToMemory(cpu, instruction, 3, file) == -1)
			{
				return -1;
			}
			break;
		case MOVR:
		case ADD:
		case SUB:
		case MUL:
			if((word = parseWord(NULL, ",", 1, lineNumber)) == NULL)
			{
				return -1;
			}
			
			if((instruction.operand1 = convertToRegister(word, lineNumber)) == INVALID_REG)
			{
				return -1;
			}
			
			if((word = parseWord(NULL, "\0", 1, lineNumber)) == NULL)
			{
				return -1;
			}
			
			if((instruction.operand2 = convertToRegister(word, lineNumber)) == INVALID_REG)
			{
				return -1;
			}
			
			if((instruction.operand2 < 4 || instruction.operand1 < 4) && ((instruction.operand2 > 3 || instruction.operand1 > 3)))
			{
				printf("\n");
				if(lineNumber != 0)
				{
					printf("Line %d: ", lineNumber);
				}
				printf("Error. Different register size\n");
				printf("For instructions' list print \"HELP\"\n\n");
				return -1;
			}
			
			if(instruction.opcode == MUL && instruction.operand1 < 4)
			{
				printf("\n");
				if(lineNumber != 0)
				{
					printf("Line %d: ", lineNumber);
				}
				printf("Error. You can multiply only 32-bit registers\n");
				printf("For instructions' list print \"HELP\"\n\n");
				return -1;
			}
			
			if(writeToMemory(cpu, instruction, 3, file) == -1)
			{
				return -1;
			}
			break;
		case DIV:
			if((word = parseWord(NULL, ",", 1, lineNumber)) == NULL)
			{
				return -1;
			}
			
			if((instruction.operand1 = convertToRegister(word, lineNumber)) == INVALID_REG)
			{
				return -1;
			}
			
			if((word = parseWord(NULL, "\0", 1, lineNumber)) == NULL)
			{
				return -1;
			}
			
			if((instruction.operand2 = convertToRegister(word, lineNumber)) == INVALID_REG)
			{
				return -1;
			}
			
			if(instruction.operand2 < 4 || instruction.operand1 > 3)
			{
				printf("\n");
				if(lineNumber != 0)
				{
					printf("Line %d: ", lineNumber);
				}
				printf("Error. Different register size\n");
				printf("For instructions' list print \"HELP\"\n\n");
				break;
			}
			
			if(writeToMemory(cpu, instruction, 3, file) == -1)
			{
				return -1;
			}
			break;
		default:
			return -1;
	}	
	
	return 0;
}

Register convertToRegister(char* str, int lineNumber)
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
		printf("\n");
		if(lineNumber != 0)
		{
			printf("Line %d: ", lineNumber);
		}
		printf("Unknown register \"%s\"\n", str);
		printf("For registers' list print \"HELP\"\n\n");
	}

    return INVALID_REG;
}

Operation convertToOperation(char* str, int lineNumber)
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
    else if (strcmp(str, "CHS") == 0) 
    {
		return CHS;
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
		printf("\n");
		if(lineNumber != 0)
		{
			printf("Line %d: ", lineNumber);
		}
		printf("Unknown operation \"%s\"\n", str);
		printf("For instructions' list print \"HELP\"\n\n");
	}

    return INVALID_OP;
}

void printStat(CPU* cpu)
{
	printf("\nRegisters:\n");
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
	printf("\n");
}

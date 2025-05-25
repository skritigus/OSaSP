#include "cli.h"
#include "instructions.h"
#include "operations.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#define MAX_LEN 256

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
	
	return word;
}

float parseNum(char* str)
{
	char* numEnd;
	float value = strtof(str, &numEnd);
	if(*numEnd != 0)
	{
		printf("\nThe second operand of operation must be number\n");
		printf("For instructions' list print \"HELP\"\n\n");
	}
	
	return value;
}

void parseInstruction(CPU* cpu) 
{
    char input[MAX_LEN];
    char* word;
    Instruction instruction = {0};
    int temp;
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
				if((word = parseWord(NULL, "\0")) == NULL)
				{
					break;
				}
				//printHelp();
				success = 1;
				break;
			case STAT:
				if((word = parseWord(NULL, "\0")) == NULL)
				{
					break;
				}
				//printStat();
				success = 1;
				break;
			case END:
				if((word = parseWord(NULL, "\0")) == NULL)
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
				trim(word);
				
				cpu->memory[cpu->lastInstruction++] = instruction.opcode;
				cpu->memory[cpu->lastInstruction++] = convertToRegister(word);
				
				success = 1;
				break;
			case MOV:
				if((word = parseWord(NULL, ",")) == NULL)
				{
					break;
				}
				trim(word);
				
				if((temp = convertToRegister(word)) == INVALID_REG)
				{
					break;
				}
				instruction.operand1 = temp;
				
				if((word = parseWord(NULL, "\0")) == NULL)
				{
					break;
				}
				trim(word);
				
				float value = parseNum(word);
				if(errno != 0)
				{
					break;
				} 
				
				cpu->memory[cpu->lastInstruction++] = instruction.opcode;
				cpu->memory[cpu->lastInstruction++] = instruction.operand1;
				cpu->memory[cpu->lastInstruction++] = floatToFixed(value);
				
				success = 1;
				break;
			case ADD:
			case SUB:
			case MUL:
			case DIV:
				if((word = parseWord(NULL, ",")) == NULL)
				{
					break;
				}
				trim(word);
				
				if((temp = convertToRegister(word)) == INVALID_REG)
				{
					break;
				}
				instruction.operand1 = temp;
				
				if((word = parseWord(NULL, "\0")) == NULL)
				{
					break;
				}
				trim(word);
				
				if((temp = convertToRegister(word)) == INVALID_REG)
				{
					break;
				}
				cpu->memory[cpu->lastInstruction++] = instruction.opcode;
				cpu->memory[cpu->lastInstruction++] = instruction.operand1;
				cpu->memory[cpu->lastInstruction++] = temp;
				
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
     else if (strcmp(str, "RFX") == 0) 
    {
		return RFX;
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

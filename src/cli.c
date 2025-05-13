#include "cli.h"
#include "instructions.h"
#include "registers.h"
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

void parseInstruction(CPU* cpu) 
{
    char input[MAX_LEN];
    char* end;
    char* token;
    char* operation;
    Instruction instruction;
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

		end = strchr(input, '\n');
		if (end != NULL)
		{
			*end = '\0';
		}

		operation = strtok(input, " ");
		if (operation == NULL) 
		{
			printf("\nWrong input. No operation was provided\n");
			printf("For instructions' list print \"HELP\"\n\n");
			continue;
		}
		
		instruction.opcode = convertToOperation(operation);
		if(instruction.opcode == 0)
		{
			printf("\nUnknown operation \"%s\"\n", operation);
			printf("For operations' list print \"HELP\"\n\n");
			continue;
		}

		switch(instruction.opcode)
		{
			case HELP:
				token = strtok(NULL, "\0");
				if (token != NULL) 
				{	
					printf("\nOperation \"%s\" can't contain any operands\n", operation);
					printf("For operations' list print \"HELP\"\n\n");
					break;
				}
				//printHelp();
				success = 1;
				break;
			case STAT:
				token = strtok(NULL, "\0");
				if (token != NULL) 
				{	
					printf("\nOperation \"%s\" can't contain any operands\n", operation);
					printf("For instructions' list print \"HELP\"\n\n");
					break;
				}
				//printStat();
				success = 1;
				break;
			case END:
				token = strtok(NULL, "\0");
				if (token != NULL) 
				{	
					printf("\nOperation \"%s\" can't contain any operands\n", operation);
					printf("For instructions' list print \"HELP\"\n\n");
					break;
				}
				exit(EXIT_SUCCESS);
			case PRINT:
				token = strtok(NULL, "\0");
				if (token == NULL) 
				{	
					printf("\nOperation \"%s\" contains one operand\n", operation);
					printf("For instructions' list print \"HELP\"\n\n");
					break;
				}
				trim(token);
				
				instruction.operand1 = convertToRegister(token);
				success = 1;
				break;
			case MOV:
				token = strtok(NULL, ",");
				if (token == NULL) 
				{
					printf("\nOperation \"%s\" must be written in this way: %s register, number\n", operation, operation);
					printf("For instructions' list print \"HELP\"\n\n");
					break;
				}
				trim(token);
				
				temp = convertToRegister(token);
				if(temp == 0)
				{
					printf("\nThe first operand of operation \"%s\" must be register\n", operation);
					printf("For registers' list print \"HELP\"\n\n");
					break;
				}
				instruction.operand1 = temp;
				
				token = strtok(NULL, "\0");
				if (!token) 
				{					
					printf("\nOperation \"%s\" must be written in this way: %s register, number\n", operation, operation);
					printf("For instructions' list print \"HELP\"\n\n");
					break;
				}
				trim(token);
				
				temp = atof(token);
				if(errno != 0)
				{
					printf("\nThe second operand of operation \"%s\" must be number\n", operation);
					printf("For instructions' list print \"HELP\"\n\n");
					break;
				}
				instruction.operand2 = floatToFixed(temp);
				success = 1;
				break;
			case ADD:
			case SUB:
			case MUL:
			case DIV:
				token = strtok(NULL, ",");
				if (token == NULL) 
				{
					printf("\nOperation \"%s\" must be written in this way: %s register, register\n", operation, operation);
					printf("For instructions' list print \"HELP\"\n\n");
					break;
				}
				trim(token);
				
				temp = convertToRegister(token);
				if(temp == 0)
				{
					printf("\nThe first operand of instruction \"%s\" must be register\n", token);
					printf("For registers' list print \"HELP\"\n\n");
					break;
				}
				instruction.operand1 = temp;
				
				token = strtok(NULL, "\0");
				if (!token) 
				{					
					printf("\nOperation \"%s\" must be written in this way: %s register, register\n", operation, operation);
					printf("For instructions' list print \"HELP\"\n\n");
					break;
				}
				trim(token);
				
				temp = convertToRegister(token);
				if(temp == 0)
				{
					printf("\nThe second operand of instruction \"%s\" must be register\n", token);
					printf("For registers' list print \"HELP\"\n\n");
					break;
				}
				instruction.operand2 = temp;
				
				success = 1;
				break;
		}	
	} while(success == 0);
	
	cpu->memory[cpu->lastInstruction++] = instruction.opcode;
	cpu->memory[cpu->lastInstruction++] = instruction.operand1;
	cpu->memory[cpu->lastInstruction++] = instruction.operand2;
}

int convertToRegister(char* str)
{
	char* start = str;
	
	while (*str) 
	{
        *str = toupper(*str);
        ++str;
    }
    
    str = start;
	
	if (strcmp(str, "REGA") == 0) 
	{
		return REGA;
    }
    else if (strcmp(str, "REGB") == 0) 
    {
		return REGB;
    }
    else if (strcmp(str, "REGR") == 0) 
    {
		return REGR;
    }

    return 0;
}

int convertToOperation(char* str)
{
	char* start = str;
	
	while (*str) 
	{
        *str = toupper(*str);
        ++str;
    }
    
    str = start;
	
	if (strcmp(str, "END") == 0) 
	{
		return END;
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

    return 0;
}

#include "cpu.h"
#include "file.h"
#include "instructions.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

CPU initCpu()
{
	CPU cpu;
	
	memset(cpu.registers, 0, sizeof(int64_t) * REGISTER_COUNT);
    cpu.lastInstruction = 0;
    cpu.currentInstruction = 0;
    memset(cpu.memory, 0, sizeof(int) * MEMORY_SIZE);
    
    return cpu;
}

void execute(CPU* cpu) 
{
	int op = cpu->memory[cpu->currentInstruction++];
	int divRes;

	switch (op) 
	{
		case END:
			exit(EXIT_SUCCESS);
		case STAT:
			printStat(cpu);
			return;
		case PRINT:
			printReg(cpu);
			return;
		case HELP:
			loadHelp();
			return;
		case MOV:
			handleMov(cpu);
			break;
		case MOVR:
			handleMovr(cpu);
			break;
		case CLR:
			handleClr(cpu);
			break;
		case CHS:
			handleChs(cpu);
			break;
		case ADD:
			if(handleAdd(cpu) == 1)
			{
				fprintf(stderr, "Overflow detected! The result is bigger than size of register\n");
			}
			break;
		case SUB:
			if(handleSub(cpu) == 1)
			{
				fprintf(stderr, "Overflow detected! The result is bigger than size of register\n");
			}
			break;
		case MUL:
			if(handleMul(cpu) == 1)
			{
				fprintf(stderr, "Overflow detected! The multiplicand is bigger than size of register\n");
			}
			break;
		case DIV:
			divRes = handleDiv(cpu);
			if(divRes == -1)
			{
				fprintf(stderr, "Overflow detected! The dividend is bigger than size of register\n");
			}
			else
			{
				if(divRes == -2)
				{
					fprintf(stderr, "Division on zero!\n");
				}
			}
			break;
		default:
			fprintf(stderr, "Unknown instruction\n");
			return;
	}
}

int writeToMemory(CPU* cpu, Instruction instruction, int opCount, void* fileOpened)
{
    if (cpu->lastInstruction + opCount >= MEMORY_SIZE)
    {
		if(fileOpened != NULL)
		{
			printf("\nError: CPU memory overflow. Too many instructions in file\n\n");
			return -1;
		}
		else
		{
			cpu->lastInstruction = 0;
			cpu->currentInstruction = 0;
		}
    }
	
    cpu->memory[cpu->lastInstruction++] = instruction.opcode;
    if(opCount > 1)
    {
		cpu->memory[cpu->lastInstruction++] = instruction.operand1;
	}
	if(opCount > 2)
	{
		cpu->memory[cpu->lastInstruction++] = instruction.operand2;
	}
    return 0;
}


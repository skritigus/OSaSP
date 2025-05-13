#include "instructions.h"
#include "operations.h"
#include "registers.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

void handleMov(CPU* cpu) 
{
	int reg = cpu->memory[cpu->currentInstruction++];
    cpu->registers[reg] = cpu->memory[cpu->currentInstruction++];
}

void handleAdd(CPU* cpu) 
{
	int reg1 = cpu->memory[cpu->currentInstruction++];
    int reg2 = cpu->memory[cpu->currentInstruction++];
	
    addOp(&cpu->registers[reg1], cpu->registers[reg2]);
}

void handleSub(CPU* cpu) 
{
    int reg1 = cpu->memory[cpu->currentInstruction++];
    int reg2 = cpu->memory[cpu->currentInstruction++];
    
	addOp(&cpu->registers[reg1], ~cpu->registers[reg2]);
	addOp(&cpu->registers[reg1], 1);
}

void handleMul(CPU* cpu) 
{
	int reg1 = cpu->memory[cpu->currentInstruction++];
    int reg2 = cpu->memory[cpu->currentInstruction++];
    
	while (cpu->registers[reg2] != 0) 
	{
		if (cpu->registers[reg2] & 1) 
		{
			addOp(&cpu->registers[REGR], cpu->registers[reg1]);
		}
		cpu->registers[reg1] <<= 1;
		cpu->registers[reg2] >>= 1;
    }
}

void handleDiv(CPU* cpu) 
{
	int reg1 = cpu->memory[cpu->currentInstruction++];
    int reg2 = cpu->memory[cpu->currentInstruction++];
    
    if (cpu->registers[reg2] == 0) {
        printf("Error. Division on zero\n");
        exit(EXIT_FAILURE);
    }

    if (cpu->registers[reg1] == INT_MIN && cpu->registers[reg2] == -1)
    {
        return;
	}
	
	cpu->registers[REGR] = divOp(cpu->registers[reg1], cpu->registers[reg2]);
}

void handlePrint(CPU* cpu) 
{
    int reg = cpu->memory[cpu->currentInstruction++];
    float num = fixedToFloat(cpu->registers[reg]);
    
    if(num == (int)num)
    {
        printf("%d\n", cpu->registers[reg]);
	}
	else
	{
		printf("%d\n", cpu->registers[reg]);
	}
}

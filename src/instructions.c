#include "instructions.h"
#include "operations.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>

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
	cpu->RCX = 0;
	int reg1 = cpu->memory[cpu->currentInstruction++];
    int reg2 = cpu->memory[cpu->currentInstruction++];
    int negreg = -cpu->registers[reg1];
    int doublereg = cpu->registers[reg1] << 1;
    int negdoublereg = negreg << 1;
    
    printf("%d  %d  %d  %d\n", cpu->registers[reg1], negreg, doublereg, negdoublereg);
    
	for (int i = 15; i >= 0; --i) 
	{	
		printf("%d  \n", i);
		cpu->RCX <<= 2;

		int bits = (cpu->registers[reg2] >> ((i << 1) - 1)) & 0x7;
		
        switch (bits) {
            case 5:
            case 6:
                printf("5 6\n");
                addOp64(&cpu->RCX, negreg);
                break;
            case 1:
            case 2:	
                printf("1 2\n");
                addOp64(&cpu->RCX, cpu->registers[reg1]);
                break;
            case 4:
				printf("4\n");
				addOp64(&cpu->RCX, negdoublereg);
                break;
            case 3:
				printf("3\n");
				addOp64(&cpu->RCX, doublereg);
                break;
        }
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
	
	cpu->registers[RCX] = divOp(cpu->registers[reg1], cpu->registers[reg2]);
}

void handleClr(CPU* cpu) 
{
	int reg = cpu->memory[cpu->currentInstruction++];
	cpu->registers[reg] ^= cpu->registers[reg];
}

void handlePrint(CPU* cpu) 
{
    int reg = cpu->memory[cpu->currentInstruction++];
    double num;
    if (reg == RCX)
    {
		num = fixed64ToFloat(cpu->RCX);
	}
	else
	{
		num = fixedToFloat(cpu->registers[reg]);
	}
    printf("%f\n", num);
    
    if(num == (int64_t)num)
    {
        printf("%ld\n", (int64_t)num);
	}
	else
	{
		printf("%f\n", num);
	}
}

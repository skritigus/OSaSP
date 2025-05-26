#include "instructions.h"
#include "operations.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>

void handleMov(CPU* cpu) 
{
	Register reg = cpu->memory[cpu->currentInstruction++];
    setRegisterValue(cpu, reg, cpu->memory[cpu->currentInstruction++]);
}

void handleAdd(CPU* cpu) 
{
	Register reg1 = cpu->memory[cpu->currentInstruction++];
    Register reg2 = cpu->memory[cpu->currentInstruction++];
	
	if(reg1 > 3 && reg2 > 3)
	{
		addOp32(getRegisterValue(cpu, reg1), *getRegisterValue(cpu, reg2));
	}
	else
	{
		addOp64((int64_t*)getRegisterValue(cpu, reg1), *(int64_t*)getRegisterValue(cpu, reg2));
	}
}

void handleSub(CPU* cpu) 
{
    Register reg1 = cpu->memory[cpu->currentInstruction++];
    Register reg2 = cpu->memory[cpu->currentInstruction++];
    
	if(reg1 > 3 && reg2 > 3)
	{
		addOp32(getRegisterValue(cpu, reg1), ~(*getRegisterValue(cpu, reg2)));
		addOp32(getRegisterValue(cpu, reg1), 1);
	}
	else
	{
		addOp64((int64_t*)getRegisterValue(cpu, reg1), ~(*(int64_t*)getRegisterValue(cpu, reg2)));
		addOp64((int64_t*)getRegisterValue(cpu, reg1), 1);
	}
}

void handleMul(CPU* cpu) 
{
	cpu->registers[RCX].value ^= cpu->registers[RCX].value;
	Register reg1 = cpu->memory[cpu->currentInstruction++];
    Register reg2 = cpu->memory[cpu->currentInstruction++];
    int32_t reg = *getRegisterValue(cpu, reg1);
    int32_t negreg = -reg;
    int32_t doublereg = (*getRegisterValue(cpu, reg1)) << 1;
    int32_t negdoublereg = negreg << 1;
    
	for (int i = 15; i >= 0; --i) 
	{	
		printf("%d  \n", i);
		cpu->registers[RCX].value <<= 2;

		int bits = (*getRegisterValue(cpu, reg2) >> ((i << 1) - 1)) & 0x7;
		
        switch (bits) {
            case 5:
            case 6:
                printf("5 6\n");
                addOp32(&cpu->registers[RCX].low, negreg);
                break;
            case 1:
            case 2:	
                printf("1 2\n");
                addOp32(&cpu->registers[RCX].low, reg);
                break;
            case 4:
				printf("4\n");
				addOp32(&cpu->registers[RCX].low, negdoublereg);
                break;
            case 3:
				printf("3\n");
				addOp32(&cpu->registers[RCX].low, doublereg);
                break;
        }
    }
    cpu->registers[RCX].value >>= FRACTIONAL_LEN; 
}

void handleDiv(CPU* cpu) 
{
	Register reg1 = cpu->memory[cpu->currentInstruction++];
    Register reg2 = cpu->memory[cpu->currentInstruction++];
    
    if (cpu->registers[reg2].value == 0) {
        printf("Error. Division on zero\n");
        exit(EXIT_FAILURE);
    }

    if (cpu->registers[reg1].value == INT_MIN && cpu->registers[reg2].value == -1)
    {
        return;
	}
	
	cpu->registers[RCX].value = divOp(cpu->registers[reg1].value, cpu->registers[reg2].value);
}

void handleClr(CPU* cpu) 
{
	Register reg = cpu->memory[cpu->currentInstruction++];
	*(int64_t*)getRegisterValue(cpu, reg) ^= *(int64_t*)getRegisterValue(cpu, reg);
}

void handlePrint(CPU* cpu) 
{
    Register reg = cpu->memory[cpu->currentInstruction++];
    double num;
    if (reg < 4)
    {
		num = fixed64ToFloat(cpu->registers[reg].value);
	}
	else
	{
		num = fixedToFloat(*getRegisterValue(cpu, reg));
	}
    
    if(num == (int64_t)num)
    {
        printf("%ld\n", (int64_t)num);
	}
	else
	{
		printf("%f\n", num);
	}
}

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

void handleMovr(CPU* cpu) 
{
	Register reg1 = cpu->memory[cpu->currentInstruction++];
	Register reg2 = cpu->memory[cpu->currentInstruction++];
    setRegisterValue(cpu, reg1, *(int64_t*)getRegisterValue(cpu, reg2));
}

int handleAdd(CPU* cpu) 
{
	Register reg1 = cpu->memory[cpu->currentInstruction++];
    Register reg2 = cpu->memory[cpu->currentInstruction++];
	
	if(reg1 > 3 && reg2 > 3)
	{
		return addOp32(getRegisterValue(cpu, reg1), *getRegisterValue(cpu, reg2));
	}
	else
	{
		return addOp64((int64_t*)getRegisterValue(cpu, reg1), *(int64_t*)getRegisterValue(cpu, reg2));
	}
}

void handleSub(CPU* cpu) 
{
    Register reg1 = cpu->memory[cpu->currentInstruction++];
    Register reg2 = cpu->memory[cpu->currentInstruction++];
    
	if(reg1 > 3 && reg2 > 3)
	{
		addOp32(getRegisterValue(cpu, reg1), convertSign32(*getRegisterValue(cpu, reg2)));
	}
	else
	{
		addOp64((int64_t*)getRegisterValue(cpu, reg1), convertSign64(*(int64_t*)getRegisterValue(cpu, reg2)));
	}
}

int handleMul(CPU* cpu) 
{
	cpu->registers[RCX].value ^= cpu->registers[RCX].value;
	Register reg1 = cpu->memory[cpu->currentInstruction++];
    Register reg2 = cpu->memory[cpu->currentInstruction++];
    int32_t mul = *getRegisterValue(cpu, reg1);
    int32_t negmul = convertSign64(mul);
    int32_t doublemul = mul << 1;
    int32_t negdoublemul = negmul << 1;
    
    printf("%d %d %d %d\n", mul, negmul, doublemul, negdoublemul);
    for (int i = 0; i < 16; ++i) 
	{	
		printf("%d  \n", i);

		int bits = (*getRegisterValue(cpu, reg2) >> ((i << 1) - 1)) & 0x7;
		
        switch (bits) 
        {
			case 1:
            case 2:	
                printf("1 2\n");
                addOp32(&cpu->registers[RCX].high, mul);
                break;
			case 3:
				printf("3\n");
				addOp32(&cpu->registers[RCX].high, doublemul);
                break;
            case 4:
				printf("4\n");
				addOp32(&cpu->registers[RCX].high, negdoublemul);
                break;
            case 5:
            case 6:
                printf("5 6\n");
                addOp32(&cpu->registers[RCX].high, negmul);
                break;
        }
        cpu->registers[RCX].value >>= 2;
    }
    cpu->registers[RCX].value >>= FRACTIONAL_LEN;
    return ((mul >> 31) & 1) ^ ((mul >> 30) & 1); 
}

void handleDiv(CPU* cpu) 
{
	Register reg1 = cpu->memory[cpu->currentInstruction++];
    Register reg2 = cpu->memory[cpu->currentInstruction++];
    
    if (*getRegisterValue(cpu, reg2) == 0) 
    {
        printf("Error. Division on zero\n");
        exit(EXIT_FAILURE);
    }

   /* if (cpu->registers[reg1 % 4].low == INT_MIN && cpu->registers[reg2 % 4].low == -1)
    {
        return;
	}*/
	
	cpu->registers[RDX].low = divOp(cpu->registers[reg1].value, *getRegisterValue(cpu, reg2));
}

void handleClr(CPU* cpu) 
{
	Register reg = cpu->memory[cpu->currentInstruction++];
	*(int64_t*)getRegisterValue(cpu, reg) ^= *(int64_t*)getRegisterValue(cpu, reg);
}

void printReg(CPU* cpu, Register reg) 
{
    double num;
    if (reg < 4)
    {
		num = fixed64ToFloat(cpu->registers[reg].value);
	}
	else
	{
		num = fixed32ToFloat(*getRegisterValue(cpu, reg));
	}
	
	printNum(num);
}

void printNum(double num)
{
	if(num == (int64_t)num)
    {
        printf("%ld\n", (int64_t)num);
	}
	else
	{
		printf("%f\n", num);
	}
}

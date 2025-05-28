#include "instructions.h"
#include "operations.h"
#include "cpu.h"
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

int handleChs(CPU* cpu) 
{
	Register reg = cpu->memory[cpu->currentInstruction++];
	
    if(reg > 3)
	{
		if(*getRegisterValue(cpu, reg) == INT32_MIN)
		{
			return -1;
		}
		setRegisterValue(cpu, reg, convertSign64(*(int64_t*)getRegisterValue(cpu, reg)));
	}
	else
	{
		if(cpu->registers[reg].value == INT64_MIN)
		{
			return -1;
		}
		cpu->registers[reg].value = convertSign64(cpu->registers[reg].value);
	}
	
	return 0;
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

int handleSub(CPU* cpu) 
{
    Register reg1 = cpu->memory[cpu->currentInstruction++];
    Register reg2 = cpu->memory[cpu->currentInstruction++];
    
	if(reg1 > 3 && reg2 > 3)
	{
		return addOp32(getRegisterValue(cpu, reg1), convertSign32(*getRegisterValue(cpu, reg2)));
	}
	else
	{
		return addOp64((int64_t*)getRegisterValue(cpu, reg1), convertSign64(*(int64_t*)getRegisterValue(cpu, reg2)));
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
    
    for (int i = 0; i < 16; ++i) 
	{	
		int bits = (*getRegisterValue(cpu, reg2) >> ((i << 1) - 1)) & 0x7;
		
        switch (bits) 
        {
			case 1:
            case 2:	
                addOp32(&cpu->registers[RCX].high, mul);
                break;
			case 3:
				addOp32(&cpu->registers[RCX].high, doublemul);
                break;
            case 4:
				addOp32(&cpu->registers[RCX].high, negdoublemul);
                break;
            case 5:
            case 6:
                addOp32(&cpu->registers[RCX].high, negmul);
                break;
        }
        cpu->registers[RCX].value >>= 2;
    }
    cpu->registers[RCX].value >>= FRACTIONAL_LEN;
    return ((mul >> 31) & 1) ^ ((mul >> 30) & 1); 
}

int handleDiv(CPU* cpu) 
{
	Register reg1 = cpu->memory[cpu->currentInstruction++];
    Register reg2 = cpu->memory[cpu->currentInstruction++];
    
    if (*getRegisterValue(cpu, reg2) == 0) 
    {
        return -2;
    }
	
	if((cpu->registers[reg1].value >> (64 - FRACTIONAL_LEN)) != 0 && (cpu->registers[reg1].value >> (64 - FRACTIONAL_LEN)) != -1)
	{
		return -1;
	}
	
	int64_t reminder = ((cpu->registers[reg1].value >> 63) & 1) == 0 ? cpu->registers[reg1].value << FRACTIONAL_LEN : convertSign64(cpu->registers[reg1].value) << FRACTIONAL_LEN;
    int64_t extendedDivisor = ((*getRegisterValue(cpu, reg2) >> 31) & 1) == 0 ? (int64_t)*getRegisterValue(cpu, reg2) << 32 : convertSign64((int64_t)*getRegisterValue(cpu, reg2)) << 32;
    int64_t negExtendedDivisor = convertSign64(extendedDivisor);
    int32_t quotient = 0;

    for (int i = 31; i >= 0; --i) 
    {
        if (((reminder >> 63) & 1) == 0) 
        {
            quotient |= (1 << i);
            reminder <<= 1;
            addOp64(&reminder, negExtendedDivisor);
        } 
        else 
        {
            reminder <<= 1;
            addOp64(&reminder, extendedDivisor);
        }
    }
	
    quotient <<= 1;
    addOp32(&quotient, 1);
    
    if (reminder < 0)
    {
		if(quotient > 0)
		{
			addOp32(&quotient, convertSign32(1));
		}
	}

	if(((cpu->registers[reg1].value >> 63) & 1) ^ ((*getRegisterValue(cpu, reg2) >> 31) & 1))
	{
		cpu->registers[RDX].low = convertSign32(quotient);
	}
	else
	{
		cpu->registers[RDX].low = quotient;
	}
	
	return 0;
}

void handleClr(CPU* cpu) 
{
	Register reg = cpu->memory[cpu->currentInstruction++];
	*(int64_t*)getRegisterValue(cpu, reg) ^= *(int64_t*)getRegisterValue(cpu, reg);
}

void printReg(CPU* cpu) 
{
	Register reg = cpu->memory[cpu->currentInstruction++];
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
		printf("%.*f\n", FRACTIONAL_LEN, num);
	}
}

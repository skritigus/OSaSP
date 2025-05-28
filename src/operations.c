#include "operations.h"
#include "cpu.h"
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <stdint.h>

int32_t floatToFixed(double num) 
{    
    return (int32_t)(num * (1 << FRACTIONAL_LEN)); 
}

float fixed32ToFloat(int32_t num) 
{	
	return (float)num / (1 << FRACTIONAL_LEN);
}

double fixed64ToFloat(int64_t num) 
{	
	return (double)num / (1 << FRACTIONAL_LEN);
}
 
int addOp32(int32_t* num1, int32_t num2) 
{    
	uint8_t carryOut = 0;
	uint8_t carry = 0;
    
    for (int i = 0; i < 4; ++i) 
    {
		uint8_t propagate;
		uint8_t generate;
		carry = 0;

		carry |= carryOut & 1;
		carryOut = 0;

		propagate = ((*num1 >> (i * 8)) & 0xFF) ^ ((num2 >> (i * 8)) & 0xFF);
		generate = ((*num1 >> (i * 8)) & 0xFF) & ((num2 >> (i * 8)) & 0xFF);
		
		for (int j = 0; j < 7; ++j) 
		{
			carry |= (((generate >> j) & 1) | (((propagate >> j) & 1) & ((carry >> j) & 1))) << (j + 1);
		}

		carryOut |= ((generate >> 7) & 1) | (((propagate >> 7) & 1) & ((carry >> 7) & 1));
        *num1 &= ~(0xFF << (i * 8));
        *num1 |= ((propagate ^ carry) & 0xFF) << (i * 8);
    }

	return (carryOut & 1) ^ ((carry >> 7) & 1);
}

int addOp64(int64_t* num1, int64_t num2) 
{    
	uint8_t carryOut = 0;
	uint8_t carry;
    
    for (int i = 0; i < 8; ++i) 
    {
		uint8_t propagate;
		uint8_t generate;
		carry ^= carry;

		carry |= carryOut & 1;
		carryOut ^= carryOut;

		propagate = ((*num1 >> (i * 8)) & 0xFF) ^ ((num2 >> (i * 8)) & 0xFF);
		generate = ((*num1 >> (i * 8)) & 0xFF) & ((num2 >> (i * 8)) & 0xFF);
		
		for (int j = 0; j < 7; ++j) 
		{
			carry |= (((generate >> j) & 1) | (((propagate >> j) & 1) & ((carry >> j) & 1))) << (j + 1);
		}

		carryOut |= ((generate >> 7) & 1) | (((propagate >> 7) & 1) & ((carry >> 7) & 1));
        *num1 &= ~((uint64_t)0xFF << (i * 8));
        *num1 |= ((uint64_t)propagate ^ carry) << (i * 8);
    }

	return (carryOut & 1) ^ ((carry >> 7) & 1);
}
 
int64_t convertSign64(int64_t num)
{
	num = ~num;
	addOp64(&num, 1);
	return num;
}

int32_t convertSign32(int32_t num)
{
	num = ~num;
	addOp32(&num, 1);
	return num;
}

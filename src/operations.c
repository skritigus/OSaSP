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
	int g = *num1 & num2;     
	int p = *num1 ^= num2;      

	g |= p & (g << 1);
	p &= (p << 1);

	g |= p & (g << 2);
	p &= p << 2;

	g |= p & (g << 4);
	p &= p << 4;

	g |= p & (g << 8);
	p &= p << 8;

	g |= p & g << 16;
		
	*num1 ^= (g << 1);
	return ((g >> 31) & 1) ^ ((g >> 30) & 1);
}

int addOp64(int64_t* num1, int64_t num2) 
{    
	int64_t g = *num1 & num2;     
	int64_t p = *num1 ^= num2;      

	g |= p & (g << 1);
	p &= (p << 1);

	g |= p & (g << 2);
	p &= p << 2;

	g |= p & (g << 4);
	p &= p << 4;

	g |= p & (g << 8);
	p &= p << 8;

	g |= p & g << 16;
	p &= p << 16;
	
	g |= p & g << 32;
		
	*num1 ^= (g << 1);
	return ((g >> 63) & 1) ^ ((g >> 62) & 1);
}
 
int32_t divOp(int64_t dividend, int32_t divisor) 
{    
    int64_t reminder = ((dividend >> 63) & 1) == 0 ? dividend << FRACTIONAL_LEN : convertSign64(dividend)<< FRACTIONAL_LEN;
    int64_t extendedDivisor = ((divisor >> 31) & 1) == 0 ? (int64_t)divisor << 32 : convertSign64((int64_t)divisor) << 32;
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

    addOp32(&quotient, quotient);
    addOp32(&quotient, 1);
    
    if (reminder < 0)
    {
		if(quotient > 0)
		{
			addOp32(&quotient, convertSign32(1));
		}
	}

	if(((dividend >> 63) & 1) ^ ((divisor >> 31) & 1))
	{
		return convertSign32(quotient);
	}
	else
	{
		return quotient;
	}
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

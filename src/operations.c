#include "operations.h"
#include "cpu.h"
#include <stdio.h>
#include <limits.h>
#include <stdint.h>

int32_t floatToFixed(double num) 
{    
    return (int32_t)(num * (1 << FRACTIONAL_LEN)); 
}

float fixedToFloat(int32_t num) 
{	
	return (float)num / (1 << FRACTIONAL_LEN);
}

double fixed64ToFloat(int64_t num) 
{	
	return (double)num / (1 << FRACTIONAL_LEN);
}

void addOp32(int32_t* num1, int32_t num2) 
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
}

void addOp64(int64_t* num1, int64_t num2) 
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
}
 
int divOp(int dividend, int divisor) 
{    
	int negative = 0;
    if ((dividend >> 31) ^ (divisor >> 31)) 
    {
        negative = 1;
	}
	
    unsigned int u_dividend = dividend == INT_MIN ? (unsigned int)INT_MAX + 1 : (unsigned int)(dividend < 0 ? -dividend : dividend);
    unsigned int u_divisor = divisor == INT_MIN ? (unsigned int)INT_MAX + 1 : (unsigned int)(divisor < 0 ? -divisor : divisor);

    unsigned int quotient = 0;
    unsigned int temp = 0;

    for (int i = 31; i >= 0; i--) 
    {
        if ((temp << 1 | ((u_dividend >> i) & 1)) >= u_divisor) 
        {
            temp = (temp << 1 | ((u_dividend >> i) & 1)) - u_divisor;
            quotient |= (1U << i);
        } 
        else 
        {
            temp = (temp << 1) | ((u_dividend >> i) & 1);
        }
    }

    return negative ? -(int)quotient : (int)quotient;
} 
 
void convertSign(int64_t* num)
{
	*num = ~(*num);
	addOp64(num, 1);
}

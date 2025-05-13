#include "instructions.h"
#include <stdio.h>
#include <limits.h>

int floatToFixed(float f) 
{
    return (int)(f * (1 << 7));
}

float fixedToFloat(int f) 
{
    return (float)f / (1 << 7);
}

void addOp(int* num1, int num2) 
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
 
void convertSign(int* num)
{
	*num = ~(*num);
	addOp(num, 1);
}

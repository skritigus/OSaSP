#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <stdint.h>

#define FRACTIONAL_LEN 14

int32_t floatToFixed(double num);
float fixedToFloat(int32_t num);
double fixed64ToFloat(int64_t num);

void addOp32(int32_t* num1, int32_t num2);
void addOp64(int64_t* num1, int64_t num2);
int divOp(int dividend, int divisor); 
void convertSign(int64_t* num);

#endif

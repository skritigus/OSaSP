#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <stdint.h>

#define FRACTIONAL_LEN 14

int32_t floatToFixed(double num);
float fixed32ToFloat(int32_t num);
double fixed64ToFloat(int64_t num);

int addOp32(int32_t* num1, int32_t num2);
int addOp64(int64_t* num1, int64_t num2);
int divOp(int dividend, int divisor); 
int64_t convertSign64(int64_t num);
int32_t convertSign32(int32_t num);

#endif

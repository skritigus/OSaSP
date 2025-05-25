#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <stdint.h>

int floatToFixed(float num);
float fixedToFloat(int num);
double fixed64ToFloat(int64_t num);

void addOp(int* num1, int num2);
void addOp64(int64_t* num1, int64_t num2);
int divOp(int dividend, int divisor); 
void convertSign(int* num);

#endif

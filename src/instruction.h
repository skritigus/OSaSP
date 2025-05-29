#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <stdint.h>
#include "register.h"

#define FRACTIONAL_LEN 11

struct CPU;
typedef struct CPU CPU;

typedef enum 
{
    INVALID_OP = -1,
    HELP,
    STAT,
    END,
    PRINT,
    CLR,
    CHS,
    MOV,
    MOVR,
    ADD,
    SUB,
    MUL,
    DIV
} Operation;

typedef struct 
{
    Operation opcode;
    Register operand1;
    int operand2;
} Instruction;

int32_t floatToFixed(double num);
float fixed32ToFloat(int32_t num);
double fixed64ToFloat(int64_t num);

int addOp32(int32_t* num1, int32_t num2);
int addOp64(int64_t* num1, int64_t num2);
int64_t convertSign64(int64_t num);
int32_t convertSign32(int32_t num);

void handleHelp(CPU* cpu);
void handleClr(CPU* cpu);
void handleMov(CPU* cpu);
void handleMovr(CPU* cpu);
int handleChs(CPU* cpu);
int handleAdd(CPU* cpu);
int handleSub(CPU* cpu);
int handleMul(CPU* cpu);
int handleDiv(CPU* cpu);

void printNum(double num);
void printReg(CPU* cpu);
void printStat(CPU* cpu);

#endif

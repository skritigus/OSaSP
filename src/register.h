#ifndef REGISTER_H
#define REGISTER_H

#include <stdint.h>

struct CPU;
typedef struct CPU CPU;

typedef enum 
{
    INVALID_REG = -1,
    RAX,
    RBX,
    RCX,
    RDX,
    RAH,
    RBH,
    RCH,
    RDH,
    RAL,
    RBL,
    RCL,
    RDL    
} Register;

typedef struct
{
	int32_t low;
	int32_t high;
} RegisterParts;

typedef union 
{
    RegisterParts parts;
    int64_t value;
} Register64;

int32_t* getRegisterValue(CPU* cpu, Register reg);
void setRegisterValue(CPU* cpu, Register reg, int64_t value);

#endif

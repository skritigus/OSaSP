#include "register.h"
#include "cpu.h"
#include <stddef.h>

int32_t* getRegisterValue(CPU* cpu, Register reg) 
{
    switch (reg) 
    {
        case RAX: 
        case RBX: 
        case RCX:
        case RDX:
            return (int32_t*)&cpu->registers[reg].value;
        case RAL: 
        case RBL: 
        case RCL:
        case RDL:
            return &cpu->registers[reg % 4].low;
        case RAH: 
        case RBH: 
        case RCH:
        case RDH:
            return &cpu->registers[reg % 4].high;
        default:
            return NULL;
    }
    
    return NULL;
}

void setRegisterValue(CPU* cpu, Register reg, int64_t value) 
{
    switch (reg) 
    {
        case RAX: 
        case RBX: 
        case RCX:
        case RDX:
            cpu->registers[reg].value = value;
            break;
        case RAL: 
        case RBL: 
        case RCL:
        case RDL:
            cpu->registers[reg % 4].low = (int32_t)value;
            break;
		case RAH: 
        case RBH: 
        case RCH:
        case RDH:
            cpu->registers[reg % 4].high = (int32_t)value;
            break;
        default:
            break;
    }
}

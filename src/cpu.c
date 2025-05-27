#include "cpu.h"
#include "instructions.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>

CPU initCpu()
{
	CPU cpu;
	
	memset(cpu.registers, 0, sizeof(int64_t) * REGISTER_COUNT);
    cpu.lastInstruction = 0;
    cpu.currentInstruction = 0;
    memset(cpu.memory, 0, sizeof(int) * MEMORY_SIZE);
    
    return cpu;
}

void execute(CPU* cpu) 
{
	int op = cpu->memory[cpu->currentInstruction++];

	switch (op) 
	{
		case END:
			return;
		case MOV:
			handleMov(cpu);
			break;
		case MOVR:
			handleMovr(cpu);
			break;
		case CLR:
			handleClr(cpu);
			break;
		case ADD:
			if(handleAdd(cpu) == 1)
			{
				fprintf(stderr, "Overflow detected! The result is bigger than size of register\n");
			}
			break;
		case SUB:
			handleSub(cpu);
			break;
		case MUL:
			if(handleMul(cpu) == 1)
			{
				fprintf(stderr, "Overflow detected! The multiplicand is bigger than size of register\n");
			}
			break;
		case DIV:
			handleDiv(cpu);
			break;
		default:
			fprintf(stderr, "Unknown instruction\n");
			return;
	}
}

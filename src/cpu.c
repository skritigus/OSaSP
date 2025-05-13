#include "cpu.h"
#include "instructions.h"
#include "registers.h"
#include <stdio.h>
#include <string.h>

CPU initCpu()
{
	CPU cpu;
	
	memset(cpu.registers, 0, sizeof(int) * REGISTER_COUNT);
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
		case ADD:
			handleAdd(cpu);
			break;
		case SUB:
			handleSub(cpu);
			break;
		case MUL:
			handleMul(cpu);
			break;
		case DIV:
			handleDiv(cpu);
			break;
		case PRINT:
			handlePrint(cpu);
			break;
		default:
			fprintf(stderr, "Unknown instruction\n");
			return;
	}
}

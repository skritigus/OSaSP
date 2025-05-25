#include <stdio.h>
#include "cpu.h"
#include "file.h"
#include "cli.h"

// TODO:
// 0. 64-bit and 32-bir REGS
// 1. MOV 64-bit
// 2. MOV REG
// 3. PRINT NOT CPU 
// 4. DIV
// 5. OTHER ADD
// 6. OTHER ARIFMETIC

int main(int argc, char* argv[]) 
{
    CPU cpu = initCpu();

    switch (argc)
    {
		case 1:
			while(1)
			{
				parseInstruction(&cpu);
				execute(&cpu);
			}
			break;
		case 2:
			loadFile(&cpu, argv[1]);
			while(1)
			{
				execute(&cpu);
			}
			break;
		default:
			fprintf(stderr, "Wrong execution. Execute program without parameters or with fileName.\n");
			break;
    } 
 
    return 0;
}

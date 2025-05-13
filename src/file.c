#include "file.h"
#include "cli.h"
#include <stdio.h>

void loadFile(CPU* cpu, const char* fileName) 
{
    FILE* file = fopen(fileName, "r");
    if (!file) {
        perror("Failed to open instruction file");
        return;
    }

    char line[MAX_LEN];
    int lineNumber = 0;

    while (fgets(line, sizeof(line), file)) 
    {
        ++lineNumber;

        if (line[0] == '\n' || line[0] == ';')
        {
            continue;
		}

        parseInstruction(cpu);
    }

    fclose(file);
}

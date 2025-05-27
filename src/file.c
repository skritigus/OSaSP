#include "file.h"
#include "cli.h"
#include <stdio.h>

#define MAX_LEN 512

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

void loadHelp() 
{
    FILE* file = fopen("help.txt", "r");
    if (!file) 
    {
        perror("Failed to open \"help.txt\"");
        return;
    }

    char line[MAX_LEN];

    while (fgets(line, sizeof(line), file)) 
    {
        fprintf(stdout, "%s", line);
    }

    fclose(file);
}

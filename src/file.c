#include "file.h"
#include "cli.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_LEN 512

void loadFile(CPU* cpu, const char* fileName) 
{
    FILE* file = fopen(fileName, "r");
    if (!file) 
    {
        perror("Failed to open instruction file");
        exit(EXIT_FAILURE);
    }

	int result;
	int lineNumber = 1;
	
    do
    {
		result = parseInstruction(cpu, file, lineNumber);		
		++lineNumber;
	} while (result == 0 || result == 2);

    fclose(file);
    
    if(result != 1)
    {
		if(result == -2)
		{
			fprintf(stderr, "Error. Missing \"END\" instruction in the end of file\n");	
		}
		exit(EXIT_FAILURE);
	}
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

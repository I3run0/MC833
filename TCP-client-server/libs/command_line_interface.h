#include <string.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct command
{
    char* opcode;
    char* func;
} Command;

Command* command(int argc, char *argv[]);




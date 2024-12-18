#pragma once

typedef struct ConsoleArguments
{
    int time; 
    char* sharedMemoryName;
} ConsoleArguments;

ConsoleArguments readConsole(int argumentsCount, char* arguments[]);
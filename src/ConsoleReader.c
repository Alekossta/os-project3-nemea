#include "ConsoleReader.h"
#include "string.h"
#include "stdlib.h"

ConsoleArguments readConsole(int argumentsCount, char* arguments[])
{
    ConsoleArguments consoleArguments;

    for(int i = 1; i < argumentsCount; i++)
    {
        if(strcmp(arguments[i], "-d") == 0 && i + 1 < argumentsCount)
        {
            i++;
            consoleArguments.time = atoi(arguments[i]);
        }
        if(strcmp(arguments[i], "-s") == 0 && i + 1 < argumentsCount)
        {
            i++;
            consoleArguments.sharedMemoryName = arguments[i];
        }
    }

    return consoleArguments;
}
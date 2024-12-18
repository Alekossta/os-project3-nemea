#include "stdio.h"
#include "ConsoleReader.h"

int main(int argumentsCount, char* arguments[])
{
    ConsoleArguments consoleArguments;
    consoleArguments = readConsole(argumentsCount, arguments);
    printf("Hello from visitor going to visit for %d and shared memory is %s!\n",
    consoleArguments.time, consoleArguments.sharedMemoryName);
}
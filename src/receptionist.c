#include "stdio.h"
#include "ConsoleReader.h"

int main(int argumentsCount, char* arguments[])
{
    ConsoleArguments consoleArguments;
    consoleArguments = readConsole(argumentsCount, arguments);
    printf("Hello from receptionist going to serve for %d and shared memory is %d!\n",
    consoleArguments.time, consoleArguments.shareMemoryId);
}
#include "stdio.h"
#include "ConsoleReader.h"

int main(int argumentsCount, char* arguments[])
{
    ConsoleArguments consoleArguments;
    consoleArguments = readConsole(argumentsCount, arguments);
    printf("Hello from monitor looking in shared memory id: %d\n", consoleArguments.shareMemoryId);
}
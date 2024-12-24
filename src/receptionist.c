#include "stdio.h"
#include "ConsoleReader.h"
#include <asm-generic/fcntl.h>
#include <SharedMemorySegment.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <time.h>

int main(int argumentsCount, char* arguments[])
{
    srand(time(NULL));

    ConsoleArguments consoleArguments;
    consoleArguments = readConsole(argumentsCount, arguments);
    printf("Hello from receptionist going to serve for %d and shared memory is %s!\n",
    consoleArguments.time, consoleArguments.sharedMemoryName);

    // open shared memory
    int smFd = shm_open(consoleArguments.sharedMemoryName, O_RDWR, 0666);
    if(smFd == -1)
    {
        perror("shm open failed");
        return -1;
    }

    SharedMemorySegment* ptr = mmap(NULL, SHARED_MEMORY_SIZE, PROT_WRITE, MAP_SHARED, smFd, 0);
    if(ptr == MAP_FAILED)
    {
        perror("mmap failed");
        return -1;
    }

    // dont have to have a mutex here cause only one receptionist will access it
    ptr->receptionistInfo.timeToServe = consoleArguments.time;
}
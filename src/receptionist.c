#include "stdio.h"
#include "ConsoleReader.h"
#include <asm-generic/fcntl.h>
#include <SharedMemorySegment.h>
#include <sys/mman.h>

int main(int argumentsCount, char* arguments[])
{
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

    pthread_mutex_lock(&(ptr->writeLock));
    ptr->receptionistInfo.timeToServe = consoleArguments.time;
    pthread_mutex_unlock(&(ptr->writeLock));
}
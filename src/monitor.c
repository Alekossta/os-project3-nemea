#include "stdio.h"
#include "ConsoleReader.h"
#include <sys/mman.h>
#include <sys/stat.h> 
#include <fcntl.h>    
#include <unistd.h>
#include "definitions.h"
#include "SharedMemorySegment.h"
#include "BarInfo.h"
#include <stdlib.h>
#include <time.h>

int main(int argumentsCount, char* arguments[])
{
    srand(time(NULL));

    ConsoleArguments consoleArguments;
    consoleArguments = readConsole(argumentsCount, arguments);
    printf("Hello from monitor looking in shared memory id: %s\n", consoleArguments.sharedMemoryName);

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

    printBarInfo(ptr->barInfo);
    printSeatsInfo(ptr->seatsInfo);

    // clean up 
    munmap(ptr, SHARED_MEMORY_SIZE);
    close(smFd);
}
#include "stdio.h"
#include "ConsoleReader.h"
#include <sys/mman.h>
#include <sys/stat.h> 
#include <fcntl.h>    
#include <unistd.h>
#include "definitions.h"
#include "SharedMemorySegment.h"
#include "BarInfo.h"
#include <semaphore.h>

int main(int argumentsCount, char* arguments[])
{
    ConsoleArguments consoleArguments;
    consoleArguments = readConsole(argumentsCount, arguments);

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

    int tableToSit = lookForTable(ptr->barInfo);
    printf("going to sit at table %d\n", tableToSit);
    if(tableToSit != -1) // means we found available table
    {
        sem_t* barSemaphore = sem_open("/bar_sem", O_RDWR);
        sem_wait(barSemaphore);
        printf("began serving me");
        // choose random food
        // simulate serving
        sleep(ptr->receptionistInfo.timeToServe);
        sem_post(barSemaphore);
    }

    // clean up shared memory
    munmap(ptr, SHARED_MEMORY_SIZE);
    close(smFd);
}
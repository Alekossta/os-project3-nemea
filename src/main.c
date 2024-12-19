#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "ConsoleReader.h"
#include <stdbool.h>
#include "definitions.h"
#include "SharedMemorySegment.h"
#include "signal.h"

volatile bool bRunning = true;

void handleSignal_CtrlC(int signal)
{
    if(signal == SIGINT)
    {
        bRunning = false;
    }
}

int main(int argumentsCount, char* arguments[])
{
    // listen for Ctrl+C signal
    signal(SIGINT, handleSignal_CtrlC);
    
    // create shared memory
    int smFd = shm_open(SHARED_MEMORY_NAME, O_CREAT | O_RDWR, 0666);
    if(smFd == -1)
    {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    // set the size of the shared memory
    int res = ftruncate(smFd,SHARED_MEMORY_SIZE);
    if(res == -1)
    {
        perror("ftruncate");
        exit(EXIT_FAILURE);
    }
    
    // map the shared memory to a local pointer
    SharedMemorySegment* ptr = mmap(NULL, SHARED_MEMORY_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, smFd, 0);
    if(ptr == MAP_FAILED)
    {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    // create and print the bar info
    ptr->barInfo = createBarInfo();
    printBarInfo(ptr->barInfo);

    printf("Shared memory created. Press Ctrl+C to clean up.\n");

    while(bRunning)
    {
        sleep(5);
    }

    munmap(ptr, SHARED_MEMORY_SIZE);
    close(smFd);
    shm_unlink(SHARED_MEMORY_NAME);
}
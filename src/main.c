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
#include "definitions.h"
#include <semaphore.h>

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
    printf("Press Ctrl+C to stop running main\n");

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

    // create a semaphore for each table
    sem_t* tableSemaphores[TABLES_NUM];
    const char* tableSemaphoreNames[TABLES_NUM] = {"/table0_sem", "/table1_sem", "/table2_sem"};
    for(int i = 0; i < TABLES_NUM; i++)
    {
        tableSemaphores[i] = sem_open(tableSemaphoreNames[i], O_CREAT | O_EXCL, 0644, CHAIRS_NUM);
        if(tableSemaphores[i] == SEM_FAILED)
        {
            perror("sem_open");
            // Cleanup previously created semaphores
            for (int j = 0; j < i; j++) {
                sem_unlink(tableSemaphores[j]);
            }

            // in case of error close shared memory too
            munmap(ptr, SHARED_MEMORY_SIZE);
            close(smFd);
            shm_unlink(SHARED_MEMORY_NAME);

            exit(EXIT_FAILURE);
        }
    }

    // run main until we press ctrl+c
    while(bRunning)
    {
        sleep(5);
    }

    // clean up shared memory
    munmap(ptr, SHARED_MEMORY_SIZE);
    close(smFd);
    shm_unlink(SHARED_MEMORY_NAME);

    // clean up semaphores
    for(int i = 0; i < TABLES_NUM; i++)
    {
        sem_close(tableSemaphores[i]);
        sem_unlink(tableSemaphoreNames[i]);
    }
}
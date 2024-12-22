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
#include <sys/wait.h>

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
    
    // setup mutex for writing lock
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&(ptr->writeLock), &attr);
    pthread_mutexattr_destroy(&attr);

    pthread_mutex_lock(&(ptr->writeLock));
    ptr->barInfo = createBarInfo();
    pthread_mutex_unlock(&(ptr->writeLock));

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
                sem_unlink(tableSemaphoreNames[j]);
            }

            // in case of error close shared memory too
            munmap(ptr, SHARED_MEMORY_SIZE);
            close(smFd);
            shm_unlink(SHARED_MEMORY_NAME);

            exit(EXIT_FAILURE);
        }
    }

    // create a semaphore for the bar queue
    sem_t* barSemaphore = sem_open("/bar_sem", O_CREAT | O_EXCL, 0644, 1);
    if(barSemaphore == SEM_FAILED)
    {
        perror("sem_open");
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
    
    // create receptionist process
    pid_t pid = fork();
    if(pid == 0)
    {
        char* args[] = {"./receptionist", "-d", "10", "-s", SHARED_MEMORY_NAME, NULL};
        if(execv(args[0], args) == - 1)
        {
            perror("execv failed");
            exit(EXIT_FAILURE);
        }
    }

    // create visitor processes
    const int testingVisitorsCount = 5;
    int pids[testingVisitorsCount];
    for(int i = 0; i < testingVisitorsCount; i++)
    {
        pids[i] = fork();
        if(pids[i] == 0)
        {
            char* args[] = {"./visitor", "-d", "10", "-s", SHARED_MEMORY_NAME, NULL};
            if(execv(args[0], args) == - 1)
            {
                perror("execv failed");
                exit(EXIT_FAILURE);
            }
        }
    }

    // run main until we press ctrl+c
    while(bRunning)
    {
        sleep(5);
    }

    waitpid(pid, NULL, 0); // Wait for the receptionist process to complete

    for(int i = 0; i < testingVisitorsCount; i++)
    {
        waitpid(pids[i], NULL, 0);
    }

    // clean up shared memory
    pthread_mutex_destroy(&(ptr->writeLock));
    munmap(ptr, SHARED_MEMORY_SIZE);
    close(smFd);
    shm_unlink(SHARED_MEMORY_NAME);

    // clean up table semaphores
    for(int i = 0; i < TABLES_NUM; i++)
    {
        sem_close(tableSemaphores[i]);
        sem_unlink(tableSemaphoreNames[i]);
    }

    // clean up bar semaphore
    sem_close(barSemaphore);
    sem_unlink("/bar_sem");
}
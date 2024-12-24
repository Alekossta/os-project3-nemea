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
#include "Util.h"
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argumentsCount, char* arguments[])
{
    srand(time(NULL) ^ getpid()); // Combine time with process ID for a unique seed

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

    pthread_mutex_lock(&(ptr->seatsLock));

    int tableToSit = lookForTable(ptr->seatsInfo);
    while(tableToSit == -1)
    {
        tableToSit = lookForTable(ptr->seatsInfo);
    }
    
    int chairToSit = takeChair(&(ptr->seatsInfo), tableToSit, getpid());
    pthread_mutex_unlock(&(ptr->seatsLock));

    sem_t* barSemaphore = sem_open("/bar_sem", O_RDWR);
    sem_wait(barSemaphore);

    pthread_mutex_lock(&(ptr->barLock));

    // 0=water,1=wine,2=water&wine
    int waterWineChoice = generateIntInRange(0,2);
    if(waterWineChoice == 0)
    {
        ptr->barInfo.waterSold++;
    }
    else if (waterWineChoice == 1)
    {
        ptr->barInfo.wineSold++;
    }
    else if(waterWineChoice == 2)
    {
        ptr->barInfo.waterSold++;
        ptr->barInfo.wineSold++;
    }

    int cheeseChoice = generateIntInRange(0,1);
    if(cheeseChoice == 1)
    {
        ptr->barInfo.cheeseSold++;
    }

    int saladChoice = generateIntInRange(0,1);
    if(saladChoice == 1)
    {
        ptr->barInfo.saladSold++;
    }

    pthread_mutex_unlock(&(ptr->barLock));

    // simulate serving
    double timeToServe = ptr->receptionistInfo.timeToServe;
    double randomTimeToServe = generateDoubleInRange(0.5 * timeToServe, timeToServe);
    printf("Began serving me for %lf\n", randomTimeToServe);
    usleep((useconds_t)(randomTimeToServe * 1e6));
    sem_post(barSemaphore);

    // go to table and sit
    printf("going to sit at table %d chair %d\n", tableToSit, chairToSit);
    double timeToSit = consoleArguments.time;
    double randomTimeToSit = generateDoubleInRange(0.7 * timeToSit, timeToSit);
    usleep((useconds_t)(randomTimeToSit * 1e6));

    // leave table

    // clean up shared memory
    munmap(ptr, SHARED_MEMORY_SIZE);
    close(smFd);
}
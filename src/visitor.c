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
#include "Logger.h"

double getElapsedSeconds(struct timespec* start, struct timespec* end)
{
    return (end->tv_sec - start->tv_sec) + (end->tv_nsec - start->tv_nsec) / 1e9;
}


int main(int argumentsCount, char* arguments[])
{
    srand(time(NULL) ^ getpid()); // Combine time with process ID for a unique seed

    ConsoleArguments consoleArguments;
    consoleArguments = readConsole(argumentsCount, arguments);

    LoggerInfo loggerInfo = openLoggingFile();

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


    struct timespec arrivedTime;
    clock_gettime(CLOCK_MONOTONIC, &arrivedTime);

    char message[256];
    snprintf(message, sizeof(message), "%d arrived at bar", getpid());
    writeToLoggingFile(loggerInfo, message);

    pthread_mutex_lock(&(ptr->seatsLock));

    snprintf(message, sizeof(message), "%d is now in front of the queue and waits", getpid());
    writeToLoggingFile(loggerInfo, message);

    int tableToSit = lookForTable(ptr->seatsInfo);
    while(tableToSit == -1)
    {
        tableToSit = lookForTable(ptr->seatsInfo);
    }



    struct timespec waitEndTime;
    clock_gettime(CLOCK_MONOTONIC, &waitEndTime);

    double timeWaited = getElapsedSeconds(&arrivedTime, &waitEndTime);
    snprintf(message, sizeof(message), "%d found a chair to sit. waited %f", getpid(), timeWaited);
    writeToLoggingFile(loggerInfo, message);
    int chairToSit = takeChair(&(ptr->seatsInfo), tableToSit, getpid(), timeWaited);


    pthread_mutex_unlock(&(ptr->seatsLock));

    sem_t* barSemaphore = sem_open("/bar_sem", O_RDWR);
    
    snprintf(message, sizeof(message), "%d waits to be served", getpid());
    writeToLoggingFile(loggerInfo, message);

    sem_wait(barSemaphore);

    snprintf(message, sizeof(message), "%d starts to be served", getpid());
    writeToLoggingFile(loggerInfo, message);

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

    // simulate serving
    double timeToServe = ptr->receptionistInfo.timeToServe;
    double randomTimeToServe = generateDoubleInRange(0.5 * timeToServe, timeToServe);
    usleep((useconds_t)(randomTimeToServe * 1e6));

    snprintf(message, sizeof(message), "%d finished being served and goes to sit", getpid());
    writeToLoggingFile(loggerInfo, message);

    sem_post(barSemaphore);

    // go to table and sit
    double timeToSit = consoleArguments.time;
    double randomTimeToSit = generateDoubleInRange(0.7 * timeToSit, timeToSit);
    usleep((useconds_t)(randomTimeToSit * 1e6));

    struct timespec visitEndTime;
    clock_gettime(CLOCK_MONOTONIC, &visitEndTime);
    double timeStayed = getElapsedSeconds(&arrivedTime, &visitEndTime);
    // leave table
    leaveChair(&(ptr->seatsInfo), tableToSit, chairToSit, timeStayed);

    snprintf(message, sizeof(message), "%d left. time stayed: %f", getpid(), timeStayed);
    writeToLoggingFile(loggerInfo, message);

    // clean up shared memory
    munmap(ptr, SHARED_MEMORY_SIZE);
    close(smFd);
    closeLogginFile(loggerInfo);

    // clean up semaphore
    sem_close(barSemaphore);
}
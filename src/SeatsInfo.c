#include "SeatsInfo.h"
#include "stdio.h"
#include <fcntl.h>

SeatsInfo createSeatsInfo()
{
    SeatsInfo seatsInfo;
    for(int i = 0; i < TABLES_NUM; i++)
    {
        seatsInfo.allowsEntry[i] = 1;
        for(int j = 0; j < CHAIRS_NUM; j++)
        {
            seatsInfo.seats[i][j] = 0;
        }
    }
    return seatsInfo;
}

void printSeatsInfo(SeatsInfo seatsInfo)
{
    for(int i = 0; i < TABLES_NUM; i++)
    {
        printf("---Table %d- Allows Entry: %d---\n", i, seatsInfo.allowsEntry[i]);
        for(int j = 0; j < CHAIRS_NUM; j++)
        {
            printf("Chair %d: %d\n", j, seatsInfo.seats[i][j]);
        }
    }
}

int lookForTable(SeatsInfo seatsInfo)
{
    // in case we dont find a table return -1
    int tableId = -1;

    for(int i = 0; i < TABLES_NUM; i++)
    {
        if(seatsInfo.allowsEntry[i])
        {
            tableId = i;
            break;
        }
    }

    return tableId;
}

int takeChair(SeatsInfo* seatsInfo, int table, pid_t visitor)
{
   if (seatsInfo->allowsEntry[table] == 0) return -1;

    int chairToTake = -1;
    int chairsFilled = 0;
    for(int i = 0; i < CHAIRS_NUM; i++)
    {
        if (seatsInfo->seats[table][i] == 0 && chairToTake == -1)
        {
            chairToTake = i;
            seatsInfo->seats[table][i] = visitor;
        }
        else if (seatsInfo->seats[table][i] != 0)
        {
            chairsFilled++;
        }
    }

    // was 3 and now 4
    if(chairToTake != -1 && chairsFilled == 3)
    {
        seatsInfo->allowsEntry[table] = 0;
    }

    return chairToTake;
}

void leaveChair(SeatsInfo* seatsInfo, int table, int chair)
{
    seatsInfo->seats[table][chair] = -1;

    int leftCounter = 0;
    for(int i = 0; i < CHAIRS_NUM; i++)
    {
        if(seatsInfo->seats[table][i] == -1)
        {
            leftCounter++;
        }
    }

    // if everyone has left reset to 0
    if(leftCounter == CHAIRS_NUM)
    {   
        for(int i = 0; i < CHAIRS_NUM; i++)
        {
            seatsInfo->seats[table][i] = 0;
        }

        seatsInfo->allowsEntry[table] = 1;
    }
}
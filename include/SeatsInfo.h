#pragma once

#include "definitions.h"
#include <unistd.h>

typedef struct {
    pid_t seats[TABLES_NUM][CHAIRS_NUM];
    int allowsEntry[TABLES_NUM];
    unsigned totalVisitorsVisited;
    unsigned currentVisitors;
    double timeVisitedSum;
    double timeWaitedSum;
} SeatsInfo;

SeatsInfo createSeatsInfo();
void printSeatsInfo();
int lookForTable(SeatsInfo seatsInfo);
int takeChair(SeatsInfo* seatsInfo, int table, pid_t visitor, double timeWaited);
void leaveChair(SeatsInfo* seatsInfo, int table, int chair, double timeStayed);
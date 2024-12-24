#pragma once

#include "definitions.h"
#include <unistd.h>

typedef struct {
    pid_t seats[TABLES_NUM][CHAIRS_NUM];
    int allowsEntry[TABLES_NUM];
} SeatsInfo;

SeatsInfo createSeatsInfo();
void printSeatsInfo();
int lookForTable(SeatsInfo seatsInfo);
int takeChair(SeatsInfo* seatsInfo, int table, pid_t visitor);
void leaveChair(SeatsInfo* seatsInfo, pid_t visitor);
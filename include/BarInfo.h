#pragma once

#include "definitions.h"

typedef struct
{
    unsigned totalVisitors;
    int seats[TABLES_NUM][CHAIRS_NUM];
} BarInfo;

BarInfo createBarInfo();
void printBarInfo(BarInfo barInfo);
#pragma once

#include "definitions.h"

typedef struct
{
    unsigned totalVisitors;
    unsigned waterSold;
    unsigned wineSold;
    unsigned cheeseSold;
    unsigned saladSold;
    int seats[TABLES_NUM][CHAIRS_NUM];
} BarInfo;

BarInfo createBarInfo();
void printBarInfo(BarInfo barInfo);
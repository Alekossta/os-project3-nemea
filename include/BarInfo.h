#pragma once

#include "definitions.h"
#include <unistd.h>

typedef struct
{
    unsigned waterSold;
    unsigned wineSold;
    unsigned cheeseSold;
    unsigned saladSold;
} BarInfo;

BarInfo createBarInfo();
void printBarInfo(BarInfo barInfo);
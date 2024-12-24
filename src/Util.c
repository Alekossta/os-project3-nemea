#include "Util.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int generateIntInRange(int min, int max)
{
    return rand() % (max - min + 1) + min;
}

double generateDoubleInRange(double min, int max)
{
    double range = max - min;
    return min + ((double)rand() / RAND_MAX) * range;    
}
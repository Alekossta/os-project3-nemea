#include "BarInfo.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

BarInfo createBarInfo()
{
    BarInfo barInfo;
    barInfo.waterSold = 0;
    barInfo.wineSold = 0;
    barInfo.cheeseSold = 0;
    barInfo.saladSold = 0;
    return barInfo;
}

void printBarInfo(BarInfo barInfo)
{
    printf("Waters sold %u\n", barInfo.waterSold);
    printf("Wine sold %u\n", barInfo.wineSold);
    printf("Cheese sold %u\n", barInfo.cheeseSold);
    printf("Salad sold %u\n", barInfo.saladSold);
}
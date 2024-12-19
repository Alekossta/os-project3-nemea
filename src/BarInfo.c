#include "BarInfo.h"
#include <stdio.h>

BarInfo createBarInfo()
{
    BarInfo barInfo;
    barInfo.totalVisitors = 0;
    barInfo.waterSold = 0;
    barInfo.wineSold = 0;
    barInfo.cheeseSold = 0;
    barInfo.saladSold = 0;
    for(int i = 0; i < TABLES_NUM; i++)
    {
        for(int j = 0; j < CHAIRS_NUM; j++)
        {
            barInfo.seats[i][j] = 5;
        }
    }
    return barInfo;
}

void printBarInfo(BarInfo barInfo)
{
    printf("Total visitors are %u\n", barInfo.totalVisitors);
    printf("Waters sold %u\n", barInfo.waterSold);
    printf("Wine sold %u\n", barInfo.wineSold);
    printf("Cheese sold %u\n", barInfo.cheeseSold);
    printf("Salad sold %u\n", barInfo.saladSold);
    for(int i = 0; i < TABLES_NUM; i++)
    {
        printf("---Table %d---\n", i+1);
        for(int j = 0; j < CHAIRS_NUM; j++)
        {
            printf("Chair %d: %d\n", j+1, barInfo.seats[i][j]);
        }
    }
}

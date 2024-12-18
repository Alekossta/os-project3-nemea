#include "BarInfo.h"
#include <stdio.h>

BarInfo createBarInfo()
{
    BarInfo barInfo;
    barInfo.totalVisitors = 0;
    for(int i = 0; i < TABLES_NUM; i++)
    {
        for(int j = 0; j < CHAIRS_NUM; j++)
        {
            barInfo.seats[i][j] = 0;
        }
    }
    return barInfo;
}

void printBarInfo(BarInfo barInfo)
{
    printf("Total visitors are %d\n", barInfo.totalVisitors);
    for(int i = 0; i < TABLES_NUM; i++)
    {
        printf("---Table %d---\n", i+1);
        for(int j = 0; j < CHAIRS_NUM; j++)
        {
            printf("Chair %d: %d\n", j+1, barInfo.seats[i][j]);
        }
    }
}

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
        barInfo.allowsEntry[i] = 1;
        for(int j = 0; j < CHAIRS_NUM; j++)
        {
            barInfo.seats[i][j] = 0;
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
        printf("---Table %d- Allows Entry: %d---\n", i, barInfo.allowsEntry[i]);
        for(int j = 0; j < CHAIRS_NUM; j++)
        {
            printf("Chair %d: %d\n", j, barInfo.seats[i][j]);
        }
    }
}

int lookForTable(BarInfo barInfo)
{
    // in case we dont find a table return -1
    int tableId = -1;

    for(int i = 0; i < TABLES_NUM; i++)
    {
        if(barInfo.allowsEntry[i])
        {
            tableId = i;
            break;
        }
    }

    return tableId;
}
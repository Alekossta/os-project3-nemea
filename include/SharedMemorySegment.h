#pragma once

#include "BarInfo.h"
#include "ReceptionistInfo.h"
#include <pthread.h>

typedef struct
{
    pthread_mutex_t writeLock;
    BarInfo barInfo;
    ReceptionistInfo receptionistInfo;
} SharedMemorySegment;
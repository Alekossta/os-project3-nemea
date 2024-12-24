#pragma once

#include "BarInfo.h"
#include "SeatsInfo.h"
#include "ReceptionistInfo.h"
#include <pthread.h>

typedef struct
{
    pthread_mutex_t seatsLock;
    pthread_mutex_t barLock;
    SeatsInfo seatsInfo;
    BarInfo barInfo;
    ReceptionistInfo receptionistInfo;
} SharedMemorySegment;
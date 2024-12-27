#pragma once

#include "BarInfo.h"
#include "SeatsInfo.h"
#include "ReceptionistInfo.h"
#include <pthread.h>

typedef struct
{
    pthread_mutex_t seatsLock;
    SeatsInfo seatsInfo;
    BarInfo barInfo;
    ReceptionistInfo receptionistInfo;
} SharedMemorySegment;
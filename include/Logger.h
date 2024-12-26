#pragma once
#include "stdio.h"
#define LOGGING_FILE_DESTINATION "./logs/loggingFile.txt"

typedef struct 
{
    FILE* logginFile;
} LoggerInfo;

LoggerInfo openLoggingFile();
void writeToLoggingFile();
void closeLogginFile(LoggerInfo loggerInfo);
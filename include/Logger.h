#pragma once
#include "stdio.h"
#define LOGGING_FILE_DESTINATION "./logs/loggingFile.txt"

typedef struct 
{
    int loggingFileFd;
} LoggerInfo;


LoggerInfo openLoggingFile();
void resetFile();
void writeToLoggingFile();
void closeLogginFile(LoggerInfo loggerInfo);
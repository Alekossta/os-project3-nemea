#include "Logger.h"

LoggerInfo openLoggingFile()
{
    LoggerInfo loggerInfo;
    fopen(LOGGING_FILE_DESTINATION, "a");
}
void writeToLoggingFile();
void closeLogginFile(LoggerInfo loggerInfo);
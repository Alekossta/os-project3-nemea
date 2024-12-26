#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/file.h>
#include <time.h>
#include "Logger.h"

void resetFile()
{
    // Open the file in write mode to truncate (clear) it
    int tempFd = open(LOGGING_FILE_DESTINATION, O_WRONLY | O_TRUNC | O_CREAT, 0644);
    if (tempFd < 0) {
        perror("open");
        exit(EXIT_FAILURE);
    }
    close(tempFd); // Close it after clearing
}

LoggerInfo openLoggingFile()
{
    LoggerInfo loggerInfo;

    // we delete the file once in the main file
    
    loggerInfo.loggingFileFd = open(LOGGING_FILE_DESTINATION, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (loggerInfo.loggingFileFd < 0) {
        perror("open");
        exit(EXIT_FAILURE);
    }
    return loggerInfo;
}

// helper function
void get_timestamp(char *buffer, size_t size) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(buffer, size, "%Y-%m-%d %H:%M:%S", t);
}

void writeToLoggingFile(LoggerInfo loggerInfo, const char* message)
{
    // Lock the file
    if (flock(loggerInfo.loggingFileFd, LOCK_EX) < 0) {
        perror("flock");
        close(loggerInfo.loggingFileFd);
        exit(EXIT_FAILURE);
    }

    // Prepare the log message with timestamp
    char timestamp[20];
    get_timestamp(timestamp, sizeof(timestamp));

    char log_entry[512];
    snprintf(log_entry, sizeof(log_entry), "[%s] %s\n", timestamp, message);

    // Write the log message
    if (write(loggerInfo.loggingFileFd, log_entry, strlen(log_entry)) < 0) {
        perror("write");
        close(loggerInfo.loggingFileFd);
        exit(EXIT_FAILURE);
    }

    // Unlock the file
    if (flock(loggerInfo.loggingFileFd, LOCK_UN) < 0) {
        perror("flock");
    }
}
void closeLogginFile(LoggerInfo loggerInfo)
{
    close(loggerInfo.loggingFileFd);
}
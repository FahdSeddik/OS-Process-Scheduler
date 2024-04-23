#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/**
 * Logger structure to hold log and performance data
 */
typedef struct Logger {
    FILE *logFile;
    FILE *perfFile;
    double totalRuntime;
    double totalWaitingTime;
    double totalTurnaroundTime;
    double totalWeightedTurnaroundTime;
    int processCount;
} Logger;

/**
 * Initializes the logger
 * @param logPath Path to the log file
 * @param perfPath Path to the performance file
 * @return Logger* Pointer to initialized Logger structure
 */
Logger* loggerInit(const char* logPath, const char* perfPath);

/**
 * Logs a process event to the scheduler log file
 * @param logger Pointer to the Logger structure
 * @param time Current simulation time
 * @param pid Process ID
 * @param state Description of the process state ("started", "stopped", "resumed", "finished")
 * @param arrival Time the process arrived
 * @param total Total runtime of the process
 * @param remain Remaining runtime of the process
 * @param wait Total wait time of the process
 * @param ta Turnaround time of the process (only for "finished")
 * @param wta Weighted turnaround time of the process (only for "finished")
 */
void loggerLogProcessEvent(Logger* logger, int time, int pid, const char* state, int arrival, int total, int remain, int wait, int ta, double wta);

/**
 * Updates performance metrics in the logger
 * @param logger Pointer to the Logger structure
 * @param runtime Runtime to add
 * @param wait Wait time to add
 * @param ta Turnaround time to add
 * @param wta Weighted Turnaround Time to add
 */
void loggerUpdatePerformanceMetrics(Logger* logger, double runtime, double wait, double ta, double wta);

/**
 * Writes the performance data to the performance file
 * @param logger Pointer to the Logger structure
 */
void loggerWritePerformanceData(Logger* logger);

/**
 * Closes the log and performance files and frees the Logger structure
 * @param logger Pointer to the Logger structure to be freed
 */
void loggerDestroy(Logger* logger);

#endif // LOGGER_H

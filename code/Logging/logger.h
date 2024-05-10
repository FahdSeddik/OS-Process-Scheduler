#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// TODO: logger needs to be adjusted to calculate standard deviation (need an array to store all values for wta)


/**
 * Logger structure to hold log and performance data
 */
typedef struct Logger {
    FILE *logFile;
    FILE *perfFile;
    FILE * memFile;
    double totalRuntime;
    double totalWaitingTime;
    double cpuWaitingTime;
    double totalTurnaroundTime;
    double totalWeightedTurnaroundTime;
    int processCount;
    int size;
    double* WTAs;
} Logger;

/**
 * Initializes the logger
 * @param logPath Path to the log file
 * @param perfPath Path to the performance file
 * @param memPath Path to the memory log file
 * @return Logger* Pointer to initialized Logger structure
 */
Logger* loggerInit(const char* logPath, const char* perfPath, const char* memPath);

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
void loggerLogEvent(Logger* logger, int time, int pid, const char* state, int arrival, int total, int remain, int wait, int ta, double wta);

/**
 * Updates logger cpu waiting time
 * @param logger  Pointer to the logger structure
 * @param waitTime wait time to add for the CPU
 */
void loggerCPUWait(Logger* logger, int waitTime);

/**
 * Updates performance metrics in the logger should be called when process stops
 * @param logger Pointer to the Logger structure
 * @param runtime Runtime to add
 * @param wait Wait time to add
 * @param ta Turnaround time to add
 * @param wta Weighted Turnaround Time to add
 */
void loggerUpdateMetrics(Logger* logger, double runtime, double wait, double ta, double wta);

/**
 * Writes the performance data to the performance file
 * @param logger Pointer to the Logger structure
 */
void loggerWritePerformanceData(Logger* logger);

/**
 * Logs a memory event to the logger.
 *
 * @param logger The logger object.
 * @param time The time of the memory event.
 * @param pid The process ID associated with the memory event.
 * @param state The state of the memory event.
 * @param memsize The size of the memory event.
 * @param addressStart The starting address of the memory event.
 * @param addressEnd The ending address of the memory event.
 */
void loggerLogMemoryEvent(Logger* logger, int time, int pid, const char* state, int memsize, int addressStart, int addressEnd);

/**
 * Closes the log and performance files and frees the Logger structure
 * @param logger Pointer to the Logger structure to be freed
 */
void loggerDestroy(Logger* logger);

#endif // LOGGER_H

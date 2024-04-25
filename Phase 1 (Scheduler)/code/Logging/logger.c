#include "logger.h"
#include <math.h>

Logger* loggerInit(const char* logPath, const char* perfPath) {
    Logger* logger = malloc(sizeof(Logger));
    if (!logger) {
        perror("Failed to allocate logger");
        return NULL;
    }

    logger->logFile = fopen(logPath, "w");
    logger->perfFile = fopen(perfPath, "w");
    if (!logger->logFile || !logger->perfFile) {
        perror("Failed to open file");
        free(logger);
        return NULL;
    }

    fprintf(logger->logFile, "#At time x process y state arr w total z remain y wait k\n");
    logger->totalRuntime = 0;
    logger->totalWaitingTime = 0;
    logger->cpuWaitingTime = 0;
    logger->totalTurnaroundTime = 0;
    logger->totalWeightedTurnaroundTime = 0;
    logger->processCount = 0;
    logger->size = 1;
    logger->WTAs = (double *)malloc(sizeof(double));
    return logger;
}

void loggerLogEvent(Logger* logger, int time, int pid, const char* state, int arrival, int total, int remain, int wait, int ta, double wta) {
    if (strcmp(state, "finished") == 0) {
        fprintf(logger->logFile, "At time %d process %d %s arr %d total %d remain %d wait %d TA %d WTA %.2f\n",
                time, pid, state, arrival, total, remain, wait, ta, wta);
    } else {
        fprintf(logger->logFile, "At time %d process %d %s arr %d total %d remain %d wait %d\n",
                time, pid, state, arrival, total, remain, wait);
    }
}

void loggerCPUWait(Logger* logger, int waitTime) {
    logger->cpuWaitingTime += waitTime;
}

void loggerUpdateMetrics(Logger* logger, double runtime, double wait, double ta, double wta) {
    logger->totalRuntime += runtime;
    logger->totalWaitingTime += wait;
    logger->totalTurnaroundTime += ta;
    logger->totalWeightedTurnaroundTime += wta;
    if(logger->processCount >= logger->size) {
        logger->size *= 2;
        logger->WTAs = realloc(logger->WTAs, logger->size * sizeof(double));
    }
    logger->WTAs[logger->processCount] = wta;
    logger->processCount++;
}

void loggerWritePerformanceData(Logger* logger) {
    double cpu_utilization = (logger->totalRuntime / (logger->totalRuntime + logger->cpuWaitingTime)) * 100;
    double avg_waiting_time = logger->totalWaitingTime / logger->processCount;
    double avg_turnaround_time = logger->totalTurnaroundTime / logger->processCount;
    double avg_wta = logger->totalWeightedTurnaroundTime / logger->processCount;
    double variance = 0;
    for(int i = 0; i < logger->processCount; i++) {
        variance += (logger->WTAs[i] - avg_wta) * (logger->WTAs[i] - avg_wta);
    }
    variance /= logger->processCount;

    fprintf(logger->perfFile, "CPU utilization = %.2f%%\n", cpu_utilization);
    fprintf(logger->perfFile, "Avg WTA = %.2f\n", avg_wta);
    fprintf(logger->perfFile, "Avg Waiting = %.2f\n", avg_waiting_time);
    fprintf(logger->perfFile, "Std WTA = %.2f", sqrt(variance));
}

void loggerDestroy(Logger* logger) {
    if (logger) {
        fclose(logger->logFile);
        fclose(logger->perfFile);
        free(logger->WTAs);
        free(logger);
    }
}

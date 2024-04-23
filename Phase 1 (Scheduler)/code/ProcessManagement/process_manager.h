#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

#include <sys/types.h>
#include <unistd.h>
#include "../Logging/logger.h"

// TODO: All methods here should take a logger pointer that should be created in scheduler
// and passed down to the algorithm function ex: HPF(messageQueueId, loggerPtr)
// and should call the logger functions inside the process manager functions
// to ensure that all logging is global

#define PREEMPT SIGUSR1
#define CONTINUE SIGUSR2

/**
 * Runs a new process using fork and exec.
 * @param logger A pointer to the logger.
 * @param remainingTime Remaining time for the process to run.
 * @param programName The name of the program to execute.
 * @return `pid_t` The PID of the forked process.
 */
pid_t pmRunProcess(Logger* logger, int remainingTime);

/**
 * Preempts the currently running process.
 * @param processId ID of the process to preempt.
 * @param logger A pointer to the logger.
 * @return `int` 0 on success, -1 on error.
 */
int pmPreemptProcess(pid_t processId, Logger* logger);

/**
 * Continues a stopped process.
 * @param processId ID of the process to continue.
 * @param logger A pointer to the logger.
 * @return `int` 0 on success, -1 on error.
 */
int pmContinueProcess(pid_t processId, Logger* logger);

#endif // PROCESS_MANAGER_H

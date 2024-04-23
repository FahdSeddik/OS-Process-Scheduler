#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

#include <sys/types.h>
#include <unistd.h>
#include "../Logging/logger.h"

// TODO: All methods here should take a logger pointer that should be created in scheduler
// and passed down to the algorithm function ex: HPF(messageQueueId, loggerPtr)
// and should call the logger functions inside the process manager functions
// to ensure that all logging is global


/**
 * Runs a new process using fork and exec.
 * @param programName The name of the program to execute.
 * @param processId ID of the process to run.
 * @param logger A pointer to the logger.
 * @return pid_t The PID of the forked process.
 */
pid_t pmRunProcess(const char* programName, int processId, Logger* logger);

/**
 * Preempts the currently running process.
 * @param processId ID of the process to preempt.
 * @param logger A pointer to the logger.
 * @return int 0 on success, -1 on error.
 */
int pmPreemptProcess(pid_t processId, Logger* logger);

/**
 * Stops the specified process, generally by sending a stop signal.
 * @param processId ID of the process to stop.
 * @param logger A pointer to the logger.
 * @return int 0 on success, -1 on error.
 */
int pmStopProcess(pid_t processId, Logger* logger);

/**
 * Continues a stopped process.
 * @param processId ID of the process to continue.
 * @param logger A pointer to the logger.
 * @return int 0 on success, -1 on error.
 */
int pmContinueProcess(pid_t processId, Logger* logger);

#endif // PROCESS_MANAGER_H

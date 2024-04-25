#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

#include <sys/types.h>
#include <unistd.h>
#include "../Logging/logger.h"
#include "../Dependencies/pcb.h"

// TODO: All methods here should take a logger pointer that should be created in scheduler
// and passed down to the algorithm function ex: HPF(messageQueueId, loggerPtr)
// and should call the logger functions inside the process manager functions
// to ensure that all logging is global

#define PREEMPT SIGUSR1
#define CONTINUE SIGUSR2

/**
 * Runs a new process using fork and exec.
 * @param programPath A path to the compiled binary for the program.
 * @param argv Array of arguments to be passed.
 * @return `pid_t` The PID of the forked process.
 * 
 * @example
 * char* schedulerArgs[2] = {"RR", "1"};
 * // providing a program path relative to root process path (process_generator.c is always root process)
 * pmRunProcess("./build/scheduler.out", schedulerArgs);
 */
pid_t pmRunProcess(const char* programPath, char * const argv[]);

/**
 * Preempts the currently running process.
 * @param pcb A Pointer to the process control block.
 * @param logger A pointer to the logger.
 * @return `int` 0 on success, -1 on error.
 */
int pmPreemptProcess(PCB* pcb, Logger* logger);

/**
 * Continues a stopped process.
 * @param pcb A Pointer to the process control block.
 * @param logger A pointer to the logger.
 * @return `int` 0 on success, -1 on error.
 */
int pmContinueProcess(PCB* pcb, Logger* logger);

/**
 * Sends `SIGINT` to `processId`
 * NOTE: This would only be used by root processes such as process_generator to kill scheduler 
 * (probably wont need it anywhere else)
 * @param processId ID of the process to kill.
 */
int pmKillProcess(pid_t processId);

#endif // PROCESS_MANAGER_H

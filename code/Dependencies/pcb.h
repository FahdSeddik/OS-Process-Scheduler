#ifndef PCB_H
#define PCB_H

#include <sys/types.h>
#include "../MemoryManagement/buddy.h"

typedef enum {
    READY, RUNNING, BLOCKED, TERMINATED
} ProcessState;

typedef struct {
    int id;                 // ID given by process generator
    int processId;          // Unique process ID (actual)
    ProcessState state;     // State of the process (READY, RUNNING, BLOCKED, TERMINATED)
    int priority;           // Priority level (used in HPF)
    int arrivalTime;        // Time when process arrives and should be considered by the scheduler
    int runningTime;        // Total time required by the process for execution
    int remainingTime;      // Remaining execution time (for SRTN)
    int waitingTime;        // Total time spent in the ready queue
    int lastExecTime;       // Last time when the process was in RUNNING state
    int startTime;          // Time when the process was first started (used for turnaround time calculation)
    int finishTime;         // Time when the process finishes execution
    int memsize;            // Memory size required by the process
    bsBlock* memoryBlock;   // Memory block allocated to the process

    // Additional fields for statistical purposes
    int turnaroundTime;    // Total time from arrival to completion
    double weightedTurnaroundTime; // Weighted turnaround time (turnaround time divided by burst time)
} PCB;

// Function declarations

/**
 * Initializes a PCB structure with the given parameters.
 * 
 * @param pcb Pointer to the PCB to initialize.
 * @param id The unique process identifier.
 * @param priority The initial priority of the process (used in HPF).
 * @param arrivalTime The time when the process is scheduled to arrive.
 * @param runningTime The total CPU burst time required by the process.
 */
void pcbInit(PCB *pcb, int id, int priority, int arrivalTime, int runningTime, int memsize, bsBlock* memoryBlock);

/**
 * Updates the state of a process represented by a PCB.
 *
 * @param pcb Pointer to the PCB whose state is to be updated.
 * @param new_state The new state to which the process should be updated.
 */
void pcbUpdateProcessState(PCB *pcb, ProcessState newState);

/**
 * Calculates and updates the waiting time for the process based on the current system time.
 *
 * @param pcb Pointer to the PCB of the process whose waiting time is to be updated.
 * @param currentTime The current time in the system clock.
 */
void pcbCalculateWaitingTime(PCB *pcb, int currentTime);

/**
 * Calculates and records the turnaround time for the process.
 * Turnaround time is calculated from the arrival time to the finish time.
 *
 * @param pcb Pointer to the PCB of the process.
 */
void pcbCalculateTurnaroundTime(PCB *pcb);

#endif // PCB_H
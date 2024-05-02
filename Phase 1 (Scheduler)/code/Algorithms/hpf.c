#include "hpf.h"
#include "../Dependencies/minheap.h"
#include "sync.h"
#include "../headers.h"
#include "../clk_utils.h"
#include "../ProcessManagement/semaphore.h"
#include "../ProcessManagement/process_manager.h"

/*
    ========================================================
    NOTE: ANY FUNCTION THAT IS IMPLEMENTED IN THIS FILE
    SHOULD HAVE ITS DECLARATION AND ITS DOC STRING IN hpf.h
    ONLY IF IT IS USED OUTSIDE THE hpf.c file
    ========================================================
*/

/**
 * Executes an iteration of the Highest Priority First (HPF) scheduling algorithm.
 * This function checks if there's a currently running process that needs attention
 * and starts a new process if the CPU is idle and processes are waiting in the queue.
 * It is designed to be called within the `initHPF()` function loop to continuously
 * manage process scheduling and execution based on HPF strategy.
 */
void execHPF();


/**
 * Starts the next process based on Highest Priority First (HPF) scheduling.
 * This function selects the highest priority process from a min-heap structure,
 * initiates its execution, and returns the process control block (PCB) of the newly
 * running process. It is intended to be used within scheduling loops where process
 * prioritization and selection are managed by priority queues.
 *
 * @return PCB* A pointer to the PCB of the newly started process, which contains
 *              all the necessary information about the process's runtime state and
 *              scheduling properties.
 */
PCB* startNextHPF();

/**
 * Handles the `SIGUSR1` signal triggered when a process terminates.
 * This function is designed to clean up resources, update the process
 * management structures, and possibly log the termination event. It
 * should be registered as a signal handler for SIGUSR1 to automatically
 * handle cleanup and state update of terminated processes.
 *
 * @param signum The signal number received, expected to be SIGUSR1.
 */
void catchTerminatedHPF(int signum);


SchedulerInfo* infoHPF = NULL;
Logger* loggerHPF = NULL;
mhMinHeap* minHeapHPF = NULL;
int semSyncTerminateHPF;

void initHPF(int msgQueueId, int semSyncRcv, int semSyncTerminate, Logger* logger) {
    int initialCapacity = 16;
    minHeapHPF = mhCreate(16);
    signal(SIGUSR1, catchTerminatedHPF);
    loggerHPF = logger;
    infoHPF = malloc(sizeof(SchedulerInfo));
    semSyncTerminateHPF = semSyncTerminate;
    schdInit(infoHPF);
    while (!infoHPF->finishGenerate || !mhIsEmpty(minHeapHPF) || infoHPF->currentlyRunning) {
        PCB* currentProcess = infoHPF->currentlyRunning;
        if (currentProcess) {
            currentProcess->remainingTime--;
            if (currentProcess->remainingTime == 0) semDown(semSyncTerminateHPF);
        }
        if (mhRcvProc(minHeapHPF, msgQueueId, semSyncRcv, true) == -1) infoHPF->finishGenerate = true;
        execHPF();
        if(!infoHPF->currentlyRunning) loggerCPUWait(loggerHPF, 1);
    }
    signal(SIGUSR1, SIG_DFL);
    mhFree(minHeapHPF);
    minHeapHPF = NULL;
    free(infoHPF);
    infoHPF = NULL;
    loggerHPF = NULL;
}

void execHPF() {
    if (infoHPF->currentlyRunning) return;
    // At this point, either no process was running or the previous process has just been terminated.
    // If the heap is not empty, start the next process.
    if (!mhIsEmpty(minHeapHPF)) infoHPF->currentlyRunning = startNextHPF(minHeapHPF, loggerHPF);
}

PCB* startNextHPF() {
    PCB* pcb = mhExtractMin(minHeapHPF);
    char remTimeStr[20];
    sprintf(remTimeStr, "%d", pcb->remainingTime);
    char* argv[] = {remTimeStr, NULL};
    pcb->processId = pmRunProcess("./build/process.out", argv, pcb, loggerHPF);
    return pcb;
}

void catchTerminatedHPF(int signum) {
    // Process has finished, so terminate and deallocate it
    if (!infoHPF->currentlyRunning) return; // Must be error
    pmFinishProcess(infoHPF->currentlyRunning, loggerHPF);
    free(infoHPF->currentlyRunning);
    infoHPF->currentlyRunning = NULL;
}
#include "srtn.h"
#include "sync.h"
#include "../headers.h"
#include "../clk_utils.h"
#include "../ProcessManagement/semaphore.h"


/*
    ========================================================
    NOTE: ANY FUNCTION THAT IS IMPLEMENTED IN THIS FILE
    SHOULD HAVE ITS DECLARATION AND ITS DOC STRING IN srtn.h
    ONLY IF IT IS USED OUTSIDE THE srtn.c file
    ========================================================
*/

/**
 * Executes an iteration of the Shortest Time Remaining Next (SRTN) scheduling algorithm.
 * This function checks if there's a currently running process that needs attention
 * and starts a new process if the CPU is idle and processes are waiting in the queue.
 * It is designed to be called within the `initSRTN()` function loop to continuously
 * manage process scheduling and execution based on SRTN strategy.
 */
void execSRTN();

/**
 * Handles the `SIGUSR1` signal triggered when a process terminates.
 * This function is designed to clean up resources, update the process
 * management structures, and possibly log the termination event. It
 * should be registered as a signal handler for SIGUSR1 to automatically
 * handle cleanup and state update of terminated processes.
 *
 * @param signum The signal number received, expected to be SIGUSR1.
 */
void catchTerminatedSRTN(int signum);

SchedulerInfo* infoSRTN = NULL;
Logger* loggerSRTN = NULL;
mhMinHeap* minHeapSRTN = NULL;


void initSRTN(int msgQueueId, int semSyncRcv, Logger* logger) {
    int initialCapacity = 16;
    minHeapSRTN = mhCreate(16);
    signal(SIGUSR1, catchTerminatedSRTN);
    loggerSRTN = logger;
    SchedulerInfo info;
    infoSRTN = &info;
    schdInit(infoSRTN);
    while (!infoSRTN->finishGenerate || !mhIsEmpty(minHeapSRTN) || infoSRTN->currentlyRunning) {
        if (mhRcvProc(minHeapSRTN, msgQueueId, semSyncRcv, true) == -1) infoSRTN->finishGenerate = true;
        execSRTN();
        if(!infoSRTN->currentlyRunning) loggerCPUWait(loggerSRTN, 1);
    }
    signal(SIGUSR1, SIG_DFL);
}

void execSRTN() {
    // TODO: implement this function
    // NOTE: you may want to change its interface
}

void catchTerminatedSRTN(int signum) {
    // TODO: implement logic when process terminates
}
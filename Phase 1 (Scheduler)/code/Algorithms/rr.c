#include "rr.h"
#include "../Dependencies/queue.h"
#include "sync.h"
#include "../headers.h"
#include "../clk_utils.h"
#include "../ProcessManagement/semaphore.h"

/*
    ========================================================
    NOTE: ANY FUNCTION THAT IS IMPLEMENTED IN THIS FILE
    SHOULD HAVE ITS DECLARATION AND ITS DOC STRING IN rr.h
    ONLY IF IT IS USED OUTSIDE THE rr.c file
    ========================================================
*/

/**
 * Executes an iteration of the Round Robin (RR) scheduling algorithm.
 * This function checks if there's a currently running process that needs attention
 * and starts a new process if the CPU is idle and processes are waiting in the queue.
 * It is designed to be called within the `initRR()` function loop to continuously
 * manage process scheduling and execution based on RR strategy.
 */
void execRR();

/**
 * Handles the `SIGUSR1` signal triggered when a process terminates.
 * This function is designed to clean up resources, update the process
 * management structures, and possibly log the termination event. It
 * should be registered as a signal handler for SIGUSR1 to automatically
 * handle cleanup and state update of terminated processes.
 *
 * @param signum The signal number received, expected to be SIGUSR1.
 */
void catchTerminatedRR(int signum);

qQueue* queueRR = NULL;
SchedulerInfo* infoRR = NULL;
Logger* loggerRR = NULL;


void initRR(int msgQueueId, int semSyncRcv, int quantum, Logger* logger) {
    queueRR = qCreate();
    signal(SIGUSR1, catchTerminatedRR);
    SchedulerInfo info;
    infoRR = &info;
    loggerRR = logger;
    schdInit(infoRR);
    while (!infoRR->finishGenerate || !qIsEmpty(queueRR) || infoRR->currentlyRunning) {
        if (qRcvProc(queueRR, msgQueueId, semSyncRcv) == -1) infoRR->finishGenerate = true;
        execRR();
        if(!infoRR->currentlyRunning) loggerCPUWait(loggerRR, 1);
    }
    signal(SIGUSR1, SIG_DFL);
}

void execRR() {
    // TODO: implement this function
    // NOTE: you may want to change its interface
}

void catchTerminatedRR(int signum) {
    // TODO: implement logic when process terminates
}
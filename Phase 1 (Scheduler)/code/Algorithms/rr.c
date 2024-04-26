#include "rr.h"
#include "../Dependencies/queue.h"
#include "sync.h"
#include "../headers.h"
#include "../clk_utils.h"
#include "../ProcessManagement/semaphore.h"
#include "../ProcessManagement/process_manager.h"

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
 * @param quantum A value for the quantum used in RR algorithm.
 */
void execRR(int quantum);

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
    infoRR = malloc(sizeof(SchedulerInfo));
    schdInit(infoRR);
    loggerRR = logger;
    while (!infoRR->finishGenerate || !qIsEmpty(queueRR) || infoRR->currentlyRunning) {
        if (infoRR->currentlyRunning) infoRR->currentlyRunning->remainingTime--;
        if (qRcvProc(queueRR, msgQueueId, semSyncRcv) == -1) infoRR->finishGenerate = true;
        execRR(quantum);
        if(!infoRR->currentlyRunning) loggerCPUWait(loggerRR, 1);
    }
    signal(SIGUSR1, SIG_DFL);
    qFree(queueRR);
    queueRR = NULL;
    free(infoRR);
    infoRR = NULL;
    loggerRR = NULL;
}

void execRR(int quantum) {
    // TODO: implement this function
    // NOTE: you may want to change its interface
    PCB* currentProcess = infoRR->currentlyRunning;
    // If there is no current process running, get one from the queue
    if (!currentProcess && !qIsEmpty(queueRR)) {
        currentProcess = qDequeue(queueRR);
        char remTimeStr[20];
        sprintf(remTimeStr, "%d", currentProcess->remainingTime);
        char* argv[] = {remTimeStr, NULL};
        pmRunProcess("./build/process.out", argv, currentProcess, loggerRR);
        infoRR->currentlyRunning = currentProcess;
    }
    if (!currentProcess) return;
    if (getClk() - currentProcess->lastExecTime < quantum) return;
    // Time slice is over, switch to next process
    pmPreemptProcess(currentProcess, loggerRR);
    qEnqueue(queueRR, currentProcess);
    PCB* nextProcess = qDequeue(queueRR);
    if (nextProcess->processId == -1) {
        char remTimeStr[20];
        sprintf(remTimeStr, "%d", nextProcess->remainingTime);
        char* argv[] = {remTimeStr, NULL};
        pmRunProcess("./build/process.out", argv, nextProcess, loggerRR);
    } else {
        pmContinueProcess(nextProcess, loggerRR);
    }
    infoRR->currentlyRunning = nextProcess;
}

void catchTerminatedRR(int signum) {
    // TODO: implement logic when process terminates
    if (!infoRR->currentlyRunning) return; // Must be error
    pmKillProcess(infoRR->currentlyRunning, loggerRR);
    free(infoRR->currentlyRunning);
    infoRR->currentlyRunning = NULL;
}
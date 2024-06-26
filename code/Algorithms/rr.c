#include "rr.h"
#include "../Dependencies/queue.h"
#include "../Dependencies/list.h"
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

/**
 * Either runs the process or continues it depending on the passed pcb.
 * @param pcb A pointer to the pcb.
 */
void startNextRR(PCB* pcb);

/**
 * Handles the waiting processes by allocating memory for them and adding them to the queue.
 * @param waitingList A pointer to the list of waiting processes.
 * @param queue A pointer to the queue of processes.
 * @param buddySystem A pointer to the buddy system allocator.
 */
void handleWaitingProcessesRR(mhMinHeap* waitingList, qQueue* queue , bsBuddySystem* buddySystem);

qQueue* queueRR = NULL;
SchedulerInfo* infoRR = NULL;
Logger* loggerRR = NULL;
int semSyncTerminateRR;
// Waiting list for processes that can't be added to the heap because of memory shortage
mhMinHeap* waitingListRR = NULL; 
bsBuddySystem* buddySystemRR = NULL;

void initRR(int msgQueueId, int semSyncRcv, int semSyncTerminate, int quantum, Logger* logger, bsBuddySystem* buddySystem) {
    queueRR = qCreate();
    waitingListRR = mhCreate(16);
    signal(SIGUSR1, catchTerminatedRR);
    infoRR = malloc(sizeof(SchedulerInfo));
    schdInit(infoRR);
    loggerRR = logger;
    buddySystemRR = buddySystem;
    semSyncTerminateRR = semSyncTerminate;
    while (!infoRR->finishGenerate || !qIsEmpty(queueRR) || infoRR->currentlyRunning) {
        PCB* currentProcess = infoRR->currentlyRunning;
        if (currentProcess) {
            currentProcess->remainingTime--;
            if (currentProcess->remainingTime == 0) semDown(semSyncTerminateRR);
        }
        if (qRcvProc(waitingListRR, msgQueueId, semSyncRcv) == -1) infoRR->finishGenerate = true;
        handleWaitingProcessesRR(waitingListRR, queueRR, buddySystemRR);
        execRR(quantum);
        if(!currentProcess) loggerCPUWait(loggerRR, 1);
    }
    signal(SIGUSR1, SIG_DFL);
    qFree(queueRR);
    queueRR = NULL;
    mhFree(waitingListRR);
    waitingListRR = NULL;
    free(infoRR);
    infoRR = NULL;
    loggerRR = NULL;
    buddySystemRR = NULL;
}

void execRR(int quantum) {
    PCB* currentProcess = infoRR->currentlyRunning;
    // If there is no current process running, get one from the queue
    if (!currentProcess && !qIsEmpty(queueRR)) {
        currentProcess = qDequeue(queueRR);
        startNextRR(currentProcess);
    }
    if (!currentProcess) return;
    if (getClk() - currentProcess->lastExecTime < quantum) return;
    // Time slice is over, switch to next process
    pmPreemptProcess(currentProcess, loggerRR);
    qEnqueue(queueRR, currentProcess);
    PCB* nextProcess = qDequeue(queueRR);
    startNextRR(nextProcess);
}

void startNextRR(PCB* pcb) {
    if (pcb->processId == -1) {
        char remTimeStr[20];
        sprintf(remTimeStr, "%d", pcb->remainingTime);
        char* argv[] = {remTimeStr, NULL};
        pmRunProcess("./build/process.out", argv, pcb, loggerRR);
    } else {
        pmContinueProcess(pcb, loggerRR);
    }
    infoRR->currentlyRunning = pcb;
}

void catchTerminatedRR(int signum) {
    if (!infoRR->currentlyRunning) return; // Must be error
    pmFinishProcess(infoRR->currentlyRunning, loggerRR, buddySystemRR);
    free(infoRR->currentlyRunning);
    infoRR->currentlyRunning = NULL;
    handleWaitingProcessesRR(waitingListRR, queueRR, buddySystemRR);
}

void handleWaitingProcessesRR(mhMinHeap* waitingList, qQueue* queue, bsBuddySystem* buddySystem) {
    while(!mhIsEmpty(waitingList)) {
        PCB* pcb = mhGetTop(waitingList);
        if (!allocateMemoryForProcess(buddySystem, pcb, loggerRR)) {
            return;
        }
        pcb = mhExtractMin(waitingList);
        qEnqueue(queue, pcb);
    }
}
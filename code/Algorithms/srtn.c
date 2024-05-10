#include "srtn.h"
#include "sync.h"
#include "../headers.h"
#include "../clk_utils.h"
#include "../Dependencies/minheap.h"
#include "../Dependencies/list.h"
#include "../ProcessManagement/semaphore.h"
#include "../ProcessManagement/process_manager.h"



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

/**
 * Handles the waiting processes by allocating memory for them and adding them to the heap.
 * @param waitingList A pointer to the list of waiting processes.
 * @param heap A pointer to the min-heap of processes.
 * @param buddySystem A pointer to the buddy system allocator.
 */
void handleWaitingProcessesSRTN(mhMinHeap* waitingList, mhMinHeap* heap, bsBuddySystem* buddySystem);

SchedulerInfo* infoSRTN = NULL;
Logger* loggerSRTN = NULL;
mhMinHeap* minHeapSRTN = NULL;
int semSyncTerminateSRTN;
// Waiting list for processes that can't be added to the heap because of memory shortage
mhMinHeap* waitingListSRTN = NULL; 
bsBuddySystem* buddySystemSRTN = NULL;

void initSRTN(int msgQueueId, int semSyncRcv, int semSyncTerminate, Logger* logger, bsBuddySystem* buddySystem) {
    int initialCapacity = 16;
    minHeapSRTN = mhCreate(initialCapacity);
    waitingListSRTN = mhCreate(initialCapacity);
    signal(SIGUSR1, catchTerminatedSRTN);
    loggerSRTN = logger;
    buddySystemSRTN = buddySystem;
    infoSRTN = malloc(sizeof(SchedulerInfo));
    semSyncTerminateSRTN = semSyncTerminate;
    schdInit(infoSRTN);
    while (!infoSRTN->finishGenerate || !mhIsEmpty(minHeapSRTN) || infoSRTN->currentlyRunning) {
        PCB* currentProcess = infoSRTN->currentlyRunning;
        if (currentProcess) {
            currentProcess->remainingTime--;
            if (currentProcess->remainingTime == 0) semDown(semSyncTerminateSRTN);
        }
        if (qRcvProc(waitingListSRTN, msgQueueId, semSyncRcv) == -1) infoSRTN->finishGenerate = true;
        handleWaitingProcessesSRTN(waitingListSRTN, minHeapSRTN, buddySystemSRTN);
        execSRTN();
        if(!infoSRTN->currentlyRunning) loggerCPUWait(loggerSRTN, 1);
    }
    signal(SIGUSR1, SIG_DFL);
    mhFree(minHeapSRTN);
    minHeapSRTN = NULL;
    mhFree(waitingListSRTN);
    waitingListSRTN = NULL;
    free(infoSRTN);
    infoSRTN = NULL;
    loggerSRTN = NULL;
    buddySystemSRTN = NULL;
}

void execSRTN() {
    PCB* currProcess = infoSRTN->currentlyRunning;
    PCB* nextProcess = NULL;
    // Check if there are processes waiting and if the CPU is idle or there's a need to preempt
    if (mhIsEmpty(minHeapSRTN)) return;

    nextProcess = mhGetTop(minHeapSRTN);  // Peek at the shortest remaining time process without removing it
    // Determine if we should preempt the current process
    if (currProcess && nextProcess->remainingTime >= currProcess->remainingTime) return;
    
    if (currProcess) {
        // Preempt the current process
        pmPreemptProcess(currProcess, loggerSRTN);
        mhInsert(minHeapSRTN, currProcess, currProcess->remainingTime);
    }

    // Remove the next process from the heap as it will now be running
    nextProcess = mhExtractMin(minHeapSRTN);

    // Start or resume the next process
    if (nextProcess->processId == -1) {  // Process has not been started yet
        char remTimeStr[20];
        sprintf(remTimeStr, "%d", nextProcess->remainingTime);
        char* argv[] = {remTimeStr, NULL};
        nextProcess->processId = pmRunProcess("./build/process.out", argv, nextProcess, loggerSRTN);
    } else {
        pmContinueProcess(nextProcess, loggerSRTN);
    }
    infoSRTN->currentlyRunning = nextProcess;
}

void catchTerminatedSRTN(int signum) {
    if (!infoSRTN->currentlyRunning) return; // Must be error
    pmFinishProcess(infoSRTN->currentlyRunning, loggerSRTN, buddySystemSRTN);
    free(infoSRTN->currentlyRunning);
    infoSRTN->currentlyRunning = NULL;
    handleWaitingProcessesSRTN(waitingListSRTN, minHeapSRTN, buddySystemSRTN);
}

void handleWaitingProcessesSRTN(mhMinHeap* waitingList, mhMinHeap* heap, bsBuddySystem* buddySystem) {
    while(!mhIsEmpty(waitingList)){
        PCB* pcb = mhGetTop(waitingList);
        if (!allocateMemoryForProcess(buddySystem, pcb, loggerSRTN)) {
            return;
        }
        pcb = mhExtractMin(waitingList);
        mhInsert(heap, pcb, pcb->remainingTime);
    }
}

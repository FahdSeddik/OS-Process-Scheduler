#include "hpf.h"
#include "../Dependencies/minheap.h"
#include "../Dependencies/list.h"
#include "sync.h"
#include "../headers.h"
#include "../clk_utils.h"
#include "../ProcessManagement/semaphore.h"
#include "../ProcessManagement/process_manager.h"
#include "../MemoryManagement/buddy.h"

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

/**
 * Handles the waiting processes by allocating memory for them and adding them to the heap.
 * @param waitingList A pointer to the list of waiting processes.
 * @param heap A pointer to the min-heap of processes.
 * @param buddySystem A pointer to the buddy system allocator.
 */
void handleWaitingProcessesHPF(lList* waitingList, mhMinHeap* heap , bsBuddySystem* buddySystem);


SchedulerInfo* infoHPF = NULL;
Logger* loggerHPF = NULL;
mhMinHeap* minHeapHPF = NULL;
int semSyncTerminateHPF;
// Waiting list for processes that can't be added to the heap because of memory shortage
lList* waitingListHPF = NULL;  
bsBuddySystem* buddySystemHPF = NULL;

void initHPF(int msgQueueId, int semSyncRcv, int semSyncTerminate, Logger* logger, bsBuddySystem* buddySystem){
    int initialCapacity = 16;
    minHeapHPF = mhCreate(initialCapacity);
    waitingListHPF = lCreate();
    signal(SIGUSR1, catchTerminatedHPF);
    loggerHPF = logger;
    buddySystemHPF = buddySystem;
    infoHPF = malloc(sizeof(SchedulerInfo));
    semSyncTerminateHPF = semSyncTerminate;
    schdInit(infoHPF);
    while (!infoHPF->finishGenerate || !mhIsEmpty(minHeapHPF) || infoHPF->currentlyRunning) {
        PCB* currentProcess = infoHPF->currentlyRunning;
        if (currentProcess) {
            currentProcess->remainingTime--;
            if (currentProcess->remainingTime == 0) semDown(semSyncTerminateHPF);
        }
        if (qRcvProc(waitingListHPF, msgQueueId, semSyncRcv) == -1) infoHPF->finishGenerate = true;
        handleWaitingProcessesHPF(waitingListHPF, minHeapHPF, buddySystemHPF);
        execHPF();
        if(!infoHPF->currentlyRunning) loggerCPUWait(loggerHPF, 1);
    }
    signal(SIGUSR1, SIG_DFL);
    mhFree(minHeapHPF);
    minHeapHPF = NULL;
    lFree(waitingListHPF);
    waitingListHPF = NULL;
    free(infoHPF);
    infoHPF = NULL;
    loggerHPF = NULL;
    buddySystemHPF = NULL;
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
    pmFinishProcess(infoHPF->currentlyRunning, loggerHPF, buddySystemHPF);
    free(infoHPF->currentlyRunning);
    infoHPF->currentlyRunning = NULL;
    // try to allocate memory for waiting processes
    handleWaitingProcessesHPF(waitingListHPF, minHeapHPF, buddySystemHPF);
}

void handleWaitingProcessesHPF(lList* waitingList, mhMinHeap* heap, bsBuddySystem* buddySystem) {
    lListNode* iter = lBegin(waitingList);
    while (iter != lEnd(waitingList)) {
        // printf("Handling waiting process %d at time %d memsize %d\n", iter->pcb->id, getClk(), iter->pcb->memsize);
        PCB* pcb = iter->pcb;
        if (!allocateMemoryForProcess(buddySystem, pcb, loggerHPF)) {
            iter = lGetNext(waitingList, iter);
            continue;
        }
        iter = lRemove(waitingList, iter);
        mhInsert(heap, pcb, pcb->priority);
    }
}
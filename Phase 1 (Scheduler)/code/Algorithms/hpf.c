#include "hpf.h"
#include "../Dependencies/minheap.h"
#include "sync.h"
#include "../headers.h"
#include "../clk_utils.h"
#include "../ProcessManagement/semaphore.h"
#include "../ProcessManagement/process_manager.h"
/**
 * Only to be used by the initHPF() function to apply an iteration
 * of the algorithm
 * @param minHeap A pointer to the minHeap that stores the PCB pointers
 * @param logger A pointer to the logger to be passed to Process manager functions
 * @param info A poitner to the scheduler info structure that holds needed data for simulation
 */
void execHPF(mhMinHeap* minHeap, Logger* logger, SchedulerInfo* info);

/**
 * Starts the next process for HPF scheduling
 * @param minHeap A pointer to the minHeap that stores the PCB pointers
 * @param logger A pointer to the logger to be passed to Process manager functions
 */
PCB* startNext(mhMinHeap* minHeap, Logger* logger);


/*
    ========================================================
    NOTE: ANY FUNCTION THAT IS IMPLEMENTED IN THIS FILE
    SHOULD HAVE ITS DECLARATION AND ITS DOC STRING IN hpf.h
    ONLY IF IT IS USED OUTSIDE THE hpf.c file
    ========================================================
*/

void initHPF(int msgQueueId, int semSyncRcv, Logger* logger) {
    int initialCapacity = 16;
    mhMinHeap* minHeap = mhCreate(16);
    SchedulerInfo info;
    schdInit(&info);
    while (!info.finishGenerate || !mhIsEmpty(minHeap) || info.currentlyRunning) {
        if (mhRcvProc(minHeap, msgQueueId, semSyncRcv, true) == -1) info.finishGenerate = true;
        execHPF(minHeap, logger, &info);
        if(!info.currentlyRunning) loggerCPUWait(logger, 1);
    }
}

void execHPF(mhMinHeap* minHeap, Logger* logger, SchedulerInfo* info) {
    // If there is a process currently running, check its status
    if (info->currentlyRunning) {
        // If the current time is still before the finish time, return immediately
        if (getClk() < info->currentlyRunning->finishTime) return;
        // Process has finished, so terminate and deallocate it
        pmKillProcess(info->currentlyRunning, logger);
        free(info->currentlyRunning);
        info->currentlyRunning = NULL; // Ensure the pointer is cleared
    }
    // At this point, either no process was running or the previous process has just been terminated.
    // If the heap is not empty, start the next process.
    if (!mhIsEmpty(minHeap)) info->currentlyRunning = startNext(minHeap, logger);
}

PCB* startNext(mhMinHeap* minHeap, Logger* logger) {
    PCB* pcb = mhExtractMin(minHeap);
    char remTimeStr[20];
    char* argv[2];
    sprintf(remTimeStr, "%d", pcb->remainingTime);
    argv[0] = remTimeStr;
    argv[1] = NULL;
    pcb->processId = pmRunProcess("./build/process.out", argv, pcb, logger);
    pcb->finishTime = getClk() + pcb->runningTime;
    return pcb;
}
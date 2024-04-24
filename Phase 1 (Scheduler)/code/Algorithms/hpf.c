#include "hpf.h"
#include "../Dependencies/minheap.h"
#include "sync.h"
#include "../headers.h"
#include "../clk_utils.h"
#include "../ProcessManagement/semaphore.h"

/**
 * Only to be used by the initHPF() function to apply an iteration
 * of the algorithm
 * @param minHeap A pointer to the minHeap that stores the PCB pointers
 * @param logger A pointer to the logger to be passed to Process manager functions
 */
void execHPF(mhMinHeap* minHeap, Logger* logger);

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
    int time = getClk();
    while (!info.finishGenerate || !mhIsEmpty(minHeap)) {
        int rcvCode = mhRcvProc(minHeap, msgQueueId, semSyncRcv, false);
        if (rcvCode == -1) info.finishGenerate = true;
        if (!mhIsEmpty(minHeap)) execHPF(minHeap, logger);
        while(time == getClk());
        time = getClk();
    }
}


void execHPF(mhMinHeap* minHeap, Logger* logger) {
    // TODO: implement this function
    // NOTE: you may want to change its interface
}
#include "srtn.h"
#include "sync.h"
#include "../headers.h"
#include "../clk_utils.h"
#include "../ProcessManagement/semaphore.h"

/**
 * Only to be used by the initSRTN() function to apply an iteration
 * of the algorithm
 * @param minHeap A pointer to the minHeap that stores the PCB pointers
 * @param logger A pointer to the logger to be passed to Process manager functions
 */
void execSRTN(mhMinHeap* minHeap, Logger* logger);


/*
    ========================================================
    NOTE: ANY FUNCTION THAT IS IMPLEMENTED IN THIS FILE
    SHOULD HAVE ITS DECLARATION AND ITS DOC STRING IN srtn.h
    ONLY IF IT IS USED OUTSIDE THE srtn.c file
    ========================================================
*/

void initSRTN(int msgQueueId, int semSyncRcv, Logger* logger) {
    int initialCapacity = 16;
    mhMinHeap* minHeap = mhCreate(16);
    SchedulerInfo info;
    schdInit(&info);
    int time = getClk();
    while (!info.finishGenerate || !mhIsEmpty(minHeap)) {
        int rcvCode = mhRcvProc(minHeap, msgQueueId, semSyncRcv, false);
        if (rcvCode == -1) info.finishGenerate = true;
        if (!mhIsEmpty(minHeap)) execSRTN(minHeap, logger);
        while(time == getClk());
        time = getClk();
    }
}

void execSRTN(mhMinHeap* minHeap, Logger* logger) {
    // TODO: implement this function
    // NOTE: you may want to change its interface
}
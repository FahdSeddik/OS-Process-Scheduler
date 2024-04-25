#include "rr.h"
#include "../Dependencies/queue.h"
#include "sync.h"
#include "../headers.h"
#include "../clk_utils.h"
#include "../ProcessManagement/semaphore.h"

/**
 * Only to be used by the initRR() function to apply an iteration
 * of the algorithm
 * @param queue A pointer to the queue that stores the PCB pointers
 * @param quantum A value for the quantum used in RR algorithm.
 * @param logger A pointer to the logger to be passed to Process manager functions
 */
void execRR(qQueue* queue, int quantum, Logger* logger);

/*
    ========================================================
    NOTE: ANY FUNCTION THAT IS IMPLEMENTED IN THIS FILE
    SHOULD HAVE ITS DECLARATION AND ITS DOC STRING IN rr.h
    ONLY IF IT IS USED OUTSIDE THE rr.c file
    ========================================================
*/

void initRR(int msgQueueId, int semSyncRcv, int quantum, Logger* logger) {
    qQueue* queue = qCreate();
    SchedulerInfo info;
    schdInit(&info);
    while (!info.finishGenerate || !qIsEmpty(queue) || info.currentlyRunning) {
        if (qRcvProc(queue, msgQueueId, semSyncRcv) == -1) info.finishGenerate = true;
        execRR(queue, quantum, logger);
        if(!info.currentlyRunning) loggerCPUWait(logger, 1);
    }
}

void execRR(qQueue* queue, int quantum, Logger* logger) {
    // TODO: implement this function
    // NOTE: you may want to change its interface
}
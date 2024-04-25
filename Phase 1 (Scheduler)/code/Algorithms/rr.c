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
    int initialCapacity = 16;
    qQueue* queue = qCreate();
    SchedulerInfo info;
    schdInit(&info);
    while (!info.finishGenerate || !qIsEmpty(queue)) {
        int rcvCode = qRcvProc(queue, msgQueueId, semSyncRcv);
        fprintf(stderr, "At %d\n", getClk());
        if (rcvCode == -1) info.finishGenerate = true;
        if (!qIsEmpty(queue)) execRR(queue, quantum, logger);
    }
}

void execRR(qQueue* queue, int quantum, Logger* logger) {
    // TODO: implement this function
    // NOTE: you may want to change its interface
}
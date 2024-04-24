#ifndef SYNC_PROCESSES_H
#define SYNC_PROCESSES_H

#include "../Dependencies/minheap.h"
#include "../Dependencies/queue.h"

/*
    ============================================================
    # This file is to ensure that receiving of a new processes #
    #  from the process generator is done in a universal way   #
    #        no matter what the scheduling algorithm is        #
    ============================================================
*/

typedef struct SchedulerInfo {
    bool finishGenerate;
    // TODO: add common attributes that will be used by all algorithms
    // and add its initialization in schdInit
} SchedulerInfo;

/**
 * Initializes the info structure for the scheduler.
 * @param info A pointer to the SchedulerInfo structure
 */
void schdInit(SchedulerInfo* info);


/**
 * Receives and processes messages from a message queue and enqueues the resultant PCBs into a queue.
 * This function is typically used to receive process data and store them in a scheduling queue.
 *
 * @param queue A pointer to the queue where the PCBs will be enqueued.
 * @param msgQueueId The message queue identifier from which messages are received.
 * @param semSyncRcv The semaphore ID used for synchronizing the receive operation.
 *
 * @return Returns 0 on successful processing of all messages, or -1 if a termination message is received.
 */
int qRcvProc(qQueue* queue, int msgQueueId, int semSyncRcv);

/**
 * Receives messages from a message queue, initializes PCBs based on these messages, and inserts them into the min-heap.
 * The key for the min-heap can be determined based on either the priority or the remaining time of the process, depending on the keyIsPrio flag.
 *
 * @param minHeap A pointer to the min-heap where the PCBs will be inserted.
 * @param msgQueueId The message queue identifier from which messages are received.
 * @param semSyncRcv The semaphore ID used for synchronizing the receive operation.
 * @param keyIsPrio A boolean flag indicating whether the priority (true) or the remaining time (false) should be used as the key in the min-heap.
 *
 * @return Returns 0 on successful processing of all messages, or -1 if a termination message is received.
 */
int mhRcvProc(mhMinHeap* minHeap, int msgQueueId, int semSyncRcv, bool keyIsPrio);

#endif // SYNC_PROCESSES_H
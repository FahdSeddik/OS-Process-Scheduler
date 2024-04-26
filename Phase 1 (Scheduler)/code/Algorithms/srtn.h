#ifndef SRTN_H
#define SRTN_H

#include "../Logging/logger.h"
#include "../Dependencies/minheap.h"
// TODO: Implement this function in srtn.c
// Note: you may change its interface as you like remove/add anything this is just a draft

/**
 * Performs Shortest Remaining Time Next (SRTN) scheduling algorithm
 * 
 * @param msgQueueId An id for the message queue.
 * @param semSyncRcv An id for the semaphore to be used for guarantee wait for process gen.
 * @param semSyncTerminate An id for the semaphore that ensures termination signals are synced.
 * @param logger A pointer to the logger.
 */
void initSRTN(int msgQueueId, int semSyncRcv, int semSyncTerminate, Logger* logger);

#endif // SRTN_H
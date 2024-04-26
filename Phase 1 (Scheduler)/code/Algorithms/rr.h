#ifndef RR_H
#define RR_H

#include "../Logging/logger.h"

// TODO: Implement this function in rr.c
// Note: you may change its interface as you like remove/add anything this is just a draft
/**
 * Performs Round Robin (RR) scheduling algorithm
 * 
 * @param msgQueueId An id for the message queue.
 * @param semSyncRcv An id for the semaphore to be used for guarantee wait for process gen.
 * @param semSyncTerminate An id for the semaphore that ensures termination signals are synced.
 * @param quantum A value for the quantum used in RR algorithm.
 * @param logger A pointer to the logger.
 */
void initRR(int msgQueueId, int semSyncRcv, int semSyncTerminate, int quantum, Logger* logger);


#endif // RR_H
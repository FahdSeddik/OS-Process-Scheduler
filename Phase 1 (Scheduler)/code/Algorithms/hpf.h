#ifndef HPF_H
#define HPF_H

#include "../Logging/logger.h"


// TODO: Implement this function in hpf.c
// Note: you may change its interface as you like remove/add anything this is just a draft

/**
 * Performs Highest Priority First (HPF) scheduling algorithm
 * @param msgQueueId An id for the message queue.
 * @param semSyncRcv An id for the semaphore to be used for guarantee wait for process gen.
 * @param logger A pointer to the logger.
 */
void initHPF(int msgQueueId, int semSyncRcv, Logger* logger);



#endif // HPF_H
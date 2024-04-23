#ifndef SRTN_H
#define SRTN_H

#include "../Logging/logger.h"

// TODO: Implement this function in srtn.c
// Note: you may change its interface as you like remove/add anything this is just a draft
/**
 * Performs Shortest Remaining Time Next (SRTN) scheduling algorithm
 * 
 * @param msgQueueId An id for the message queue.
 * @param logger A pointer to the logger.
 */
void SRTN(int msgQueueId, Logger* logger);



#endif // SRTN_H
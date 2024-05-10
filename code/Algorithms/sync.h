#ifndef SYNC_PROCESSES_H
#define SYNC_PROCESSES_H

#include "../Dependencies/list.h"
#include "../Dependencies/minheap.h"
#include "../Dependencies/bool.h"
#include "../Logging/logger.h"

/*
    ============================================================
    # This file is to ensure that receiving of a new processes #
    #  from the process generator is done in a universal way   #
    #        no matter what the scheduling algorithm is        #
    ============================================================
*/

typedef struct SchedulerInfo {
    bool finishGenerate;
    PCB* currentlyRunning;
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
 * @param list A pointer to the list where the PCBs will be appended.
 * @param msgQueueId The message queue identifier from which messages are received.
 * @param semSyncRcv The semaphore ID used for synchronizing the receive operation.
 *
 * @return Returns 0 on successful processing of all messages, or -1 if a termination message is received.
 */
int qRcvProc(mhMinHeap* list, int msgQueueId, int semSyncRcv);

/**
 * Allocates memory for a process and assigns the memory block to the process.
 * This function is typically used to allocate memory for a process before adding it to the scheduling queue.
 *
 * @param buddySystem A pointer to the buddy system allocator used for memory allocation.
 * @param pcb A pointer to the PCB structure for which memory is allocated.
 * @param logger A pointer to the logger used for logging memory allocation events.
 *
 * @return Returns a pointer to the allocated memory block if successful, or NULL if memory allocation fails.
 */
bsBlock* allocateMemoryForProcess(bsBuddySystem* buddySystem, PCB* pcb, Logger* logger);


#endif // SYNC_PROCESSES_H
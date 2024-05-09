#include "sync.h"
#include "../ProcessManagement/message_queue.h"
#include "../ProcessManagement/semaphore.h"
#include <stdio.h>
#include <stdlib.h>


void schdInit(SchedulerInfo* info) {
    info->finishGenerate = false;
    info->currentlyRunning = NULL;
    // TOOD: add here initialization for any attributes added in sync.h
}


int qRcvProc(lList* list, int msgQueueId, int semSyncRcv) {
    semDown(semSyncRcv);
    ProcessMessage message;
    message.mtype = MESSAGE_MTYPE;
    while(mqReceiveNonBlocking(msgQueueId, &message) == 1) {
        // printf("Rec id: %d\n", message.id);
        // Process generator finished processes
        if(message.id == -1) return -1;
        PCB* pcb = (PCB *)malloc(sizeof(PCB));
        pcbInit(pcb, message.id, message.priority, message.arrivalTime, message.runningTime, message.memsize, NULL);
        lInsert(list, pcb);
    }
    return 0;
}

bsBlock* allocateMemoryForProcess(bsBuddySystem* buddySystem, PCB* pcb) {
    bsBlock* memoryBlock = bsAllocate(buddySystem, pcb->memsize);
    if (!memoryBlock) {
        printf("Can't allocate memory of %d bytes for process %d\n", pcb->memsize, pcb->id);
        return NULL;
    }
    printf("Allocated memory of %d bytes for process %d\n", pcb->memsize, pcb->id);
    pcb->memoryBlock = memoryBlock;
    return memoryBlock;
}
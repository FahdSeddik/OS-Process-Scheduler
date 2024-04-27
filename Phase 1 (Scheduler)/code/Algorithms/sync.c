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


int qRcvProc(qQueue* queue, int msgQueueId, int semSyncRcv) {
    semDown(semSyncRcv);
    ProcessMessage message;
    message.mtype = MESSAGE_MTYPE;
    while(mqReceiveNonBlocking(msgQueueId, &message) == 1) {
        printf("Rec id: %d\n", message.id);
        // Process generator finished processes
        if(message.id == -1) return -1;
        PCB* pcb = (PCB *)malloc(sizeof(PCB));
        pcbInit(pcb, message.id, message.priority, message.arrivalTime, message.runningTime);
        qEnqueue(queue, pcb);
    }
    return 0;
}

int mhRcvProc(mhMinHeap* minHeap, int msgQueueId, int semSyncRcv, bool keyIsPrio) {
    semDown(semSyncRcv);
    ProcessMessage message;
    message.mtype = MESSAGE_MTYPE;
    while(mqReceiveNonBlocking(msgQueueId, &message) == 1) {
        printf("Rec id: %d\n", message.id);
        // Process generator finished processes
        if(message.id == -1) return -1;
        PCB* pcb = (PCB *)malloc(sizeof(PCB));
        pcbInit(pcb, message.id, message.priority, message.arrivalTime, message.runningTime);
        mhInsert(minHeap, pcb, keyIsPrio ? pcb->priority:pcb->remainingTime);
    }
    return 0;
}
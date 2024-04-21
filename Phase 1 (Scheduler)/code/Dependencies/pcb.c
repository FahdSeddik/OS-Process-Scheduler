// pcb.c
#include "pcb.h"

void pcb_initPCB(PCB *pcb, int id, int priority, int arrivalTime, int runningTime) {
    pcb->id = id;
    pcb->priority = priority;
    pcb->arrivalTime = arrivalTime;
    pcb->runningTime = runningTime;
    pcb->remainingTime = runningTime;
    pcb->waitingTime = 0;
    pcb->lastExecTime = 0;
    pcb->startTime = 0;
    pcb->finishTime = 0;
    pcb->turnaroundTime = 0;
    pcb->weightedTurnaroundTime = 0.0;
    pcb->state = READY;
}

void pcb_updateProcessState(PCB *pcb, ProcessState newState) {
    pcb->state = newState;
}

void pcb_calculateWaitingTime(PCB *pcb, int current_time) {
    if (pcb->state == READY) {
        pcb->waitingTime += (current_time - pcb->lastExecTime);
    }
    pcb->lastExecTime = current_time; // Update last execution time to current time
}

void pcb_calculateTurnaroundTime(PCB *pcb) {
    pcb->turnaroundTime = pcb->finishTime - pcb->arrivalTime;
    pcb->weightedTurnaroundTime = (double)pcb->turnaroundTime / pcb->runningTime;
}

#include "process_manager.h"
#include <signal.h>
#include <stdio.h>
#include "../clk_utils.h"
#include <unistd.h>
#include "../MemoryManagement/buddy.h"

pid_t pmRunProcess(const char* programPath, char * const argv[], PCB* pcb, Logger* logger) {

    pid_t pid = fork();

    if (pid == -1) {
        // Fork failed
        perror("Failed to fork process");
        return -1;
    } else if (pid == 0) {
        // Child process
        if(pcb) free(pcb);
        if(logger) free(logger);
        execvp(programPath, argv);
        // If execl returns, there was an error
        perror("Failed to execute process");
        _exit(1);
    }

    if(pcb && logger) {
        pcb->processId = pid;
        // printf("Child Started pid= %d\n", pcb->processId);
        int time = getClk();
        pcb->startTime = time;
        pcbCalculateWaitingTime(pcb, time);
        pcbUpdateProcessState(pcb, RUNNING);
        loggerLogEvent(logger, time, pcb->id, "started", pcb->arrivalTime,
        pcb->runningTime, pcb->remainingTime, pcb->waitingTime, 0, 0);
    }
    // Only parent process reaches here
    return pid;
}

int pmPreemptProcess(PCB* pcb, Logger* logger) {
    int result = kill(pcb->processId, PREEMPT);
    // printf("Child pid= %d, preempted\n", pcb->processId);
    usleep(1000 * 10);
    int time = getClk();
    pcbCalculateWaitingTime(pcb, time);
    pcbUpdateProcessState(pcb, READY);
    loggerLogEvent(logger, time, pcb->id, "stopped", pcb->arrivalTime, pcb->runningTime, pcb->remainingTime, pcb->waitingTime, 0, 0);
    return result;
}

int pmContinueProcess(PCB* pcb, Logger* logger) {
    // printf("Child pid= %d, resume\n", pcb->processId);
    int time = getClk();
    pcbCalculateWaitingTime(pcb, time);
    pcbUpdateProcessState(pcb, RUNNING);
    loggerLogEvent(logger, getClk(), pcb->id, "resumed", pcb->arrivalTime, pcb->runningTime, pcb->remainingTime, pcb->waitingTime, 0, 0);
    kill(pcb->processId, SIGCONT);
    return kill(pcb->processId, CONTINUE);
}

int pmFinishProcess(PCB* pcb, Logger* logger, bsBuddySystem* buddySystemHPF) {
    // printf("Child pid= %d, finished\n", pcb->processId);
    pcbUpdateProcessState(pcb, TERMINATED);
    int time = getClk();
    pcb->finishTime = time;
    pcbCalculateTurnaroundTime(pcb);
    loggerUpdateMetrics(logger, pcb->runningTime, pcb->waitingTime, pcb->turnaroundTime, pcb->weightedTurnaroundTime);
    loggerLogEvent(logger, time, pcb->id, "finished", pcb->arrivalTime, pcb->runningTime, 0, pcb->waitingTime, pcb->turnaroundTime, pcb->weightedTurnaroundTime);
    loggerLogMemoryEvent(logger, time, pcb->id, "freed", pcb->memsize, pcb->memoryBlock->addressStart, pcb->memoryBlock->addressStart + pcb->memoryBlock->size - 1);
    waitpid(pcb->processId, NULL, 0);
    // deallocate memory
    bsDeallocate(buddySystemHPF, pcb->memoryBlock);
    return kill(pcb->processId, SIGINT);
}
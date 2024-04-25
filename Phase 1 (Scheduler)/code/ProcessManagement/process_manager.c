#include "process_manager.h"
#include <signal.h>
#include <stdio.h>
#include "../clk_utils.h"

pid_t pmRunProcess(const char* programPath, char * const argv[]) {

    pid_t pid = fork();

    if (pid == -1) {
        // Fork failed
        perror("Failed to fork process");
        return -1;
    } else if (pid == 0) {
        // Child process
        execvp(programPath, argv);
        // If execl returns, there was an error
        perror("Failed to execute process");
        _exit(1);
    }

    // Only parent process reaches here
    return pid;
}

int pmPreemptProcess(PCB* pcb, Logger* logger) {
    loggerLogProcessEvent(logger, getClk(), pcb->processId, "stopped", pcb->arrivalTime, pcb->runningTime, pcb->remainingTime, pcb->waitingTime, 0, 0);
    return kill(pcb->processId, PREEMPT);
}

int pmContinueProcess(PCB* pcb, Logger* logger) {
    loggerLogProcessEvent(logger, getClk(), pcb->processId, "resumed", pcb->arrivalTime, pcb->runningTime, pcb->remainingTime, pcb->waitingTime, 0, 0);
    kill(pcb->processId, SIGCONT);
    return kill(pcb->processId, CONTINUE);
}

int pmKillProcess(pid_t processId) {
    return kill(processId, SIGINT);
}
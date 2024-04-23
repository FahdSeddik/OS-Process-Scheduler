#include "rr.h"

/*
    ===========================================================
        NOTE: ANY FUNCTION THAT IS IMPLEMENTED IN THIS FILE
        SHOULD HAVE ITS IMPLEMENTED AND ITS DOC STRING IN rr.h
    ===========================================================
*/

void RR(int msgQueueId, int quantum, Logger* logger) {
    // TODO: implement this function
    // NOTE: you may want to change its interface

    // I left your previous implementation here if you want it

    // int clk = 0;
    // bool processGeneratorIsDone = false; // process generator is done
    // PCB *currentProcessPCB = NULL;
    // int nextSwtichTime = 0; // process switching time

    // while (!isAllProcessesCompleted(processQueue)) {
    //     clk = getClk();
    //     receiveNewProcesses();
    //     if (clk == nextSwtichTime) { // you can exec the next one
    //         currentProcessPCB = q_dequeue(processQueue);
    //         if (currentProcessPCB != NULL) {
    //             logProcessExecution(currentProcessPCB, false);
    //             executeProcessRR(currentProcessPCB, quantum);
    //             bool willFinish = isProcessFinishNextSwitch(currentProcessPCB, &nextSwtichTime);
    //             // if the process is not finished add it to the queue
    //             if (!willFinish)
    //                 q_enqueue(processQueue, currentProcessPCB);
    //         }
    //     }
    // }
}

// // checks if there are new processes in the message queue
// // and adds them to the ready queue
// void receiveNewProcesses() {
//     struct message processReceived;
//     while (1) {
//         // receive a message from the queue
//         if (msgrcv(processMsgqID, &processReceived, sizeof(processReceived), 0, IPC_NOWAIT) != -1) {
//             // create a new process
//             PCB *newProcessPCB = (PCB *)malloc(sizeof(PCB));
//             pcb_initPCB(newProcessPCB, -1, processReceived.priority, processReceived.arrivalTime, processReceived.runningTime);
//             // add the process to the ready list
//             q_enqueue(processQueue, newProcess); // TODO: enque should accept process
//         } else {
//             break;
//         }
//     }
// }


// /**
//  * Checks the status of the process and updates the remaining time
//  * returns true if the process will be finished
//  */
// bool isProcessFinishNextSwitch(PCB *pcb, int *finishTime) {
//     // then check if the remaining time is less than the quantum
//     if (pcb->remainingTime <= quantum) {
//         // willFinish = true the process will be finished and don't add it to the queue
//         finishTime = getClk() + pcb->remainingTime;
//         return true;
//     } else {
//         if (getClk() - pcb->lastExecTime >= quantum) {
//             logProcessExecution(pcb, true);
//             // if the process was running for the whole quantum then stop it and return false
//             if (kill(pcb->id, SIGSTOP) == -1) {
//                 printf("Error stopping process\n");
//                 return false;
//             }
//             pcb->remainingTime -= quantum;
//             finishTime = getClk() + quantum;
//             return false;
//         }
//     }
// }


// bool executeProcessRR(PCB *pcb, int quantum) {
//     pid_t pid;
//     int status;

//     // if the process remaining time is equal to runningTime then its the first time
//     if (pcb->remainingTime == pcb->runningTime) {
//         // create a new process
//         pid = fork();
//         if (pid == 0) {
//             // Child process
//             char remainingTimeStr[10];
//             sprintf(remainingTimeStr, "%d", pcb->remainingTime); // convert remaining time to string
//             char *args[] = {"./process.out", remainingTimeStr, NULL};
//             execv(args[0], args);
//             // If execv returns, it must have failed.
//             printf("Unknown command\n");
//             exit(0);
//         } else if (pid > 0) {
//             // Parent process
//             pcb->id = pid;
//             pcb_updateProcessState(pcb, RUNNING); // update the state of the process
//             pcb->startTime = getClk();
//             pcb->lastExecTime = getClk();
//         } else {
//             // Fork failed
//             printf("Fork failed!\n");
//             return false;
//         }
//     } else {
//         pcb->lastExecTime = getClk();
//         // else this process was stopped before and need to be resumed
//         if (kill(pcb->id, SIGCONT) == -1) {
//             printf("Error resuming process\n");
//             return false;
//         }
//     }
// }

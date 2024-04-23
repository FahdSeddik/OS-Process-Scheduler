#include "headers.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // For sleep and system calls
#include <time.h>   // For simulating clock
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <signal.h>
#include "Algorithms/hpf.h"
#include "Algorithms/rr.h"
#include "Algorithms/srtn.h"
#include "Logging/logger.h"
#include "ProcessManagement/message_queue.h"
// structure for message queue holds process data
struct msg_buffer {
    int priority;
    int arrivalTime;
    int runningTime;
} message;

// #==================#
// # GLOBAL VARIABLES #
// #==================#
int processMsgqID = -1;

// files
FILE *schedulerLog = NULL;
FILE *schedulerPerf = NULL;

// function declarations
// void sigusr1Handler(int signum);
// void sigusr2Handler(int signum);
// void openFiles();
// int createMessageQueue();
// void setupSignals();

// #==================#
int main(int argc, char *argv[]) {
    initClk();

    if (argc < 2) {
        perror("Invalid number of arguments");
        exit(-1);
    }

    // processMsgqID = createMessageQueue(); // channel betweeen scheduler and process generator
    // openFiles();
    // setupSignals();
    Logger* logger = loggerInit("./logs/scheduler.log", "./logs/scheduler.perf");
    int msgQueueId = mqCreate("./Keys/key1", 0);

    char *algorithm = argv[1];
    int quantum = 0;

    if (strcmp(algorithm, "HPF") == 0) {
        HPF(msgQueueId, logger);
    } else if (strcmp(algorithm, "SRTN") == 0) {
        SRTN(msgQueueId, logger);
    } else if (strcmp(algorithm, "RR") == 0) {
        if (argc < 3) {
            perror("Invalid number of arguments");
            exit(-1);
        }
        quantum = atoi(argv[2]);
        RR(msgQueueId, quantum, logger);
    } else {
        perror("Invalid algorithm");
        exit(-1);
    }

    loggerWritePerformanceData(logger);

    // cleanup
    destroyClk(true);
    fclose(schedulerLog);
    loggerDestroy(logger);
    fclose(schedulerPerf);
    msgctl(processMsgqID, IPC_RMID, NULL);
    return 0;
}

// // ============================================
// // ============ GENERAL FUNCTIONS =============
// // ============================================
// void openFiles() {
//     // files
//     schedulerLog = fopen("Scheduler.log", "w");
//     if (schedulerLog == NULL) {
//         perror("Error in opening Scheduler.log file!");
//         exit(-1);
//     }

//     schedulerPerf = fopen("Scheduler.perf", "w");
//     if (schedulerPerf == NULL) {
//         perror("Error in opening Scheduler.perf file!");
//         exit(-1);
//     }
// }

// int createMessageQueue() {
//     key_t MSGKEY;
//     MSGKEY = ftok("keyfile", 33); // TODO: change 33 if needed
//     int processMsgqID = msgget(MSGKEY, 0666 | IPC_CREAT);
//     if (processMsgqID == -1) {
//         perror("Error in creating process message queue!");
//         exit(-1);
//     }
//     return processMsgqID;
// }

// void setupSignals() {
//     struct sigaction sa;

//     sa.sa_handler = sigusr1Handler;
//     sigemptyset(&sa.sa_mask);
//     sa.sa_flags = 0;
//     if (sigaction(SIGUSR1, &sa, NULL) == -1) {
//         perror("sigaction");
//         exit(1);
//     }

//     sa.sa_handler = sigusr2Handler;
//     sigemptyset(&sa.sa_mask);
//     sa.sa_flags = 0;
//     if (sigaction(SIGUSR2, &sa, NULL) == -1) {
//         perror("sigaction");
//         exit(1);
//     }
// }

void logPerformance() {
    // TODO: implement
}



// void logProcessExecution(PCB *currentProcessPCB, bool isStopped) {
//     // state is started , stopped, finished, resumed
//     // while the process state is READY, RUNNING, BLOCKED, TERMINATED
//     // map the state to the corresponding string
//     char *state;

//     switch (currentProcessPCB->state) {
//         case READY:
//             state = "started";
//             break;
//         case RUNNING:
//             if (isStopped) {
//                 state = "stopped";
//             } else {
//                 state = "resumed";
//             }
//             break;
//         case BLOCKED:
//             state = "blocked";
//             break;
//         case TERMINATED:
//             state = "finished";
//             break;
//         default:
//             state = "unknown";
//     }

//     if (state == "finished") {
//         pcb_calculateTurnaroundTime(currentProcessPCB);
//         fprintf(schedulerLog, "At time %d process %d state %s arr %d total %d remain %d wait %d TA %d WTA %f\n",
//                 getClk(), currentProcess->id, state, currentProcess->arrivalTime, currentProcess->waitingTime, currentProcess->runningTime, currentProcess->remainingTime, currentProcess->waitingTime, currentProcessPCB->turnaroundTime, currentProcessPCB->weightedTurnaroundTime);
//     }
//     else {
//         fprintf(schedulerLog, "At time %d process %d state %s arr %d total %d remain %d wait %d\n",
//                 getClk(), currentProcess->id, state, currentProcess->arrivalTime, currentProcess->waitingTime, currentProcess->runningTime, currentProcess->remainingTime, currentProcess->waitingTime);
//     }
// }

// bool isAllProcessesCompleted(q_Queue *processQueue) {
//     if (processGeneratorIsDone && isEmpty(processQueue) == 1) {
//         return true;
//     }
//     return false;
// }




// // the process is finished
// void sigusr1Handler(int signum)
// {
//     currentProcess->state = TERMINATED;
//     logProcessExecution(currentProcess);
// }

// // process generator is done
// void sigusr2Handler(int signum)
// {
//     processGeneratorIsDone = true;
// }
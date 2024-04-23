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

int main(int argc, char *argv[]) {
    initClk();

    if (argc < 2) {
        perror("Invalid number of arguments");
        exit(-1);
    }

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
    loggerDestroy(logger);
    return 0;
}

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
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
#include "ProcessManagement/semaphore.h"
#include "clk_utils.h"


Logger* logger = NULL;
void terminate(int signum) {
    destroyClk(true);
    if(logger) loggerDestroy(logger);
    signal(SIGINT, SIG_DFL);
    kill(getpgrp(), SIGINT);
    raise(SIGINT);
}

int main(int argc, char *argv[]) {
    initClk();
    if (argc < 1) {
        perror("Invalid number of arguments");
        exit(-1);
    }
    signal(SIGINT, terminate);
    logger = loggerInit("./logs/scheduler.log", "./logs/scheduler.perf");
    int msgQueueId = mqCreate("./Keys/key1", 0);
    int semSyncRcv = semCreate("./Keys/key1", 0);

    char *algorithm = argv[0];
    int quantum = 0;

    if (strcmp(algorithm, "HPF") == 0) {
        initHPF(msgQueueId, semSyncRcv, logger);
    } else if (strcmp(algorithm, "SRTN") == 0) {
        initSRTN(msgQueueId, semSyncRcv, logger);
    } else if (strcmp(algorithm, "RR") == 0) {
        if (argc < 2) {
            perror("Invalid number of arguments");
            exit(-1);
        }
        quantum = atoi(argv[1]);
        initRR(msgQueueId, semSyncRcv, quantum, logger);
    } else {
        perror("Invalid algorithm");
        exit(-1);
    }

    loggerWritePerformanceData(logger);

    // cleanup
    destroyClk(true);
    loggerDestroy(logger);
    // SIGINT process generator to clear resources
    kill(getppid(), SIGINT);
    return 0;
}
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
#include <errno.h>
#include "./MemoryManagement/buddy.h"

Logger* logger = NULL;
int semSyncTerminate;
bsBuddySystem* buddySystem;
void terminate(int signum) {
    destroyClk(true);
    if(logger) loggerDestroy(logger);
    semDelete(semSyncTerminate);
    signal(SIGINT, SIG_DFL);
    kill(getpgrp(), SIGINT);
    raise(SIGINT);
}

int main(int argc, char *argv[]) {
    initClk();
    if (argc < 1) {
        errno = EINVAL;
        perror("Invalid number of arguments");
        exit(-1);
    }
    signal(SIGINT, terminate);
    logger = loggerInit("./logs/scheduler.log", "./logs/scheduler.perf");
    int msgQueueId = mqCreate("./Keys/key1", 0);
    int semSyncRcv = semCreate("./Keys/key1", 0);
    semSyncTerminate = semCreate("./Keys/key1", 2);
    semInitialize(semSyncTerminate, 0);
    char *algorithm = argv[0];
    int quantum = 0;
    buddySystem = bsCreate();

    if (strcmp(algorithm, "HPF") == 0) {
        initHPF(msgQueueId, semSyncRcv, semSyncTerminate, logger, buddySystem);
    } else if (strcmp(algorithm, "SRTN") == 0) {
        initSRTN(msgQueueId, semSyncRcv, semSyncTerminate, logger, buddySystem);
    } else if (strcmp(algorithm, "RR") == 0) {
        if (argc < 2) {
            errno = EINVAL;
            perror("Invalid number of arguments");
            exit(-1);
        }
        quantum = atoi(argv[1]);
        initRR(msgQueueId, semSyncRcv, semSyncTerminate, quantum, logger, buddySystem);
    } else {
        errno = EINVAL;
        perror("Invalid algorithm");
        exit(-1);
    }

    loggerWritePerformanceData(logger);

    // cleanup
    destroyClk(true);
    loggerDestroy(logger);
    semDelete(semSyncTerminate);
    bsDestroy(buddySystem);
    // SIGINT process generator to clear resources
    kill(getppid(), SIGINT);
    return 0;
}
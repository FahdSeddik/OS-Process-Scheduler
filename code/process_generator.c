#include "headers.h"
#include "clk_utils.h"
#include "ProcessManagement/message_queue.h"
#include "ProcessManagement/semaphore.h"
#include "ProcessManagement/process_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

void clearResources(int);
void readInput(ProcessMessage **processes, int *processNum, const char* filename);
bool validateArguments(int argc, char *argv[]);
int msgQueueId, semSyncRcv, semClockAwake;
ProcessMessage* processes;
int main(int argc, char * argv[]) {
    if (!validateArguments(argc, argv)) {
        return EXIT_FAILURE;
    }
    signal(SIGINT, clearResources);
    processes = NULL;
    int processNum = 0;
    readInput(&processes, &processNum, argv[1]);
    msgQueueId = mqCreate("./Keys/key1", 0);
    semSyncRcv = semCreate("./Keys/key1", 0);
    semClockAwake = semCreate("./Keys/key1", 1);
    semInitialize(semClockAwake, 0);
    semInitialize(semSyncRcv, 0);
    pmRunProcess("./build/clk.out", NULL, NULL, NULL);
    initClk();
    int sent = 0;
    char** schedArgv = (char **)malloc((argc - 1) * sizeof(char *));;
    schedArgv[0] = argv[2];
    schedArgv[1] = NULL;
    if (argc > 3) {
        schedArgv[1] = argv[3];
        schedArgv[2] = NULL;
    }
    int schdPID = pmRunProcess("./build/scheduler.out", schedArgv, NULL, NULL);
    while (true) {
        semDown(semClockAwake);
        int time = getClk();
        printf("PGEN At %d\n", time);
        while (sent < processNum && processes[sent].arrivalTime == time) {
            // printf("\tSENT id: %d\n", processes[sent].id);
            mqSend(msgQueueId, processes[sent]);
            sent++;
            if (sent == processNum) {
                ProcessMessage dummy;
                dummy.mtype = 1, dummy.id = -1;
                mqSend(msgQueueId, dummy);
            }
        }
        semUp(semSyncRcv);
    }
    raise(SIGINT);
}

void readInput(ProcessMessage **processes, int *processNum, const char* filename) {
    FILE *file = fopen(filename, "r");

    if (!file) {
        perror("Unable to open the file");
        raise(SIGINT);
    }

    // Skipping the header line
    char buffer[256];
    if (fgets(buffer, sizeof(buffer), file) == NULL) {
        fclose(file);
        perror("Failed to read the header line");
        raise(SIGINT);
    }

    ProcessMessage temp;
    int count = 0;
    size_t capacity = 10;  // Initial capacity
    *processes = malloc(capacity * sizeof(ProcessMessage));

    if (*processes == NULL) {
        perror("Failed to allocate memory for processes");
        fclose(file);
        raise(SIGINT);
    }

    while (fscanf(file, "%d %d %d %d %d", &temp.id, &temp.arrivalTime, &temp.runningTime, &temp.priority, &temp.memsize) == 5) {
        if (count >= capacity) {
            capacity *= 2;  // Double the capacity
            *processes = realloc(*processes, capacity * sizeof(ProcessMessage));
            if (*processes == NULL) {
                perror("Failed to reallocate memory for processes");
                fclose(file);
                raise(SIGINT);
            }
        }
        (*processes)[count] = temp;
        (*processes)[count].mtype = 1;  // All messages use the same message type
        count++;
    }

    *processNum = count;
    fclose(file);
}

bool validateArguments(int argc, char *argv[]) {
    if (argc < 3) {
        errno = EINVAL;
        perror("Invalid number of arguments");
        return false;
    }

    if (access(argv[1], F_OK) == -1) {
        errno = ENOENT;
        perror("File does not exist");
        return false;
    }

    // Check that argv[2] is either HPF, SRTN, or RR
    if (strcmp(argv[2], "HPF") != 0 && strcmp(argv[2], "SRTN") != 0 && strcmp(argv[2], "RR") != 0) {
        errno = EINVAL;
        perror("Invalid scheduling algorithm");
        return false;
    }

    if (strcmp(argv[2], "RR") == 0 && argc < 4) {
        errno = EINVAL;
        perror("Invalid number of arguments");
        return false;
    }

    // Check that the quantum is a positive integer
    if (strcmp(argv[2], "RR") == 0 && atoi(argv[3]) <= 0) {
        errno = EINVAL;
        perror("Invalid quantum value");
        return false;
    }

    return true;
}

void clearResources(int signum) {
    semDelete(semSyncRcv);
    semDelete(semClockAwake);
    mqDelete(msgQueueId);
    free(processes);
    signal(SIGINT, SIG_DFL);
    destroyClk(true);
}

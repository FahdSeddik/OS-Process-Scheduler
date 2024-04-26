#include "headers.h"
#include "clk_utils.h"
#include "ProcessManagement/message_queue.h"
#include "ProcessManagement/semaphore.h"
#include "ProcessManagement/process_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void clearResources(int);
void promptUser(char ***argv, int *argc);
void readInput(ProcessMessage **processes, int *processNum);
int msgQueueId, semSyncRcv, semClockAwake;
ProcessMessage* processes;
int main(int argc, char * argv[])
{
    signal(SIGINT, clearResources);
    char** userArgv;
    int userArgc;
    promptUser(&userArgv, &userArgc);
    processes = NULL;
    int processNum = 0;
    readInput(&processes, &processNum);
    msgQueueId = mqCreate("./Keys/key1", 0);
    semSyncRcv = semCreate("./Keys/key1", 0);
    semClockAwake = semCreate("./Keys/key1", 1);
    semInitialize(semClockAwake, 0);
    semInitialize(semSyncRcv, 0);
    pmRunProcess("./build/clk.out", NULL, NULL, NULL);
    initClk();
    int sent = 0;
    int schdPID = pmRunProcess("./build/scheduler.out", userArgv, NULL, NULL);
    while (true) {
        semDown(semClockAwake);
        int time = getClk();
        printf("PGEN At %d\n", time);
        while(sent < processNum && processes[sent].arrivalTime == time) {
            printf("\tSENT id: %d\n", processes[sent].id);
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

void promptUser(char ***argv, int *argc) {
    printf("Welcome! Please choose a scheduling algorithm:\n");
    printf("1. Highest Priority First (HPF)\n");
    printf("2. Shortest Remaining Time Next (SRTN)\n");
    printf("3. Round Robin (RR)\n");

    int choice;
    printf("Enter your choice (1-3): ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            *argc = 1; // Number of arguments excluding the command itself
            *argv = (char **)malloc((*argc + 1) * sizeof(char *));
            (*argv)[0] = "HPF"; // Command name
            (*argv)[1] = NULL; // Null-terminated array of arguments
            break;
        case 2:
            *argc = 1;
            *argv = (char **)malloc((*argc + 1) * sizeof(char *));
            (*argv)[0] = "SRTN";
            (*argv)[1] = NULL;
            break;
        case 3:
            // For RR, ask the user for the time quantum
            *argc = 2;
            *argv = (char **)malloc((*argc + 1) * sizeof(char *));
            (*argv)[0] = "RR";
            char *quantumStr = malloc(20); // Allocate memory for the quantum string
            printf("Enter the time quantum for Round Robin: ");
            scanf("%d", &choice);
            sprintf(quantumStr, "%d", choice); // Convert integer to string
            (*argv)[1] = quantumStr; // Assign the string to the arguments array
            (*argv)[2] = NULL;
            break;
        default:
            printf("Invalid choice. Please choose a number between 1 and 3.\n");
            exit(EXIT_FAILURE);
    }
}

void readInput(ProcessMessage **processes, int *processNum) {
    const char *filename = "./processes.txt";
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

    while (fscanf(file, "%d %d %d %d", &temp.id, &temp.arrivalTime, &temp.runningTime, &temp.priority) == 4) {
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


void clearResources(int signum) {
    //TODO Clears all resources in case of interruption
    semDelete(semSyncRcv);
    semDelete(semClockAwake);
    mqDelete(msgQueueId);
    free(processes);
    signal(SIGINT, SIG_DFL);
    destroyClk(true);
}

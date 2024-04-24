#include "headers.h"
#include "clk_utils.h"
#include "ProcessManagement/message_queue.h"
#include "ProcessManagement/semaphore.h"
#include "ProcessManagement/process_manager.h"
#include <stdio.h>
#include <stdlib.h>

void clearResources(int);
void promptUser(char ***argv, int *argc);
void readInput(ProcessMessage **processes, int *processNum);
int msgQueueId,semSyncRcv;
ProcessMessage* processes;
int main(int argc, char * argv[])
{
    signal(SIGINT, clearResources);
    // TODO Initialization
    // 1. Read the input files.
    // 2. Ask the user for the chosen scheduling algorithm and its parameters, if there are any.
    // 3. Initiate and create the scheduler and clock processes.
    // 4. Use this function after creating the clock process to initialize clock
    initClk();
    // TODO Generation Main Loop
    // 5. Create a data structure for processes and provide it with its parameters.
    // 6. Send the information to the scheduler at the appropriate time.
    // 7. Clear clock resources

    // Pseudo code: (could be modified)
    // 1. Ask user which algorithm do they want using a function (HPF, SRTN, RR with quantum)
    // 2. Read Input file using a function
    // 3. Create messageQueue using mqCreate() // sync with who wrote the scheduler to know params
    // 4. Create a semaphore to be used in synchronization using semCreate() // sync with who wrote the scheduler to know params
    // 5. Initiate scheduler using pmRunProcess sending it params read from user input (algorithm, quantum (if applicable))
    // 6. Call program loop function that would do the following:
    //      1. Have a while loop with a condition that it still didnt finish all processes
    //      2. Should down the semaphore to ensure that if you have something to send in message queue, the scheduler will wait for it (scheduler has up)
    //      3. Should send *all* processes that arrive at the current clock using the message queue mqSend()
    //      4. Should wait until next clock using one-line busy wait while(time == getClk());
    char** userArgv;
    int userArgc;
    promptUser(&userArgv, &userArgc);
    processes = NULL;
    int processNum = 0;
    readInput(&processes, &processNum);
    msgQueueId = mqCreate("./Keys/key1", 0);
    semSyncRcv = semCreate("./Keys/key1", 0);
    semInitialize(semSyncRcv, 0);
    int time = getClk();
    int sent = 0;
    int schdPID = pmRunProcess(NULL, "./build/scheduler.out", userArgv);
    while (sent < processNum) {
        while(processes[sent].arrivalTime == time) {
            mqSend(msgQueueId, processes[sent]);
            sent++;
        }
        semUp(semSyncRcv);
        while(time == getClk());
        time = getClk();
    }
    ProcessMessage finished;
    finished.id=-1;
    mqSend(msgQueueId, finished);
    int status;
    while (waitpid(schdPID, &status, WNOHANG) == 0);
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
            (*argv)[2] = NULL;
            printf("Enter the time quantum for Round Robin: ");
            scanf("%d", &choice);
            sprintf((*argv)[1], "%d", choice); // Convert integer to string
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
        exit(EXIT_FAILURE);
    }

    // Skipping the header line
    char buffer[256];
    if (fgets(buffer, sizeof(buffer), file) == NULL) {
        fclose(file);
        perror("Failed to read the header line");
        exit(EXIT_FAILURE);
    }

    ProcessMessage temp;
    int count = 0;
    size_t capacity = 10;  // Initial capacity
    *processes = malloc(capacity * sizeof(ProcessMessage));

    if (*processes == NULL) {
        perror("Failed to allocate memory for processes");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    while (fscanf(file, "%d %d %d %d", &temp.id, &temp.arrivalTime, &temp.runningTime, &temp.priority) == 4) {
        if (count >= capacity) {
            capacity *= 2;  // Double the capacity
            *processes = realloc(*processes, capacity * sizeof(ProcessMessage));
            if (*processes == NULL) {
                perror("Failed to reallocate memory for processes");
                fclose(file);
                exit(EXIT_FAILURE);
            }
        }
        (*processes)[count] = temp;
        (*processes)[count].mtype = 1;  // All messages use the same message type
        count++;
    }

    *processNum = count;
    fclose(file);
}


void clearResources(int signum)
{
    //TODO Clears all resources in case of interruption
    semDelete(semSyncRcv);
    mqDelete(msgQueueId);
    free(processes);
    signal(SIGINT, SIG_DFL);
    destroyClk(true);
    raise(SIGINT);
}

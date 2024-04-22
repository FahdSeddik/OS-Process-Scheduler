#include "headers.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // For sleep and system calls
#include <time.h>   // For simulating clock
#include "Dependencies/queue.h"
#include "Dependencies/pcb.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

// structure for message queue holds process data
struct msg_buffer
{
    int priority;
    int arrivalTime;
    int runningTime;
} message;

// = GLOBAL VARIABLES = //
// =================== //
int processMsgqID = -1;

// files
FILE *schedulerLog = NULL;
FILE *schedulerPerf = NULL;

// function declarations
void sigusr1Handler(int signum);
void sigusr2Handler(int signum);
void openFiles();
int create_message_queue();
void setupSignals();

// ================
int main(int argc, char *argv[])
{
    initClk();

    if (argc < 2)
    {
        perror("Invalid number of arguments");
        exit(-1);
    }

    processMsgqID = create_message_queue(); // channel betweeen scheduler and process generator
    openFiles();
    setupSignals();

    fprintf(schedulerLog, "#At time x process y state arr w total z remain y wait k\n");

    char *algorithm = argv[1];
    int quantum = 0;

    if (strcmp(algorithm, "HPF") == 0)
    {
        HPF(&processTable);
    }
    else if (strcmp(algorithm, "SRTN") == 0)
    {
        SRTN(&processTable);
    }
    else if (strcmp(algorithm, "RR") == 0)
    {
        if (argc < 3)
        {
            perror("Invalid number of arguments");
            exit(-1);
        }
        quantum = atoi(argv[2]);
        q_Queue *processTable = createQueue();
        RRSimulation(&processTable, quantum);
    }
    else
    {
        perror("Invalid algorithm");
    }

    // cleanup
    destroyClk(true);
    free(processTable);
    fclose(schedulerLog);
    fclose(schedulerPerf);
    msgctl(processMsgqID, IPC_RMID, NULL);
    return 0;
}

// ============================================
// ============ GENERAL FUNCTIONS =============
// ============================================
void openFiles()
{
    // files
    schedulerLog = fopen("Scheduler.log", "w");
    if (schedulerLog == NULL)
    {
        perror("Error in opening Scheduler.log file!");
        exit(-1);
    }

    schedulerPerf = fopen("Scheduler.perf", "w");
    if (schedulerPerf == NULL)
    {
        perror("Error in opening Scheduler.perf file!");
        exit(-1);
    }
}

int create_message_queue()
{
    key_t MSGKEY;
    MSGKEY = ftok("keyfile", 33); // TODO: change 33 if needed
    int processMsgqID = msgget(MSGKEY, 0666 | IPC_CREAT);
    if (processMsgqID == -1)
    {
        perror("Error in creating process message queue!");
        exit(-1);
    }
    return processMsgqID;
}

void setupSignals()
{
    struct sigaction sa;

    sa.sa_handler = sigusr1Handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGUSR1, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(1);
    }

    sa.sa_handler = sigusr2Handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGUSR2, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(1);
    }
}

void logPerformance()
{
    // TODO: implement
}

// ============================================
// ================== RR ======================
// ============================================
void RRSimulation(q_Queue *processQueue, int quantum)
{
    int clk = 0;
    bool processGeneratorIsDone = false; // process generator is done
    PCB *currentProcessPCB = NULL;
    int nextSwtichTime = 0; // process switching time

    while (!isAllProcessesCompleted(processQueue))
    {
        clk = getClk();
        receiveNewProcesses();
        if (clk == nextSwtichTime) // you can exec the next one
        {
            currentProcessPCB = q_dequeue(processQueue);
            if (currentProcessPCB != NULL)
            {
                logProcessExecution(currentProcessPCB, false);
                executeProcessRR(currentProcessPCB, quantum);
                bool willFinish = isProcessFinishNextSwitch(currentProcessPCB, &nextSwtichTime);
                // if the process is not finished add it to the queue
                if (!willFinish)
                    q_enqueue(processQueue, currentProcessPCB);
            }
        }
    }
}

// checks if there are new processes in the message queue
// and adds them to the ready queue
void receiveNewProcesses()
{
    struct message processReceived;
    while (1)
    {
        // receive a message from the queue
        if (msgrcv(processMsgqID, &processReceived, sizeof(processReceived), 0, IPC_NOWAIT) != -1)
        {
            // create a new process
            PCB *newProcessPCB = (PCB *)malloc(sizeof(PCB));
            pcb_initPCB(newProcessPCB, -1, processReceived.priority, processReceived.arrivalTime, processReceived.runningTime);
            // add the process to the ready list
            q_enqueue(processQueue, newProcess); // TODO: enque should accept process
        }
        else
        {
            break;
        }
    }
}

void logProcessExecution(PCB *currentProcessPCB, bool isStopped)
{
    // state is started , stopped, finished, resumed
    // while the process state is READY, RUNNING, BLOCKED, TERMINATED
    // map the state to the corresponding string
    char *state;

    switch (currentProcessPCB->state)
    {
    case READY:
        state = "started";
        break;
    case RUNNING:
        if (isStopped)
            state = "stopped";
        else
            state = "resumed";
        break;
    case BLOCKED:
        state = "blocked";
        break;
    case TERMINATED:
        state = "finished";
        break;
    default:
        state = "unknown";
    }

    if (state == "finished")
    {
        pcb_calculateTurnaroundTime(currentProcessPCB);
        fprintf(schedulerLog, "At time %d process %d state %s arr %d total %d remain %d wait %d TA %d WTA %f\n",
                getClk(), currentProcess->id, state, currentProcess->arrivalTime, currentProcess->waitingTime, currentProcess->runningTime, currentProcess->remainingTime, currentProcess->waitingTime, currentProcessPCB->turnaroundTime, currentProcessPCB->weightedTurnaroundTime);
    }
    else
    {
        fprintf(schedulerLog, "At time %d process %d state %s arr %d total %d remain %d wait %d\n",
                getClk(), currentProcess->id, state, currentProcess->arrivalTime, currentProcess->waitingTime, currentProcess->runningTime, currentProcess->remainingTime, currentProcess->waitingTime);
    }
}

bool isAllProcessesCompleted(q_Queue *processQueue)
{
    if (processGeneratorIsDone && isEmpty(processQueue) == 1)
    {
        return true;
    }
    return false;
}

bool executeProcessRR(PCB *pcb, int quantum)
{
    pid_t pid;
    int status;

    // if the process remaining time is equal to runningTime then its the first time
    if (pcb->remainingTime == pcb->runningTime)
    {
        // create a new process
        pid = fork();
        if (pid == 0)
        {
            // Child process
            char remainingTimeStr[10];
            sprintf(remainingTimeStr, "%d", pcb->remainingtime); // convert remaining time to string
            char *args[] = {"./process.out", remainingTimeStr, NULL};
            execv(args[0], args);
            // If execv returns, it must have failed.
            printf("Unknown command\n");
            exit(0);
        }
        else if (pid > 0)
        {
            // Parent process
            pcb->id = pid;
            pcb_updateProcessState(pcb, RUNNING); // update the state of the process
            pcb->startTime = getClk();
            pcb->lastExecTime = getClk();
        }
        else
        {
            // Fork failed
            printf("Fork failed!\n");
            return false;
        }
    }
    else
    {
        pcb->lastExecTime = getClk();
        // else this process was stopped before and need to be resumed
        if (kill(pcb->id, SIGCONT) == -1)
        {
            printf("Error resuming process\n");
            return false;
        }
    }
}

/**
 * Checks the status of the process and updates the remaining time
 * returns true if the process will be finished
 */
bool isProcessFinishNextSwitch(PCB *pcb, int *finishTime)
{
    // then check if the remaining time is less than the quantum
    if (pcb->remainingTime <= quantum)
    {
        // willFinish = true the process will be finished and don't add it to the queue
        finishTime = getClk() + pcb->remainingTime;
        return true;
    }
    else
    {
        if (getClk() - pcb->lastExecTime >= quantum)
        {
            logProcessExecution(pcb, true);
            // if the process was running for the whole quantum then stop it and return false
            if (kill(pcb->id, SIGSTOP) == -1)
            {
                printf("Error stopping process\n");
                return false;
            }
            pcb->remainingTime -= quantum;
            finishTime = getClk() + quantum;
            return false;
        }
    }
}

// the process is finished
void sigusr1Handler(int signum)
{
    currentProcess->state = TERMINATED;
    logProcessExecution(currentProcess);
}

// process generator is done
void sigusr2Handler(int signum)
{
    processGeneratorIsDone = true;
}
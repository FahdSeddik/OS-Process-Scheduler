#include "headers.h"

void clearResources(int);

int main(int argc, char * argv[])
{
    signal(SIGINT, clearResources);
    // TODO Initialization
    // 1. Read the input files.
    // 2. Ask the user for the chosen scheduling algorithm and its parameters, if there are any.
    // 3. Initiate and create the scheduler and clock processes.
    // 4. Use this function after creating the clock process to initialize clock
    initClk();
    // To get time use this
    int x = getClk();
    printf("current time is %d\n", x);
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

    destroyClk(true);
}

void clearResources(int signum)
{
    //TODO Clears all resources in case of interruption
}

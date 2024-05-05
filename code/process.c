#include "headers.h"
#include "clk_utils.h"
#include "ProcessManagement/semaphore.h"
/* Modify this file as needed*/
int remainingtime, semSyncTerminate;
bool shouldCount = true;
int time = 0;

void stopProcess(int signum) {
    // printf("Preempt\n");
    shouldCount = false;
    raise(SIGSTOP);
}

void continueProcess(int signum) {
    // printf("Cont timeprev=%d\n", time);
    shouldCount = true;
    time = getClk();
}

void finish(int signum) {
    semUp(semSyncTerminate);
    kill(getppid(), SIGUSR1);
    signal(SIGINT, SIG_DFL);
    destroyClk(false);
    raise(SIGINT);
}

int main(int agrc, char * argv[])
{
    initClk();
    signal(SIGUSR1, stopProcess);
    signal(SIGUSR2, continueProcess);
    signal(SIGINT, finish);
    remainingtime = atoi(argv[0]);
    time = getClk();
    semSyncTerminate = semCreate("./Keys/key1", 2);
    while (remainingtime > 0) {
        if(!shouldCount) continue;
        while(time == getClk());
        int curTime = getClk();
        remainingtime -= curTime - time;
        time = curTime;
        // printf("In Process: %d, rem= %d\n", getpid(), remainingtime);
    }
    raise(SIGINT);
    return 0;
}

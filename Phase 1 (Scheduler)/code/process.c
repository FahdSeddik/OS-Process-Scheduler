#include "headers.h"
#include "clk_utils.h"
/* Modify this file as needed*/
int remainingtime;
bool shouldCount = true;
int time = 0;

void stopProcess(int signum) {
    shouldCount = false;
    raise(SIGSTOP);
}

void continueProcess(int signum) {
    shouldCount = true;
    time = getClk();
}

void finish(int signum) {
    // TODO: some logic m4 3aref lw hane7tag wala la2
    
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
    remainingtime = atoi(argv[1]);
    time = getClk();
    while (remainingtime > 0) {
        if(!shouldCount) continue;
        while(time == getClk());
        int curTime = getClk();
        remainingtime -= curTime - time;
        time = curTime;
    }
    raise(SIGINT);
    return 0;
}

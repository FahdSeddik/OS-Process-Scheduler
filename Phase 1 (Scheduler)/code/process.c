#include "headers.h"

/* Modify this file as needed*/
int remainingtime;

void timeStep(int signum) {
    remainingtime--;
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
    signal(SIGUSR1, timeStep);
    signal(SIGINT, finish);
    remainingtime = atoi(argv[1]);
    while (remainingtime > 0);
    raise(SIGINT);
    return 0;
}

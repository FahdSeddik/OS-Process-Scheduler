#include "headers.h"

int remainingtime;

int main(int argc, char *argv[])
{
    initClk();

    // Check if the command-line argument is provided
    if (argc < 2)
    {
        printf("Error: remaining time not provided\n");
        return 1;
    }

    // Convert the command-line argument to integer
    remainingtime = atoi(argv[1]);

    // Get the start time
    int starttime = getClk();

    while (getClk() - starttime < remainingtime)
    {
        // Do nothing, just wait
    }

    kill(getppid(), SIGUSR1);

    destroyClk(false);

    return 0;
}
#include "process_manager.h"
#include <signal.h>
#include <stdio.h>

pid_t pmRunProcess(const char* programName, int processId, Logger* logger) {
    // TODO: perform logging here
    // Hint: you may need to adjust parameters/include extra files

    pid_t pid = fork();

    if (pid == -1) {
        // Fork failed
        perror("Failed to fork process");
        return -1;
    } else if (pid == 0) {
        // Child process
        execl(programName, programName, NULL);
        // If execl returns, there was an error
        perror("Failed to execute process");
        _exit(1);
    }

    // Only parent process reaches here
    return pid;
}

int pmPreemptProcess(pid_t processId, Logger* logger) {
    // TODO: perform logging here
    // Hint: you may need to adjust parameters/include extra files
    return kill(processId, SIGSTOP);
}

int pmStopProcess(pid_t processId, Logger* logger) {
    // TODO: perform logging here
    // Hint: you may need to adjust parameters/include extra files
    return kill(processId, SIGSTOP);
}

int pmContinueProcess(pid_t processId, Logger* logger) {
    // TODO: perform logging here
    // Hint: you may need to adjust parameters/include extra files
    return kill(processId, SIGCONT);
}

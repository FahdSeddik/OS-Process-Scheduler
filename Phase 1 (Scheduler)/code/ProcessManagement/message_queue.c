#include "message_queue.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>

int mqCreate(void) {
    return msgget(IPC_PRIVATE, 0666 | IPC_CREAT);
}

int mqSend(int msgQId, ProcessMessage message) {
    if (msgsnd(msgQId, &message, sizeof(ProcessMessage) - sizeof(long), 0) == -1) {
        perror("Failed to send message");
        return -1;
    }
    return 0;
}

int mqReceiveNonBlocking(int msgQId, ProcessMessage *message) {
    int ret = msgrcv(msgQId, message, sizeof(ProcessMessage) - sizeof(long), MESSAGE_MTYPE, IPC_NOWAIT);
    if (ret == -1) {
        if (errno == ENOMSG) {
            // No message available
            return 1;
        }
        perror("Failed to receive message");
        return -1;
    }
    return 0;
}

int mqDelete(int msgQId) {
    if (msgctl(msgQId, IPC_RMID, NULL) == -1) {
        perror("Failed to delete message queue");
        return -1;
    }
    return 0;
}

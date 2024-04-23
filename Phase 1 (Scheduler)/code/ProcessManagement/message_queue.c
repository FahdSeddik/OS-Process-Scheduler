#include "message_queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int mqCreate(const char* path, int id) {
    key_t key;
    int msgid;

    // Generate a unique key from the given path and project identifier
    key = ftok(path, id);
    if (key == -1) {
        perror("ftok error");
        return -1;
    }

    // Attempt to create the message queue, or access it if it already exists
    msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget error");
        return -1;
    }

    return msgid;
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

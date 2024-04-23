#include "semaphore.h"
#include <stdio.h>
#include <stdlib.h>

int semCreate(const char* path, int id) {
    key_t key;
    int semid;

    // Generate a unique key
    key = ftok(path, id);
    if (key == -1) {
        perror("ftok error");
        return -1;
    }

    // Create semaphore or access it if it already exists
    semid = semget(key, 1, 0666 | IPC_CREAT);
    if (semid == -1) {
        perror("semget error");
        return -1;
    }

    return semid;
}

int semInitialize(int semid, int value) {
    union semun {
        int val;
        struct semid_ds *buf;
        unsigned short *array;
    } arg;
    arg.val = value;

    if (semctl(semid, 0, SETVAL, arg) == -1) {
        perror("semctl error");
        return -1;
    }

    return 0;
}

int semDown(int semid) {
    struct sembuf op = {0, -1, 0}; // P operation: subtract 1

    if (semop(semid, &op, 1) == -1) {
        perror("semop error");
        return -1;
    }

    return 0;
}

int semUp(int semid) {
    struct sembuf op = {0, 1, 0}; // V operation: add 1

    if (semop(semid, &op, 1) == -1) {
        perror("semop error");
        return -1;
    }

    return 0;
}

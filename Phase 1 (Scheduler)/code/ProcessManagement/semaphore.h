#ifndef SEMAPHORE_MANAGER_H
#define SEMAPHORE_MANAGER_H

#include <sys/ipc.h>
#include <sys/sem.h>

/**
 * Creates a new semaphore or accesses an existing one using a unique key generated by ftok.
 * @param path The path used as a basis for generating the IPC key.
 * @param id The project identifier for the IPC key.
 * @return `int` The semaphore identifier on success, or -1 on error.
 */
int semCreate(const char* path, int id);

/**
 * Initializes a semaphore to a specified value.
 * @param semid The semaphore identifier.
 * @param value The initial value of the semaphore.
 * @return `int` 0 on success, or -1 on error.
 */
int semInitialize(int semid, int value);

/**
 * Performs a P operation (wait) on the semaphore, decrementing its value by one.
 * @param semid The semaphore identifier.
 * @return `int` 0 on success, or -1 on error.
 */
int semDown(int semid);

/**
 * Performs a V operation (signal) on the semaphore, incrementing its value by one.
 * @param semid The semaphore identifier.
 * @return `int` 0 on success, or -1 on error.
 */
int semUp(int semid);

#endif // SEMAPHORE_MANAGER_H

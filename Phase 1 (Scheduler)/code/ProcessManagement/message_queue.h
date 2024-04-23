#ifndef MESSAGE_QUEUE_H
#define MESSAGE_QUEUE_H

#include <sys/ipc.h>
#include <sys/msg.h>

#define MESSAGE_MTYPE 1  // This is a default message type for simple examples

/**
 * Structure to hold process data to be sent via message queues.
 */
typedef struct {
    long mtype;         // Message type, must be > 0
    int processId;      // Process ID
    int arrivalTime;    // Arrival time of the process
    int runningTime;    // Running time of the process
    int priority;       // Priority of the process
} ProcessMessage;

/**
 * Creates a message queue and returns its ID.
 * @return int Message queue ID, -1 on error.
 */
int mqCreate(void);

/**
 * Sends a process message to the specified message queue.
 * @param msgQId The message queue ID.
 * @param message The ProcessMessage to send.
 * @return int 0 on success, -1 on failure.
 */
int mqSend(int msgQId, ProcessMessage message);

/**
 * Receives a process message from the specified message queue without blocking.
 * @param msgQId The message queue ID.
 * @param message Pointer to where the received message should be stored.
 * @return int 0 if message received, -1 on error, 1 if no message is available.
 */
int mqReceiveNonBlocking(int msgQId, ProcessMessage *message);

/**
 * Deletes the specified message queue.
 * @param msgQId The message queue ID.
 * @return int 0 on success, -1 on failure.
 */
int mqDelete(int msgQId);

#endif // MESSAGE_QUEUE_H

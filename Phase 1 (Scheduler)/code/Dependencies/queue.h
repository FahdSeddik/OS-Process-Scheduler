#ifndef Q_QUEUE_H
#define Q_QUEUE_H
#include "bool.h"

/**
 * Structure representing a node in the queue.
 */
typedef struct qQueueNode {
    int processId;            // Process ID stored in the queue
    struct qQueueNode *next; // Pointer to the next node in the queue
} qQueueNode;

/**
 * Structure representing the queue, with pointers to the front and rear nodes.
 */
typedef struct qQueue {
    qQueueNode *front; // Pointer to the front node of the queue
    qQueueNode *rear;  // Pointer to the rear node of the queue
} qQueue;

// Function prototypes

/**
 * Allocates and initializes a new empty queue.
 * @return A pointer to the initialized queue or NULL if memory allocation fails.
 */
qQueue *qCreate();

/**
 * Adds a new node with the given process ID to the end of the queue.
 * @param queue Pointer to the queue where the node will be added.
 * @param processId The process ID to store in the new node.
 */
void qEnqueue(qQueue *queue, int processId);

/**
 * Removes the node from the front of the queue and returns its process ID.
 * @param queue Pointer to the queue.
 * @return The process ID of the dequeued node, or -1 if the queue is empty.
 */
int qDequeue(qQueue *queue);

/**
 * Retrieves the process ID of the front node of the queue without removing it.
 * @param queue Pointer to the queue.
 * @return The process ID of the front node, or -1 if the queue is empty.
 */
int qGetFront(const qQueue *queue);

/**
 * Frees all nodes in the queue and the queue structure itself.
 * 
 * @param queue Pointer to the queue to be freed.
 */
void qFree(qQueue *queue);

/**
 * Checks if the queue is empty.
 * 
 * @param queue Pointer to the queue.
 */
bool qIsEmpty(const qQueue *queue);

#endif // QUEUE_H

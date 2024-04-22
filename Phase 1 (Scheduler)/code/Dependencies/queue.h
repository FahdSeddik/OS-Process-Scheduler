#ifndef Q_QUEUE_H
#define Q_QUEUE_H

/**
 * Structure representing a node in the queue.
 */
typedef struct q_QueueNode
{
    int processId;            // Process ID stored in the queue
    struct q_QueueNode *next; // Pointer to the next node in the queue
} q_QueueNode;

/**
 * Structure representing the queue, with pointers to the front and rear nodes.
 */
typedef struct q_Queue
{
    q_QueueNode *front; // Pointer to the front node of the queue
    q_QueueNode *rear;  // Pointer to the rear node of the queue
} q_Queue;

// Function prototypes

/**
 * Allocates and initializes a new empty queue.
 * @return A pointer to the initialized queue or NULL if memory allocation fails.
 */
q_Queue *q_createQueue();

/**
 * Adds a new node with the given process ID to the end of the queue.
 * @param queue Pointer to the queue where the node will be added.
 * @param processId The process ID to store in the new node.
 */
void q_enqueue(q_Queue *queue, int processId);

/**
 * Removes the node from the front of the queue and returns its process ID.
 * @param queue Pointer to the queue.
 * @return The process ID of the dequeued node, or -1 if the queue is empty.
 */
int q_dequeue(q_Queue *queue);

/**
 * Retrieves the process ID of the front node of the queue without removing it.
 * @param queue Pointer to the queue.
 * @return The process ID of the front node, or -1 if the queue is empty.
 */
int q_getFront(const q_Queue *queue);

/**
 * Frees all nodes in the queue and the queue structure itself.
 * @param queue Pointer to the queue to be freed.
 */
void q_freeQueue(q_Queue *queue);

/**
 * Checks if the queue is empty.
 */
bool q_isEmpty(const q_Queue *queue);

#endif // QUEUE_H

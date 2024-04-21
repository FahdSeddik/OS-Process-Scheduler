#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

q_Queue* q_createQueue() {
    q_Queue *queue = (q_Queue *) malloc(sizeof(q_Queue));
    if (!queue) {
        fprintf(stderr, "Unable to allocate memory for queue\n");
        return NULL;
    }
    queue->front = queue->rear = NULL;
    return queue;
}

void q_enqueue(q_Queue *queue, int processId) {
    q_QueueNode *newNode = (q_QueueNode *) malloc(sizeof(q_QueueNode));
    if (!newNode) {
        fprintf(stderr, "Unable to allocate memory for queue node\n");
        return;
    }
    newNode->processId = processId;
    newNode->next = NULL;

    if (queue->rear == NULL) {
        queue->front = queue->rear = newNode;
        return;
    }

    queue->rear->next = newNode;
    queue->rear = newNode;
}

int q_dequeue(q_Queue *queue) {
    if (queue->front == NULL) {
        fprintf(stderr, "Queue is empty\n");
        return -1;  // Indicate queue is empty
    }

    q_QueueNode *temp = queue->front;
    int processId = temp->processId;
    queue->front = queue->front->next;

    if (queue->front == NULL)
        queue->rear = NULL;

    free(temp);
    return processId;
}

int q_getFront(const q_Queue *queue) {
    if (queue->front == NULL) {
        fprintf(stderr, "Attempt to get front from an empty queue\n");
        return -1; // Indicating the queue is empty
    }
    return queue->front->processId;
}

void q_freeQueue(q_Queue *queue) {
    while (queue->front != NULL) {
        q_QueueNode *temp = queue->front;
        queue->front = queue->front->next;
        free(temp);
    }
    free(queue);
}

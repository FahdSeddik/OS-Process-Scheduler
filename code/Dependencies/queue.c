#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

qQueue *qCreate() {
    qQueue *queue = (qQueue *)malloc(sizeof(qQueue));
    if (!queue) {
        fprintf(stderr, "Unable to allocate memory for queue\n");
        return NULL;
    }
    queue->front = queue->rear = NULL;
    return queue;
}

void qEnqueue(qQueue *queue, PCB* pcb) {
    qQueueNode *newNode = (qQueueNode *)malloc(sizeof(qQueueNode));
    if (!newNode) {
        fprintf(stderr, "Unable to allocate memory for queue node\n");
        return;
    }
    newNode->pcb = pcb;
    newNode->next = NULL;

    if (queue->rear == NULL) {
        queue->front = queue->rear = newNode;
        return;
    }

    queue->rear->next = newNode;
    queue->rear = newNode;
}

PCB* qDequeue(qQueue *queue) {
    if (queue->front == NULL) {
        fprintf(stderr, "Queue is empty\n");
        return NULL; // Indicate queue is empty
    }

    qQueueNode *temp = queue->front;
    PCB* pcb = temp->pcb;
    queue->front = queue->front->next;

    if (queue->front == NULL)
        queue->rear = NULL;

    free(temp);
    return pcb;
}

PCB* qGetFront(const qQueue *queue) {
    if (queue->front == NULL) return NULL;
    return queue->front->pcb;
}

void qFree(qQueue *queue) {
    while (queue->front != NULL) {
        qQueueNode *temp = queue->front;
        queue->front = queue->front->next;
        free(temp);
    }
    free(queue);
}

bool qIsEmpty(const qQueue *queue) {
    return queue->front == NULL;
}
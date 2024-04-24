#include "minheap.h"
#include <stdio.h>
#include <stdlib.h>

static void mhSwapHeapNode(mhHeapNode *a, mhHeapNode *b) {
    mhHeapNode temp = *a;
    *a = *b;
    *b = temp;
}

void mhMinHeapify(mhMinHeap *minHeap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->elements[left].key < minHeap->elements[smallest].key)
        smallest = left;

    if (right < minHeap->size && minHeap->elements[right].key < minHeap->elements[smallest].key)
        smallest = right;

    if (smallest != idx) {
        mhSwapHeapNode(&minHeap->elements[idx], &minHeap->elements[smallest]);
        mhMinHeapify(minHeap, smallest);
    }
}

mhMinHeap* mhCreate(int capacity) {
    mhMinHeap *minHeap = (mhMinHeap *) malloc(sizeof(mhMinHeap));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->elements = (mhHeapNode *) malloc(capacity * sizeof(mhHeapNode));
    return minHeap;
}

void mhInsert(mhMinHeap *minHeap, PCB* pcb, int key) {
    if (minHeap->size == minHeap->capacity) {
        printf("MinHeap overflow\n");
        return;
    }

    int i = minHeap->size++;
    minHeap->elements[i].pcb = pcb;
    minHeap->elements[i].key = key;

    // Heapify up
    while (i != 0 && minHeap->elements[(i - 1) / 2].key > minHeap->elements[i].key) {
        mhSwapHeapNode(&minHeap->elements[i], &minHeap->elements[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

PCB* mhExtractMin(mhMinHeap *minHeap) {
    if (minHeap->size <= 0) {
        fprintf(stderr, "MinHeap is empty\n");
        return NULL;  // Return NULL if heap is empty
    }
    mhHeapNode root = minHeap->elements[0];
    minHeap->elements[0] = minHeap->elements[minHeap->size - 1];
    minHeap->size--;
    mhMinHeapify(minHeap, 0);

    return root.pcb;
}

PCB* mhGetTop(const mhMinHeap *minHeap) {
    if (minHeap->size <= 0) return NULL;
    return minHeap->elements[0].pcb;
}

void mhFree(mhMinHeap * minHeap) {
    free(minHeap->elements);
    minHeap->elements = NULL;
    free(minHeap);
}
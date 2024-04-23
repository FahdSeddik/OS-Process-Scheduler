#include "minheap.h"
#include <stdio.h>
#include <stdlib.h>

static void mh_swapHeapNode(mhHeapNode *a, mhHeapNode *b) {
    mhHeapNode temp = *a;
    *a = *b;
    *b = temp;
}

void mh_minHeapify(mhMinHeap *minHeap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->elements[left].key < minHeap->elements[smallest].key)
        smallest = left;

    if (right < minHeap->size && minHeap->elements[right].key < minHeap->elements[smallest].key)
        smallest = right;

    if (smallest != idx) {
        mh_swapHeapNode(&minHeap->elements[idx], &minHeap->elements[smallest]);
        mh_minHeapify(minHeap, smallest);
    }
}

mhMinHeap* mhCreate(int capacity) {
    mhMinHeap *minHeap = (mhMinHeap *) malloc(sizeof(mhMinHeap));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->elements = (mhHeapNode *) malloc(capacity * sizeof(mhHeapNode));
    return minHeap;
}

void mhInsert(mhMinHeap *minHeap, int processId, int key) {
    if (minHeap->size == minHeap->capacity) {
        printf("MinHeap overflow\n");
        return;
    }

    int i = minHeap->size++;
    minHeap->elements[i].processId = processId;
    minHeap->elements[i].key = key;

    // Heapify up
    while (i != 0 && minHeap->elements[(i - 1) / 2].key > minHeap->elements[i].key) {
        mh_swapHeapNode(&minHeap->elements[i], &minHeap->elements[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

mhHeapNode mhExtractMin(mhMinHeap *minHeap) {
    if (minHeap->size <= 0)
        return (mhHeapNode){-1, -1};

    mhHeapNode root = minHeap->elements[0];
    minHeap->elements[0] = minHeap->elements[minHeap->size - 1];
    minHeap->size--;
    mh_minHeapify(minHeap, 0);

    return root;
}

mhHeapNode mhGetTop(const mhMinHeap *minHeap) {
    if (minHeap->size <= 0) {
        fprintf(stderr, "MinHeap is empty\n");
        return (mhHeapNode){-1, -1};  // Return invalid HeapNode if heap is empty
    }
    return minHeap->elements[0];
}

void mhFree(mhMinHeap * minHeap) {
    free(minHeap->elements);
    minHeap->elements = NULL;
    free(minHeap);
}
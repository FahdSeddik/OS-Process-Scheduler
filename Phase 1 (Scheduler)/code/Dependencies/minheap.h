#ifndef MH_MINHEAP_H
#define MH_MINHEAP_H

#include "pcb.h"

/**
 * Structure representing a single node in the MinHeap.
 * Contains the process ID and the key used for comparison in heap operations.
 */
typedef struct mhHeapNode {
    PCB* pcb;
    int key; // This key is the value used for comparison.
} mhHeapNode;

/**
 * Structure representing the MinHeap itself.
 * Includes the array of heap elements, its capacity, and current size.
 */
typedef struct mhMinHeap {
    mhHeapNode *elements; // Pointer to array of elements in heap
    int capacity;       // Maximum possible size of min heap
    int size;           // Current number of elements in min heap
} mhMinHeap;

// Function prototypes

/**
 * Creates and initializes a MinHeap with a specified capacity.
 * @param capacity The maximum number of elements the heap can hold.
 * @return Pointer to the newly created MinHeap.
 */
mhMinHeap* mhCreate(int capacity);

/**
 * Inserts a new node with a given process ID and key into the MinHeap.
 * @param minHeap Pointer to the MinHeap.
 * @param pcb Pointer to the process control block to be inserted.
 * @param key The key used for ordering in the heap.
 */
void mhInsert(mhMinHeap *minHeap, PCB* pcb, int key);

/**
 * Extracts the node with the minimum key from the MinHeap.
 * @param minHeap Pointer to the MinHeap.
 * @return The PCB pointer for the min entry, or NULL if empty
 */
PCB* mhExtractMin(mhMinHeap *minHeap);

/**
 * Retrieves the minimum element from the MinHeap without removing it.
 * @param minHeap Constant pointer to the MinHeap.
 * @return The minimum PCB pointer, or NULL if empty
 */
PCB* mhGetTop(const mhMinHeap *minHeap); 

/**
 * Frees the MinHeap
 * @param minHeap Pointer to the MinHeap
 */
void mhFree(mhMinHeap * minHeap);

#endif // MINHEAP_H

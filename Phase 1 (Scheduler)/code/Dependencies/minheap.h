#ifndef MH_MINHEAP_H
#define MH_MINHEAP_H

/**
 * Structure representing a single node in the MinHeap.
 * Contains the process ID and the key used for comparison in heap operations.
 */
typedef struct mh_HeapNode {
    int processId;
    int key; // This key is the value used for comparison.
} mh_HeapNode;

/**
 * Structure representing the MinHeap itself.
 * Includes the array of heap elements, its capacity, and current size.
 */
typedef struct mh_MinHeap {
    mh_HeapNode *elements; // Pointer to array of elements in heap
    int capacity;       // Maximum possible size of min heap
    int size;           // Current number of elements in min heap
} mh_MinHeap;

// Function prototypes

/**
 * Creates and initializes a MinHeap with a specified capacity.
 * @param capacity The maximum number of elements the heap can hold.
 * @return Pointer to the newly created MinHeap.
 */
mh_MinHeap* mh_createMinHeap(int capacity);

/**
 * Inserts a new node with a given process ID and key into the MinHeap.
 * @param minHeap Pointer to the MinHeap.
 * @param processId The process ID to be added.
 * @param key The key used for ordering in the heap.
 */
void mh_insertMinHeap(mh_MinHeap *minHeap, int processId, int key);

/**
 * Extracts the node with the minimum key from the MinHeap.
 * @param minHeap Pointer to the MinHeap.
 * @return The HeapNode with the smallest key.
 */
mh_HeapNode mh_extractMin(mh_MinHeap *minHeap);

/**
 * Retrieves the minimum element from the MinHeap without removing it.
 * @param minHeap Constant pointer to the MinHeap.
 * @return The minimum HeapNode. If the heap is empty, returns a node with processId and key as -1.
 */
mh_HeapNode mh_getTop(const mh_MinHeap *minHeap); 

/**
 * Frees the MinHeap
 * @param minHeap Pointer to the MinHeap
 */
void mh_free(mh_MinHeap * minHeap);

#endif // MINHEAP_H

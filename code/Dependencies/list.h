#ifndef L_LiST_H
#define L_LiST_H

#include "pcb.h"

/**
 * Structure representing a node in the linked list.
 */
typedef struct lListNode {
    PCB* pcb;            // Process ID stored in the node
    struct lListNode *next; // Pointer to the next node in the list
    struct lListNode *prev; // Pointer to the previous node in the list
} lListNode;

/**
 * Structure representing the linked list, with pointers to the front and rear nodes.
 */
typedef struct lList {
    lListNode *begin; // Pointer to the front node of the list
    lListNode *end;  // Pointer to the one past the last node of the list
} lList;

// Function prototypes

/**
 * Allocates and initializes a new empty list.
 * @return A pointer to the initialized list or NULL if memory allocation fails.
 */
lList *lCreate();

/**
 * Adds a new node with the given process ID to the end of the list.
 * @param list Pointer to the list where the node will be added.
 * @param pcb Pointer to the process control block (PCB).
 */
void lInsert(lList *list, PCB* pcb);

/**
 * Removes a given node from the list.
 * @param list Pointer to the list.
 * @param node Pointer to the node to be removed.
 * @return Pointer to the next node in the list.
 */
lListNode *lRemove(lList *list, lListNode *node);

/**
 * Retrieves the next node in the list.
 * @param list Pointer to the list.
 * @param node Pointer to the current node.
 * @return Pointer to the next node in the list.
 */
lListNode *lGetNext(const lList *list, const lListNode *node);

/**
 * Retrieves the first node in the list.
 * @param list Pointer to the list.
 * @return Pointer to the first node in the list.
 */
lListNode *lBegin(const lList *list);

/**
 * Retrieves the end node in the list.
 * @param list Pointer to the list.
 * @return Pointer to the end node in the list.
 */
lListNode *lEnd(const lList *list);

/**
 * Free all nodes in the list and the list structure itself.
 * @param list Pointer to the list to be freed.
 */
void lFree(lList *list);

#endif // L_LiST_H
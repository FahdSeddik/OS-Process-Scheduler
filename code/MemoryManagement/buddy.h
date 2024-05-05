#ifndef BUDDY_H
#define BUDDY_H
#include <stdio.h>
#include <stdlib.h>

#define BS_MAX_SIZE 1024
#define BS_MIN_BLOCK_SIZE 64

typedef struct bsBlock {
    int size;
    int isFree;
    int addressStart;
} bsBlock;

typedef struct bsBuddySystem {
    // array of smallest possible chunks
    bsBlock** addressBook;
} bsBuddySystem;

/**
 * Creates the buddy system.
 * 
 * @return Pointer to the buddy system struct.
 */
bsBuddySystem* bsCreate();

/**
 * Allocates a block of memory with the specified size.
 * 
 * @param buddySystem Pointer to the buddy system struct.
 * @param size Size of the memory block to allocate.
 * @return Pointer to the allocated memory block, or NULL if allocation fails.
 */
bsBlock* bsAllocate(bsBuddySystem* buddySystem, int size);

/**
 * Deallocates a previously allocated block of memory.
 * 
 * @param buddySystem Pointer to the buddy system struct.
 * @param block Pointer to the memory block to deallocate.
 */
void bsDeallocate(bsBuddySystem* buddySystem, bsBlock* block);

/**
 * Displays the address book.
 * 
 * @param buddySystem Pointer to the buddy system struct.
 */
void bsDisplayAddressBook(bsBuddySystem* buddySystem);

#endif // BUDDY_H
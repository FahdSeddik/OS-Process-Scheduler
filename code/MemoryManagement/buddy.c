#include "buddy.h"
#include <string.h>

bsBlock* createBlock(int size, int addressStart) {
    bsBlock* block = (bsBlock*)malloc(sizeof(bsBlock));
    block->size = size;
    block->isFree = 1;
    block->addressStart = addressStart;
    return block;
}

bsBuddySystem* bsCreate() {
    bsBuddySystem* buddySystem = (bsBuddySystem*)malloc(sizeof(bsBuddySystem));
    buddySystem->addressBook = (bsBlock**)malloc(sizeof(bsBlock*) * BS_MAX_SIZE / BS_MIN_BLOCK_SIZE);
    buddySystem->addressBook[0] = createBlock(BS_MAX_SIZE, 0);
    // set rest of the blocks to NULL
    for (int i = 1; i < BS_MAX_SIZE / BS_MIN_BLOCK_SIZE; i++) buddySystem->addressBook[i] = NULL;
    return buddySystem;
}

bsBlock* bsAllocate(bsBuddySystem* buddySystem, int size) {
    // get best fit block
    // split if needed
    // maintain address book
    // return block
    bsBlock* bestFitBlock = NULL;
    int bestFitIndex = -1;
    for (int i = 0; i < BS_MAX_SIZE / BS_MIN_BLOCK_SIZE; i++) {
        if (buddySystem->addressBook[i] != NULL && buddySystem->addressBook[i]->isFree) {
            if (buddySystem->addressBook[i]->size >= size) {
                if (bestFitBlock == NULL || buddySystem->addressBook[i]->size < bestFitBlock->size) {
                    bestFitBlock = buddySystem->addressBook[i];
                    bestFitIndex = i;
                }
            }
        }
    }
    if (bestFitBlock == NULL) return NULL;
    while (bestFitBlock->size >= size * 2 && bestFitBlock->size >= BS_MIN_BLOCK_SIZE * 2) {
        // split
        bsBlock* newBlock1 = createBlock(bestFitBlock->size / 2, bestFitBlock->addressStart);
        bsBlock* newBlock2 = createBlock(bestFitBlock->size / 2, bestFitBlock->addressStart + bestFitBlock->size / 2);
        free(buddySystem->addressBook[bestFitIndex]);
        buddySystem->addressBook[bestFitIndex] = NULL;
        // compute index based on address start
        int idx1 = newBlock1->addressStart / BS_MIN_BLOCK_SIZE;
        int idx2 = newBlock2->addressStart / BS_MIN_BLOCK_SIZE;
        if(buddySystem->addressBook[idx1] != NULL) printf("Overwriting Address book at %d\n", idx1);
        if(buddySystem->addressBook[idx2] != NULL) printf("Overwriting Address book at %d\n", idx2);
        buddySystem->addressBook[idx1] = newBlock1;
        buddySystem->addressBook[idx2] = newBlock2;
        // get left buddy
        bestFitBlock = newBlock1;
    }
    bestFitBlock->isFree = 0;
    return bestFitBlock;
}

/**
 * Utility function to merge blocks.
 * Merges the block with its buddy if possible.
 * 
 * @param buddySystem Pointer to the buddy system struct.
 * @param block Pointer to the block to merge.
 */
void bsMerge(bsBuddySystem* buddySystem, bsBlock* block) {
    int idx = block->addressStart;
    int buddyIdx = idx ^ block->size;
    idx /= BS_MIN_BLOCK_SIZE;
    buddyIdx /= BS_MIN_BLOCK_SIZE;
    if (buddyIdx >= BS_MAX_SIZE / BS_MIN_BLOCK_SIZE) return;
    if (buddySystem->addressBook[buddyIdx] != NULL && buddySystem->addressBook[buddyIdx]->isFree) {
        // merge
        bsBlock* buddy = buddySystem->addressBook[buddyIdx];
        int minAddress = block->addressStart < buddy->addressStart ? block->addressStart : buddy->addressStart;
        bsBlock* mergedBlock = createBlock(block->size * 2, minAddress);
        free(buddySystem->addressBook[idx]);
        free(buddySystem->addressBook[buddyIdx]);
        buddySystem->addressBook[idx] = NULL;
        buddySystem->addressBook[buddyIdx] = NULL;
        int mergedIdx = mergedBlock->addressStart / BS_MIN_BLOCK_SIZE;
        if(buddySystem->addressBook[mergedIdx] != NULL) printf("Overwriting Address book at %d\n", mergedIdx);
        buddySystem->addressBook[mergedIdx] = mergedBlock;
        bsMerge(buddySystem, mergedBlock);
    }
}

void bsDeallocate(bsBuddySystem* buddySystem, bsBlock* block) {
    block->isFree = 1;
    // merge with buddy
    bsMerge(buddySystem, block);
}

void bsDisplayAddressBook(bsBuddySystem* buddySystem) {
    printf("=== Start Address Book ===\n");
    for (int i = 0; i < BS_MAX_SIZE / BS_MIN_BLOCK_SIZE; i++) {
        if (buddySystem->addressBook[i] != NULL) {
            printf("Block %d: Size %d, Free %d, Address %d\n", i, buddySystem->addressBook[i]->size, buddySystem->addressBook[i]->isFree, buddySystem->addressBook[i]->addressStart);
        }
    }
    printf("=== End Address Book ===\n");
}

void bsDestroy(bsBuddySystem* buddySystem) {
    for (int i = 0; i < BS_MAX_SIZE / BS_MIN_BLOCK_SIZE; i++) {
        if (buddySystem->addressBook[i] != NULL) {
            free(buddySystem->addressBook[i]);
            buddySystem->addressBook[i] = NULL;
        }
    }
    free(buddySystem->addressBook);
    free(buddySystem);
}
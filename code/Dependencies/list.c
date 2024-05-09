#include "list.h"
#include <stdlib.h>
#include <stdio.h>

lList *lCreate() {
    lList *list = (lList *) malloc(sizeof(lList));
    if (!list) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }
    lListNode *end = (lListNode *) malloc(sizeof(lListNode));
    if (!end) {
        free(list);
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }
    end->next = NULL;
    end->prev = NULL;
    list->begin = end;
    list->end = end;
    return list;
}

void lInsert(lList *list, PCB* pcb) {
    lListNode *newEnd = (lListNode *) malloc(sizeof(lListNode));
    if (!newEnd) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }
    list->end->pcb = pcb;
    list->end->next = newEnd;
    newEnd->prev = list->end;
    newEnd->next = NULL;
    list->end = newEnd;
}


lListNode *lRemove(lList *list, lListNode *node) {
    lListNode *nextNode = node->next;
    if (node == NULL) {
        fprintf(stderr, "Cannot remove a NULL node\n");
        return NULL;
    } else if (node == list->end) {
        fprintf(stderr, "Cannot remove the end node\n");
        return NULL;
    } else if (node == list->begin) {
        list->begin = node->next;
        list->begin->prev = NULL;
    } else {
        node->prev->next = node->next;
        nextNode->prev = node->prev;
    }
    free(node);
    return nextNode;
}

lListNode *lGetNext(const lList *list, const lListNode *node) {
    if (node == list->end) {
        fprintf(stderr, "Cannot get next node from the end node\n");
        return NULL;
    }
    return node->next;
}


lListNode *lBegin(const lList *list) {
    return list->begin;
}

lListNode *lEnd(const lList *list) {
    return list->end;
}

void lFree(lList *list) {
    lListNode *current = list->begin;
    while (current != NULL) {
        lListNode *next = current->next;
        free(current);
        current = next;
    }
    free(list);
    list = NULL;
}
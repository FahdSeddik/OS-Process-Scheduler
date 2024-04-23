#include "process_table.h"
#include <stdio.h>
#include <stdlib.h>

void pctInit(ProcessTable *table) {
    table->size = 0;
    for (int i = 0; i < MAX_PROCESSES; i++) {
        table->processes[i] = NULL;
    }
}

int pctAddProcess(ProcessTable *table, PCB *process) {
    if (table->size >= MAX_PROCESSES) return -1;
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (table->processes[i] == NULL) {
            table->processes[i] = process;
            table->size++;
            return 0;
        }
    }
    return -1; // This line should not be reached
}

PCB* pctFindProcessById(ProcessTable *table, int id) {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (table->processes[i] != NULL && table->processes[i]->id == id) {
            return table->processes[i];
        }
    }
    return NULL;
}

int pctRemoveProcess(ProcessTable *table, int id) {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (table->processes[i] != NULL && table->processes[i]->id == id) {
            free(table->processes[i]);
            table->processes[i] = NULL;
            table->size--;
            return 0;
        }
    }
    return -1;
}

void pctDisplay(ProcessTable *table) {
    printf("Process Table:\n");
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (table->processes[i] != NULL) {
            printf("Process ID %d, State %d\n", table->processes[i]->id, table->processes[i]->state);
        }
    }
}

void pctClear(ProcessTable *table) {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (table->processes[i] != NULL) {
            free(table->processes[i]);
            table->processes[i] = NULL;
        }
    }
    table->size = 0;
}

void pctFree(ProcessTable *table) {
    pctClear(table);
    free(table);
}
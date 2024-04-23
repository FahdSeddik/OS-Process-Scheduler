#ifndef PCT_H
#define PCT_H

// Maximum number of processes the table can handle
#define MAX_PROCESSES 256
#include "pcb.h"

typedef struct {
    PCB *processes[MAX_PROCESSES];  // Array of pointers to PCBs
    int size;                       // Current number of processes in the table
} ProcessTable;

// Function prototypes


/**
 * Initializes a process table.
 *
 * @param table Pointer to the process table to initialize.
 */
void pctInit(ProcessTable *table);

/**
 * Adds a new PCB to the process table.
 *
 * @param table Pointer to the process table.
 * @param process Pointer to the PCB to add.
 * @return int Returns 0 if successful, or -1 if the table is full.
 */
int pctAddProcess(ProcessTable *table, PCB *process);

/**
 * Finds a PCB in the process table by its ID.
 *
 * @param table Pointer to the process table.
 * @param id The ID of the process to find.
 * @return PCB* Returns a pointer to the PCB if found, or NULL if not found.
 */
PCB* pctFindProcessById(ProcessTable *table, int id);

/**
 * Removes a PCB from the process table by its ID.
 *
 * @param table Pointer to the process table.
 * @param id The ID of the process to remove.
 * @return int Returns 0 if successful, or -1 if the process is not found.
 */
int pctRemoveProcess(ProcessTable *table, int id);

/**
 * Displays all processes in the process table.
 *
 * @param table Pointer to the process table.
 */
void pctDisplay(ProcessTable *table);

/**
 * Clears/Frees all records from process table
 * 
 * @param table Pointer to the process table.
 */
void pctClear(ProcessTable *table);

/**
 * Clears all records from process table and then frees the
 * table as well
 * 
 * @param table Pointer to the process table.
 */
void pctFree(ProcessTable *table);

#endif // PCT_H
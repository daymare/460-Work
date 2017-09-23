
#define NPROC 9
#define FREE 0
#define READY 1
#define SSIZE 1024

typedef struct Proc
{
    struct Proc *next; // next proc
    int *ksp; // proc stack pointer
    int pid;
    int status; // free, ready, and other stuff in the future
    int priority; // priority value
    int kstack[SSIZE]; // process kernel mode stack
} Proc;

Proc proc[NPROC];
Proc *running; // process currently running
Proc *freeList; // list of free proc structures
Proc *readyQueue; // list of processes ready to run



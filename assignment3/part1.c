


    p = &proc[i];
    p->pid = i;
    p->status = READY;
 
    // set kstack to resume to body
    // stack = r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,r14
    //         1  2  3  4  5  6  7  8  9  10 11  12  13  14
    for (j=1; j<11; j++)
        p->kstack[SSIZE-j] = 0;
    p->kstack[SSIZE-1] = (int)body;  // set pc to be the address of body // in dec reg=address ORDER !!!
    p->ksp = &(p->kstack[SSIZE-14]);


int kfork(int func, int priority)
{
    int i;
    PROC *p = get_proc(&freeList);

    if (p==0)
    {
        printf("no more PROC, kfork failed\n");
        return -1;
        // return -1 for FAIL
    }
    p->status = READY;
    p->priority = priority;
    // set kstack for proc resume to execute func()
    for (i=1; i<11; i++)
        p->kstack[SSIZE-i] = 0;

    p->kstack[SSIZE-1] = func;

    // initialize stack for body function
    p->kstack[SSIZE-12] = 0;
    p->kstack[SSIZE-13] = p->ppid;
    p->kstack[SSIZE-14] = p->pid;
    p->kstack[SSIZE-15] = 1234; // param 1

    // all "saved" regs = 0
    // resume execution address
    p->ksp = &(p->kstack[SSIZE-14]); // saved ksp
    enqueue(&readyQueue, p);
    // enter p into readyQueue
    printf("%d kforked a new proc %d\n", running->pid, p->pid);
    printf("freeList = "); printList(readyQueue);
    return p->pid;
}

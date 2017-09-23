
#include "proc.c"
#include "queue.c"

int kernel_init()
{
}


int body()
{
}

int kfork(int func, int priority)
{
    // get a new process
    int i;
    Proc *p = get_proc(&freeList);
    if (p == NULL)
    {
        printf("no more Proc's in free list, kfork failed\n");
    }

    // 
}


// terminate a process
void kexit()
{
}

int scheduler()
{
    if (running->status == READY)
        enqueue(&readyQueue, running);
    running = dequeue(&readyQueue);
}


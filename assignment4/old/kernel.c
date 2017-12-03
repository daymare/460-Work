/********************************************************************
Copyright 2010-2017 K.C. Wang, <kwang@eecs.wsu.edu>
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
********************************************************************/

/********************
#define  SSIZE 1024
#define  NPROC  9
#define  FREE   0
#define  READY  1
#define  SLEEP  2
#define  BLOCK  3
#define  ZOMBIE 4
#define  printf  kprintf
 
typedef struct proc{
  struct proc *next;
  int    *ksp;
  int    status;
  int    pid;

  int    priority;
  int    ppid;
  struct proc *parent;
  int    event;
  int    exitCode;
  int    kstack[SSIZE];
}PROC;
***************************/

#include "list.c"
#include "queue.c"

#define NPROC 7

Proc proc[NPROC];
Proc *running;
Proc *sleepingList;
Proc *freeList;
Proc *readyQueue;


int procsize = sizeof(Proc);
int body();

int kernel_init()
{
    int i, j; 
    Proc *p;

    kprintf("kernel_init()\n");
    for (i=0; i<NPROC; i++){
        p = &proc[i];
        p->pid = i;
        p->status = FREE;
        p->next = &proc[i+1];

        if (i == NPROC-1)
        {
            p->next = NULL;
        }

        // set kstack to resume to body
        // stack = r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,r14
        //         1  2  3  4  5  6  7  8  9  10 11  12  13  14
        for (j=1; j<15; j++)
            p->kstack[SSIZE-j] = 0;
        p->kstack[SSIZE-1] = (int)body;  // in dec reg=address ORDER !!!
        p->ksp = &(p->kstack[SSIZE-14]);
    }

    freeList = &proc[0];
    running = dequeue(&freeList);
    running->priority = 0;

    running->status = READY;
    kprintf("running = %d\n", running->pid);
}


int kfork(int function, int priority)
{
    // disable interrupts
    int SR = interrupts_off();

    // get a new proc
    Proc * newProc = dequeue(&freeList);
    if (newProc == NULL)
    {
        printf("no more PROCS! Kfork failed!\n");
        return -1;
    }

    printf("got a proc, pid: %d\n", newProc->pid);

    // initialize new process
    newProc->status = READY;
    newProc->priority = priority;
    newProc->ppid = running->pid;

    // set stack for proc resume to execute function
    for (int i = 1; i < 15; i++)
    {
        newProc->kstack[SSIZE-i] = 0; // set all "saved" registers to 0
    }

    newProc->kstack[SSIZE-1] = function;
    newProc->ksp = &(newProc->kstack[SSIZE-14]);

    // enter into ready queue
    enqueue(&readyQueue, newProc);

    // print debug information
    printf("%d kforked a new proc %d\n", running->pid, newProc->pid);
    printf("free list = "); printList(freeList);
    printf("readyQueue = "); printList(readyQueue);

    interrupts_on(SR);

    return newProc->pid;
}

int ksleep(int event)
{
    int SR = interrupts_off(); // disable interrupts and get original CPSR
    running->event = event;
    running->status = SLEEP;
    
    enqueue(&sleepingList, running);

    tswitch();
    interrupts_on(SR); // enable interrupts and restore original CPSR
}

int kwakeup(int event)
{
    int statusRegister = interrupts_off();
    Proc* current = sleepingList;
    Proc* next = NULL;

    while (current != NULL)
    {
        next = current->next;
        if (current->event == event)
        {
            removeProc(&sleepingList, current);
            current->status = READY;
            enqueue(&readyQueue, current);
        }

        current = next;
    }

    interrupts_on(statusRegister);
}

int scheduler()
{
    int ocolor = color;
    color = YELLOW;

    //kprintf("proc %d in scheduler \n", running->pid);
    //printf("ready queue before: ");
    //printList(readyQueue);

    if (running->status == READY)
        enqueue(&readyQueue, running);
    running = dequeue(&readyQueue);

    //printf("ready queue after: ");
    //printList(readyQueue);
    //printf("proc %d now running \n", running->pid);

    color = ocolor;
}


int body()
{
  char c; char line[64];
  int pid;
  kprintf("proc %d resume to body()\n", running->pid);
  while(1){
    pid = running->pid;
    if (pid==0) color=WHITE;
    if (pid==1) color=GREEN;
    if (pid==2) color=RED;
    if (pid==3) color=CYAN;
    if (pid==4) color=PURPLE;

    kprintf("proc %d in body() input a char [s] : ", running->pid);
    c = kgetc(); 
    printf("%c\n", c);

    switch(c){
      case 's': tswitch(); break;
    }
  }
}


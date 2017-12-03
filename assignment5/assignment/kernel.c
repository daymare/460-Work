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


#include "list.c"
#include "queue.c"


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

    // happy kernel message!
    kprintf("kernel_init()\n");

    // init all procs
    for (i=0; i<NPROC; i++)
    {
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

        // set proc to its proper pgdir
        // TODO potential bug, pid may need to be multiplied by 0x4000 for an unknown reason
        p->pgdir = (int*)(PROC_PGTABLE_START_LOCATION + p->pid*PGTABLE_SIZE);
    }

    // set free list
    freeList = &proc[0];

    // initialize running as proc 0
    running = dequeue(&freeList);
    running->priority = 0;
    running->status = READY;
    kprintf("running = %d\n", running->pid);

    // set pgtable kernel memory
    int* first_level_mtable = (int*) LEVEL_ONE_PGTABLE_LOCATION; 
    int* second_level_mtable = (int*) PROC_PGTABLE_START_LOCATION;

    for (int i = 0; i < NUM_PGTABLES; i++)
    {
        for (int j = 0; j < PGTABLE_SIZE/2; j++)
        {
            // copy low 2048 entries of mtable
            // set up kernel mode space for each proc pgtable
            second_level_mtable[j] = first_level_mtable[j];
        }
        // advance second level mtable to next section
        second_level_mtable += PGTABLE_SIZE;
    }
    
    // zero out pgtable high 2048 entries for umode memory
    second_level_mtable = (int*) PROC_PGTABLE_START_LOCATION;
    for (int i = 0; i < NUM_PGTABLES; i++)
    {
        for (int j = PGTABLE_SIZE/2; j < PGTABLE_SIZE; j++)
        {
            second_level_mtable[j] = 0;
        }

        // every proc except P0 gets a block
        if (i != 0)
        {
            mtable[PGTABLE_SIZE/2] = (UMODE_MEM_START + (i-1)*UMODE_BLOCK_SIZE) | 0xC3E;
        }

        mtable += PGTABLE_SIZE;
    }
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

    // enqueue running
    if (running->status == READY)
    {
        enqueue(&readyQueue, running);
    }

    // get next process to run
    Proc* old = running;
    running = dequeue(&readyQueue);
    
    // switch to new pgdir
    if (running != old)
    {
        // switch to new running's pgdir; flush TLB and I&D caches
        switchPgdir((u32)running->pgdir);
    }


    //printf("ready queue after: ");
    //printList(readyQueue);
    //printf("proc %d now running \n", running->pid);

    color = ocolor;
}

int kgetpid()
{
    return running->pid;
}

int kgetppid()
{
    return running->ppid;
}

int kchname(char *s)
{
    // fetch s from umode;
    strcpy(running->name, s);
}

// syscall handler
int svc_handler(volatile int a, int b, int c, int d)
{
    int r = -1; // default bad return value
    switch(a)
    {
        case 0: r = kgetpid(); break;
        case 1: r = kgetppid(); break;
        case 2: r = kps(); break;
        case 3: r = kchname((char*) b); break;
        case 90: r = kgetc() & 0x7F; break;
        case 91: r = kputc(b); break;
        default: printf("invalid syscall %d\n", a); break;
    }

    // replace r0 in kstack with r
    running->kstack[SSIZE-14] = r; 
}


// print out all running processess
char *pstatus[] = { "FREE", "READY", "SLEEP", "BLOCK", "ZOMBIE" };
int kps()
{
    Proc* current_proc;
    for (int i = 0; i < NPROC; i++)
    {
        current_proc = &proc[i];
        printf("proc[%d]: pid=%d ppid=%d", i, p->pid, p->ppid);
        if (p == running)
        {
            printf("%s ", "RUNNING");
        }
        else
        {
            printf("%s ", pstatus[p->status]);
        }
        printf(" name=%s\n", p->name);
    }
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


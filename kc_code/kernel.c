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
#define NPROC 5
PROC proc[NPROC], *running;
int procsize = sizeof(PROC);
int body();

int init()
{
  int i, j; 
  PROC *p;
  
  kprintf("kernel_init()\n");
  for (i=0; i<NPROC; i++){
    p = &proc[i];
    p->pid = i;
    p->status = READY;
 
    // set kstack to resume to body
    // stack = r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,r14
    //         1  2  3  4  5  6  7  8  9  10 11  12  13  14
    for (j=1; j<15; j++)
        p->kstack[SSIZE-j] = 0;
    p->kstack[SSIZE-1] = (int)body;  // in dec reg=address ORDER !!!
    p->ksp = &(p->kstack[SSIZE-14]);
 
    p->next = p + 1;
  }
  proc[NPROC-1].next = &proc[0]; // circular proc list
  running = &proc[0];
  kprintf("running = %d\n", running->pid);
}

int scheduler()
{
  int ocolor = color;
  color = YELLOW;
  kprintf("proc %d in scheduler ", running->pid);
  running = running->next;
  kprintf("next running = %d\n", running->pid);
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

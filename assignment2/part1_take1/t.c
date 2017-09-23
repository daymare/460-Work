
#include "type.h"
#include "string.c"
#include "exceptions.c"

#include "kbd.c"
#include "vid.c"

#define NPROC 5
PROC proc[NPROC], *running;
int procsize = sizeof(PROC);

void copy_vectors(void) {
    extern u32 vectors_start, vectors_end;
    u32 *vectors_src = &vectors_start;
    u32 *vectors_dst = (u32 *)0;

    while(vectors_src < &vectors_end)
      *vectors_dst++ = *vectors_src++;
}

void IRQ_handler()
{
    int vicstatus, sicstatus;
    int ustatus, kstatus;

    // read VIC status register to find out which interrupt
    vicstatus = VIC_STATUS; // VIC_STATUS=0x10140000=status reg
    sicstatus = SIC_STATUS;  
    //kprintf("vicstatus=%x sicstatus=%x\n", vicstatus, sicstatus);
    if (vicstatus & 0x80000000){
       if (sicstatus & 0x08){
          kbd_handler();
       }
    }
}

int body();

int init()
{
  int i, j; 
  PROC *p;
  
  kprintf("kernel_init() .... ");
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
  printf("done\n");
}

int scheduler()
{
  int ocolor = color;
  color = YELLOW;
  printf("proc %d in scheduler ", running->pid);
  running = running->next;
  printf("next running = %d\n", running->pid);
  color = ocolor;
}  


int body()
{
  char c; char line[64];
  int pid;
  printf("proc %d resume to body()\n", running->pid);
  while(1){
    pid = running->pid;
    if (pid==0) color=WHITE;
    if (pid==1) color=GREEN;
    if (pid==2) color=RED;
    if (pid==3) color=CYAN;
    if (pid==4) color=PURPLE;

    printf("proc %d in body() input a char [s] : ", running->pid);
    c = kgetc(); 
    printf("%c\n", c);

    switch(c){
      case 's': tswitch(); break;
    }
  }
}

int main()
{ 
   fbuf_init();
   kbd_init();
   color = WHITE;

   printf("Welcome to WANIX in Arm\n");
   init();
   printf("P0 switch process\n");
   while(1){
     tswitch();
   }
}

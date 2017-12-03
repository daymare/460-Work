

#include "type.h"
#include "string.c"

#include "exceptions.c"
#include "kbd.c"
#include "timer.c"
#include "vid.c"


void copy_vectors(void) {
    extern u32 vectors_start, vectors_end;
    u32 *vectors_src = &vectors_start;
    u32 *vectors_dst = (u32 *)0;

    while(vectors_src < &vectors_end)
      *vectors_dst++ = *vectors_src++;
}

// set up MMU using 1mb sections to ID map VA to PA
int mkptable() // build level 1 page table using 1mb sections
{
    int pentry;
    int *ptable;

    printf("1. build level-1 pagetable at 32 KB\n");
    ptable = (int*)0x8000;
    for (int i = 0; i < 4096; i++)
    {
        ptable[i] = 0;
    }

    printf("2. fill 258 entries of pgtable to ID map 258 mb virtual addresses to physical addresses\n");
    pentry = 0x412; // ap = 01, domain = 0000, cb = 00, type = 02
    for (int i = 0; i < 512; i++)
    {
        ptable[i] = pentry;
        pentry += 0x100000;
    }

    printf("3. finished building level-1 page table\n");
}


// data exception handler
int data_chandler()
{
    u32 fault_status;
    u32 fault_addr;
    u32 domain;
    u32 status;

    int spsr = get_spsr();
    printf("data abort exception in ");

    if ((spsr & 0x1F) == 0x13)
    {
        printf("SVC mode\n");
    }

    fault_status = get_fault_status();
    fault_addr = get_fault_addr();
    domain = (fault_status & 0xF0) >> 4;
    status = fault_status & 0xF;
    printf("status = %x: domain = %x status = %x (0x5 = Trans Invalid)\m", fault_status, domain, status);
    printf("VA addr = %x\n", fault_addr);
}

int irq_chandler()
{
    unsigned int vicstatus, sicstatus;
    unsigned int ustatus, kstatus;

    // read VIC status register to find out which interrupt
    vicstatus = VIC_STATUS; // VIC_STATUS=0x10140000=status reg
    sicstatus = SIC_STATUS;  

    //printf("vicstatus=%x sicstatus=%x\n", vicstatus, sicstatus);
    if (vicstatus & (1<<31)){
       if (sicstatus & (1<<3)){
           kbd_handler();
       }
    }

    if (vicstatus & (1<<4))
    {
        timer_handler(0);
    }
}

int main()
{
    int* p;
    int line[128];

    // clear enabled interrupts
    VIC_INTENABLE = 0;
    SIC_ENSET = 0;

    kbd_init();
    timer_init();

    color = WHITE;

    printf("test MMU protection: try to access virtual address = 0x00200000\n");
    p = (int *)0x00200000; *p = 123;
    printf("test MMU protection: try to access virtual address = 0x02000000\n");
    p = (int *)0x02000000; *p = 123;
    printf("test MMU protection: try to access virtual address = 0x1A000000\n");
    p = (int *)0x1A000000; *p = 123;
    printf("test MMU protection: try to access virtual address = 0x20000000\n");
    p = (int *)0x20000000; *p = 123;

    while(1)
    {
        printf("spinning, whee!");
        kgets(line);
    }
}































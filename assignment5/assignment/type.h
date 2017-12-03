
#ifndef TYPE_H
#define TYPE_H

//#include <stddef.h>

typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned int   u32;

#define UART0_BASE_ADDR 0x101f1000
#define UART0_DR   (*((volatile u32 *)(UART0_BASE_ADDR + 0x000)))
#define UART0_FR   (*((volatile u32 *)(UART0_BASE_ADDR + 0x018)))
#define UART0_IMSC (*((volatile u32 *)(UART0_BASE_ADDR + 0x038)))

#define UART1_BASE_ADDR 0x101f2000
#define UART1_DR   (*((volatile u32 *)(UART1_BASE_ADDR + 0x000)))
#define UART1_FR   (*((volatile u32 *)(UART1_BASE_ADDR + 0x018)))
#define UART1_IMSC (*((volatile u32 *)(UART1_BASE_ADDR + 0x038)))

#define KBD_BASE_ADDR 0x10006000
#define KBD_CR (*((volatile u32 *)(KBD_BASE_ADDR + 0x000)))
#define KBD_DR (*((volatile u32 *)(KBD_BASE_ADDR + 0x008)))

#define TIMER0_BASE_ADDR 0x101E2000
#define TIMER1_BASE_ADDR 0x101E2020

#define TIMER_LOAD_OFFSET 0x0 
#define TIMER_VALUE_OFFSET 0x1 
#define TIMER_CONTROL_OFFSET 0x2 
#define TIMER_INTCLR_OFFSET 0x3
#define TIMER_RIS_OFFSET 0x4 
#define TIMER_MIS_OFFSET 0x5 
#define TIMER_BGLOAD_OFFSET 0x6 


#define TIMER0_LR (*((volatile u32 *)(UART0_BASE_ADDR + 0x000)))
#define TIMER0_BR (*((volatile u32 *)(UART0_BASE_ADDR + 0x032)))

#define VIC_BASE_ADDR 0x10140000
#define VIC_STATUS    (*((volatile u32 *)(VIC_BASE_ADDR + 0x000)))
#define VIC_INTENABLE (*((volatile u32 *)(VIC_BASE_ADDR + 0x010)))
#define VIC_VADDR     (*((volatile u32 *)(VIC_BASE_ADDR + 0x030)))

#define SIC_BASE_ADDR 0x10003000
#define SIC_STATUS    (*((volatile u32 *)(SIC_BASE_ADDR + 0x000)))
#define SIC_INTENABLE (*((volatile u32 *)(SIC_BASE_ADDR + 0x008)))
#define SIC_ENSET     (*((volatile u32 *)(SIC_BASE_ADDR + 0x008)))
#define SIC_PICENSET  (*((volatile u32 *)(SIC_BASE_ADDR + 0x020)))

#define BLUE   0
#define GREEN  1
#define RED    2
#define CYAN   3
#define YELLOW 4
#define PURPLE 5
#define WHITE  6

#define  SSIZE 1024

// PROC status defines
#define  FREE   0
#define  READY  1
#define  SLEEP  2
#define  BLOCK  3
#define  ZOMBIE 4

#define  printf  kprintf


#define LEVEL_ONE_PGTABLE_LOCATION 0x4000 // location of level one page table

#define PROC_PGTABLE_START_LOCATION 0x600000 // start location of pgtables (6mb)
#define PGTABLE_SIZE 4096 // number of pointers in a page table
#define NUM_PGTABLES 64 // number of total proc page tables

#define UMODE_MEM_START 0x800000 // umode memory starts at 8mb
#define UMODE_BLOCK_SIZE 0x100000 // umode blocks are 1mb each

#define NPROC 9
 
typedef struct proc{
  struct proc *next; // next pointer for queues and lists

  // stack pointers
  int *ksp;
  int *usp;

  int *upc;
  int *ucpsr; // user mode cpsr
  int *pgdir; // level 1 page table pointer

  int    status;

  // id's
  int    pid;
  int    ppid;

  char name[64];

  int    priority;
  struct proc *parent;
  int    event;
  int    exitCode;
  int    kstack[SSIZE]; // kernel mode stack
}Proc;

#endif

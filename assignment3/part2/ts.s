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

.text
.code 32

.global vectors_start, vectors_end
.global main, proc, procsize
.global tswitch, scheduler, running
.global interrupts_off, interrupts_on

reset_handler:

    //ldr sp, =svc_stack_top

    // set SVC sp to proc[0] high end
    LDR r0, =proc
    LDR r1, =procsize
    LDR r2,[r1, #0]
    ADD r0, r0, r2 // r0 = high end of proc[0]
    MOV sp, r0 // svc stack pointer = high end of proc[0]

    // copy vector table to address 0
    BL copy_vectors

    // go in IRQ mode to set IRQ stack 
    MRS r0, cpsr
    BIC r1, r0, #0x1F
    ORR r1, r1, #0x12
    MSR cpsr, r1
    LDR sp, =irq_stack_top


    // go back to SVC mode
    MRS r0, cpsr
    BIC r1, r0, #0x1F
    ORR r1, r1, #0x13
    BIC r0, r0, #0x80  // enable IRQ interrupt
    MSR cpsr, r0

    // enable irq interrupts
    bl interrupts_on

    // call main() in C
    BL main
    B .

my_reset_handler:
    // 

.align 4
irq_handler:

  sub	lr, lr, #4
  stmfd	sp!, {r0-r12, lr}

  bl IRQ_handler  
  // TODO debug here

  ldmfd	sp!, {r0-r12, pc}^


// TODO figure out wtf is going on
/*
irq_handler:
    stmfd sp!, {r0-r12, lr}

    // stack spsr
    mrs r12, spsr
    stmfd sp!, {r12}

*/




tswitch:
//       1  2  3  4  5  6  7  8  9  10  11  12  13  14
//       ---------------------------------------------
// stack=r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,r14
//       ---------------------------------------------

  stmfd	sp!, {r0-r12, lr}

  // lock 
  MRS r0, cpsr
  ORR r0, r0, #0x80   // set bit means MASK off IRQ interrupt 
  MSR cpsr, r0

  LDR r0, =running     // r0=&running
  LDR r1, [r0, #0]     // r1->runningPROC
  str sp, [r1, #4]     // running->ksp = sp

  bl	scheduler

  LDR r0, =running
  LDR r1, [r0, #0]     // r1->runningPROC
  lDR sp, [r1, #4]

  // unlock
  MRS r0, cpsr
  BIC r0, r0, #0x80   // clear bit means UNMASK IRQ interrupt
  MSR cpsr, r0
  
  ldmfd	sp!, {r0-r12, pc}


lock:
  MRS r4, cpsr
  ORR r4, r4, #0x80   // set bit means MASK off IRQ interrupt 
  MSR cpsr, r4
  mov pc, lr

unlock:
  MRS r4, cpsr
  BIC r4, r4, #0x80   // clear bit means UNMASK IRQ interrupt
  MSR cpsr, r4
  mov pc, lr	

// int_on()/int_off(): turn on/off IRQ interrupts
interrupts_on: // may pass parameter in r0
  msr cpsr, r0
  mov pc, lr

interrupts_off: // may pass parameter in r0
  mrs r0, cpsr
  MRS r4, cpsr
  ORR r4, r4, #0x80   // set bit means MASK off IRQ interrupt 
  MSR cpsr, r4
  mov pc, lr
	
vectors_start:
  LDR PC, reset_handler_addr
  LDR PC, undef_handler_addr
  LDR PC, swi_handler_addr
  LDR PC, prefetch_abort_handler_addr
  LDR PC, data_abort_handler_addr
  B .
  LDR PC, irq_handler_addr
  LDR PC, fiq_handler_addr

reset_handler_addr:          .word reset_handler
undef_handler_addr:          .word undef_handler
swi_handler_addr:            .word swi_handler
prefetch_abort_handler_addr: .word prefetch_abort_handler
data_abort_handler_addr:     .word data_abort_handler
irq_handler_addr:            .word irq_handler
fiq_handler_addr:            .word fiq_handler

vectors_end:
	
.end

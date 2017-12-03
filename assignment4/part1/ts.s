

.text
.code 32
.global vectors_start, vectors_end
.global get_fault_status, get_fault_addr, get_spsr

.global interrupts_off
.global interrupts_on

.global reset_handler
.global mkptable


reset_handler:
    
    // set SVC stack
    ldr sp, =svc_stack_top

    bl fbuf_init

    // copy vector table to address 0
    bl copy_vectors

    // build level 1 page table
    bl mkptable

    // set translation table base register to 0x4000
    mov r0, #0x8000
    mcr p15, 0, r0, c2, c0, 0 // set TTB register
    mcr p15, 0, r0, c8, c7, 0 // flush TTB 

    // set domain 0
    mov r0, #1
    mcr p15, 0, r0, c3, c0, 0

    // enable MMU
    mrc p15, 0, r0, c1, c0, 0 // get c1 into r0
    orr r0, r0, #0x00000001 // set bit 0 to 1
    mcr p15, 0, r0, c1, c0, 0 // write to c1
    nop // time to allow MMU to start (?)
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    mrc p15, 0, r2, c2, c0, 0 // read TLB base reg c2 into r2
    mov r2, r2

    // set ABT stack
    msr cpsr, #0x97 // go to abt mode
    ldr sp, =abt_stack_top

    // set UND stack
    msr cpsr, #0x9B
    ldr sp, =und_stack_top

    // set irq stack
    msr cpsr, #0x92
    ldr sp, =irq_stack_top

    // go back to svc mode
    msr cpsr, #0x13 // svc mode with irq enabled

    bl main
    b .

swi_handler:

data_handler:
    sub lr, lr, #4
    stmfd sp!, {r0-r12, lr}
    bl data_chandler // call data handler in c.
    ldmfd sp!, {r0-r12, pc}^

irq_handler:
    sub lr, lr, #4
    stmfd sp!, {r0-r12, lr}
    bl irq_chandler // call irq handler in c.
    ldmfd sp!, {r0-r12, pc}^

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
data_abort_handler_addr:     .word data_handler
irq_handler_addr:            .word irq_handler
fiq_handler_addr:            .word fiq_handler

vectors_end:


get_fault_status: // read and return MMU register 5
    mrc p15, 0, r0, c5, c0, 0 // read DFSR (data fault status register) c5
    mov pc, lr

get_fault_addr: // read and return MMU register 6
    mrc p15, 0, r0, c6, c0, 0 // read DFAR (data fault address register)
    mov pc, lr

get_spsr:
    mrs r0, spsr
    mov pc, lr


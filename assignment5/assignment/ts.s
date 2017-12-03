.text
.code 32

.global vectors_start
.global vectors_end

.global get_fault_status
.global get_fault_addr
.global get_spsr
.global get_cpsr

.global interrupts_off
.global interrupts_on

.global reset_handler
.global mkptable

// TODO potential bugs, offsets for proc structure might be off.


reset_handler:
    bl fbuf_init

    // set SVC sp to proc[0] high end
    LDR r0, =proc
    LDR r1, =procsize
    LDR r2,[r1, #0]
    ADD r0, r0, r2 // r0 = high end of proc[0]
    MOV sp, r0 // svc stack pointer = high end of proc[0]


    // set ABT stack
    msr cpsr, #0x97 // go to abt mode
    ldr sp, =abt_stack_top

    // set UND stack
    msr cpsr, #0x9B
    ldr sp, =und_stack_top

    // set irq stack
    msr cpsr, #0x92
    ldr sp, =irq_stack_top

    // copy vector table to address 0
    msr cpsr, #0x93 // svc mode
    msr spsr, #0x10 // set spsr to umode with irq enabled
    bl copy_vectors // copy vector table to address 0

    // build level 1 page table 
    bl mkptable

    // initialize mmu control register c1
    // bit 12 = 1: EnIcachel bist 9-8=rs=11; bit2=1: enDcache;
    // all other bits of c1=0 for default
    ldr r0, regC1 // load r0 with 0x1304
    mcr p15, 0, r0, c1, c0, 0 // write to mmu control reg c1

    // set translation table base register 
    ldr r0, MTABLE
    mcr p15, 0, r0, c2, c0, 0 // set TTB register
    mcr p15, 0, r0, c8, c7, 0 // flush TTB register

    // set domain 0
    mov r0, #1 //b01 for client
    mcr p15, 0, r0, c3, c0, 0 

    // enable MMU
    mrc p15, 0, r0, c1, c0, 0 // get c1 into r0
    orr r0, r0, #0x00000001 // set bit 0 to 1
    mcr p15, 0, r0, c1, c0, 0 // write to c1 to enable mmu
    nop // time to allow mmu to start
    nop
    nop
    nop
    nop

    // this is no longer needed?
    //mrc p15, 0, r2, c2, c0, 0 // read TLB base reg c2 into r2
    //mov r2, r2

    // go back to svc mode (no longer needed?)
    //msr cpsr, #0x13 // svc mode with irq enabled

    bl main
    b .

MTABLE: .word 0x4000 // mtable at 16kb
regC1: .word 0x1304 // p15 control register c1 setting


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
  mrs r4, cpsr
  orr r4, r4, #0x80   // set bit means MASK off IRQ interrupt 
  msr cpsr, r4
  mov pc, lr

tswitch:
//       1  2  3  4  5  6  7  8  9  10  11  12  13  14
//       ---------------------------------------------
// stack=r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,r14
//       ---------------------------------------------

  stmfd	sp!, {r0-r12, lr}

  // lock 
  mrs r0, cpsr
  orr r0, r0, #0x80   // set bit means MASK off IRQ interrupt 
  msr cpsr, r0

  ldr r0, =running     // r0=&running
  ldr r1, [r0, #0]     // r1->runningPROC
  str sp, [r1, #4]     // running->ksp = sp

  bl	scheduler

  ldr r0, =running
  ldr r1, [r0, #0]     // r1->runningPROC
  ldr sp, [r1, #4]

  // unlock
  mrs r0, cpsr
  bic r0, r0, #0x80   // clear bit means UNMASK IRQ interrupt
  msr cpsr, r0
  
  ldmfd	sp!, {r0-r12, pc}


switchPgdir: // switch to a new proc pgdir during task switch
    // input: r0 contains PA of new procs pgdir
    mcr p15, 0, r0, c2, c0, 0 // set ttb in c2
    mov r1, #0
    mcr p15, 0, r1, c8, c7, 0 // flush ttb
    mcr p15, 0, 41, c7, c10, 0 // flush cache
    mrc p15, 0, r2, c2, c0, 0

    // set domain AP bits to client mode: check ap bits
    mov r0, #0x5 // 0101: domain1| domain0=client
    mcr p15, 0, r0, c3, c0, 0

    mov pc, lr // return

svc_entry: 
    // r0-r3 contain syscall params, do not disturb
    ldr r4, =running // save address of running proc into r4
    ldr r5, [r4, #0]
    mov r6, spsr
    str r6, [r5, #16] // save umode sr in proc.ucpsr at offset 16

    // go to sys mode
    mrs r6, cpsr // r6 = svc mode cpsr
    mov r7, r6 // save a copy of cpsr into r7
    orr r6, r6, #0x1F // r6 = sys mode
    msr cpsr, r6 // change cpsr to sys mode

    // save umode sp, pc into running proc
    str sp, [r5, #12] // save usp into proc.usp at offset 12
    str lr, [r5, #16] // save upc into proc.upc at offset 16

    // go back to svc mode
    msr cpsr, r7

    // replace saved lr in kstack with umode pc at syscall
    mov r6, sp
    add r6, r6, #52 // move r6 to stack entry 13

    // enable interrupts
    mrs r6, cpsr
    bic r6, r6, #52 // entry 13 => offset=13*4 = 52
    msr cpsr, r6
    bl svc_handler // call svc_handler in c

goUmode:
    // restore spsr?
    ldr r4, =running // r4 = running proc address
    ldr r5, [r4, #0] // r5 -> PROC of running
    ldr r6, [r5, #20] // umodr spsr?
    msr spsr, r6 // restore spsr of THIS process

    // go to sys mode
    mrs r6, cpsr // r6 = sys mode cpsr
    mov r7, r6 // save a copy of cpsr
    orr r6, r6, #0x1F // r6 = sys mode
    msr cpsr, r6 // change to sys mode

    // restore umode sp from proc usp
    ldr sp, [r5, #12] // restore umode from PROC.usp
    // go back to svc  mode
    msr cpsr, r3

    // pop registers from kstack and restore umode cpsr
    ldmfd sp!, {r0-r12, pc}^



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

get_cpsr:
    mrs r0, cpsr
    mov pc, lr

get_fault_status: // read and return MMU register 5
    mrc p15, 0, r0, c5, c0, 0 // read DFSR (data fault status register) c5
    mov pc, lr

get_fault_addr: // read and return MMU register 6
    mrc p15, 0, r0, c6, c0, 0 // read DFAR (data fault address register)
    mov pc, lr

get_spsr:
    mrs r0, spsr
    mov pc, lr


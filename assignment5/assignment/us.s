// entry point for all user mode programs
// contains some utility functions for user mode programs

.global entryPoint
.global main
.global syscall
.global getcsr
.global getAddr
.global get_entry

.text
.code 32
.global _exit // syscall to exit syscall(99, 0, 0, 0)

entryPoint:
    bl main
    bl _exit

get_entry:
    mov r0, #entryPoint
    mov pc, lr

syscall:
    swi #0
    mov pc, lr

get_cpsr:
    mrs r0, cpsr
    mov pc, lr

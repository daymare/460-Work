
    .global start, sum

// goal sum up a list
start:
    ldr sp, =stack_top // set stack pointer
    bl main // call sum
stop: b stop

sum: // int sum(int a,b,c,d,e,f): compute the sum of all these parameters
    // upon entry, stack top contains e, f passed by main
    // establish stack frame

    stmfd sp!, {fp, lr} // save r0-r4 and lr on stack
    add fp, sp, #4 // fp -> saved lr on stack

    mov r0, #0 // r0 = 0

    // compute the sum of all parameters
    // first 4 parameters in r0-r3
    add r0, r0, r1
    add r0, r0, r2
    add r0, r0, r3
    ldr r3, [fp, #4] // load e into r3
    add r0, r0, r3
    ldr r3, [fp, #8] // load f into r3
    add r0, r0, r3

    // return to caller
    sub sp, fp, #4
    ldmfd sp!, {fp, pc} // pop stack, return to caller


.text
.code 32
   .global start
   .global setjmp, longjmp

start:
    ldr sp, =stack_top
    bl main
    b . 


setjmp:
    stmfd   sp!, {fp, lr}
    add     fp, sp, #4
    ldr     r1, [fp]
    str     r1, [r0]
    ldr     r1, [fp, #-4]
    str     r1, [r0, #4]
    mov     r0, #0
    sub     sp, fp, #4
    ldmfd   sp!, {fp, pc}

longjmp:
    stmfd   sp!, {fp, lr}
    add     fp, sp, #4
    ldr     r2, [r0]
    str     r2, [fp]
    ldr     r2, [r0, #4]
    str     r2, [fp, #-4]
    mov     r0, r1
    sub     sp, fp, #4
    ldmfd   sp!, {fp, pc}

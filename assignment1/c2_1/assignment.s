
    .text
    .global start

start:
    mov r0, #1 // r0 = 1
    mov r1, #2 // r1 = 2
    add r1, r1, r0 // r1 = r1 + r0
    ldr r2, =result // r2 = &result
    str r1, [r2] // result = r1
stop: b stop
    .data
result: .word 0 // word location



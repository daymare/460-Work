
   .global reset_start

reset_start:
    ldr sp, =stack_top
    bl main
    b . 




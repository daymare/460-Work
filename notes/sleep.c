
#include "proc.c"

// maybe we should have a event manager that manages event keys
// or we could probably just specify which devices are mapped to what blocks of keys and let them self manage
// or we could just wait on the address of the variable we are waiting for

int sleep(int eventKey)
{
    // disable interrupts
    // enable interrupts
}

int wakeup(int eventKey)
{
}


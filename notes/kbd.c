
#define BUFFER_SIZE = 1000

typedef struct KBD
{
    char * base; // base address
    char buf[BUFFER_SIZE];
} KBD;

KBD kbd;

void kbd_handler()
{
    struct KBD *kp = &kbd;
    scanCode = *(kp->base + KDATA); // read scan code in data register
    // what is KDATA?

    if (scanCode & 0x80)
    {
        // bla
    }
    // bla
}



int kgetc()
{
    char c;
    KBD *kp = &kbd;

    while(1)
    {
        lock();

    }
}



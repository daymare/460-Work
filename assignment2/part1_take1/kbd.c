
#include "keymap"
#include "vid.c"


// keyboard register byte offsets off of base
#define KCNTL 0x00 // 7-6- 5(0=AT)4=RxIntEn 3=TxIntEn
#define KSTAT 0x04 // 7-6=TxE 5=TxBusy 4=RXFull 3=RxBusy
#define KDATA 0x08 // data register;

#define KCLK 0x0C // clock divisor register; (not used)
#define KISTA 0x10 // interrupt status register;(not used)


typedef volatile struct kbd
{ // base = 0x10006000
    char *base; // base address of KBD, as char *
    char buf[128]; // input buffer
    int head, tail, data, room; // control variables
} KBD;
volatile KBD kbd; // KBD data structure


// initialize the keyboard
int kbd_init()
{
    KBD *kp = &kbd;
    kp->base = (char *)0x10006000;
    *(kp->base+KCNTL) = 0x14; // 00010100=INTenable, Enable
    *(kp->base+KCLK) = 8;
    // PL051 manual says a value 0 to 15
    kp->data = 0; kp->room = 128; // counters
    kp->head = kp->tail = 0; // index to buffer
}


// handle a keyboard input
void kbd_handler()
{
    u8 scode, c;
    int i;
    KBD *kp = &kbd;
    color = RED;
    // int color in vid.c file
    scode = *(kp->base+KDATA); // read scan code in data register
    if (scode & 0x80) // ignore key releases
        return;

    c = unsh[scode];
    // map scan code to ASCII
    if (c != '\r')
        printf("kbd interrupt: c=%x %c\n", c, c);

    kp->buf[kp->head++] = c;
    kp->head %= 128; // enter key into CIRCULAR buf[ ]
    kp->data++; kp->room--; // update counters

    printf("keyboard handler received key %d", c);
}

int kgetc() // main program calls kgetc() to return a char
{
    char c;
    KBD *kp = &kbd;
    unlock();
    // enable IRQ interrupts
    while(kp->data <= 0); // wait for data; READONLY

    lock(); // disable IRQ interrupts

    c = kp->buf[kp->tail++];// get a c and update tail index
    kp->tail %= 128;
    kp->data--; kp->room++; // update with interrupts OFF

    unlock();

    // enable IRQ interrupts
    return c;
}

int kgets(char s[ ])
{
    // get a string from KBD
    char c;
    while((c=kgetc()) != '\r')
    {
        *s++ = c;
    }
    *s = 0;
    return strlen(s);
}

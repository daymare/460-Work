#define TXFE 0x80
#define RXFF 0x40
#define TXFF 0x20
#define RXFE 0x10
#define BUSY 0x08

#define UDR 0x00
#define UFR 0x18

typedef volatile struct uart{
    char *base;
    int n;
} UART;
UART uart[4];

int uart_init()
{
    int i;
    UART *up;
    for (i = 0; i < 4; i++)
    {
        up = &uart[i];
        up->base = (char *)(0x101F1000 + i*0x1000);
        up->n = 1;
    }
    uart[3].base = (char *)(0x10009000);
}

int ugetc(UART *up)
{
    while(*(up->base + UFR) & RXFE);
    return *(up->base + UDR);
}

int uputc(UART *up, char c)
{
    while(*(up->base + UFR) & TXFF);
    *(up->base + UDR) = c;
}

int ugets(UART *up, char *s)
{
    while((*s = ugetc(up)) != '\0')     // modification of code here
    {
        uputc(up, *s);
        s++;
    }
    *s = '\0';              // modification of code here
}

int uprints(UART *up, char *s)
{
    while(*s)
        uputc(up, *s++);
}

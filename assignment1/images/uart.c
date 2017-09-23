
#define TXFE 0x80
#define RXFF 0x40
#define TXFF 0x20
#define RXFE 0x10
#define BUSY 0x08

#define UDR 0x00
#define UFR 0x18

typedef volatile struct Uart
{
    char *base; // base address as char* (why char*?)
    int n; // uart number (0-3)
}Uart;

Uart uart[4];

// initialize uart drivers
int uart_init()
{
    int i;
    Uart *uart_p;

    for (i = 0; i < 4; i++)
    {
        uart_p = &uart[i];
        uart_p->base = (char *)(0x101F1000 + i * 0x1000);
        uart_p->n = i;
    }

    uart[3].base = (char *)(0x10009000); // uart 3 is special for some reason
}

char ugetc(Uart *uart_p)
{
    while (*(uart_p->base + UFR) & RXFE); // loop if flag register indicates read buffer is empty
    return *(uart_p->base+UDR);
}

void uputc(Uart *uart_p, char c)
{
    while (*(uart_p->base + UFR) & TXFF) // loop if flag register indicates write buffer full
    *(uart_p->base+UDR) = c; // write char to data register
}

// get a string from the 
void ugets(Uart *uart_p, char *s)
{
    char c = ugetc(uart_p);
    while (c != '\r')
    {
        *s = c;
        s++;
    }
    *s = 0;
}

// print a string to the uart 
void uprints(Uart *uart_p, char *s)
{
    while (*s != '\0')
    {
        uputc(uart_p, *s);
        s++;
    }
}






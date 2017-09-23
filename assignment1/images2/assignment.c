
#include "defines.h"
#include "vid.c"
#include "uart.c"

extern char _binary_image1_start;

int main()
{
    char c;
    char *p;

    UART* uart_p = &uart[0];

    fbuf_init();

    while (1)
    {
        p = &_binary_image1_start;
        show_bmp(p, 0, 80);
    }
}

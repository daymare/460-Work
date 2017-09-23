
#include "vid.c"
#include "uart.c"

extern char image_start;

int main()
{
    char c;
    char *p;

    Uart* uart_p = &uart[0];

    fbuf_init();

    while (1)
    {
        p = &image;
        show_bmp(p, 0, 80);
    }
}

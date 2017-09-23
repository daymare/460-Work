

#include "uart.c"


int main()
{
    int i;
    char string[64];
    Uart *uart_p;

    uart_init();
    uart_p = &uart[0]; // uart pointer = uart zero
    uprints(uart_p, "test");
}

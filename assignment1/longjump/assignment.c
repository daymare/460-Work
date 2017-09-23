
#include "type.h"
#include "defines.h"
#include "vid.c"
#include "uart.c"

int color, env[2];
UART* up;

int A()
{
    printf("enter A\n");
    B();
    printf("exit A\n");
}

int B()
{
    char line[8];
    printf("enter B\n");
    printf("enter y|n from UART port\n");
    uprintf(up, "long jump? : ");
    ugets(up, line);
    if (line[0] == 'y')
    {
        longjmp(env, 1234);
    }
    printf("exit B\n");
}

int main()
{
    int r;
    
    uart_init();

    up = &uart[0];

    fbuf_init();
    color = GREEN;

    r = setjmp(env);
    if (r == 0)
    {
        A();
        printf("normal return\n");
    }
    else
    {
        printf("back to main via long jump r=%d\n", r);
    }

    while(1);
}

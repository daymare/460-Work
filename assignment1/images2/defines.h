#ifndef DEFINES
#define DEFINES


#define UDR 0x00
#define UFR 0x18

#define TXFE 0x80
#define RXFF 0x40
#define TXFF 0x20
#define RXFE 0x10
#define BUSY 0x08

#define RED     0
#define BLUE    1
#define GREEN   2
#define WHITE   3

typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned int   u32;

typedef volatile struct uart{
    char *base;
    int n; 
}UART;

UART uart[4];

typedef unsigned char u8;
extern char _binary_font_start;
int color;
u8 cursor;
int volatile *fb;
int row, col, scroll_row;
unsigned char *font;
int width = 640;

char tab[] = {'0','1','2','3','4','5','6','7','8','9'};

#endif

/********************************************************************
Copyright 2010-2017 K.C. Wang, <kwang@eecs.wsu.edu>
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
********************************************************************/

// kbd.c file
#define N_SCAN 64

#include "vid.c"

#include "keymap"
/********* byte offsets; for char *base *********/
#define KCNTL    0x00 // 7- 6-    5(0=AT)  4=RxIntEn 3=TxIntEn  2   1   0
#define KSTAT    0x04 // 7- 6=TxE 5=TxBusy 4=RXFull  3=RxBusy   2   1   0
#define KDATA    0x08 // data register;
#define KCLK     0x0C // clock register;
#define KISTA    0x10 // int status register;

// special scan codes
#define SHIFT_BASE_CODE 42
#define CONTROL_BASE_CODE 29
#define ALT_BASE_CODE 56

typedef volatile struct kbd{ // base = 0x1000 6000
  char *base;         // base address of KBD, as char *
  char buf[128];
  int shift_on;
  int control_on;
  int alt_on;
  int head, tail, data, room;
}KBD;

extern int color;
volatile KBD kbd;
int kputc(char);

int kbd_init()
{
   KBD *kp = &kbd;
   kp->base = (char *)0x10006000;
   *(kp->base+KCNTL) = 0x14; // 0001 0100
   *(kp->base+KCLK)  = 8;
   kp->data = 0;kp->room = 128; 
   kp->head = kp->tail = 0;
   kp->shift_on = 0;
   kp->control_on = 0;

   /* enable KBD IRQ */
   VIC_INTENABLE |= 1<<31;   // SIC to VIC's IRQ31
   SIC_ENSET |= 1<<3;        // KBD int=3 on SIC
}

void kbd_handler()
{
  u8 scode, c;
  KBD *kp = &kbd;
  scode = *(kp->base+KDATA);

    // debug printf
    //kprintf("scan code: %d", scode & ~0x80);

  u8 base_code = scode & ~0x80;
  u8 is_key_press = (scode & 0x80) ? 0 : 1;
  u8 is_key_release = !is_key_press;

  // check for special keys 
  u8 is_special = 0;
    switch(base_code)
    {
        case SHIFT_BASE_CODE:
            kp->shift_on = is_key_press;
            is_special = 1;
            break;
        case CONTROL_BASE_CODE:
            kp->control_on = is_key_press;
            is_special = 1;
            break;
        case ALT_BASE_CODE:
            kp->alt_on = is_key_press;
            is_special = 1;
            break;
    }

    // filter out releases and special keys
    if (is_key_release || is_special)
      return;

    if (kp->control_on)
    {
        kprintf("CNTRL-%c ", sh[base_code]);
        return;
    }
    else if(kp->alt_on)
    {
        kprintf("ALT-%c ", sh[base_code]);
        return;
    }

    // place a keyboard key into the buffer
    if (kp->shift_on)
    {
        c = sh[base_code];
    }
    else
    {
        c = unsh[base_code];
    }

    kp->buf[kp->head++] = c;
    kp->head %= 128;
    kp->data++; kp->room--;
}

int kputc(char);  // kputc() in vid.c driver

int kgetc()
{
  char c;
  KBD *kp = &kbd;
  while(kp->data <= 0); // wait for data > 0; RONLY, no need to lock
  c = kp->buf[kp->tail++];
  kp->tail %= 128;

  // updating variables: must disable interrupts
  int_off();
    kp->data--; kp->room++;
  int_on();
  return c;
}

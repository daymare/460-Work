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

#include "type.h"
#include "string.c"

#include "exceptions.c"
#include "kbd.c"
#include "timer.c"
#include "vid.c"
//#include "uart.c"

#include "kernel.c"



void copy_vectors(void) {
    extern u32 vectors_start, vectors_end;
    u32 *vectors_src = &vectors_start;
    u32 *vectors_dst = (u32 *)0;

    while(vectors_src < &vectors_end)
      *vectors_dst++ = *vectors_src++;
}

void IRQ_handler()
{
    unsigned int vicstatus, sicstatus;
    unsigned int ustatus, kstatus;

    // read VIC status register to find out which interrupt
    vicstatus = VIC_STATUS; // VIC_STATUS=0x10140000=status reg
    sicstatus = SIC_STATUS;  

    //printf("vicstatus=%x sicstatus=%x\n", vicstatus, sicstatus);
    if (vicstatus & (1<<31)){
       if (sicstatus & (1<<3)){
           kbd_handler();
       }
    }

    if (vicstatus & (1<<4))
    {
        timer_handler(0);
    }
}

int timer_task()
{
    Timer* t = &timer[0];

    while(1)
    {
        ksleep(EVENT_ONE_TICK);

        // every second update the wall clock
        if (t->tick == 0)
        {
            // seconds
            t->clock[7] = '0' + (t->second%10);
            t->clock[6] = '0' + (t->second/10);

            // minutes
            t->clock[4] = '0' + (t->minute%10);
            t->clock[3] = '0' + (t->minute/10);

            // hours
            t->clock[1] = '0' + (t->hour%10);
            t->clock[0] = '0' + (t->hour/10);
        }

        for (int i = 0; i < 8; i++)
            kpchar(t->clock[i], 0, 70+i);

        /*printf("timer task running! time: %s\n", t->clock);*/

    }
}

int kbd_task()
{
    char line[128];
    while(1)
    {
        printf("Keyboard task id: %d sleep for a line from keyboard\n", running->pid);
        kgets(line);
        printf("line = %s\n", line);
    }
}

int main()
{ 
    // clear enabled interrupts
    VIC_INTENABLE = 0;
    SIC_ENSET = 0;

    fbuf_init();
    //uart_init();
    kbd_init();
    timer_init();

    color = WHITE;

    // init kernel
    printf("Welcome to WANIX in Arm\n");
    kernel_init();

    // create subtasks
    printf("P0 create tasks\n");
    kfork((int)kbd_task, 1);
    kfork((int)timer_task, 1);
    //kfork((int)body, 1);

    // p0 run when no other task is runnable
    printf("P0 switch process\n");
    char line[128];

    timer_start(0);
    while(1){
         if (readyQueue != NULL)
         {
             tswitch();
         }
         //printf("bla");
         //kgets(line);
         //printf("line: %s", line);
    }
}

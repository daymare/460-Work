

// event codes!
// event codes 101-199 are reserved for the timer
#define EVENT_ONE_TICK 101
#define EVENT_ONE_SECOND 102

#define DIVISOR 64

typedef volatile struct timer
{
    u32 *base;
    int tick;
    int second;
    int minute;
    int hour;
    char clock[16];
}Timer;

volatile Timer timer[4];

void timer_init()
{
    printf("timer init!\n");

    timer[0].base = (u32*)TIMER0_BASE_ADDR;
    timer[1].base = (u32*)TIMER1_BASE_ADDR;

    for (int i = 0; i < 2; i++)
    {
        *(timer[i].base + TIMER_LOAD_OFFSET) = 0x0;
        *(timer[i].base + TIMER_VALUE_OFFSET) = 0xFFFFFFF;
        *(timer[i].base + TIMER_RIS_OFFSET) = 0x0;
        *(timer[i].base + TIMER_MIS_OFFSET) = 0x0;
        *(timer[i].base + TIMER_LOAD_OFFSET) = 0x100;
        *(timer[i].base + TIMER_CONTROL_OFFSET) = 0x66;
        *(timer[i].base + TIMER_BGLOAD_OFFSET) = 0x1C00;
        strcpy(timer[i].clock, "00:00:00");
        timer[i].tick = 0;
        timer[i].second = 0;
        timer[i].minute = 0;
        timer[i].hour = 0;
        timer_clear_interrupt(i);
    }
    
    // enable timer interrupts
    VIC_INTENABLE |= 1<<4;   // SIC to VIC's IRQ31
    timer_clear_interrupt(0);
}

void timer_start(int n)
{
    Timer* tpr;
    tpr = &timer[n];
    *(tpr->base + TIMER_CONTROL_OFFSET) |= 0x80; // enable
}

void timer_clear_interrupt(int n)
{
    Timer* tpr = &timer[n];
    *(tpr->base + TIMER_INTCLR_OFFSET) = 0xFFFFFFFF; // clear the interrupt register
}

void timer_handler(int n)
{
    Timer *t = &timer[n];
    t->tick++;

    kwakeup(EVENT_ONE_TICK);

    if (t->tick == 9)
    {
        t->tick = 0;
        t->second++;

        // wakeup any processes that are waiting on a second
        kwakeup(EVENT_ONE_SECOND);

        if (t->second == 60)
        {
            t->second = 0;
            t->minute++;
            if (t->minute == 60)
            {
                t->minute = 0;
                t->hour++;
                t->hour %= 24;
            }
        }
    }

    timer_clear_interrupt(0);
    timer_clear_interrupt(1);
}



#include <timer/timer.h>

volatile uint32_t ticks = 0;

void timer_tick()
{
    ticks++;
}

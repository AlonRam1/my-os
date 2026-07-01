#include "vga.h"
#include <stdint.h>
void isr0_handler()
{
    volatile uint16_t* vga = (uint16_t*)0xB8000;
    vga[0] = 0x0F58; // 'X'
}

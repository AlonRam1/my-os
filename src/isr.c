#include "vga.h"
#include <stdint.h>

void puts(const char* s);

void isr0_handler()
{
    volatile uint16_t* vga = (uint16_t*)0xB8000; //reminder: 0xB8000 is the start of vga in MMIO
    puts("INTERRUPT CALLED: DEVIDE BY ZERO\n");
}

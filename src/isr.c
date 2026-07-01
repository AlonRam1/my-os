#include "vga.h"
#include <stdint.h>

void puts(const char* s);

void isr0_handler()
{
    volatile uint16_t* vga = (uint16_t*)0xB8000; //reminder: 0xB8000 is the start of vga in MMIO
    puts("INTERRUPT 0 CALLED\n");
}

void isr1_handler()
{
    volatile uint16_t* vga = (uint16_t*)0xB8000; //reminder: 0xB8000 is the start of vga in MMIO
    puts("INTERRUPT 1 CALLED\n");
}

void isr3_handler()
{
    volatile uint16_t* vga = (uint16_t*)0xB8000; //reminder: 0xB8000 is the start of vga in MMIO
    puts("INTERRUPT 3 CALLED\n");
}

void isr4_handler()
{
    volatile uint16_t* vga = (uint16_t*)0xB8000; //reminder: 0xB8000 is the start of vga in MMIO
    puts("INTERRUPT 4 CALLED\n");
}

void isr5_handler()
{
    volatile uint16_t* vga = (uint16_t*)0xB8000; //reminder: 0xB8000 is the start of vga in MMIO
    puts("INTERRUPT 5 CALLED\n");
}

void isr6_handler()
{
    volatile uint16_t* vga = (uint16_t*)0xB8000; //reminder: 0xB8000 is the start of vga in MMIO
    puts("INTERRUPT 6 CALLED\n");
}

void isr8_handler()
{
    volatile uint16_t* vga = (uint16_t*)0xB8000; //reminder: 0xB8000 is the start of vga in MMIO
    puts("INTERRUPT 8 CALLED\n");
}

void isr13_handler()
{
    volatile uint16_t* vga = (uint16_t*)0xB8000; //reminder: 0xB8000 is the start of vga in MMIO
    puts("INTERRUPT 13 CALLED\n");
}

void isr14_handler()
{
    volatile uint16_t* vga = (uint16_t*)0xB8000; //reminder: 0xB8000 is the start of vga in MMIO
    puts("INTERRUPT 14 CALLED\n");
}

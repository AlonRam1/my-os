#include <vga/vga.h>
#include <pic/pic.h>
#include <kernel/task.h>
#include <timer/timer.h>
#include <kernel/scheduler.h>
#include <kernel/syscall.h>
#include <drivers/keyboard/keyboard.h>
#include <stdint.h>

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
   volatile uint16_t* vga = (uint16_t*)0xB8000;
   puts("INTERRUPT 8 CALLED\n");
}

void isr13_handler()
{		
    puts("INTERRUPT 13 CALLED\n");
}

void isr14_handler(uint32_t* regs)
{
   puts("INTERRUPT 14 CALLED\n"); 
}

void isr128_handler(uint32_t* regs)
{
    syscall_handler(regs);
}

uint32_t irq0_handler(uint32_t* esp)
{
    timer_tick();

    pic_eoi(0);

    return schedule(esp);
}

void irq1_handler(void)
{
    keyboard_handler();
}

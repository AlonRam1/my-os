#include "vga/vga.h"
#include "pic/pic.h"
#include "task/task.h"
#include "timer/timer.h"
#include "scheduler/scheduler.h"
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
    uint32_t syscall = regs[7];

    if (syscall == 1)
    {
        char c = (char)regs[4];
        putchar(c);
    }
}

uint32_t irq0_handler(uint32_t* esp)
{
    timer_tick();

    pic_eoi(0);

    return schedule(esp);
}

void irq1_handler(void)
{
    //keyboard lookup table (name stands for keyboard US)
    static const char kbdus[128] = {
    0,
    27,        // Esc
    '1','2','3','4','5','6','7','8','9','0','-','=',
    '\b',
    '\t',
    'q','w','e','r','t','y','u','i','o','p','[',']',
    '\n',
    0,
    'a','s','d','f','g','h','j','k','l',';','\'','`',
    0,
    '\\',
    'z','x','c','v','b','n','m',',','.','/',
    0,
    '*',
    0,
    ' ',
};

    uint8_t sc = inb(0x60); //0x60 is the keyboard data port

    // ignore key releases
    if (sc & 0x80) //if given value is a key release (marked by bit 7 -> hence why we use mask 0x80)
    {
        pic_eoi(1);
        return;
    }

    char c = kbdus[sc];

    if (c)
        putchar(c);

    pic_eoi(1); //notify PIC of end-of-interrupt

}

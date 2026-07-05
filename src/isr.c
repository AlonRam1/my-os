#include "vga.h"
#include "pic.h"
#include <stdint.h>

void puts(const char* s);
uint8_t inb(uint16_t port);
void pic_eoi(uint8_t irq);
void pic_remap(void);
void puthex(uint8_t v);



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
    volatile uint16_t* vga = (uint16_t*)0xB8000;
    puts("INTERRUPT 13 CALLED CALLED\n");
}
void isr14_handler()
{
    volatile uint16_t* vga = (uint16_t*)0xB8000;
    puts("INTERRUPT 14 CALLED\n");
}

volatile uint32_t ticks=0;

void irq0_handler()
{
    ticks++;
    pic_eoi(0); //notify PIC of end-of-interrupt
}



void irq1_handler(void)
{
    //keyboard lookup table (name stands for keyboard US (layout))
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

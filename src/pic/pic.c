#include "pic.h"
#include <stdint.h>

/*
 * PIC ports:
 * Master: 0x20-0x21
 * Slave : 0xA0-0xA1
 */

//write byte to hardware port
void outb(uint16_t port, uint8_t val)
{
    asm volatile ("outb %0, %1" :: "a"(val), "Nd"(port));
}

//read byte from hardware port
uint8_t inb(uint16_t port)
{
    uint8_t ret;
    asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}


//move IRQs 0-7 to INTs 32-39 and 8-15 to 40-47 to avoid conflicts with existing INTs
void pic_remap(void)
{
    uint8_t a1, a2;

    //Save masks
    a1 = inb(0x21);
    a2 = inb(0xA1);

    //Start initialization sequence (cascade mode)
    outb(0x20, 0x11);
    outb(0xA0, 0x11);

    //Set vector offsets
    outb(0x21, 0x20); // IRQ0–7 -> INT32–39
    outb(0xA1, 0x28); // IRQ8–15 -> INT40–47

    //Tell Master PIC there is a slave PIC at IRQ2
    outb(0x21, 0x04);

    //Tell Slave PIC its cascade identity
    outb(0xA1, 0x02);

    //Set mode: 8086/88 (not 8080 mode)
    outb(0x21, 0x01);
    outb(0xA1, 0x01);

    //Restore masks
    outb(0x21, a1);
    outb(0xA1, a2);
}

//"end of interrupt" -> have to tell PIC that interrupt is done manually.
void pic_eoi(uint8_t irq)
{
    // If IRQ came from slave PIC
    if (irq >= 8)
    {
        outb(0xA0, 0x20); // slave EOI
    }

    // master EOI
    outb(0x20, 0x20);
}

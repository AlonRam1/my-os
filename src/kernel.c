#include "vga.h"
#include "idt.h"
#include <stdint.h>

void kmain(void)
{
    volatile uint16_t cs;
    puts("WHAT'S GOOD NIGGA?? WELCOME TO MYOS\n");
    
    idt_init(); //load IDT
    puts("IDT loaded\n");

    asm volatile("int $0x0"); //REMOVE: INT0 TEST
    asm volatile("int $0x1"); 
    asm volatile("int $0x3");
    asm volatile("int $0x4");
    asm volatile("int $0x5");
    asm volatile("int $0x6");
    //asm volatile("int $0x8");
    //asm volatile("int $0x13");
    //asm volatile("int $0x14");
    

    puts("AFTER INT\n");

    //enter idle loop until next interrupt
    while (1)
        asm volatile("hlt");
}

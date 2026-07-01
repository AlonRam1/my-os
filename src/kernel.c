#include "vga.h"
#include "idt.h"
#include <stdint.h>

void kmain(void)
{
    volatile uint16_t cs;

    puts("BOOT OK\n");

    asm volatile("mov %%cs, %0" : "=r"(cs));

    puts("CS captured\n");

    idt_init();

    puts("IDT loaded\n");

    asm volatile("int $0x0");

    puts("AFTER INT\n");

    while (1)
        asm volatile("hlt");
}

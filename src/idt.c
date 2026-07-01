#include "idt.h"

extern void isr0();

static struct idt_entry idt[256] __attribute__((aligned(16))); //the IDT (static so scope is entire program. __attribute ... is a gcc extension that makes the variable address divisible by 16)
static struct idt_ptr idtp; //pointer to the IDT

//function that fills a row in the IDT
static void set_gate(int n, uint32_t handler)
{
    idt[n].base_lo = handler & 0xFFFF; //lower half of isr address
    idt[n].base_hi = (handler >> 16) & 0xFFFF; //upper half of isr address
    idt[n].sel = 0x10; //segment selector. points to the kernel code segment (0x10 is boilerplate)
    idt[n].always0 = 0; //padding
    idt[n].flags = 0x8E; //flags (0x8E is boilerplate)
}

void idt_init()
{
    //zero IDT out
    for (int i = 0; i < 256; i++)
        set_gate(i, 0);

    //start filling IDT
    set_gate(0, (uint32_t)isr0);

    //build IDT pointer
    idtp.limit = sizeof(idt) - 1;
    idtp.base  = (uint32_t)&idt;

    asm volatile("lidt %0" : : "m"(idtp)); //load IDT pointer into IDT pointer register
}

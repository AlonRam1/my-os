#include <arch/i386/idt.h>
#include <arch/i386/isr.h>

static struct idt_entry idt[256] __attribute__((aligned(16))); //the IDT (static so scope is entire program. __attribute ... is a gcc extension that makes the variable address divisible by 16)
static struct idt_ptr idtp; //pointer to the IDT

//function that fills a row in the IDT
static void set_gate(int n, uint32_t handler)
{
    idt[n].base_lo = handler & 0xFFFF; //lower half of isr address
    idt[n].base_hi = (handler >> 16) & 0xFFFF; //upper half of isr address
    idt[n].sel = 0x08; //segment selector. points to the kernel code segment (0x10 is boilerplate)
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
    set_gate(1, (uint32_t)isr1);
    set_gate(3, (uint32_t)isr3);
    set_gate(4, (uint32_t)isr4);
    set_gate(5, (uint32_t)isr5);
    set_gate(6, (uint32_t)isr6);
    set_gate(8, (uint32_t)isr8);
    set_gate(13, (uint32_t)isr13);
    set_gate(14, (uint32_t)isr14);
    set_gate(32,(uint32_t)irq0);
    set_gate(33,(uint32_t)irq1);
    //usermode test
    set_gate(128, (uint32_t)isr128);
    idt[128].flags = 0xEE;


    //build IDT pointer
    idtp.limit = sizeof(idt) - 1;
    idtp.base  = (uint32_t)&idt;

    asm volatile("lidt %0" : : "m"(idtp)); //load IDT pointer into IDT pointer register
}

#include "idt.h"

extern void isr0();

static struct idt_entry idt[256] __attribute__((aligned(16)));
static struct idt_ptr idtp;

static void set_gate(int n, uint32_t handler)
{
    idt[n].base_lo = handler & 0xFFFF;
    idt[n].base_hi = (handler >> 16) & 0xFFFF;

    idt[n].sel = 0x10;
    idt[n].always0 = 0;
    idt[n].flags = 0x8E;
}

void idt_init()
{
    for (int i = 0; i < 256; i++)
        set_gate(i, 0);

    set_gate(0, (uint32_t)isr0);

    idtp.limit = sizeof(idt) - 1;
    idtp.base  = (uint32_t)&idt;

    asm volatile("lidt %0" : : "m"(idtp));
}

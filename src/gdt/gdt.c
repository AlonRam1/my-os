#include "gdt.h"
#include "tss/tss.h"

extern void gdt_load(struct gdt_ptr*);

static struct gdt_entry gdt[6];
static struct gdt_ptr gp;

static void gdt_set_gate(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
{
    gdt[num].base_low = base & 0xFFFF;
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high = (base >> 24) & 0xFF;

    gdt[num].limit_low = limit & 0xFFFF;
    gdt[num].granularity = (limit >> 16) & 0x0F;

    gdt[num].granularity |= gran & 0xF0;

    gdt[num].access = access;
}

static void gdt_set_tss(int num, uint32_t base, uint32_t limit)
{
    gdt[num].base_low = base & 0xFFFF;
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high = (base >> 24) & 0xFF;

    gdt[num].limit_low = limit & 0xFFFF;
    gdt[num].granularity = (limit >> 16) & 0x0F;

    gdt[num].granularity |= 0x00;

    gdt[num].access = 0x89;
}

void gdt_init()
{
    gp.limit = sizeof(gdt) - 1;
    gp.base = (uint32_t)&gdt;

    //null
    gdt_set_gate(0,0,0,0,0);

    //kernel code
    gdt_set_gate(1,0,0xFFFFFFFF,0x9A,0xCF);

    //kernel data
    gdt_set_gate(2,0,0xFFFFFFFF,0x92,0xCF);

    //user code
    gdt_set_gate(3,0,0xFFFFFFFF,0xFA,0xCF);

    //user data
    gdt_set_gate(4,0,0xFFFFFFFF,0xF2,0xCF);

    //TSS
    gdt_set_tss(5, (uint32_t)&tss, sizeof(struct tss_entry) - 1);

    gdt_load(&gp);
}

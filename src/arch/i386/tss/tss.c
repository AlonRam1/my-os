#include <arch/i386/tss.h>

extern void tss_load();

struct tss_entry tss;

void tss_set_kernel_stack(uint32_t esp)
{
    tss.esp0 = esp;
}

void tss_init()
{
    for (int i = 0; i < sizeof(struct tss_entry); i++)
        ((uint8_t*)&tss)[i] = 0;

    tss.ss0 = 0x10;      //kernel data selector
    tss.esp0 = 0x500000; //temporary kernel stack
    tss.iomap = sizeof(struct tss_entry);
    tss_load();
}

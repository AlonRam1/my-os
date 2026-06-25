#include <stdint.h>

void kmain(void)
{
    volatile uint16_t* vga = (volatile uint16_t*)0xB8000;

    const char* msg = "Hello kernel";

    for (uint32_t i = 0; i < 12; i++)
    {
        vga[i] = (0x0F << 8) | msg[i];
    }

    while (1)
    {
        __asm__ volatile ("hlt");
    }
}

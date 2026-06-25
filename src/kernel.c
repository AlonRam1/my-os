#include <stdint.h>

void kmain(void)
{
    //0xB8000 = start of VGA text buffer in memory. coordinates on screen are mapped to addresses (memory mapped IO)	
    volatile uint16_t* vga = (volatile uint16_t*)0xB8000;

    const char* msg = "Hello kernel";

    for (uint32_t i = 0; i < 12; i++)
    {
	//vga screen cell format:
	//[[ background | foreground | character ]
	//    4 bits        4 bits       8 bits
        vga[i] = (0x0F << 8) | msg[i]; // now vga[i] is 0x0F00 (OR) msg[i](char=8 bits)
    }

    while (1)
    {
	//__asm__ = GCC format for writing assembly    
        __asm__ volatile ("hlt");
    }
}

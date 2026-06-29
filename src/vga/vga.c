#include <stdint.h>

// vga.c
volatile uint16_t* vga = (uint16_t*)0xB8000;

static int row = 0;
static int col = 0;

void putchar(char c)
{
    if (c == '\n')
    {
        row++;
        col = 0;
        return;
    }

    vga[row * 80 + col] = (0x0F << 8) | c;
    col++;

    if (col >= 80)
    {
        col = 0;
        row++;
    }
}

void puts(const char* s)
{
    while (*s)
        putchar(*s++);
}

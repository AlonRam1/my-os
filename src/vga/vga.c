#include <stdint.h>

volatile uint16_t* vga = (uint16_t*)0xB8000;

static int row = 0;
static int col = 0;

void putchar(char c)
{
    asm volatile("cli");

    if (c == '\n')
    {
        row++;
        col = 0;
    }
    else
    {
        vga[row * 80 + col] = (0x0F << 8) | c;
        col++;

        if (col >= 80)
        {
            col = 0;
            row++;
        }
    }

    if (row >= 25)
    {
        row = 0;   // temporary wrap (no scrolling yet)
        col = 0;
    }

    asm volatile("sti");
}

void puts(const char* s)
{
    asm volatile("cli");

    while (*s)
    {
        char c = *s++;

        if (c == '\n')
        {
            row++;
            col = 0;
        }
        else
        {
            vga[row * 80 + col] = (0x0F << 8) | c;
            col++;

            if (col >= 80)
            {
                col = 0;
                row++;
            }
        }

        if (row >= 25)
        {
            row = 0;
            col = 0;
        }
    }

    asm volatile("sti");
}

const char hex[] = "0123456789ABCDEF";

void puthex(uint8_t v)
{

    asm volatile("cli");

    char high = hex[(v >> 4) & 0xF];
    char low  = hex[v & 0xF];

    vga[row * 80 + col] = (0x0F << 8) | high;
    col++;

    vga[row * 80 + col] = (0x0F << 8) | low;
    col++;

    //page bounds
    if (col >= 80)
    {
        col = 0;
        row++;
    }

    if (row >= 25)
    {
        row = 0;
        col = 0;
    }

    asm volatile("sti");
}
